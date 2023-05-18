// // Example 1 - Receiving single characters

// char receivedChar;
int on = 1;
int off = 0;
// boolean newData = false;

// void setup() {
//     Serial.begin(9600);
//     Serial.println("<Arduino is ready>");
// }

// void loop() {
//     recvOneChar();
//     showNewData();
// }

// void recvOneChar() {
//     if (Serial.available() > 0) {
//         receivedChar = Serial.read();
//         newData = true;
//     }
// }

// void showNewData() {
//     if (newData == true) {
//       Serial.println(receivedChar);
//       if (on == receivedChar) {
//         Serial.println("LED on");
//         digitalWrite(LED_BUILTIN, HIGH);
//       } else if (off == receivedChar) {
//         Serial.println("LED off");
//         digitalWrite(LED_BUILTIN, LOW);
//       }
      
//       newData = false;
//     }
// }

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

int dataNumber = 0;             // new for this version

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvWithEndMarker();
    showNewNumber();
}

void recvWithEndMarker() {
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

void showNewNumber() {
    if (newData == true) {
        dataNumber = 0;             // new for this version
        dataNumber = atoi(receivedChars);   // new for this version
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        Serial.print("Data as Number ... ");    // new for this version
        Serial.println(dataNumber);     // new for this version

        if (on == dataNumber) {
          Serial.println("LED on");
          digitalWrite(LED_BUILTIN, HIGH);
        } else if (off == dataNumber) {
          Serial.println("LED off");
          digitalWrite(LED_BUILTIN, LOW);
        }

        newData = false;
    }
}
