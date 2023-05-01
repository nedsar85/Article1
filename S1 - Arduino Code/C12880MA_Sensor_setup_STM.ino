// This code is based and modified according to:
// Groupgets, “C12880MA - Github Repository,” 2016.
// https://github.com/groupgets/c12880ma/blob/master/arduino_c12880ma_example/arduino_c12880ma_example.ino
// This codes adapts the STM board inputs and adds a serial interface
// to work with Python.

#define SPEC_ST        PB7   //D4
#define SPEC_CLK       PB6   //D5
#define SPEC_VIDEO     PA0   //A0
#define SPEC_CHANNELS  288   // New Spec Channel
#define INT_TIME 1

char userInput;    // 24-05 added to test, can delete if needed
int c;             // 24-05 added to test, can delete if needed

void setup()
{
  Serial.begin(9600); // Baud Rate set to 115200

  //Set desired pins to OUTPUT
   pinMode(SPEC_ST, OUTPUT);
  pinMode(SPEC_CLK, OUTPUT);

  digitalWrite(SPEC_CLK, HIGH); // Set SPEC_CLK High
  digitalWrite(SPEC_ST, LOW); // Set SPEC_ST Low
}

uint16_t data[SPEC_CHANNELS];

/*
 * This functions reads spectrometer data from SPEC_VIDEO
 * Look at the Timing Chart in the Datasheet for more info
 */

// ############# READING SPECTRO ###################
void readSpectrometer() {                            
  uint8_t delayTime = 1; // delay time

  // Start clock cycle and set start pulse (SPEC_ST) to signal start
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delayTime);

  //Sample for a period of time (This occurs while ST is high)
  
  for (int i = 0; i < 10; i++) {

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);
  }

  for (int i = 0; i < INT_TIME; i++) {     // (Integration TIME )
   digitalWrite(SPEC_CLK, HIGH);
   digitalWrite(SPEC_CLK, LOW);  
   }
  
  //Set SPEC_ST to low
  digitalWrite(SPEC_ST, LOW);
  
  //Sample for a period of time
  for (int i = 0; i < 87; i++) {

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);

  }

  //One more clock pulse before the actual read

  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);

  //Read from SPEC_VIDEO
  
  for (int i = 0; i < SPEC_CHANNELS; i++) {

     data[i] = analogRead(SPEC_VIDEO);

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);

  }

 //Set SPEC_ST to high
   digitalWrite(SPEC_ST, HIGH);
 
 //Sample for a small amount of time
  for (int i = 0; i < 7; i++) {

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);

  }

  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
}

/// ############### END Read SPECTRO #################
                                                          ///// 30/03/2022 NESTOR: CLEAR UNTIL THIS POINT ↑

/*
 * The function below prints out data to the terminal
 */
void printData(){
  for (int i = 0; i < SPEC_CHANNELS; i++){
    Serial.print(data[i]);
    Serial.print (" "); //for CSV matters   
    delay(INT_TIME); 
    }
  Serial.println("");
}

void loop(){
userInput = Serial.read();  // PYTHON INTERFACE 
if (userInput == 'y') {     // If "y" is received     
while (true) {              // start spectrometer readings 
  readSpectrometer();         
  printData();
  delay(1); 
}}
}


/* Check: 
// https://stackoverflow.com/questions/22583391/peak-signal-detection-in-realtime-timeseries-data*/
