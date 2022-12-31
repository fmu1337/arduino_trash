
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
uint8_t temp_cel[8] = {B00111,B00101,B00111,B00000,B00000,B00000,B00000};
uint8_t tochki[8] = {B0, B00000, B0, B0, B0, B0, B10101};
uint8_t bukva_P[8] = {0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
uint8_t bukva_Ya[8] = {B01111, B10001, B10001, B01111, B00101, B01001, B10001};
uint8_t bukva_L[8] = {0x3, 0x7, 0x5, 0x5, 0xD, 0x9, 0x19};
uint8_t bukva_Lm[8] = {0, 0, B01111, B00101, B00101, B10101, B01001};
uint8_t bukva_Mz[8] = {0x10, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x1E};
uint8_t bukva_I[8] = {0x11, 0x13, 0x13, 0x15, 0x19, 0x19, 0x11};
uint8_t bukva_D[8] = {B01111, B00101, B00101, B01001, B10001, B11111, 0x11};
uint8_t bukva_G[8] = {B11111, B10001, B10000, B10000, B10000, B10000, B10000};
uint8_t bukva_IY[8] = {B01110, B00000, B10001, B10011, B10101, B11001, B10001};
uint8_t bukva_Z[8] = {B01110, B10001, B00001, B00010, B00001, B10001, B01110};
uint8_t bukva_ZH[8] = {B10101, B10101, B10101, B11111, B10101, B10101, B10101};
uint8_t bukva_Y[8] = {B10001, B10001, B10001, B01010, B00100, B01000, B10000};
uint8_t bukva_B[8] = {B11110, B10000, B10000, B11110, B10001, B10001, B11110};
uint8_t bukva_CH[8] = {B10001, B10001, B10001, B01111, B00001, B00001, B00001};
uint8_t bukva_IYI[8] = {B10001, B10001, B10001, B11001, B10101, B10101, B11001};
uint8_t bukva_TS[8] = {B10010, B10010, B10010, B10010, B10010, B10010, B11111, B00001};

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

byte o0[8] =
{
  B00100,
  B10001,
  B10011,
  B10101,
  B10101,
  B11001,
  B10001,
};   

LiquidCrystal_I2C lcd(0x27,16,2);      // Задаем адрес и размерность дисплея.

void setup() {
  
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, o0);
 // lcd.createChar(2, o1);
  lcd.setCursor(0,0);
  lcd.print("0");
  lcd.setCursor(1,0);
  lcd.print("0");
  lcd.setCursor(2,0);
  lcd.print("\1");
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  delay(2000);

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  lcd.createChar(1, bukva_L);
  lcd.createChar(2, bukva_ZH);
  lcd.createChar(3, bukva_P);
  lcd.createChar(4, temp_cel);
  
  String sHumi =  String(h, DEC);
  String sTemp =  String(t, DEC);

  char cHumi[2];
  char cTemp[2];
  sHumi.toCharArray(cHumi, 2);
  sTemp.toCharArray(cTemp, 2);

  char buff[32] = {'B', '\1', 'A', '\2', 'H', 'O', 'C', 'T', 'b', ':', h,'%',' ','T','E','M','\3','E','P','A','T','Y','P','A',':',t, '\4', 'C'};
  Serial.print(buff);
  lcd.setCursor(0,0);
  lcd.print(buff);
}
