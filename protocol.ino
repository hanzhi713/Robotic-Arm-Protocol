#include "ServoProtocol.h"
ServoProtocol *sp = new ServoProtocol();
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int servoPins[] = {3, 5, 6, 9, 10, 11};

// rotation limits of the servos
const int servoLimits[] = {0, 180, 0, 180, 0, 180, 0, 116, 0, 180, 0, 80};

// record server angles
int servoAngles[] = {90, 90, 90, 90, 90, 45};


// LCD2004 I2C display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Servo control
Servo servos[6];

/**
   @param a The input integer
   @return Three digit string
 * */
char* intToChar(int a) {
  static char s[3];
  s[0] = a / 100 % 10 + 48;
  s[1] = a / 10 % 10 + 48;
  s[2] = a % 10 + 48;
  return s;
}
/**
   @param a The string to print
   @param start starting column on the LCD
   @param len The length of the string
   @param line The line number on the LCD
 * */
void lcdPrint(char a[], int start, int len, int line) {
  lcd.setCursor(start, line);
  for (int i = 0; i < len; i++) {
    lcd.print(a[i]);
  }
}
/**
   update the display of the LCd
 * */
void updateLcd() {
  for (int i = 0; i < 6; i += 2) {
    int lN = i / 2 + 1;
    lcd.setCursor(0, lN);
    lcd.print(i + 1);
    lcd.print(": ");
    lcd.print(servoAngles[i]);
    lcdPrint(intToChar(servoAngles[i]), 3, 3, lN);
    lcd.setCursor(7, lN);
    lcd.print(" ");
    lcd.print(i + 2);
    lcd.print(": ");
    lcdPrint(intToChar(servoAngles[i + 1]), 11, 3, lN);
    //  Serial.print(servoAngles[0]);
    //  Serial.print(",");
    //  Serial.print(servoAngles[1]);
    //  Serial.print(",");
    //  Serial.println(servoAngles[2]);
    //  Serial.print(servoAngles[3]);
    //  Serial.print(",");
    //  Serial.print(servoAngles[4]);
    //  Serial.print(",");
    //  Serial.println(servoAngles[5]);
  }
}
/**
   @param servo The index of the servo (0 to 5)
   @param deltaDegree
 * */
void rotate(int servo, int deltaDegree) {
  if (deltaDegree > 0 && servoAngles[servo] < servoLimits[servo * 2 + 1]) {
    servoAngles[servo] += deltaDegree;
  }
  else if (deltaDegree < 0 && servoAngles[servo] > servoLimits[servo * 2]) {
    servoAngles[servo] += deltaDegree;
  }
  servos[servo].write(servoAngles[servo]);
}

void rotateAbsolute() {
  for (int i = 0; i < 6; i++) {
    servos[i].write(servoAngles[i]);
  }
  updateLcd();
}

void setup() {
  Serial.begin(19200);
  // initialize LCD
  lcd.begin();
  lcd.print("Servo status:");

  // initialize servos
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(servoAngles[i]);
  }
  updateLcd();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= 2) {
    int *degrees = sp->read();
    if (degrees != nullptr) {
      for (int i = 0; i < 6; i++) {
        if (servoLimits[2 * i] <= degrees[i] && degrees[i] < servoLimits[2 * i + 1])
          servoAngles[i] = degrees[i];
      }
      Serial.print(0xff);
      delete[] degrees;
      rotateAbsolute();
    }
  }
}
