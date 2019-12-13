//
//  RGBViewController.swift
//  SmartControlV2
//
//  Created by brantley leaphart on 12/11/19.
//  Copyright © 2019 Self. All rights reserved.
//

import UIKit

class RGBViewController: UIViewController {
    
    @IBOutlet weak var redSlider: UISlider!
    @IBOutlet weak var greenSlider: UISlider!
    @IBOutlet weak var blueSlider: UISlider!
    @IBOutlet weak var RedTF: UITextField!
    @IBOutlet weak var GreenTF: UITextField!
    @IBOutlet weak var BlueTF: UITextField!
    
    var r: Int = 0
    
    var g: Int = 0
    
    var b: Int = 0
    
    
    @IBAction func RedChanged(_ sender: Any) {
        r = Int(redSlider.value)
        RedTF.text = String(r)
        //Self.view.backgroundColor = Color
        
        self.view.backgroundColor = UIColor(red: CGFloat(r)/255, green: CGFloat(g)/255, blue: CGFloat(b)/255, alpha: 1)
    }
    @IBAction func GreenChanged(_ sender: Any) {
        g = Int(greenSlider.value)
        GreenTF.text = String(g)
        self.view.backgroundColor = UIColor(red: CGFloat(r)/255, green: CGFloat(g)/255, blue: CGFloat(b)/255, alpha: 1)
    }
    
    @IBAction func BlueChanged(_ sender: Any) {
        b = Int(blueSlider.value)
        BlueTF.text = String(b)
        self.view.backgroundColor = UIColor(red: CGFloat(r)/255, green: CGFloat(g)/255, blue: CGFloat(b)/255, alpha: 1)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        let mainVC = segue.destination as! ViewController
        mainVC.redVal = r
        mainVC.greenVal = g
        mainVC.blueVal = b
//        mainVC.sendRed()
//        mainVC.sendGreen()
//        mainVC.sendBlue()
//        mainVC.rLabel.text = RedTF.text
//        mainVC.gLabel.text = GreenTF.text
//        mainVC.bLabel.text = BlueTF.text
    }
}
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */


