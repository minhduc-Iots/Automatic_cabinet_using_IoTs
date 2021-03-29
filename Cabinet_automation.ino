#define C1 4
#define C2 3
#define C3 2
#define H1 digitalRead(8)
#define H2 digitalRead(7)
#define H3 digitalRead(6)
#define H4 digitalRead(5)
#include "NFC.h"
#include <LiquidCrystal_I2C.h>
#include <String.h>
#include <Servo.h>
Servo SV1;
LiquidCrystal_I2C Lcd(0x3f, 16, 2);
/*-------------------------------------------------------------*/
void setup() {
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  SV1.attach(A1, 600, 2300);
  SETUP_NFC();
  Lcd.init();
  Lcd.backlight();
  Serial.begin(115200);
}
unsigned long RFID;
unsigned long RFID_lock;
unsigned long RFID_data;
unsigned long Password;
unsigned long Password_data;
int k;
int s1 = 90;
/*-------------------------------------------------------------*/
void loop()
{
  int bp = BANPHIM();
  while ((RFID_data == 0) || (Password_data == 0))
  {
    SetupNewUser();
    Lcd.setCursor(0, 0);
    Lcd.print("Successful");
    delay(2000);
    Lcd.setCursor(0, 0);
    Lcd.print("Trang thai tu          ");
    Lcd.setCursor(0, 1);
    Lcd.print("Close            ");
  }
  if (Serial.available()) {
    char x = Serial.read();
    if  (x == '8')
    {
      sv1(90, 10);
      Lcd.setCursor(0, 0);
      Lcd.print("Trang thai tu          ");
      Lcd.setCursor(0, 1);
      Lcd.print("Close            ");
    } if  (x == '7')
    {
      sv1(0, 10);
      Lcd.setCursor(0, 0);
      Lcd.print("Trang thai tu          ");
      Lcd.setCursor(0, 1);
      Lcd.print("OPEN            ");
    }
    if  (x == '3')
    {
      RFID_lock = 0;
      Lcd.setCursor(0, 0);
      Lcd.print("RFID OFF         ");
    }
    if (x == '4')
    {
      RFID_lock = 1;
      Lcd.setCursor(0, 0);
      Lcd.print("RFID ON         ");
    }
    if (x == '5')
    {
      Lcd.setCursor(1, 0);
      Lcd.print("Password Off         ");
      Password = 0;
    }
    if (x == '6')
    {
      Lcd.setCursor(1, 0);
      Lcd.print("Password On         ");
    }
  }
  RFID = CHECK_NFC();
  bp = BANPHIM();
  if ((RFID != 0) || (bp == 11))
  {
    int q = 0;
    while ((RFID != 0) && (RFID_lock != 0))
    {
      if (RFID == RFID_data)
      {
        sv1(0, 10);
        Serial.write('1');
        Lcd.setCursor(0, 0);
        Lcd.print("Trang thai tu          ");
        Lcd.setCursor(0, 1);
        Lcd.print("OPEN            ");
        RFID = 0;
      }
    }
    if (RFID_lock == 0)
    {
      sv1(90, 10);
      Serial.write('0');
      Lcd.setCursor(0, 0);
      Lcd.print("Trang thai tu          ");
      Lcd.setCursor(0, 1);
      Lcd.print("RFID failed           ");
    }
    if (bp == 11)
    {
      k = 1;
      int i = 0;
      Lcd.setCursor(0, 0);
      Lcd.print("Nhap mat khau");
      while (k == 1)
      {
        int kb = BANPHIM();
        if (kb != 12) {
          Serial.println(kb);
          if (kb == 10)
          {
            Password = Password / 10;
            Lcd.setCursor(i - 1, 1);
            Lcd.print(" ");
            i--;
          }
          else {
            Password = Password * 10 + kb;
            Lcd.setCursor(i, 1);
            Lcd.print(kb);
            delay(500);
            Lcd.setCursor(i, 1);
            Lcd.print("*                     ");
            i++;
          }
        }
        if (i == 8)
        {
          k = 0;
        }
      }
      if (Password == Password_data) {
        sv1(0, 10);
        Serial.write('1');
        Lcd.setCursor(0, 0);
        Lcd.print("Trang thai tu          ");
        Lcd.setCursor(0, 1);
        Lcd.print("OPEN            ");
      }
      else
      {
        bp = 10;
      }
    }
  }
  if (bp == 10)
  {
    sv1(90, 10);
    Serial.write('0');
    Lcd.setCursor(0, 0);
    Lcd.print("Trang thai tu          ");
    Lcd.setCursor(0, 1);
    Lcd.print("CLOSE              ");
  }
}
/*-------------------------------------------------------------*/
void SetupNewUser()
{
  while ((RFID_data == 0) || (Password_data == 0))
  {
    int i = 0;
    while (RFID_data == 0)
    {
      Lcd.setCursor(0, 0);
      Lcd.print("Moi quet the...        ");
      RFID_data = CHECK_NFC();
    }
    Lcd.clear();
    while (Password_data == 0) {
      Lcd.setCursor(0, 0);
      Lcd.print("Nhap mat khau");
      Lcd.setCursor(0, 1);
      Lcd.print("Mat khau 8 ki tu");
      k = 1;
      while (k == 1)
      {
        int kb = BANPHIM();
        if (kb != 12) {
          Serial.println(kb);
          if (kb == 10)
          {
            Password = Password / 10;
            Lcd.setCursor(i - 1, 1);
            Lcd.print(" ");
            i--;
          }
          else {
            Password = Password * 10 + kb;
            Lcd.setCursor(i, 1);
            Lcd.print(kb);
            delay(500);
            Lcd.setCursor(i, 1);
            Lcd.print("*                     ");
            i++;
          }
        }
        if (i == 8)
        {
          k = 0;
        }
      }
      Password_data = Password;
      Password = 0;
    }
  }
  Lcd.clear();
}
/*-------------------------------------------------------------*/
void sv1(int goc, int dly) {
  if (goc > s1) {
    for (s1; s1 <= goc; s1++) {
      SV1.write(s1);
      delay(dly);
    }
  }
  if (goc < s1) {
    for (s1; s1 >= goc; s1--) {
      SV1.write(s1);
      delay(dly);
    }
  }
}
/*-------------------------------------------------------------*/
int BANPHIM()
{
  //dua cot len 5v
  digitalWrite(C1, 1); digitalWrite(C2, 1); digitalWrite(C3, 1);
  //dua cot 1 xuong 0v
  digitalWrite(C1, 0); digitalWrite(C2, 1); digitalWrite(C3, 1);
  //kiem tra tung hang
  if (H1 == 0) {
    while (H1 == 0) {};
    return 1;
  }
  if (H2 == 0) {
    while (H2 == 0) {};
    return 4;
  }
  if (H3 == 0) {
    while (H3 == 0) {};
    return 7;
  }
  if (H4 == 0) {
    while (H4 == 0) {};
    return 10;
  }
  //dua cot 2 xuong 0v
  digitalWrite(C1, 1); digitalWrite(C2, 0); digitalWrite(C3, 1);
  //kiem tra tung hang
  if (H1 == 0) {
    while (H1 == 0) {};
    return 2;
  }
  if (H2 == 0) {
    while (H2 == 0) {};
    return 5;
  }
  if (H3 == 0) {
    while (H3 == 0) {};
    return 8;
  }
  if (H4 == 0) {
    while (H4 == 0) {};
    return 0;
  }
  //dua cot 3 xuong 0v
  digitalWrite(C1, 1); digitalWrite(C2, 1); digitalWrite(C3, 0);
  //kiem tra tung hang
  if (H1 == 0) {
    while (H1 == 0) {};
    return 3;
  }
  if (H2 == 0) {
    while (H2 == 0) {};
    return 6;
  }
  if (H3 == 0) {
    while (H3 == 0) {};
    return 9;
  }
  if (H4 == 0) {
    while (H4 == 0) {};
    return 11;
  }
  return 12;
}
/*-------------------------------------------------------------*/
