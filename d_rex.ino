#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

unsigned  long preMS = 0;

#define longdelay 1000
#define shortdelay 150

#define SIZEMAP 256
#define CHANCE 7           // if 15 chance is 1\15 for spawning badblock
#define POS 3               // position of player
#define POSTGAMEDELAY 2000  // delay for gameend
#define JUMPDELAY 3         // jump in sec


uint8_t dino[8]  =  {B01110,B01110,B00100,B11111,B00100,B01010,B10001};
uint8_t block[8] =  {B11111,B11111,B11111,B11111,B11111,B11111,B11111};
uint8_t clean[8] =  {B00000,B00000,B00000,B00000,B00000,B00000,B00000};

bool bBlinkinInit;
bool bBlinkinInitState;
bool buff[SIZEMAP];
bool bInJump;

int  iFlyTime;
int  iHoldTime;
int  completed;

LiquidCrystal_I2C lcd(0x27,16,2);      // Р—Р°РґР°РµРј Р°РґСЂРµСЃ Рё СЂР°Р·РјРµСЂРЅРѕСЃС‚СЊ РґРёСЃРїР»РµСЏ.

void setup () 
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  digitalWrite(2, HIGH);
  bBlinkinInit = false;
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, dino);
  lcd.createChar(2, block);
  lcd.createChar(3, clean);
/*  lcd.setCursor(0,1);
  lcd.print("Loading... /  ");
  lcd.print("\1"); 
  delay (longdelay);
  lcd.setCursor(0,1);
  lcd.print("Loading... |  ");
  lcd.print("  "); 
  delay (longdelay);
  lcd.setCursor(0,1);
  lcd.print("Loading... /  ");
  lcd.print("\1"); 
  delay (longdelay);
  lcd.setCursor(0,1);
  lcd.print("Loading... -  ");
  lcd.print("  "); 
  delay (longdelay);  
  lcd.setCursor(0,1);
  lcd.print("Loading... /  ");
  lcd.print("\1"); 
  delay (longdelay);
  lcd.setCursor(0,1);
  lcd.print("Loading... |  ");
  lcd.print(" "); 
  delay (longdelay);  
  lcd.setCursor(0,1);
  lcd.print("Loading...    ");
  lcd.print("\1"); 
  delay (longdelay);  

  dinofun(0, false);
  delay (100);  
  dinofun(1, false);
  delay (100);
  dinofun(0, true);
  delay (100);  
  dinofun(1, true); 
  delay (100);
  */
  enableinit();
}

void generatemap()
{
  for (int localcount = 0; localcount <= SIZEMAP; localcount++)
  {
    if (random(CHANCE-1) != 0 || localcount < 11 || buff[localcount-1] == true || buff[localcount-2] == true) buff[localcount] = false;
    else buff[localcount] = true;
  }  
}

void enableinit()
{
  lcd.home();
  lcd.print("  PRESS BUTTON ");
  lcd.setCursor(0,1);
  lcd.print(" TO PLAY ");
  lcd.print("\1"); 
  lcd.print(" dREX ");
  bBlinkinInit = true;  
  generatemap();
  completed = 0;
}

void dinofun(int posx, bool arabic)
{
  if (arabic)
  {
      for (int localcount = 16; localcount >= -1; localcount--)
      {
        if (localcount != -1)
        {
          lcd.setCursor(0,posx);
          lcd.print("                ");
          lcd.setCursor(localcount,posx);
          lcd.print("\1"); 
          delay(250);  
        }
        else 
        {
          lcd.setCursor(0,posx);
          lcd.print("                ");
        }
     }
  }
  else 
  {
    for (int localcount = 0; localcount <= 16; localcount++)
    {
      lcd.setCursor(0,posx);
      lcd.print("                ");
      lcd.setCursor(localcount,posx);
      lcd.print("\1"); 
      delay(250);  
   }
 }
}

void loop () 
{
  unsigned long currentMillis = millis();
  
  bool pressed;
  if (digitalRead (2) == HIGH) pressed = false;
  else pressed = true;
    
  if (bBlinkinInit)
  {
    if (pressed) bBlinkinInit = false;  
    if (currentMillis - preMS >= longdelay)
    {
      preMS = currentMillis;
      lcd.setCursor(9,1);
      if (bBlinkinInitState)
      {
           bBlinkinInitState = false; 
           lcd.print("\1");
      }
      else 
      {
            bBlinkinInitState = true;
            lcd.print(" ");
      }
    }
  }
  else 
  {
    if (currentMillis - preMS >= shortdelay)
    {
      preMS = currentMillis;
      lcd.clear();
      
      
      if ((pressed || iFlyTime-- > 0))
      {
        lcd.setCursor(POS, 0);
        bInJump = true;
        if (iFlyTime < 0) iFlyTime = JUMPDELAY;   
      }
      else
      {
        iHoldTime--;
        bInJump = false;
        lcd.setCursor(POS, 1);
      }
      lcd.print("\1");
      
      for (int localcount = 0; localcount < 16; localcount++)
      {
        if (localcount != POS)
        {
          lcd.setCursor(localcount, 1);
          if (buff[localcount+completed])lcd.print("\2");
          else lcd.print("\3");
        }
      }
      
      int percwin = (completed*100)/SIZEMAP;
      lcd.setCursor(11,0);
      if (completed[buff+POS] && !bInJump){lcd.print("FAIL");delay(POSTGAMEDELAY);enableinit();}
      else if (percwin > 100)             {lcd.print("WIN!");delay(POSTGAMEDELAY);enableinit();}
      else  {lcd.print(percwin); lcd.print("%");}
      completed++;
    }
//    delay(100);
  }
}

