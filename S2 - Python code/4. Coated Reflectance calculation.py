import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import csv           
from ini_read import getINI # Gives acces to getINI Function
from serial.tools import list_ports
import numpy as np
import glob
import pandas as pd
import os.path
from scipy.signal import find_peaks
import math

#### GLOBAL VARIABLES ##### 
headers = ['P1','P2','P3','P4','P5','P6','P7','P8','P9','P10','P11','P12','P13','P14','P15','P16','P17','P18','P19','P20','P21','P22','P23','P24','P25','P26','P27',
'P28','P29','P30','P31','P32','P33','P34','P35','P36','P37','P38','P39','P40','P41','P42','P43','P44','P45','P46','P47','P48','P49','P50','P51','P52','P53','P54','P55',
'P56','P57','P58','P59','P60','P61','P62','P63','P64','P65','P66','P67','P68','P69','P70','P71','P72','P73','P74','P75','P76','P77','P78','P79','P80','P81','P82','P83',
'P84','P85','P86','P87','P88','P89','P90','P91','P92','P93','P94','P95','P96','P97','P98','P99','P100','P101','P102','P103','P104','P105','P106','P107','P108','P109',
'P110','P111','P112','P113','P114','P115','P116','P117','P118','P119','P120','P121','P122','P123','P124','P125','P126','P127','P128','P129','P130','P131','P132','P133',
'P134','P135','P136','P137','P138','P139','P140','P141','P142','P143','P144','P145','P146','P147','P148','P149','P150','P151','P152','P153','P154','P155','P156','P157',
'P158','P159','P160','P161','P162','P163','P164','P165','P166','P167','P168','P169','P170','P171','P172','P173','P174','P175','P176','P177','P178','P179','P180','P181',
'P182','P183','P184','P185','P186','P187','P188','P189','P190','P191','P192','P193','P194','P195','P196','P197','P198','P199','P200','P201','P202','P203','P204','P205',
'P206','P207','P208','P209','P210','P211','P212','P213','P214','P215','P216','P217','P218','P219','P220','P221','P222','P223','P224','P225','P226','P227','P228','P229',
'P230','P231','P232','P233','P234','P235','P236','P237','P238','P239','P240','P241','P242','P243','P244','P245','P246','P247','P248','P249','P250','P251','P252','P253',
'P254','P255','P256']

##### PATHS ######
path_calib=r'DEFINE YOUR PATH\calibration_data_C12666MA.csv'
path_refData=r'DEFINE YOUR PATH\REFERENCE_DATA_AVG.csv'
path_Noise = r'DEFINE YOUR PATH\DARK_NOISE_AVG.csv'

####### FUNCTION DEFINITION ##########

def get_x_values():
    df = pd.read_csv (path_calib) #(imports data from path_calib)
    r_data = df.s1_xxxxxx       # <------------SENSOR SERIAL NUMBER--------------- MODIFY FOR EACH SENSOR!
    r_data = r_data.values.tolist()    # Convert pandas DataFrame to List of list
    xs = r_data
    return(xs)

def get_ref_data():
    df = pd.read_csv (path_refData) #(imports data from path_ref_Data)
    r_data = df.values.tolist()    # Convert pandas DataFrame to List of list
    r_data = (r_data[0])   #<- extracts list inside of a listprint(ref_data)
    r_data = np.array(r_data) #<-numpy convert LIST TO ARRAY
    return r_data

def get_darkNoise_data():
    df = pd.read_csv (path_Noise)    #(imports data from path_Noise)
    dNdata = df.values.tolist()    # Convert pandas DataFrame to List of list
    dNdata = (dNdata[0])   #<- extracts list inside of a listprint(dNoise)
    dNdata = np.array(dNdata) #<-numpy convert LIST TO ARRAY
    return dNdata

def get_y_values(dNoise_data,ref_data):

    #--- READ ARDUINO DATA
    arduinoData = serialCom.readline().decode().strip('\r\n')
    y_values = [int(x) for x in arduinoData.split()] # turns data from ard to int

         #Secure initial good reading
    if (len(y_values))< numPixels or (len(y_values))> numPixels:   # THIS is to ensure that the first reading has a correct pixel size (check getIni file)
        y_values = range(0,numPixels,1) ### This avoids physical reconnection of the USB cable
         
    #-- OPEN CSV file "f2" (CAPTURE RAW INTENSITY COUNTS)

    # Duplicated Header issue solved as follows: had to import OS.path
    with open('S1_INTENSITY_COUNTS.csv', 'a') as f2:
         file_is_empty = os.stat('S1_INTENSITY_COUNTS.csv').st_size == 0 # Check if file is empty
         writer = csv.writer(f2, lineterminator='\n')
         if file_is_empty: # If file is empty, write the headers
             writer.writerow(headers)
         writer.writerow(y_values) # if not empty, append the y_data

    #--- REFLECTANCE CALCULATION (Raw-Noise/Reference-Noise)
    y_REF_values = ((y_values - dNoise_data) / (ref_data - dNoise_data))

    #--- SMOOTHING FACTOR  
    filter_lenght = smoothing_factor
    y_REF_values = np.convolve (y_REF_values,np.ones((filter_lenght)),mode='same')   #modes: same, valid, full
    y_REF_values /= filter_lenght


    #~~~~OPEN CSV file "f1" (CAPTURE REFLECTANCE)
    with open ("S1_REFLECTANCE_data.csv","a") as f1:
        file_is_empty = os.stat("S1_REFLECTANCE_data.csv").st_size == 0 # Check if file is empty
        writer = csv.writer(f1, lineterminator='\n') #lineterminator='\n') delimiter=','
        if file_is_empty: # If file is empty, write the headers
            writer.writerow(headers)
        writer.writerow(y_REF_values) # <- Writes to Csv 
        print (y_REF_values)
   
    return (y_REF_values) # Returns the calculated reflectance Measurements as y_REF_Values


def get_thickness(len_peaks,len_peaks2,n,lambda_1,lambda_2, theta):
    if len_peaks > len_peaks2:
        deltaM = len_peaks #<- consider positive peaks
    if len_peaks2 >= len_peaks:
        deltaM = len_peaks2 #<- consider negative peaks
    #print (deltaM) #debug: activate as required

    #THICKNESS CALCULATION:
    n_sq = n**2
    sin_theta_sq = (math.sin (theta))**2
    denom1 = 2*(math.sqrt(n_sq - sin_theta_sq))
    group1 = (deltaM /denom1)

    inv_lambda1 = 1/lambda_1
    inv_lambda2 = 1/lambda_2
    group2 = (1/(inv_lambda2 - inv_lambda1))
    thickness = abs(group1*group2)

    #~~~~OPEN CSV file "f3" (CAPTURE THICKNESS)
    th = []  #<- saving the thickness in a list to enable csv writing:
    th.append(thickness)
    header=['S1_Thickness']
    with open('S1_THICKNESS_data.csv', 'a') as f3:
         file_is_empty = os.stat('S1_THICKNESS_data.csv').st_size == 0 # Check if file is empty
         writer = csv.writer(f3, lineterminator='\n')
         if file_is_empty: # If file is empty, write the header
            writer.writerow(header)
            print (header)
         writer.writerow(th)
              
    return (thickness)
 

###### PROGRAM START  ##########

##### ( VOID MAIN ) ######

x_values = get_x_values()                                                                    #<- GET X VALUES (Imported)
ref_data = get_ref_data()                                                                    #<- GETS REFERENCE DATA (Imported)
dNoise_data = get_darkNoise_data()                                                           #<- GETS DARK NOISE DATA (Imported)


## Asking user to start/Importing variables
userInput = input('Get Data points?')
if userInput == 'y'or 'Y':
    # Get intialization data from external file getINI()
    iniData = getINI()   
    n = float (iniData['n']) #Gets refractive index of coating
    lambda_1 = int (iniData['lambda_1']) #Gets start of spectrum
    lambda_2 = int (iniData['lambda_2']) #Gets end of spectrum
    theta = int (iniData['theta']) #Gets incident angle
    smoothing_factor = int (iniData['smoothing_factor']) #Gets smoothing factor
    numPixels = int (iniData['numPixels']) #Gets data from external textfile (NOTE: this is a dictionary string defined by the key '')
    
# Clearing serial COM and reset ARDUINO
    serialCom = serial.Serial('COMX',9600) # Use your preferred COM PORT
    #time.sleep(4)
    serialCom.flushInput()
    serialCom.write(b'y') #Sending "y" to ARDUINO
    serialCom.setDTR(True)

#Figure animate PRE-setup
    fig = plt.figure()
    ax1 = fig.add_subplot(1,1,1)
    y_REF_values=[] # THIS IS TO TRICK "animate" so it can start and then y becomes: get_y_values!


######## VOID LOOP ###########

while True:
    def animate(i, x_values, y_REF_values):#This function iterates for each frame of the animation. We named the iteration variable i. Each i step will be a frame.           
            # Sub-Main (setup)
            ax1.clear() # REMOVE OR ADD TO SEE REFRESHED SCREEN OR OVERLAP
            y_REF_values = np.array(y_REF_values) #<-numpy convert Y LIST TO ARRAY

#~~~~~~~~~~~X: GET X_VALUES
            x_values = np.array(x_values) #<-numpy convert X LIST TO ARRAY             
            # Limiting x_values
            x_values = x_values[57:170]  #[57:170] -> 450:700nm

#~~~~~~~~~~~Y: GET RAW INTENSITY COUNTS (GET_Y_VALUES FUNCTION)
            y_REF_values = get_y_values(dNoise_data,ref_data)                                            #<- get_y_values FUNCTION
            # Limiting r_REF_values
            y_REF_values = y_REF_values[57:170]   #[57:170] -> 450:700nm


############ PLOT OPTIONS ##############
                
#~~~~~~~~~~~ CALCULATING PEAKS
            #FIND (+)PEAKS (had to convert y and x to array so the following can work
            peaks,_= find_peaks(y_REF_values, prominence=0.15) # <- This gets the POSITIVE index of the peak!
            
            plt.plot(x_values, y_REF_values)     #(Prominence finds the most significant peaks!!!!
            plt.plot(x_values[peaks], y_REF_values[peaks], 'v')   # SOLVED!!!!finds the peak position in x and plot the peak value in y

            #FIND (-) PEAKS 
            peaks2,_= find_peaks(-y_REF_values, prominence=0.15) # <- This gets the NEGATIVE index of the peak!
            plt.plot(x_values[peaks2], y_REF_values[peaks2], ".")  #Plot Negative 

#~~~~~~~~~~~#CALL GET_THICKNESS FUNCTION (Stores thickness values to CSV file)
            len_peaks = len (peaks)
            len_peaks2 = len (peaks2)
            thickness = get_thickness(len_peaks,len_peaks2,n,lambda_1,lambda_2,theta)                    #<- get_thickness() FUNCTION

#~~~~~~~~~~~ANIMATE LINE SET DEFINITION            
            line, = ax1.plot(x_values, y_REF_values)
            line.set_xdata(x_values)
            line.set_ydata(y_REF_values)
           
#~~~~~~~~~~~Plot
            ax1.set_xlim([450, 700]) # <- MODIFY AS REQUIRED
            #ax1.set_ylim([0, 1])
            ax1.set_title("S1_thickness(nm)={:.2f}".format(thickness)) #Live Title
            ax1.set_xlabel('Wavelenght (nm)')
            ax1.set_ylabel('Reflectance')

            time.sleep(0.2)
            return line

    ani = animation.FuncAnimation(fig, animate, fargs = (x_values,y_REF_values), interval=100) #animate will be called once, and will loop forever
    plt.show()
