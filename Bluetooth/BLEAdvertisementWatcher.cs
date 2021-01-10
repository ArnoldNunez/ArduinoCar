using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.Advertisement;
using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace ArduinoCar.Bluetooth
{
    /// <summary>
    /// Wraps the <see cref="BluetoothLEAdvertisementWatcher"/>
    /// </summary>
    public class BLEAdvertisementWatcher
    {
        private readonly object _threadLock = new object();
        private readonly BluetoothLEAdvertisementWatcher _watcher;
        private readonly Dictionary<string, BLEDevice> _discoveredDevices;
        private readonly GattServiceIds _gattServiceIds;
        


        /// <summary>
        /// Fired when the bluetooth listener stops listening
        /// </summary>
        public event Action StoppedListening = () => {};

        /// <summary>
        /// Fired when the bluetooth listener starts listening
        /// </summary>
        public event Action StartedListening = () => { };

        /// <summary>
        /// Fire when a device is discovered.
        /// </summary>
        public event Action<BLEDevice> DeviceDiscovered = (BLEDevice) => { };

        /// <summary>
        /// Fired when a new device is discovered.
        /// </summary>
        public event Action<BLEDevice> NewDeviceDiscovered = (device) => { };

        /// <summary>
        /// Fired when a device name changes
        /// </summary>
        public event Action<BLEDevice> DeviceNameChanged = (device) => { };

        /// <summary>
        /// Fires when a device times out
        /// </summary>
        public event Action<BLEDevice> DeviceTimeout = (device) => { };



        /// <summary>
        /// Indicates if this watcher is listening for advertisements
        /// </summary>
        public bool Listening => _watcher.Status == BluetoothLEAdvertisementWatcherStatus.Started;

        /// <summary>
        /// A list of discovered devices
        /// </summary>
        public IReadOnlyCollection<BLEDevice> DiscoveredDevices
        {
            get
            {
                // Clean up any timeouts
                CleanupTimeouts();

                // Lock because if list changes at point of querying it will crash
                lock (_threadLock)
                {
                    // Convert to read only list
                    return _discoveredDevices.Values.ToList().AsReadOnly();
                }
            }
        }

        /// <summary>
        /// The timeout in seconds that a device is removed from the <see cref="DiscoveredDevices"/>
        /// list if it is not re-advertise within this time.
        /// </summary>
        public int HeartbeatTimeout { get; set; } = 30;



        /// <summary>
        /// Constructor
        /// </summary>
        public BLEAdvertisementWatcher(GattServiceIds gattIds)
        {
            _gattServiceIds = gattIds ?? throw new ArgumentNullException(nameof(gattIds));

            _discoveredDevices = new Dictionary<string, BLEDevice>();

            _watcher = new BluetoothLEAdvertisementWatcher
            {
                ScanningMode = BluetoothLEScanningMode.Active
            };

            _watcher.Received += WatcherAdvertisementReceivedAsync;

            _watcher.Stopped += (watcher, e) =>
            {
                StoppedListening();
            };
        }



        /// <summary>
        /// Starts listening for advertisements
        /// </summary>
        public void StartListening()
        {
            lock (_threadLock)
            {
                if (Listening) { return; }

                _watcher.Start();
            }

            StartedListening();
        }

        /// <summary>
        /// Stops listening for advertisements
        /// </summary>
        public void StopListening()
        {
            lock (_threadLock)
            {
                if (!Listening) { return; }

                _watcher.Stop();
                _discoveredDevices.Clear();
            }
        }



        /// <summary>
        /// Listens out for watcher advertisements
        /// </summary>
        /// <param name="sender">The watcher</param>
        /// <param name="args">The arguments</param>
        private async void WatcherAdvertisementReceivedAsync(BluetoothLEAdvertisementWatcher sender, BluetoothLEAdvertisementReceivedEventArgs args)
        {
            // Cleanup the timeouts
            CleanupTimeouts();

            // Get BLE device info
            BLEDevice device = await GetBluetoothLEDeviceAsync(args.BluetoothAddress, args.Timestamp, args.RawSignalStrengthInDBm);

            if (device == null) { return; }

            // Check if this a new discovery
            bool newDiscovery = false;
            string existingName = default;
            bool nameChanged = default;

            lock (_threadLock)
            {
                newDiscovery = !_discoveredDevices.ContainsKey(device.DeviceId);

                if (!newDiscovery)
                {
                    existingName = _discoveredDevices[device.DeviceId].Name;
                }

                // Check if name has changed
                nameChanged = !newDiscovery &&
                    !string.IsNullOrEmpty(device.Name) &&
                    existingName != device.Name;

                // Check for listening status due to threading
                if (!Listening)
                    return;

                // Add/update the device in dictionary
                _discoveredDevices[device.DeviceId] = device;
            }

            // Device discovered event
            DeviceDiscovered(device);

            // If device name changed
            if (nameChanged)
            {
                DeviceNameChanged(device);
            }

            // If new discover...
            if (newDiscovery)
            {
                // If listening thread sleeps, this code wont continue so maybe fire in
                // in task.
                NewDeviceDiscovered(device);
            }
        }

        /// <summary>
        /// Connects to the BLE device and extracts more information from the bluetooth device
        /// </summary>
        /// <param name="address">The bluetooth address of the device to connect to</param>
        /// <param name="broadcastTime">The time the broadcast message was received</param>
        /// <param name="rssi">The signal strength in db</param>
        /// <returns></returns>
        private async Task<BLEDevice> GetBluetoothLEDeviceAsync(ulong address, DateTimeOffset broadcastTime, short rssi)
        {
            // Get bluetooth device info
            using BluetoothLEDevice device = await BluetoothLEDevice.FromBluetoothAddressAsync(address);

            if (device == null) { return null; }


            GattDeviceServicesResult gatt = await device.GetGattServicesAsync();

            if (gatt.Status == GattCommunicationStatus.Success)
            {
                // Loop each gatt service
                foreach (GattDeviceService service in gatt.Services)
                {
                    // This id contains the gatt profile assigned number we want
                    // TODO: Get more info and connect.
                    Guid gattProfileId = service.Uuid;
                }
            }


            // Return the new device information
            return new BLEDevice
            (
                deviceId: device.DeviceId,
                name: device.Name,
                address: device.BluetoothAddress,
                rssi: rssi,
                broadcastTime: broadcastTime,
                connected: device.ConnectionStatus == BluetoothConnectionStatus.Connected,
                canPair: device.DeviceInformation.Pairing.CanPair,
                paired: device.DeviceInformation.Pairing.IsPaired
            );
        }

        /// <summary>
        /// Prune any timed out devices that we have not heard from
        /// </summary>
        private void CleanupTimeouts()
        {
            lock (_threadLock)
            {
                // The oldest allowed date time threshold, now minus heartbeat timeout
                DateTime threshold = DateTime.Now - TimeSpan.FromSeconds(HeartbeatTimeout);

                // Any devices that have not sent a new broadcast within the heartbeat time
                _discoveredDevices.Where(d => d.Value.BroadcastTime < threshold).ToList().ForEach(device =>
                {
                    // Remove device
                    _discoveredDevices.Remove(device.Key);

                    // Inform listeners
                    DeviceTimeout(device.Value);
                });
            }
        }
    }
}
