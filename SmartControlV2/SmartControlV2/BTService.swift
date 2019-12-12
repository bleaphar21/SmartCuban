//
//  BTService.swift
//
//  Created by Brantley Leaphart and Alexander Stewart

import Foundation
import CoreBluetooth

let BLEServiceUUID = CBUUID(string: "ac4f4e48-c67a-4a85-b98f-385a7bc1fe59")
let modeCharUUID = CBUUID(string: "862b4730-806e-4133-a965-b4d7dc14b68b")
let temperatureCelsiusUUID = CBUUID(string: "1101")
let redValueUUID = CBUUID(string: "2101")
let greenValueUUID = CBUUID(string: "3101")
let blueValueUUID = CBUUID(string: "4101")
let BLEServiceChangedStatusNotification = "kBLEServiceChangedStatusNotification"

class BTService: NSObject, CBPeripheralDelegate {
    var peripheral: CBPeripheral?
    var modeChar: CBCharacteristic?
    var temperatureCelsius: CBCharacteristic?
    var redValue: CBCharacteristic?
    var greenValue: CBCharacteristic?
    var blueValue: CBCharacteristic?
    
    init(initWithPeripheral peripheral: CBPeripheral) {
        super.init()
        
        self.peripheral = peripheral
        self.peripheral?.delegate = self
    }
    
    deinit {
        self.reset()
    }
    
    func startDiscoveringServices() {
        self.peripheral?.discoverServices([BLEServiceUUID])
    }
    
    func reset() {
        if peripheral != nil {
            peripheral = nil
        }
        
        // Deallocating therefore send notification
        self.sendBTServiceNotificationWithIsBluetoothConnected(false)
    }
    
    // Mark: - CBPeripheralDelegate

    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        let uuidsForBTService: [CBUUID] = [temperatureCelsiusUUID, modeCharUUID, redValueUUID, greenValueUUID, blueValueUUID]
        
        if (peripheral != self.peripheral) {
            // Wrong Peripheral
            return
        }
        
        if (error != nil) {
            return
        }
        
        if ((peripheral.services == nil) || (peripheral.services!.count == 0)) {
            // No Services
            return
        }
        
        for service in peripheral.services! {
            if service.uuid == BLEServiceUUID {
                peripheral.discoverCharacteristics(uuidsForBTService, for: service)
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        if (peripheral != self.peripheral) {
            // Wrong Peripheral
            return
        }
        
        if (error != nil) {
            return
        }
        
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                switch characteristic.uuid {
                case modeCharUUID:
                    self.modeChar = (characteristic)
                    peripheral.setNotifyValue(true, for: characteristic)
                    
                    // Send notification that Bluetooth is connected and all required characteristics are discovered
                    self.sendBTServiceNotificationWithIsBluetoothConnected(true)
                    
                case temperatureCelsiusUUID:
                    self.temperatureCelsius = (characteristic)
                    peripheral.setNotifyValue(true, for: characteristic)
                    
                    // Send notification that Bluetooth is connected and all required characteristics are discovered
                    self.sendBTServiceNotificationWithIsBluetoothConnected(true)
                    
                case redValueUUID:
                    self.redValue = (characteristic)
                    peripheral.setNotifyValue(true, for: characteristic)
                    
                    // Send notification that Bluetooth is connected and all required characteristics are discovered
                    self.sendBTServiceNotificationWithIsBluetoothConnected(true)
                
                case greenValueUUID:
                    self.greenValue = (characteristic)
                    peripheral.setNotifyValue(true, for: characteristic)
                    
                    // Send notification that Bluetooth is connected and all required characteristics are discovered
                    self.sendBTServiceNotificationWithIsBluetoothConnected(true)
                    
                case blueValueUUID:
                    self.blueValue = (characteristic)
                    peripheral.setNotifyValue(true, for: characteristic)
                    
                    // Send notification that Bluetooth is connected and all required characteristics are discovered
                    self.sendBTServiceNotificationWithIsBluetoothConnected(true)
                default:
                    print("no matching characteristic...")
                }
            }
        }
    }
    
    // Mark: - Private
    
    func writeRedValue(r: inout Int) {
        if let redValue = self.redValue {
            let data = Data(bytes: &r,
                            count: MemoryLayout.size(ofValue: r))
            self.peripheral?.writeValue(data, for: redValue, type: CBCharacteristicWriteType.withResponse)
        }
    }
    
    func sendBTServiceNotificationWithIsBluetoothConnected(_ isBluetoothConnected: Bool) {
        let connectionDetails = ["isConnected": isBluetoothConnected]
        NotificationCenter.default.post(name: Notification.Name(rawValue: BLEServiceChangedStatusNotification), object: self, userInfo: connectionDetails)
    }
    
}

