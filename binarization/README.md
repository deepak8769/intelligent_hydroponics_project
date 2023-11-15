This file does the binarization of the image, and then, using the simple algorithm of sliding window(With modification), we can calculate the Height of the plant.
We need to provide a reference to make it work accurately (To calibrate).
For calibration, we do the following
Wait till the LEDs on the board start to blink. 
Once started, We have 10 sec to do the referencing.
Then, put the scale at 23 cm height. 
What does it do?
It takes the photo and then calculates the Height in terms of pixel_Height; then, with the actual Height (23 cm), we can get the relation between the pixel_Height and the real_height.
After calibration, the model will keep running and capture images every 5 minutes (You can change it). And then calculate the actual Height. And push the value after each loop to the IOT platform (Blynk).

--------------Update---------------
If the light is gone in between, and the setup is not disturbed, the model will start with the previous calibration(Stored online in the IOT platform).
If we want to calibrate once again, we have to push a button on the Blynk app; it will start the model in calibration mode; then, using the above instruction, we can calibrate and move the calibration data to the IOT platform. 
We are now ready to calculate the actual Height.
---------------Benifits-------------
The above model is robust regarding camera angle and distance from the plant. (There is no Hardcoding). We have to calibrate it before starting.
The above model is also tuned to handle the Light-cut scenario and will now give false readings. 

