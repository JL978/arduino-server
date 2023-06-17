#include <arduino-timer.h>
#include <Servo.h>

// Buzzer
int buzzerPin = 9;
int buzzerLowTone = 1000;
int buzzerHighTone = 2000;

// Pusher
Servo pushServo; // create servo object to control a servo
int pushServoPin = 3;

// serial receiver
const byte numChars = 32;
char receivedChars[numChars];   
boolean newData = false;

int receivedSerialNumber = 0;            

// app states
int ON = 1;
int OFF = 0;
boolean isActive = false;

auto timer = timer_create_default();
// auto timerTask = NULL;

// config
int shockInterval = 3000;

void setup() {
    pushServo.attach(pushServoPin);
    pushServo.write(1);
    // pushButton();
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
}

void serialDataBuilder() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void processSerialData() {
    if (newData == true) {
        receivedSerialNumber = 0;             // new for this version
        receivedSerialNumber = atoi(receivedChars);   // new for this version

        newData = false;
    }
}

void playTone(int freq, int toneDelay) {
  tone(buzzerPin, freq);
  delay(toneDelay);
  noTone(buzzerPin);
}

void zapWarning() {
  playTone(buzzerLowTone, 250);
  playTone(buzzerLowTone, 250);
  playTone(buzzerHighTone, 750);
}

void pushButton() {
  pushServo.attach(pushServoPin);
  pushServo.write(1);
  pushServo.write(40);
  delay(300);
  pushServo.write(1);
  delay(300);
  pushServo.detach();
}

void zap(void *) {
  Serial.println("ZAP");
  zapWarning();
  pushButton();
  return true;
}

void loop() {
    timer.tick();
    serialDataBuilder();
    processSerialData();

    if (receivedSerialNumber == ON && isActive == false) {
      isActive = true;      
      Serial.println("Active");
      if (timer.empty()) {
        timer.every(shockInterval, zap);
      }
      // digitalWrite(LED_BUILTIN, HIGH);
    } else if (receivedSerialNumber == OFF && isActive == true) {
      isActive = false;
      Serial.println("Ded");
      // digitalWrite(LED_BUILTIN, LOW);
      if (!timer.empty()){
        timer.cancel();
      }
    }
}



