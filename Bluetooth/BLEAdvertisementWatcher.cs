using System;
using System.Collections.Generic;
using System.Linq;
using Windows.Devices.Bluetooth.Advertisement;

namespace ArduinoCar.Bluetooth
{
    /// <summary>
    /// Wraps the <see cref="BluetoothLEAdvertisementWatcher"/>
    /// </summary>
    public class BLEAdvertisementWatcher
    {
        private object _threadLock = new object();
        private readonly BluetoothLEAdvertisementWatcher _watcher;
        private readonly Dictionary<ulong, BLEDevice> _discoveredDevices;



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
        public BLEAdvertisementWatcher()
        {
            _discoveredDevices = new Dictionary<ulong, BLEDevice>();

            _watcher = new BluetoothLEAdvertisementWatcher
            {
                ScanningMode = BluetoothLEScanningMode.Active
            };

            _watcher.Received += WatcherAdvertisementReceived;

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
        private void WatcherAdvertisementReceived(BluetoothLEAdvertisementWatcher sender, BluetoothLEAdvertisementReceivedEventArgs args)
        {
            // Cleanup the timeouts
            CleanupTimeouts();

            BLEDevice device = null;

            // Is new discover?
            bool newDiscovery = !_discoveredDevices.ContainsKey(args.BluetoothAddress);

            // Name Changed?
            bool nameChanged = !newDiscovery && !string.IsNullOrEmpty(args.Advertisement.LocalName) 
                && _discoveredDevices[args.BluetoothAddress].Name != args.Advertisement.LocalName;

            lock (_threadLock)
            {
                string name = args.Advertisement.LocalName;

                // If not a new discovery and name is blank don't bother setting it
                if (string.IsNullOrEmpty(name) && !newDiscovery)
                {
                    name = _discoveredDevices[args.BluetoothAddress].Name;
                }

                // Creat new device info class
                device = new BLEDevice
                (
                    address: args.BluetoothAddress,
                    name: name,
                    broadcastTime: args.Timestamp,
                    rssi: args.RawSignalStrengthInDBm
                );

                // Add/update the device in dictionary
                _discoveredDevices[args.BluetoothAddress] = device;
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
