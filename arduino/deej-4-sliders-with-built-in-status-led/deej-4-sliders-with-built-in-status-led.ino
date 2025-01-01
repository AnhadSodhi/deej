const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3};

int analogSliderValues[NUM_SLIDERS];

// Define the pin for the built-in LED
const int builtInLED = LED_BUILTIN;

// Variable to track last received "1"
unsigned long lastReceivedTime = 0;
const unsigned long timeout = 1000; // Timeout period in milliseconds

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  pinMode(builtInLED, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  delay(10);

  // Check if there is data available on the Serial port
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '1') {
      lastReceivedTime = millis(); // Update the last received time
    }
  }

  // Check if "1" has been received within the timeout period
  if (millis() - lastReceivedTime <= timeout) {
    // "1" received recently, turn the built-in LED ON
    digitalWrite(builtInLED, HIGH);
  } else {
    // "1" not received, turn the built-in LED OFF
    digitalWrite(builtInLED, LOW);
  }
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
