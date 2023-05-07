This repository contains the Supplementary info of the article DOI: 

• S1 - ARDUINO CODE TO ENABLE THE SPECTROMETERS:

○ C12666MA Arduino code notes:
This code is based and modified according to:
https://doi.org/10.1016/j.snb.2019.127537 who based the code on:
Groupgets, “C12880MA - Github Repository,” 2016.
https://github.com/groupgets/c12880ma/blob/master/arduino_c12880ma_example/arduino_c12880ma_example.ino
 
nedsar85 version: This codes adapts the STM board inputs and adds a serial interface
to work with Python.

See Spec sheet for timing charts:
https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/c12666ma_kacc1216e.pdf

Note: Visit groupgets github repositories for examples on C12880MA and C12666MA: 
https://github.com/orgs/groupgets/repositories


• S2 - PYTHON CODE TO ENABLE REFLECTANCE CURVES AND THICKNESS CALCULATION

○ 1. Code to find the intial peak calibration. (Set peak to 90% of total available counts ~950 if using a 10bit ADC)

○ 2. Code to calculate the Reference spectrum to be used for reflectance calculation

○ 3. Code to calculate sensor the Dark Noise (Turn-off your light source to take this measurement)

○ 4. Code to calulate the Reflectance Curves and Thickness calculation using IIM 



• S3 - STL Files for 3D printed device

Folder containing STL files
