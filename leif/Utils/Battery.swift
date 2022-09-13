//
// Battery.swift
// SystemKit
//
// The MIT License
//
// Copyright (C) 2014-2017  beltex <https://github.com/beltex>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
import IOKit
import Foundation

/**
Battery statistics for OS X, read-only.
http://www.apple.com/batteries/
TODO: None of this will work for iOS as I/O Kit is a private framework there
*/
public struct Battery {
    
    //--------------------------------------------------------------------------
    // MARK: PUBLIC ENUMS
    //--------------------------------------------------------------------------
    
    
    /// Temperature units
    public enum TemperatureUnit {
        case celsius
        case fahrenheit
        case kelvin
    }
    
    
    //--------------------------------------------------------------------------
    // MARK: PRIVATE ENUMS
    //--------------------------------------------------------------------------
    
    
    /// Battery property keys. Sourced via 'ioreg -brc AppleSmartBattery'
    fileprivate enum Key: String {
        case ACPowered        = "ExternalConnected"
        case FullyCharged     = "FullyCharged"
        case IsCharging       = "IsCharging"
        case BatteryData      = "BatteryData"
    }
    
    
    //--------------------------------------------------------------------------
    // MARK: PRIVATE PROPERTIES
    //--------------------------------------------------------------------------
    
    
    /// Name of the battery IOService as seen in the IORegistry
    fileprivate static let IOSERVICE_BATTERY = "AppleSmartBattery"
    
    
    fileprivate var service: io_service_t = 0
    
    
    //--------------------------------------------------------------------------
    // MARK: PUBLIC INITIALIZERS
    //--------------------------------------------------------------------------
    
    
    public init() { }
    
    
    //--------------------------------------------------------------------------
    // MARK: PUBLIC METHODS
    //--------------------------------------------------------------------------
    
    
    /**
    Open a connection to the battery.
    
    :returns: kIOReturnSuccess on success.
    */
    @discardableResult
    public mutating func open() -> kern_return_t {
        if service != 0 {
            #if DEBUG
                print("WARNING - \(#file):\(#function) - " +
                        "\(Battery.IOSERVICE_BATTERY) connection already open")
            #endif
            return kIOReturnStillOpen
        }
        
        
        // TODO: Could there be more than one service? serveral batteries?
        service = IOServiceGetMatchingService(kIOMainPortDefault,
                  IOServiceNameMatching(Battery.IOSERVICE_BATTERY))
        
        if service == 0 {
            #if DEBUG
                print("ERROR - \(#file):\(#function) - " +
                        "\(Battery.IOSERVICE_BATTERY) service not found")
            #endif
            return kIOReturnNotFound
        }
        
        return kIOReturnSuccess
    }
    
    
    /**
    Close the connection to the battery.
    
    :returns: kIOReturnSuccess on success.
    */
    @discardableResult
    public mutating func close() -> kern_return_t {
        let result = IOObjectRelease(service)
        service    = 0     // Reset this incase open() is called again
        
        #if DEBUG
            if (result != kIOReturnSuccess) {
                print("ERROR - \(#file):\(#function) - Failed to close")
            }
        #endif
        
        return result
    }    
    
    //--------------------------------------------------------------------------
    // MARK: PUBLIC METHODS - BATTERY
    //--------------------------------------------------------------------------

    /**
    Is the machine powered by AC? Plugged into the charger.
    
    :returns: True if it is, false otherwise.
    */
    public func isACPowered() -> Bool {
        let prop = IORegistryEntryCreateCFProperty(service,
                                                   Key.ACPowered.rawValue as CFString?,
                                                   kCFAllocatorDefault, 0)
        return prop!.takeUnretainedValue() as! Bool
    }
    
    
    /**
    Is the battery charging?
    
    :returns: True if it is, false otherwise.
    */
    public func isCharging() -> Bool {
        let prop = IORegistryEntryCreateCFProperty(service,
                                                   Key.IsCharging.rawValue as CFString?,
                                                   kCFAllocatorDefault, 0)
        return prop!.takeUnretainedValue() as! Bool
    }
    
    
    /**
    Is the battery fully charged?
    
    :returns: True if it is, false otherwise.
    */
    public func isCharged() -> Bool {
        let prop = IORegistryEntryCreateCFProperty(service,
                                                   Key.FullyCharged.rawValue as CFString?,
                                                   kCFAllocatorDefault, 0)
        return prop!.takeUnretainedValue() as! Bool
    }
    
    /**
    Returns the current power from the adapter if available.
    
    :returns: a float representing watts
    */
    public func getPowerDrawInWatts() -> Float {
        let prop = IORegistryEntryCreateCFProperty(service,
                                                   Key.BatteryData.rawValue as CFString?,
                                                   kCFAllocatorDefault, 0)
        let dict = prop!.takeUnretainedValue() as? NSDictionary
        
        if dict == nil {
            return 0.0
        }

        let uint = dict!["AdapterPower"] as! UInt
        let string = String(uint, radix: 16, uppercase: false)
        let watts = UInt32(string, radix: 16)

        if watts != nil {
            return Float(bitPattern: watts!)
        } else {
            return 0.0
        }
    }
}
