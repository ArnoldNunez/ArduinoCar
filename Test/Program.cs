using ArduinoCar.Bluetooth;
using System;
using System.Threading;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            BLEAdvertisementWatcher watcher = new BLEAdvertisementWatcher();

            watcher.StartedListening += () =>
            {
                Console.WriteLine("Started listening");
            };

            watcher.StoppedListening += () =>
            {
                Console.WriteLine("Stopped listening");
            };

            watcher.NewDeviceDiscovered += (device) =>
            {
                Console.WriteLine($"New device: {device}");
            };

            watcher.StartListening();
            Console.ReadLine();
        }
    }
}
