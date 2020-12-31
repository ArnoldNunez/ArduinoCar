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
                // Lock because if list changes at point of querying it will crash
                lock (_threadLock)
                {
                    // Convert to read only list
                    return _discoveredDevices.Values.ToList().AsReadOnly();
                }
            }
        }



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

            // Listen out for new advertisements
            _watcher.Received += (watcher, e) =>
            {
                StoppedListening();
            };
        }



        /// <summary>
        /// Starts listening for advertisements
        /// </summary>
        public void StartListening()
        {
            if (Listening) { return; }

            _watcher.Start();
            StartedListening();
        }

        /// <summary>
        /// Stops listening for advertisements
        /// </summary>
        public void StopListening()
        {
            if (!Listening) { return; }

            _watcher.Stop();
        }



        /// <summary>
        /// Listens out for watcher advertisements
        /// </summary>
        /// <param name="sender">The watcher</param>
        /// <param name="args">The arguments</param>
        private void WatcherAdvertisementReceived(BluetoothLEAdvertisementWatcher sender, BluetoothLEAdvertisementReceivedEventArgs args)
        {
            BLEDevice device = null;

            // Is new discover?
            bool newDiscovery = !_discoveredDevices.ContainsKey(args.BluetoothAddress);

            lock (_threadLock)
            {
                string name = args.Advertisement.LocalName;

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

            // If new discover...
            if (newDiscovery)
            {
                // If listening thread sleeps, this code wont continue so maybe fire in
                // in task.
                NewDeviceDiscovered(device);
            }
        }
    }
}
