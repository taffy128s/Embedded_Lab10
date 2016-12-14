#include <Arduino_FreeRTOS.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

const int greenPin = 2;
const int yellowPin = 3;
const int redPin = 4;
const int buzzerPin = 5;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
long randomNum;
long actualTime;
int state;
int buzzer;
TaskHandle_t mainHandle;

void mainTask(void *pvParameters);
void randomTask(void *pvParameters);

void setup() { 
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  state = 0;
  buzzer = 0;
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  xTaskCreate(mainTask, (const portCHAR *) "main", 128, NULL, 1, &mainHandle);
  xTaskCreate(randomTask, (const portCHAR *) "random", 128, NULL, 1, NULL);
  xTaskCreate(buzzerTask, (const portCHAR *) "buzzer", 128, NULL, 1, NULL);
}
void loop() { }

void mainTask(void *pvParameters) {
  while (1) {
    state = 1;
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    for (int i = 0; i < 4; i++) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Green Light");
      lcd.setCursor(0, 1);
      lcd.print(4 - i);
      delay(1000);
    }
    state = 2;
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);
    for (int i = 0; i < 2; i++) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yellow Light");
      lcd.setCursor(0, 1);
      lcd.print(2 - i);
      delay(1000);
    }
    state = 3;
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, HIGH);
    for (int i = 0; i < 6; i++) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Red Light"); 
      lcd.setCursor(0, 1);
      lcd.print(6 - i);
      delay(1000);
    }
  }
}

void randomTask(void *pvParameters) {
  while (1) {
    randomNum = random(0, 6);
    actualTime = (randomNum + 7) * 1000;
    Serial.println(actualTime);
    delay(actualTime);
    vTaskSuspend(mainHandle);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(yellowPin, LOW);
    buzzer = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Green Light");
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(0, 1);
      lcd.print(4 - i);
      delay(1000);
    }
    buzzer = 0;
    if (state == 1) {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(yellowPin, LOW);
    } else if (state == 2) {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(yellowPin, HIGH);
    } else if (state == 3) {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(yellowPin, LOW);
    }
    vTaskResume(mainHandle);
  }
}

void buzzerTask(void *pvParameters) {
  while (1) {
    if (buzzer) {
      for (int i = 0; i < 4; i++) {
        if(i%2 == 0) tone(buzzerPin,698);   
        else noTone(buzzerPin);
        delay(500);
      }
    }
    delay(100);
  }
}

