#include <Keypad.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int stat = 0;
char temp[4];

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
}
  
void loop(){
  char key = keypad.getKey();
  if (stat == 0) {
    lcd.clear();
    lcd.noBacklight();
    if (key == '*')
      stat =1;
  } else {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Enter password:");
    if (key) {
      Serial.println(key);
      temp[stat++] = key;
      if (stat == 4) {
        if (temp[1] == '1' && temp[2] == '2' && temp[3] == '3') {
          stat = 0;
        } else {
          stat = 1;
        }
      }
    }
  }
}
