//
//  ViewController.swift
//  SmartControl
//
//  Created by brantley leaphart and Alexander Stewart on 12/1/19.
//  Copyright © 2019 Self. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var modeVal: UILabel!
    
    @IBOutlet weak var rLabel: UILabel!
    
    @IBOutlet weak var gLabel: UILabel!
    @IBOutlet weak var bLabel: UILabel!
    @IBOutlet weak var imgBluetoothStatus: UIImageView!
    
    @IBOutlet weak var weatherLabel: UILabel!
    
    var timerTXDelay: Timer?
    var allowTX = true
    var lastPosition: UInt8 = 255
    var temp: Int = 0
    
    var redVal: Int = 0
    
    var blueVal: Int = 0
    
    var greenVal: Int = 0
    var lastTime: Date = Date();
    
    @IBAction func rtBttn(_ sender: UIButton) {
        modeVal.text = "Real Time"
        self.sendMode(modeVal.text!)
        self.getWeatherData()
        self.sendTemp()
    }

    
    @IBAction func ManualBttn(_ sender: UIButton) {
        modeVal.text = "Manual"
        self.sendMode(modeVal.text!)
    }
    
    @IBAction func ccBttn(_ sender: UIButton) {
        modeVal.text = "Color Clock"
        self.sendMode(modeVal.text!)
    }
    
    func getWeatherData(){
    let session = URLSession.shared
    let weatherURL = URL(string: "http://api.openweathermap.org/data/2.5/weather?q=Providence,us?&units=metric&APPID=aecbea292e94c35540bea8d16fe7c303")!
    let dataTask = session.dataTask(with: weatherURL) {
        (data: Data?, response: URLResponse?, error: Error?) in
        if let error = error {
            print("Error:\n\(error)")
        } else {
            if let data = data {
                let dataString = String(data: data, encoding: String.Encoding.utf8)
                print("All the weather data:\n\(dataString!)")
                if let jsonObj = try? JSONSerialization.jsonObject(with: data, options: .allowFragments) as? NSDictionary {
                    if let mainDictionary = jsonObj.value(forKey: "main") as? NSDictionary {
                        if let temperature = mainDictionary.value(forKey: "temp") {
                            DispatchQueue.main.async {
                                self.weatherLabel.text = "\(temperature)°C"
                                self.temp = (temperature as! NSNumber).intValue
                            }
                        }
                    } else {
                        print("Error: unable to find temperature in dictionary")
                    }
                } else {
                    print("Error: unable to convert json data")
                }
            } else {
                print("Error: did not receive data")
            }
        }
    }
    dataTask.resume()
    }
    
    @IBAction func pushBttn(_ sender: Any) {
        self.sendRGB()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(true)
        rLabel.text = String(redVal)
        gLabel.text = String(greenVal)
        bLabel.text = String(blueVal)
        self.getWeatherData()
        // Watch Bluetooth connection
        NotificationCenter.default.addObserver(self, selector: #selector(ViewController.connectionChanged(_:)), name: NSNotification.Name(rawValue: BLEServiceChangedStatusNotification), object: nil)
        
        // Start the Bluetooth discovery process
        let disc = btDiscoverySharedInstance
        
    }
    
    @IBAction func passBack(_ unwindSegue: UIStoryboardSegue) {
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
                        // Send current slider position
                    } else {
                        self.imgBluetoothStatus.image = UIImage(named: "Disconnect")
                    }
                }
            });
        }
    
        /*
        * This function sends the mode over from the ios code
        */
        func sendMode(_ position: String) {
            if !allowTX{
                return
            }
            
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
    
    /*
     * This function is where the code that pushes to the characteristics is called
     */
    func sendRGB() {
        
        if let bleService = btDiscoverySharedInstance.bleService {
            bleService.writeRedValue(r: &redVal)
            bleService.writeGreenValue(g: &greenVal)
            bleService.writeBlueValue(b: &blueVal)
        }
        
        allowTX = false
        if timerTXDelay == nil {
            timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
        }
        allowTX = true;
        
    }
    
    
    func sendGreen() {
        
        if let bleService = btDiscoverySharedInstance.bleService {
        }
        
        allowTX = false
        if timerTXDelay == nil {
            timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
        }
        allowTX = true;
    }
    
    func sendBlue() {
        if let bleService = btDiscoverySharedInstance.bleService {
        }
        
        allowTX = false
        if timerTXDelay == nil {
            timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
        }
        allowTX = true;
        
    }
    
    func sendTemp() {
        print(self.temp)
        if let bleService = btDiscoverySharedInstance.bleService {
            bleService.writeTemp(t: &temp)
        }
        
        allowTX = false
        if timerTXDelay == nil {
            timerTXDelay = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(ViewController.timerTXDelayElapsed), userInfo: nil, repeats: false)
        }
        allowTX = true;
        
    }
    
        @objc func timerTXDelayElapsed() {
            self.allowTX = true
            self.stopTimerTXDelay()
            
          
        }
        
        func stopTimerTXDelay() {
            if self.timerTXDelay == nil {
                return
            }
            
            timerTXDelay?.invalidate()
            self.timerTXDelay = nil
        }
        
    }

