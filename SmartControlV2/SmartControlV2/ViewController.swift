//
//  ViewController.swift
//  SmartControl
//
//  Created by brantley leaphart on 12/1/19.
//  Copyright © 2019 Self. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var modeVal: UILabel!
    
    @IBOutlet weak var rLabel: UILabel!
    
    @IBOutlet weak var gLabel: UILabel!
    @IBOutlet weak var bLabel: UILabel!
    @IBOutlet weak var imgBluetoothStatus: UIImageView!
    
    var timerTXDelay: Timer?
    var allowTX = true
    var lastPosition: UInt8 = 255
    
    
    var redVal: Int = 0
    
    var blueVal: Int = 0
    
    var greenVal: Int = 0
    
    @IBAction func rtBttn(_ sender: UIButton) {
        modeVal.text = "Real Time"
        self.sendMode(modeVal.text!)
    }

    
    
    @IBAction func ManualBttn(_ sender: UIButton) {
        modeVal.text = "Manual"
        self.sendMode(modeVal.text!)
    }
    
    @IBAction func ccBttn(_ sender: UIButton) {
        modeVal.text = "Color Clock"
        self.sendMode(modeVal.text!)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(true)
        rLabel.text = String(redVal)
        gLabel.text = String(greenVal)
        bLabel.text = String(blueVal)
        
        // Watch Bluetooth connection
        NotificationCenter.default.addObserver(self, selector: #selector(ViewController.connectionChanged(_:)), name: NSNotification.Name(rawValue: BLEServiceChangedStatusNotification), object: nil)
        
        // Start the Bluetooth discovery process
        let disc = btDiscoverySharedInstance
        
    }
    
    @IBAction func passBack(_ unwindSegue: UIStoryboardSegue) {
        self.sendRed()
        self.sendGreen()
        self.sendBlue()
}
        
        deinit {
            NotificationCenter.default.removeObserver(self, name: NSNotification.Name(rawValue: BLEServiceChangedStatusNotification), object: nil)
        }
        
        override func viewWillDisappear(_ animated: Bool) {
            super.viewWillDisappear(animated)
            
            self.stopTimerTXDelay()
        }
        
        @objc func connectionChanged(_ notification: Notification) {
            // Connection status changed. Indicate on GUI.
            let userInfo = (notification as NSNotification).userInfo as! [String: Bool]
            
            DispatchQueue.main.async(execute: {
                // Set image based on connection status
                if let isConnected: Bool = userInfo["isConnected"] {
                    if isConnected {
                        self.imgBluetoothStatus.image = UIImage(named: "Connect")
                        //self.sendPosition()
                        // Send current slider position
                    } else {
                        self.imgBluetoothStatus.image = UIImage(named: "Disconnect")
                    }
                }
            });
        }
        
        func sendMode(_ position: String) {
            if !allowTX{
                return
            }
            
 
//            else if position == NULL{
//                return
//            }
            
            if let bleService = btDiscoverySharedInstance.bleService {
                switch(position){
                case "Real Time":
                    var x: Int = 1
                    bleService.writeMode(m: &x)
                case "Manual":
                    var x: Int = 2
                    bleService.writeMode(m: &x)
                case "Color Clock":
                    var x: Int = 3
                    bleService.writeMode(m: &x)
                default:
                    return
                }
                
            }
            
            allowTX = false
            if timerTXDelay == nil {
                timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
            }
            
        }
    
    func sendRed() {
        if !allowTX{
            return
        }
        
        
        //            else if position == NULL{
        //                return
        //            }
        
        if let bleService = btDiscoverySharedInstance.bleService {
            bleService.writeRedValue(r: &redVal)
        }
        
        allowTX = false
        if timerTXDelay == nil {
            timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
        }
        
    }
    
    
    func sendGreen() {
        if !allowTX{
            return
        }
        
        
        //            else if position == NULL{
        //                return
        //            }
        
        if let bleService = btDiscoverySharedInstance.bleService {
            bleService.writeGreenValue(g: &greenVal)
        }
        
        allowTX = false
        if timerTXDelay == nil {
            timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
        }
        
    }
    
    func sendBlue() {
        if !allowTX{
            return
        }
        
        
        //            else if position == NULL{
        //                return
        //            }
        
        if let bleService = btDiscoverySharedInstance.bleService {
            bleService.writeBlueValue(b: &blueVal)
        }
        
        allowTX = false
        if timerTXDelay == nil {
            timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
        }
        
    }
        
        @objc func timerTXDelayElapsed() {
            self.allowTX = true
            self.stopTimerTXDelay()
            
            // Send current slider position
          
        }
        
        func stopTimerTXDelay() {
            if self.timerTXDelay == nil {
                return
            }
            
            timerTXDelay?.invalidate()
            self.timerTXDelay = nil
        }
        
    }

