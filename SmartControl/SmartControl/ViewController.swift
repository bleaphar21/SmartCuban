//
//  ViewController.swift
//  SmartControl
//
//  Created by brantley leaphart on 12/1/19.
//  Copyright © 2019 Self. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var rVal: UISlider!
    @IBOutlet weak var gVal: UISlider!
    @IBOutlet weak var bVal: UISlider!
    
    @IBOutlet weak var modeVal: UILabel!
    
    @IBAction func rtBttn(_ sender: UIButton) {
        modeVal.text = "Real Time"
    }
    
    
    @IBAction func ManualBttn(_ sender: UIButton) {
        modeVal.text = "Manual"
    }
    
    @IBAction func ccBttn(_ sender: UIButton) {
        modeVal.text = "Color Clock"
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }
    
    
}

