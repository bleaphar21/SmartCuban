# SmartCuban

# Created by: Brantley Leaphart and Alexander Stewart

# Background Info: As a part of a real time and embedded systems course at Brown University, Alex and Brantley decided to explore their passion for technology and fashion through the creation of an adaptive necklace. As we enter into a decade, we are seeing this become an increasingly popular trend amongst designers like Virgil Abiloh of Louis Vuitton and Off-White. 

# Design process: After coming up with the idea, iterations of the design began. Starting with gray mockups, colored digital drawings, wire frames, and then fabrics. 

# Materials: Acryllic (laser cut into links using the illustrator file), wires, an arduino nano 33 ble, an arduino uno, breadboard, and power supply

# Codebase: The project is designed using an arduino nano 33 ble sense and arduino uno all controlled through an IOS app. 

# How to run: The app has several modes which are able to alternate the functionality of the necklace. Hitting manual will take the user to a separate view controller in which they are able to use an rgb slider and preview their color in app before hitting confirm and being taken back to the initial screen where the values from the slider will be held. On this screen, hit push changes to send over the information from the app over to the periphery of the nano which is serving as a master in slave-master I2C communication. 
