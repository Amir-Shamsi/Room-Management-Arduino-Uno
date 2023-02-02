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
  sendViaUART(LDR_AVG, temp);
  updateLCD(LDR_AVG, temp);
