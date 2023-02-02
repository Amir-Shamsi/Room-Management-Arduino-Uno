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

void setMode(){
  is_lamps_auto = ! digitalRead(MODE_PIN2);
  is_fan_auto = ! digitalRead(MODE_PIN3);
}

void runLampAutoMode(int value){
  if(0 <= value && value < 25)
    analogWrite(LAMPS_PIN, map(100, 0, 100, 0, 255));
  else if(25 <= value && value < 50)
    analogWrite(LAMPS_PIN, map(75, 0, 100, 0, 255));
  else if(50 <= value && value < 75)
    analogWrite(LAMPS_PIN, map(50, 0, 100, 0, 255));
  else if(75 <= value && value <= 100)
    analogWrite(LAMPS_PIN, map(25, 0, 100, 0, 255));
}

int getLightAVG(){
  LDR1Value = map(analogRead(LDR1_PIN) * 0.0048828125, 0, 5, 0, 100);
  LDR2Value = map(analogRead(LDR2_PIN) * 0.0048828125, 0, 5, 0, 100);
  int LDR_AVG = (LDR1Value + LDR2Value) / 2;
  return LDR_AVG;
}

void updateLCD(int LDR_AVG, int temp){
  lcd.setCursor(0,1);
  sprintf(text, "%d | %d^C", LDR_AVG, temp);
  lcd.print(text);
}

int getTemp(){
  int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(TEMP_PIN);
  temp_val = (temp_adc_val * 4.88);
  temp_val = (temp_val/10);
  return (int)temp_val;
}

void runLampManualMode(int value){
    analogWrite(LAMPS_PIN, map(value, 0, 100, 0, 255));
}

void manageLamps(int LDR_AVG, int manualLight){
  if (is_lamps_auto)
    runLampAutoMode(LDR_AVG);
  else
    runLampManualMode(manualLight);
}

void runFanAutoMode(int value){
  if(value <= 25)
    digitalWrite(FANS_PIN, 0);
  else if(30 <= value && value < 35)
    analogWrite(FANS_PIN, map(30, 0, 100, 0, 255));
  else if(35 <= value && value < 40)
    analogWrite(FANS_PIN, map(50, 0, 100, 0, 255));
  else if(40 <= value && value < 45)
    analogWrite(FANS_PIN, map(70, 0, 100, 0, 255));
  else if(45 <= value && value <= 50)
    analogWrite(FANS_PIN, map(100, 0, 100, 0, 255));
}
void runFanManualMode(int value){
    analogWrite(FANS_PIN, map(value, 0, 100, 0, 255));
}

void manageFan(int temp, int manualTemp){
  if (is_fan_auto)
    runFanAutoMode(temp);
  else
    runFanManualMode(manualTemp);
}

unsigned long last_time = 0;
void sendViaUART(int LDR_AVG, int temp){
  if ( millis() - last_time > 1000 ){
    last_time = millis();

    Serial.print("\r\nTemp:");
    Serial.print(temp);
    Serial.print(" Light:");
    Serial.print(LDR_AVG);

    Serial.print("\n\rManual Light:");
    Serial.print(manualLight);
    Serial.print("\n\rManual Temp:");
    Serial.print(manualTemp);

  }
}

int convert(String str){
	int x=0;
  int i=1;
	char ch = str[i];
	while(ch>=48 && ch <=57){
		x=x*10+(ch-48);
		i++;
		ch=str[i];
	}
	return x;
}

void receiveViaUART(){
    if(Serial.available( )>0) {
        String buffer = Serial.readString();
        if (buffer[0] == 'L') {
            manualLight = convert(buffer);
        }
        else if (buffer[0] == 'T') {
            manualTemp = convert(buffer);
        }
        else{
            // print some error message
            // preserve current valeus
        }
    }
}

void loop() {
  setMode();

  int LDR_AVG = getLightAVG();
  int temp = getTemp();
  sendViaUART(LDR_AVG, temp);
  updateLCD(LDR_AVG, temp);
  receiveViaUART();
  manageLamps(LDR_AVG, manualLight);
  manageFan(temp, manualTemp);
  
}