This repository contains the Supplementary info of the article DOI: 

• S1 - ARDUINO CODE TO ENABLE THE SPECTROMETERS:

○ C12880MA Arduino code notes:
This code is based on and modified according to: Groupgets, “C12880MA - Github Repository,” 2016.  https://github.com/groupgets/c12880ma/blob/master/arduino_c12880ma_example/arduino_c12880ma_example.ino

See Spec sheet for timing charts: 
https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/c12880ma_kacc1226e.pdf

○ C12666MA Arduino code notes:
This code is a modified from the original sketch from: tasich01, “C12666MA - Github Repository,” 2021. 
https://github.com/ktasich01/C12666MA/blob/main/c12666ma_code.ino

See Spec sheet for timing charts:
https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/c12666ma_kacc1216e.pdf


• S2 - PYTHON CODE TO ENABLE REFLECTANCE CURVES AND THICKNESS CALCULATION

○ 1. Code to find the intial peak calibration. (Set peak to 90% of total available counts ~950 if using a 10bit ADC)

○ 2. Code to calculate the Reference spectrum to be used for reflectance calculation

○ 3. Code to calculate sensor the Dark Noise (Turn-off your light source to take this measurement)

○ 4. Code to calulate the Reflectance Curves and Thickness calculation using IIM 


• S3 - STL Files for 3D printed device

Folder containing STL files
