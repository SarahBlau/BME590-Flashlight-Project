// pre processing directives
// PWM pins are 3, 5, 6, 9, 10, 11
#define  FLASHING_RATE 2
#define BUTTON_PIN 2
#define BATTERY_READ_PIN A1 // A1? 
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
int debounceTime = 250; // unsigned long?
float medBatteryVolt = (4.5/5)*1023;
float lowBatteryVolt = (4.0/5)*1023;
unsigned long previousMillisBatt = 0;
int now = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BATTERY_READ_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BATTERY_LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), detect_button, RISING); //when pin 2 goes from high to low do interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(BUTTON_PIN));
  debounce2();
  detect_press();
  shine_LED();
  battery_life();
}

void detect_button(){
    // NO PRINT STATEMENTS
    button = true;
}

void debounce(){
//  if(button == true){
//    Serial.print("Int!");
//    Serial.print(millis());
//  }
  now = millis();
  if ((now - lastButtonTime) < debounceTime){
    button = false;
    //Serial.print(millis() - lastButtonTime);
    //Serial.print("Debounce!");
  }
  else{
    lastButtonTime = millis();
    //Serial.print("Press!");
  }
  //Serial.print(button);
}

void debounce2(){
  if(button == true){
    now = millis();
  }
  if((now - lastButtonTime) < debounceTime){
    button = false;
  }
  else{
    lastButtonTime = millis();
  }
}

void detect_press() {
  if (button == true) {
    mode += 1;
    Serial.print(mode);
    button = false;
  }
  if (mode == 6) {
    mode = 1;
    Serial.print(mode);
  }
}

void shine_LED() {
  switch (mode) {
    case 1:
      analogWrite(LED_PIN, 255);
      break;
    case 2:
      analogWrite(LED_PIN, 150);
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
  int batteryVolt = analogRead(BATTERY_READ_PIN); 
  unsigned long currentMillisBatt = millis();
  
  if(float(batteryVolt) <= medBatteryVolt && float(batteryVolt) > lowBatteryVolt){
    // slow flashing
    if (currentMillisBatt - previousMillisBatt >= 1000) {
        previousMillisBatt = currentMillisBatt;
        if (ledStateBatt == LOW) {
          ledStateBatt = HIGH;
        }
        else {
          ledStateBatt = LOW;
        }
    }
    digitalWrite(BATTERY_LED_PIN, ledStateBatt);
  }
  
  if(float(batteryVolt) <= lowBatteryVolt){
    // fast flashing
    if (currentMillisBatt - previousMillisBatt >= interval) {
        previousMillisBatt = currentMillisBatt;
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
