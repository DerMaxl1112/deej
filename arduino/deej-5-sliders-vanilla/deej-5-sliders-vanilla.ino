// Slider variables
const int NUM_SLIDERS = 6;

const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A6, A7};

int analogSliderValues[NUM_SLIDERS];

// Switch variables
const int NUM_SWITCHES = 12;            // Number of Switches

const int column_pin[4] = {2, 3, 4, 5};       // readout pins           |
const int row_pin[3] = {12, 11, 10};          // digital power pins     -

bool SwitchValues_2d[4][3] = {false};   // Switch state in 2d layout
bool SwitchValues_1d[12] = {false};     // Switch state in 1d layout

//--------------------------------------------------------------------------------------------------

void setup() {
  // set pins to in and output
  // Sliders
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  // Switches
  for(int i=0; i<4; i++)
    pinMode(column_pin[i], INPUT);

  for(int i=0; i<3; i++)
    pinMode(row_pin[i], OUTPUT);

    Serial.begin(9600);
}

//--------------------------------------------------------------------------------------------------

void loop() {
  //Update everything
  updateSwitchValues();
  updateSliderValues();
  formatSwitchValues(); 

  sendValues(); // Actually send data (all the time)
  delay(10);
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
