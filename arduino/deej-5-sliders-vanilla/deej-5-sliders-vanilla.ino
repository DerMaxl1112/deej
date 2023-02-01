// Slider variables
const int NUM_SLIDERS = 6;                                        // Number of Analog inputs

const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A6, A7};   // Pins of Analog inputs

int analogSliderValues[NUM_SLIDERS];                              // Analog readout

// Switch variables
const int NUM_SWITCHES = 12;                                      // Number of Switches

const int column_pin[4] = {2, 3, 4, 5};                           // Pins of Digital Inputs
const int row_pin[3] = {12, 11, 10};                              // Pins of Digital Outputs

bool SwitchValues_2d[4][3] = {false};                             // Switch state in 2d layout (Matrix)
bool SwitchValues_1d[12] = {false};                               // Switch state in 1d layout (Single line for serial print)

// Standby variables
bool serialConnectionOpen = false;                                // true while Serial com port opened
String thempSerialReadout;                                        // Read variable for pc commands
int standbyTimeCounter = 0;                                       // Counts how long no response from pc


//--------------------------------------------------------------------------------------------------

void setup() {
  // set pins to in and output
  // Sliders
  for (int i = 0; i < NUM_SLIDERS; i++)
    pinMode(analogInputs[i], INPUT);

  // Switches
  for(int i=0; i<4; i++)
    pinMode(column_pin[i], INPUT);

  for(int i=0; i<3; i++)
    pinMode(row_pin[i], OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);                                   // Status LED

  Serial.begin(9600);                                             // Start Serial connection
}

//--------------------------------------------------------------------------------------------------

void loop() {

  if(serialConnectionOpen == true){                               // Check if serial connection open
    
    //Update everything
    updateSwitchValues();                                         // Function to get newest Digital Switch position
    updateSliderValues();                                         // Function to get newest Analog position
    formatSwitchValues();                                         // Function to format Switch states from matrix to single line

    delay(10);                                                    // just for safety
    
    sendValues();                                                 // Function to send data to the pc

    thempSerialReadout = Serial.read();                           // read what pc send
    
    if(thempSerialReadout != "99"){                               // check if pc send 'c' ('c' == "99")

      standbyTimeCounter ++;                                      // if pc hasn't sen 'c' count up

      if(standbyTimeCounter >= 100){                              // if pc hasn't send life sign in 100 cycles close connection
        serialConnectionOpen = false;                             // set connection variable to false
        standbyTimeCounter = 0;                                   // reset standbyTimeCounter
        Serial.println("closed");                                 // send close to pc
        digitalWrite(LED_BUILTIN, LOW);                           // turn off status LED
      }
    }
    else{                                                         // if pc send 'c'
      standbyTimeCounter = 0;                                     // reset standbyTimeCounter
    }
  }
  else{                                                           // if serial connection isn't opend
    thempSerialReadout = Serial.read();                           // read what pc send

    if(thempSerialReadout == "111"){                              // check if pc send 'o' ('o' == "111")
      serialConnectionOpen = true;                                // set connection variable to true
      Serial.println("opend");                                    // send opend to pc
      digitalWrite(LED_BUILTIN, HIGH);                             // turn on status LED
    }
    else{                                                         // if pc didn't send 'o'
      delay(10000);                                               // wait for 10 sec
    }
  }
}

//--------------------------------------------------------------------------------------------------

void updateSwitchValues() {
  for(int i = 0; i<3; i++) {
    digitalWrite(row_pin[i], HIGH);
    delay(1);

    for(int j = 0; j<4; j++) {
      SwitchValues_2d[j][i] = digitalRead(column_pin[j]);
    }
    digitalWrite(row_pin[i], LOW);
  }
}

//--------------------------------------------------------------------------------------------------

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

//--------------------------------------------------------------------------------------------------

void formatSwitchValues() {
  int count = 0;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 4; j++) {
      SwitchValues_1d[count] = SwitchValues_2d[j][i];
      count++;
    }
  }
}

//--------------------------------------------------------------------------------------------------

void sendValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += "s";                               // s -> Slider
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  if(NUM_SWITCHES > 0){
    builtString += String("|");
  }

  for (int i = 0; i < NUM_SWITCHES; i++) {
    builtString += "b";                               // b -> Button
    builtString += String((int)SwitchValues_1d[i]);

    if (i < NUM_SWITCHES - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}
