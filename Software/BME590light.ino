// pre processing directives
// PWM pins are 3, 5, 6, 9, 10, 11
#define  FLASHING_RATE 2
#define BUTTON_PIN 2
#define BATTERY_READ_PIN 1 // A1? 
#define LED_PIN 9
#define BATTERY_LED_PIN 11


// global variables
bool button = false;
int mode = 0;
int interval = 1000 / FLASHING_RATE;
unsigned long previousMillis = 0;
int ledState = LOW;
int ledStateBatt = LOW;
int lastButtonTime = 0; // unsigned long?
int debounceTime = 100; // unsigned long?
float medBatteryVolt = 4.5;
float lowBatteryVolt = 4.0;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BATTERY_READ_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BATTERY_LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), detect_button, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  detect_press();
  shine_LED();
  battery_life();
}

void detect_button(){
  if ((millis() - lastButtonTime) > debounceTime){
    button = true;
    lastButtonTime = millis();
  }
}

void detect_press() {
  if (button) {
    mode += 1;
    button = false;
  }
  if (mode == 6) {
    mode = 1;
  }
}

void shine_LED() {
  switch (mode) {
    case 1:
      analogWrite(LED_PIN, 255);
      break;
    case 2:
      analogWrite(LED_PIN, 175);
      break;
    case 3:
      analogWrite(LED_PIN, 75);
      break;
    case 4:
      analogWrite(LED_PIN, 0);
      break;
    case 5:
      // use millis for flashing
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (ledState == LOW) {
          ledState = HIGH;
        }
        else {
          ledState = LOW;
        }
      }
      digitalWrite(LED_PIN, ledState);
      break;
  }
}

void battery_life() {
  float batteryVolt = analogRead(BATTERY_READ_PIN); // is float the right data type?
  unsigned long previousMillisBatt = 0;
  
  if(batteryVolt <= medBatteryVolt && batteryVolt > lowBatteryVolt){
    // slow flashing
    if (currentMillis - previousMillisBatt >= 1000) {
        previousMillisBatt = currentMillis;
        if (ledStateBatt == LOW) {
          ledStateBatt = HIGH;
        }
        else {
          ledStateBatt = LOW;
        }
    }
    digitalWrite(BATTERY_LED_PIN, ledStateBatt);
  }
  
  if(batteryVolt <= lowBatteryVolt){
    // fast flashing
    if (currentMillis - previousMillisBatt >= interval) {
        previousMillisBatt = currentMillis;
        if (ledStateBatt == LOW) {
          ledStateBatt = HIGH;
        }
        else {
          ledStateBatt = LOW;
        }
    }
    digitalWrite(BATTERY_LED_PIN, ledStateBatt);
  }
}
