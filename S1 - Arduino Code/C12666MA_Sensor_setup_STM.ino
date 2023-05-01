// This code is a modified from the original sketch from ktasich01
// https://github.com/ktasich01/C12666MA/blob/main/c12666ma_code.ino
// This code removes the external ADC and uses the internal ADC instead. 
// This codes adapts the STM board inputs and adds a serial interface
// to work with Python.

#define SPEC_ST        PB7   //D4
#define SPEC_CLK       PB6   //D5
#define SPEC_VIDEO     PA0   //A0 

#define SPEC_CHANNELS    256
uint16_t data[SPEC_CHANNELS];
char userInput;    // 24-05 added to test, can delete if needed

  
void setup() {
  pinMode(SPEC_ST, OUTPUT);
  pinMode(SPEC_CLK, OUTPUT);
  digitalWrite(SPEC_ST, HIGH);
  digitalWrite(SPEC_CLK, HIGH);
 
  analogReadResolution(10);
//  analogReference (eAnalogReference);
  Serial.begin(9600);
 
}

  
void readSpectrometer()
{
  int delay_time = 1;     // delay per half clock (in microseconds).  This ultimately conrols the integration time.
  int idx = 0;
  int read_time = 35;      
  int intTime = 800; 
  int accumulateMode = false;
  int i;

  // Step 1: start leading clock pulses
  for (int i = 0; i < SPEC_CHANNELS; i++) {
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }

  // Step 2: Send start pulse to signal start of integration/light collection
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delay_time);

  // Step 3: Integration time -- sample for a period of time determined by the intTime parameter
  int blockTime = delay_time * 8; // 8us 
  long int numIntegrationBlocks = ((long)intTime * (long)1000) / (long)blockTime; // 800,000/8=100,000 integration blocks
  for (int i = 0; i < numIntegrationBlocks; i++) {   // 100,000 integration blocks
    // Four clocks per pixel
    // First block of 2 clocks -- measurement        05/03: ALL THIS HAPPENS IN 0.8SEC↓ (800000us)
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);

    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }


  // Step 4: Send start pulse to signal end of integration/light collection
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delay_time);

  // Step 5: Read Data 2 (this is the actual read, since the spectrometer has now sampled data)
  idx = 0;
  for (int i = 0; i < SPEC_CHANNELS; i++) {
    // Four clocks per pixel
    // First block of 2 clocks -- measurement
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);

    // Analog value is valid on low transition
    if (accumulateMode == false) {
      data[idx] = analogRead(SPEC_VIDEO);
    } else {
      data[idx] += analogRead(SPEC_VIDEO);
    }
    idx += 1;
    if (delay_time > read_time) delayMicroseconds(delay_time - read_time);   

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);

    // Second block of 2 clocks -- idle
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }

  // Step 6: trailing clock pulses
  for (int i = 0; i < SPEC_CHANNELS; i++) {
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }
}

void print_data()
{
  for (int i = 0; i < SPEC_CHANNELS; i++) 
  {
    //data[i]=data[i]*1.10;
    Serial.print(data[i]);
    Serial.print(" "); //Serial.print(',');
  }
  Serial.println("");
}

void loop() 
{  
  userInput = Serial.read();    // PYTHON INTERFACE 
  if (userInput == 'y') {       // If "y" is received 
    while (true) {              // start spectrometer readings
      readSpectrometer();
      print_data();
      delay(10); // 10 ms was the original baseline
    }  
  }
}
