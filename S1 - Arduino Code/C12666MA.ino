// This code is inspired from the original sketch from Peter Jansen
// https://github.com/tricorderproject/arducordermini and 
// from Kurt Kiefer (Groupgets):
// https://github.com/groupgets/c12666ma/blob/master/arduino_csv_example/arduino_csv_example.ino
// See the end of the code for Copyright notice.
// 
// Nestor Sanchez version: This code adapts the STM board inputs and adds a serial interface
// to work with Python.
// See STM github enable STM board with Arduino: https://github.com/stm32duino/Arduino_Core_STM32

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


// Copyright (c) 2015, Pure Engineering
// All rights reserved.
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.

//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
