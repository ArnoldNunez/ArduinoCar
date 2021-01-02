using System;

namespace ArduinoCar.Bluetooth
{
    /// <summary>
    /// Information about a bluetooth LE device
    /// </summary>
    public class BLEDevice
    {
        /// <summary>
        /// The permanent unique id of this device
        /// </summary>
        public string DeviceId { get; set; }

        /// <summary>
        /// The time of the broadcast advertisement message of the device
        /// </summary>
        public DateTimeOffset BroadcastTime { get; }

        /// <summary>
        /// The address of the device
        /// </summary>
        public ulong Address { get; }

        /// <summary>
        /// The name of the device
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// The signal strength in dB
        /// </summary>
        public short SignalStrengthDB { get; }

        /// <summary>
        /// Indicates if we are connected to this device
        /// </summary>
        public bool Connected { get; }

        /// <summary>
        /// Indicates if this device supports pairing
        /// </summary>
        public bool CanPair { get; }

        /// <summary>
        /// Indicates if we are currently paired to this device
        /// </summary>
        public bool Paired { get; set; }



        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="address">Bluetooth device address</param>
        /// <param name="name">Device name</param>
        /// <param name="rssi">Signal strength dbm</param>
        /// <param name="broadcastTime">The broadcast time of the discover</param>
        /// <param name="connected">If connected to the device</param>
        /// <param name="canPair">If we can pair to the device</param>
        /// <param name="isPaired">If we are paired to the device</param>
        /// <param name="deviceId">The unique id of the device</param>
        public BLEDevice(ulong address, string name, short rssi, DateTimeOffset broadcastTime, bool connected, bool canPair, bool paired, string deviceId)
        {
            Address = address;
            Name = name;
            SignalStrengthDB = rssi;
            BroadcastTime = broadcastTime;
            Connected = connected;
            CanPair = canPair;
            Paired = paired;
            DeviceId = deviceId;
        }



        /// <summary>
        /// User friendly ToString
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return $"{ (string.IsNullOrEmpty(Name) ? "[No Name]" : Name) } [{DeviceId}] ({SignalStrengthDB})";
        }
    }
}
