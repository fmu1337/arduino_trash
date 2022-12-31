
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include "DHT.h"

unsigned long time_now = 0;
unsigned long count_text = 0;

// LCD 
LiquidCrystal_I2C lcd(0x27,16,2);

// DHT
DHT dht(2, DHT11);

// DOORS
Servo myservo[2];

#define DOOR_LEFT 0
#define DOOR_RIGHT 1

#define DOOR_CLOSE 0
#define DOOR_OPEN 1

#define DOOR_CLOSED false
#define DOOR_OPENED true

#define RFID_MODE 0
//0 - read / 1 - write

int servo_coords[2][2] = 
{
  {15,95}, //DOOR LEFT
  {115,35}  //DOOR RIGHT
};

bool DoorState = DOOR_CLOSED;
unsigned long door_cooldown = 0;

// RFID

unsigned long uidDec, uidDecTemp;
#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          53          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

uint8_t temp_cel[8] = {B00111, B00101, B00111, B00000, B00000, B00000, B00000};

void setup()
{
  Serial.begin(9600);
  // DHT
  dht.begin();
  // LCD
  lcd.init();                     
  lcd.setBacklight(0);// Включаем подсветку дисплея 
  lcd.setCursor(0, 0);
  lcd.print("IT Future");
  lcd.setCursor(0, 1);
  lcd.print("Smart House");
  
  lcd.createChar(1, temp_cel);
  //DOORS
  myservo[DOOR_LEFT].attach(9);
  myservo[DOOR_RIGHT].attach(10);
  /*
  delay(500);
  myservo[DOOR_LEFT].write(servo_coords[DOOR_LEFT][DOOR_OPEN]);
  myservo[DOOR_RIGHT].write(servo_coords[DOOR_RIGHT][DOOR_OPEN]);
  delay(3000);
  */
  myservo[DOOR_LEFT].write(servo_coords[DOOR_LEFT][DOOR_CLOSE]);
  myservo[DOOR_RIGHT].write(servo_coords[DOOR_RIGHT][DOOR_CLOSE]);
  DoorState = DOOR_CLOSED;
  //delay(3000);
  // RFID
  SPI.begin();
  mfrc522.PCD_Init();
}
 
void loop()
{
  if (millis() > door_cooldown + 5000 && mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    uidDec = 0;
    bool isvalid = false;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      uidDecTemp = mfrc522.uid.uidByte[i];
      uidDec = uidDec * 256 + uidDecTemp;

      Serial.println("Card UID: ");
      Serial.println(uidDec);

      if (uidDec == 396130853 || uidDec == 3317162578 ||  uidDec == 145777220 ||  uidDec == 2816061900) // Сравниваем Uid метки, если он равен заданому то серва открывает.
      {
          isvalid = true;
          if(DoorState == DOOR_CLOSED)
          {
            tone(6, 200, 500);
            myservo[DOOR_LEFT].write(servo_coords[DOOR_LEFT][DOOR_OPEN]);
            myservo[DOOR_RIGHT].write(servo_coords[DOOR_RIGHT][DOOR_OPEN]);
            DoorState = DOOR_OPENED;
            lcd.setBacklight(255);
            time_now = millis() + 6000;
            lcd.setCursor(0, 1);
            lcd.print("Wellcome!        ");
          }
          else
          {
            tone(6, 500, 500);
            myservo[DOOR_LEFT].write(servo_coords[DOOR_LEFT][DOOR_CLOSE]);
            myservo[DOOR_RIGHT].write(servo_coords[DOOR_RIGHT][DOOR_CLOSE]);
            DoorState = DOOR_CLOSED;
            lcd.setBacklight(0);
            lcd.setCursor(0, 1);
            lcd.print("           ");
          }
      }
    }
      
    door_cooldown = millis();
  }
  if(millis() > time_now + 2000)
  {
    time_now = millis();
    switch(count_text)
    {
      case 0:
      {
        lcd.setCursor(0, 1);
        lcd.print("Smart House       ");
        count_text = 1;
        break;
      }
      case 1:
      {
        float t = dht.readTemperature();
        lcd.setCursor(0, 1);
        lcd.print("Temperature: ");
        lcd.print(int(t));
        char temp2[]={'\1', 67, '\0'};
        lcd.print(temp2);
        count_text = 2;
        break;
      }
      case 2:
      {
        float h = dht.readHumidity();
        lcd.setCursor(0, 1);
        lcd.print("Humidity: ");
        lcd.print(int(h));
        lcd.print("%    ");
        count_text = 0;
        break;
      }
    }
  }
}
