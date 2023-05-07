import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv           
from get_x_values import X_values # Gives acces to get_x_values Function
from serial.tools import list_ports

####### FUNCTION DEFINITION ##########

def get_y_values():
    arduinoData = serialCom.readline().decode().strip('\r\n') 
    y_values = [int(x) for x in arduinoData.split()]

    if (len(y_values))< 256 or (len(y_values))> 256:   ### THIS is to ensure that the first reading has a size=256
         print(len(y_values))
         y_values = range(0,256,1) ### This avoids physical reconnection of the USB cable
    print (y_values)
    return y_values

###### VOID MAIN ######

###### CALLING EXTERNAL FILES  ########
#iniData = getINI()   # <- GET NUMROWS FROM EXTERNAL FILE
#numRowsCollect = int (iniData['numRowsCollect']) #Gets data from external textfile (NOTE: this is a dictionary string defined by the key '')
#numPixels = int (iniData['numPixels']) #Gets data from external textfile (NOTE: this is a dictionary string defined by the key '')
get_x_values = X_values()
x_values = (X_values()) # < - Getting x Values (Imported differently because it is a list)

# CSV creation truncate method
f = open ("intensityCounts.csv","w",newline='')
f.truncate()


#Writing headers to CSV file
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

writer = csv.writer(f, delimiter=',')
writer.writerow(headers)

#Figure setup
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
y_values=[] # THIS IS TO TRICK "animate" so it can start and then y becomes: get_y_values

# Asking user to start/Importing variables
userInput = input('Get Data points?')
if userInput == 'y'or 'Y':
    #userInput = input('How many cycles?')
    #loops = userInput
    #loops = int (loops)
    #print (loops)
      
# Clearing serial COM and reset ARDUINO
    serialCom = serial.Serial('COMx',9600) # Use your preferred COM port
    #time.sleep(4)
    serialCom.write(b'y') #Sending "y" to ARDUINO
    serialCom.flushInput()
    serialCom.setDTR(True)

                        ############# VOID LOOP ################

#for i in range(0,numRowsCollect):      #<-Activate to limit cycles

while True:
    def animate(i, x_values, y_values):           #This function iterates for each frame of the animation. We named the iteration variable i. Each i step will be a frame.
            ax1.clear() # REMOVE OR ADD TO SEE REFRESHED SCREEN OR OVERLAP
            y_values = get_y_values()
            line, = ax1.plot(x_values, y_values)
            line.set_xdata(x_values)
            line.set_ydata(y_values)

            writer.writerow(y_values) # <- Writes to Csv 
            print (y_values)  
            
            time.sleep(0.8)
            return line

    ani = animation.FuncAnimation(fig, animate, fargs = (x_values,y_values), interval=100) #animate will be called once, and will loop forever
    plt.show()
f.close()    
