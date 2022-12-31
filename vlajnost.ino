
  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

 int sensorValue;
LiquidCrystal_I2C lcd(0x27,16,2);      // Задаем адрес и размерность дисплея.

void setup () 
{
  lcd.init();
  lcd.backlight();
  pinMode (2, OUTPUT);
  Serial.begin (9600);
}
 
void loop () 
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");

  sensorValue = analogRead (A0);
  sensorValue = 1024 - sensorValue;
  
  int percent = sensorValue/10;

  
  lcd.setCursor(0,0);
  lcd.print("==");
  if (percent > 10)
  {
    lcd.setCursor(3,0);
    lcd.print("#");
  }
  if (percent > 20)
  {
    lcd.setCursor(4,0);
    lcd.print("#");
  }
  if (percent > 30)
  {
    lcd.setCursor(5,0);
    lcd.print("#");
  }
  if (percent > 40)
  {
    lcd.setCursor(6,0);
    lcd.print("#");
  }
  if (percent > 50)
  {
    lcd.setCursor(7,0);
    lcd.print("#");
  }
  if (percent > 60)
  {
    lcd.setCursor(8,0);
    lcd.print("#");
  }
  if (percent > 70)
  {
    lcd.setCursor(9,0);
    lcd.print("#");
  }
  if (percent > 80)
  {
    lcd.setCursor(10,0);
    lcd.print("#");
  }
  if (percent > 90)
  {
    lcd.setCursor(11,0);
    lcd.print("#");
  }
  if (percent > 95)
  {
    lcd.setCursor(12,0);
    lcd.print("#");
  }
  
  lcd.setCursor(14,0);
  lcd.print("==");
  Serial.println (sensorValue, DEC);
  
/*  lcd.setCursor(10,1);
  lcd.print("A:");
  lcd.print(sensorValue);
  lcd.print("%");
*/

  lcd.setCursor(2,1);

  if (percent > 99) percent = 99;
  lcd.print("Humidity:");
  if (percent < 10) 
  {
      lcd.print(" ");
  }
  lcd.print(percent);
  lcd.print("%       ");

  
  delay (500);

}
