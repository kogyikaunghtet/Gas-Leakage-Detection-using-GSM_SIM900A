#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
SoftwareSerial gsm (9,10);
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int fiftyFlag = 0;
int hundredFlag = 0;
int greenLed = 6, yellowLed = 7, redLed = 13, buzzer = 8;
int gasValue;
void setup() {
  Serial.begin(9600);
  gsm.begin (9600);
  lcd.begin(20, 4);
  pinMode (greenLed, OUTPUT);
  pinMode (yellowLed, OUTPUT);
  pinMode (redLed, OUTPUT);
  pinMode (buzzer, OUTPUT);
  gsm.println("AT+CNMI=2,2,0,0,0");
  delay(500);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println((char)26);
  delay(500);
}
void loop() {
  gasValue = analogRead (A0);
  lcd.setCursor (0,0);
  lcd.print ("  Gas Value = ");
  lcd.print (gasValue);
  if (gasValue < 600){
    lcd.setCursor (0,1);
    lcd.print ("    Normal Mode.    ");
    lcd.setCursor (0,2);
    lcd.print ("GSM Module is Ready.");
    digitalWrite (greenLed, HIGH);
    digitalWrite (yellowLed, LOW);
    digitalWrite (redLed, LOW);
    digitalWrite (buzzer, LOW);
  }
  if (gasValue > 600 && gasValue < 800 && fiftyFlag == 0){
    digitalWrite (greenLed, LOW);
    digitalWrite (yellowLed, HIGH);
    digitalWrite (redLed, LOW);
    digitalWrite (buzzer, HIGH);
    fiftyPercent();
    lcd.setCursor (0,1);
    lcd.print ("Gas Leakage = 50%");
    fiftyFlag = 1;
    lcd.setCursor (0,2);
    lcd.print ("Alert Message Sent !");
    delay (2000);
  }
  if (gasValue < 600 && fiftyFlag == 1){
    fiftyFlag = 0;
  }
  if (gasValue > 800 && hundredFlag == 0){
    digitalWrite (greenLed, LOW);
    digitalWrite (yellowLed, LOW);
    digitalWrite (redLed, HIGH);
    digitalWrite (buzzer, HIGH);
    hundredPercent();
    lcd.setCursor (0,1);
    lcd.print ("Gas Leakage = 100%");
    hundredFlag = 1;
    lcd.setCursor (0,2);
    lcd.print ("Alert Message Sent !");
    delay (2000);
  }
  if (gasValue < 800 && hundredFlag == 1){
    hundredFlag = 0;
  }
}

void fiftyPercent(){
  gsm.println("AT+CMGF=1");
  delay(500);
  gsm.println("AT+CMGS=\"+959123456789\"\r"); // replace your phone number
  delay(500);
  gsm.println("Warning !!!");
  delay (100);
  gsm.println ("Gas Leakage = 50%");
  delay(100);
  gsm.println((char)26);
  delay(500);
  Serial.println ("Done");
  gsm.println("AT+CMGF=1");
  delay(500);
}

void hundredPercent(){
  gsm.println("AT+CMGF=1");
  delay(500);
  gsm.println("AT+CMGS=\"+959691718453\"\r");
  delay(500);
  gsm.println("Warning !!!");
  delay (100);
  gsm.println ("Gas Leakage = 100%");
  delay(100);
  gsm.println((char)26);
  delay(500);
  Serial.println ("Done");
}
