#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Server.h>

#define LAMPS_PIN 6
#define LDR1_PIN A1
#define LDR2_PIN A0
#define MODE_PIN2 2
#define MODE_PIN3 3
#define TEMP_PIN A2
#define FANS_PIN 5

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int LDR1Value = 0, LDR2Value = 0;
int manualTemp = 0, manualLight = 0;
bool is_lamps_auto = true, is_fan_auto = false;
char text[15];
char input_buffer[6];

void setup() {
pinMode(LDR1_PIN, INPUT);
pinMode(LDR2_PIN, INPUT);
Serial.begin(9600);
lcd.begin(16, 2);
lcd.print("Light | Temp:");
}
