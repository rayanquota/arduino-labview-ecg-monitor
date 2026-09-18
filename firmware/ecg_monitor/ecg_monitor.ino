unsigned long lastSampleTime = 0;
const unsigned long interval = 2000; // 2000 microseconds = 2ms = 500Hz

// --- LED CONFIGURATION ---
const int ledPin = 3;           
const int threshold = 950;      // Adjusted for your 900+ raw data readings

// --- NEW ANTI-DOUBLE-TRIGGER VARIABLES ---
unsigned long lastDebounceTime = 0;
const unsigned long lockoutPeriod = 250; // Ignore changes for 250ms after a peak
bool peakDetected = false;
// ------------------------------------------

void setup() {
  Serial.begin(115200); 
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Check if it's time for a new sample
  if (micros() - lastSampleTime >= interval) {
    lastSampleTime += interval; 

    int signal = analogRead(A0);
    Serial.println(signal); 

    // --- SMART THRESHOLD LED LOGIC ---
    if (signal >= threshold) {
      // Only turn on if we aren't locked out by a recent peak
      if (!peakDetected && (millis() - lastDebounceTime > lockoutPeriod)) {
        digitalWrite(ledPin, HIGH);   
        peakDetected = true;
        lastDebounceTime = millis();    // Reset the lockout timer
      }
    } else {
      // Only turn off if the lockout period has passed
      if (peakDetected && (millis() - lastDebounceTime > lockoutPeriod)) {
        digitalWrite(ledPin, LOW);    
        peakDetected = false;
      }
    }
    // ----------------------------------
  }
}