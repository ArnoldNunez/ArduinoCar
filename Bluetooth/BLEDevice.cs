using System;

namespace ArduinoCar.Bluetooth
{
    /// <summary>
    /// Information about a bluetooth LE device
    /// </summary>
    public class BLEDevice
    {
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
        /// Constructor
        /// </summary>
        public BLEDevice(ulong address, string name, short rssi, DateTimeOffset broadcastTime)
        {
            Address = address;
            Name = name;
            SignalStrengthDB = rssi;
            BroadcastTime = broadcastTime;
        }



        /// <summary>
        /// User friendly ToString
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return $"{ (string.IsNullOrEmpty(Name) ? "[No Name]" : Name) } {Address} ({SignalStrengthDB})";
        }
    }
}
