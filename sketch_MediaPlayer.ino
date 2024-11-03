#include "String.h"
const int analogInPin = A0;  // Connection of the LED controller to control the 7-segment display
const int analogOutPin = 9;

int sensorValue = 0;
int outputValue = 0;

String Buffer;
int ones = 0;
int tens = 0;
int hundreds = 0;
int thousands = 0;

int number = 0;

String Protocol;
String Version = "0x01|";

void setup() {
    for (int i = 2; i < 14; i++)
      pinMode(i, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(3);
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);
}

void loop() {
   analogWrite(analogOutPin, outputValue);
   sensorValue = analogRead(analogInPin);
   outputValue = map(sensorValue, 0, 1023, 0, 255);
   int val = outputValue;

   Protocol = Version + String(val) + ";";  // Composition of the protocol string
   Serial.print(Protocol);                  // Transmission of the protocol

  if (Serial.available()) {  // Executes when Serial receives a message
   Buffer = Serial.readString(); // Reads received message
   Buffer.remove(Buffer.indexOf(':'), 1); // Removes ':' from the string
   number = Buffer.toInt();  // Converts string to integer
  }

  manage();
}

//========= Splits 'number' into ones, tens, hundreds, and thousands place ============
void manage() {
    ones = number % 10;
    tens = (number / 10) % 10;
    hundreds = (number / 100) % 10;
    thousands = (number / 1000) % 10;

    control(0, ones);
    delay(3);
    control(1, tens);
    delay(3);
    control(2, -1);
    delay(3);
    control(2, hundreds);
    delay(3);
    control(3, thousands);
    delay(3);
}

//================================ Segment Control ============================================
void control(int seg, int val) {
   switch (seg) {
      case 0: segment(3);
              display_number(val);
              break;
      case 1: segment(2);
              display_number(val);
              break;
      case 2: segment(1);
              display_number(val);
              break;
      case 3: segment(0);
              display_number(val);
              break;
    };
}

//========================= Selects the Segment to Activate ===================================
void segment(int number) {
    switch (number) {
        case 0: digitalWrite(13, HIGH);
                digitalWrite(10, LOW);
                digitalWrite(11, LOW);
                digitalWrite(12, LOW);
                break;
        case 1: digitalWrite(12, HIGH);
                digitalWrite(10, LOW);
                digitalWrite(11, LOW);
                digitalWrite(13, LOW);
                break;
        case 2: digitalWrite(11, HIGH);
                digitalWrite(10, LOW);
                digitalWrite(12, LOW);
                digitalWrite(13, LOW);
                break;
        case 3: digitalWrite(10, HIGH);
                digitalWrite(11, LOW);
                digitalWrite(12, LOW);
                digitalWrite(13, LOW);
                break;
        default: digitalWrite(13, LOW);
                 digitalWrite(10, LOW);
                 digitalWrite(11, LOW);
                 digitalWrite(12, LOW);
                 break;
    };
}

//================================ Display the Specified Number =============================
void display_number(int number) {
    switch (number) {
        case 0: seg_zero(); break;
        case 1: seg_one(); break;
        case 2: seg_two(); break;
        case 3: seg_three(); break;
        case 4: seg_four(); break;
        case 5: seg_five(); break;
        case 6: seg_six(); break;
        case 7: seg_seven(); break;
        case 8: seg_eight(); break;
        case 9: seg_nine(); break;
        default: dot(); break;
    };
}

//====================== Settings and Values for Numbers 0-9 and "." ===========================
void off() {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
}

void seg_zero() {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}

void seg_one() {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
}

void seg_two() {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}

void seg_three() {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}

void seg_four() {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
}

void seg_five() {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
}

void seg_six() {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
}

void seg_seven() {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}

void seg_eight() {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}

void seg_nine() {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}

void dot() {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
}
