#include <XBee.h>

#define STARTUP_DELAY 1000
#define THS 10
#define TIMETOWAIT 100

int sensorPin = 18;    // select the input pin for the potentiometer
int but = 0;
int txLed = 11;
int rxLed = 12;

int sensorValue = 0;  // variable to store the value coming from the sensor
int butValue = 1;
long timePass = 0;
long shotTime = 0;
bool butPressed = false;

XBee xbee = XBee();
HardwareSerial Uart = HardwareSerial(); // For Teensy 2.0 only.

// TX
uint8_t payload[] = { 's' };

Tx16Request tx = Tx16Request(0x3D3D, payload, sizeof(payload));


// Functions
void flashLed(int pin, int times, int wait) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(but, INPUT_PULLUP);
  pinMode(txLed, OUTPUT);
  pinMode(rxLed, OUTPUT);

  //start USB serial  
  Serial.begin(9600);

  // start serial1 for xbee UART serial
  Uart.begin(58824); // Working with xbee(57600)
  xbee.setSerial(Uart);

  // Start signal
  flashLed(txLed, 1, 50);
  flashLed(rxLed, 1, 50);    
  flashLed(txLed, 1, 50);    
  
}

void loop() {
  sensorValue = analogRead(sensorPin);
  butValue = digitalRead(but);
  timePass = millis() - shotTime; 
  
//      Serial.println(sensorValue);
  if (sensorValue > THS && timePass > TIMETOWAIT){
    Serial.println("shot");
    shotTime = millis();

    // Send xbee msg
    payload[0] = 's';
    xbee.send(tx);
    flashLed(txLed, 1, 50);
    }else{
      //button
      if (!butValue && !butPressed){

        Serial.println("butPressed");    
        payload[0] = 'b';
        xbee.send(tx);
        flashLed(rxLed, 1, 50);
        butPressed = true;
      }else if(digitalRead(but)){
        butPressed = false;
      } 
    }
  
  delay(20);
  
}
