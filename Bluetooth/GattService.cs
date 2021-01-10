using System;
using System.Collections.Generic;
using System.Text;

namespace ArduinoCar.Bluetooth
{
    /// <summary>
    /// Details about a specific GATT service.
    /// </summary>
    public class GattService
    {
        /// <summary>
        /// The human readable name for the service
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// The uniform identifier that is unique to this service
        /// </summary>
        public string UniformTypeIdentifier { get; }

        /// <summary>
        /// The 16-bit assigned number for this service.
        /// The bluetooth GATT Service UUID contains this.
        /// </summary>
        public ushort AssignedNumber { get; }

        /// <summary>
        /// The type of specification that this service is.
        /// </summary>
        public string ProfileSpecification { get; set; }



        /// <summary>
        /// Constructor
        /// </summary>
        public GattService(string name, string uniformIdentifier, ushort assignedNumber, string specification)
        {
            Name = name;
            UniformTypeIdentifier = uniformIdentifier;
            AssignedNumber = assignedNumber;
            ProfileSpecification = specification;
        }
    }
}
