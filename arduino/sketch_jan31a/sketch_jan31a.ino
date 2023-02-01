String thempSerialReadout;                                        // Read variable for pc commands

void setup() {
  // set pins to in and output

  pinMode(LED_BUILTIN, OUTPUT);                                   // Status LED

  Serial.begin(9600);                                             // Start Serial connection
}

void loop() {

    thempSerialReadout = Serial.read();                           // read what pc send
    if(thempSerialReadout == "111"){
      digitalWrite(LED_BUILTIN, HIGH); 
    }
    //if(thempSerialReadout == "99"){
    //  digitalWrite(LED_BUILTIN, LOW); 
    //}
}
