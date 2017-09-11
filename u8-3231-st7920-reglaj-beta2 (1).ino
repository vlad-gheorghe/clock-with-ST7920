
/******************************************************************************
OLEd Analog Clock using U8GLIB Library

visit https://code.google.com/p/u8glib/ for full details of the U8GLIB library and
full instructions for use.

by Chris Rouse Oct 2014 https://github.com/rydepier/Arduino-OLED-Clock


RTC DS3132
Optional Temperature Sensor dalas

Wire RTC:
  VCC +5v
  GND GND
  SDA Analog pin 4
  SCL Analog pin 5
Wire OLED:
  VCC +5v
  GND GND
  SDA Analog pin 4
  SCL Analog pin 5

  reglaj ora si data by niq_ro from http://nicuflorica.blogspot.com




******************************************************************************/

// Add libraries
  #include "U8glib.h"
  #include <SPI.h>
  #include <Wire.h>
  #include "RTClib.h"
  
// setup u8g object
//  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 
  U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7);
//

// Setup RTC
  RTC_DS1307 RTC;
  char monthString[37]= {"JanFebMarAprMayJunJulAugSepOctNovDec"};
  int  monthIndex[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
  String thisMonth = "";
  String thisTime = "";
  String thisDay="";

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 22 on the Arduino Due
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
//float maxTemperature=0;
//float minTemperature=200;
//char charMinTemperature[10];
//char charMaxTemperature[10];
char timeChar[100];
char dateChar[50];
char temperatureChar[10];
float temperature = 0;
float previousTemperature = 0;
float temperatura, temperatura0;
byte hh, mm, ss;
byte yy, ll, dd, zz;
#define meniu 9 
#define minus 10
#define plus 11
int nivel = 0; 
int hh1, mm1, zz1, dd1, ll1, yy1;   
int maxday;
boolean initial = 1;
char chartemp[3];
char daysOfTheWeek[7][12] = {"Duminica", "Luni", "Marti", "Miercuri", "Joi", "Vineri", "Simbata"};
void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  

  //u8g.setFont(u8g_font_6x10); 
  u8g.drawRFrame(0,0,128,64,3);
  u8g.setFont(u8g_font_courB10);
  //
  //***** RTC **********
  DateTime now = RTC.now();
  // display date at bottom of screen

  if (nivel == 0)
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  hh1=(now.hour());
  mm1=(now.minute());
  zz1=now.dayOfTheWeek();
  yy1=now.year();
  ll1=now.month();
  dd1=now.day();
  delay(500);
 // tft.fillScreen(ST7735_BLACK);
  //tft.fillRect(0,0,128,160,ST7735_BLACK);
  }}
  thisDay = String(now.day(), DEC) + "-"; 
  thisMonth="";
  for (int i=0; i<=2; i++){
    thisMonth += monthString[monthIndex[now.month()-1]+i];
  }  
  thisDay=thisDay + thisMonth + "-"; 
  thisDay=thisDay + String(now.year() , DEC);
  const char* newDay = (const char*) thisDay.c_str(); 
  u8g.drawStr(20,62, newDay);  u8g.setFont(u8g_font_6x13); 
  u8g.drawStr(3,50, (daysOfTheWeek[now.dayOfTheWeek()]));
  // ********************* 
  // display time in digital format
  thisTime=""; 
  u8g.setFont(u8g_font_timB24n);
//  u8g.setFont(u8g_font_courB12);
  //  u8g.setFont(u8g_font_timB18);
   // u8g.setFont(u8g_font_helvB14);
  thisTime=String(now.hour()) + ":";
  if (now.minute() < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(now.minute()) + ":";
  if (now.second() < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(now.second());
  const char* newTime = (const char*) thisTime.c_str();
  u8g.drawStr(5,39, newTime);  
  u8g.setFont(u8g_font_6x12);  u8g.setPrintPos(3,12);u8g.print("Temperatura ");
sensors.requestTemperatures(); temperature = sensors.getTempCByIndex(0); 
 float temperatureC = temperature;
  String thisTemp1 = String(temperatureC) + " C";
  u8g.drawCircle(112, 5, 2);
   // printing output as follows used less program storage space
   const char* thisTemp = (const char*) thisTemp1.c_str();
   u8g.setFont(u8g_font_7x13B);  u8g.drawStr(75,12,thisTemp);
if (nivel == 1)   // change hours
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
   delay(500);
  
  }    


dtostrf(hh1,3, 0, chartemp); 
     
      delay(50);
      
    u8g.setFont(u8g_font_5x8);
  u8g.setColorIndex(3);
  u8g.drawStr(60,50,"Set-Ora");
   u8g.drawStr(100,50,chartemp);
    
      delay(50);
      
if (digitalRead(plus) == LOW)
    {
    hh1 = hh1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    hh1 = hh1-1;
    delay(150);
    }    
if (hh1 > 23) hh1 = 0;
if (hh1 < 0) hh1 = 23;       
     
}  // end loop nivel = 1 (change the hours)

if (nivel == 2)   // change minutes
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
 
  }    


Serial.println(mm1);
dtostrf(mm1,3, 0, chartemp); 
     
      delay(50);
     
      u8g.setColorIndex(3);u8g.setFont(u8g_font_5x8);
  u8g.drawStr(60,50,"Set-Min");
   u8g.drawStr(100,50,chartemp);
      delay(50);
     
if (digitalRead(plus) == LOW)
    {
    mm1 = mm1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    mm1 = mm1-1;
    delay(150);
    }    
if (mm1 > 59) mm1 = 0;
if (mm1 < 0) mm1 = 59;       
     
}  // end loop nivel = 2 (change the minutes)

if (nivel == 3)   // change day in week
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);

  }    


dtostrf(zz1,3, 0, chartemp); 
    
      delay(50);
     
      u8g.setColorIndex(3);u8g.setFont(u8g_font_5x8);
  u8g.drawStr(60,50,"Set-Zi");
   u8g.drawStr(100,50,chartemp);
//tft.fillRect(10,80,100,18,ST7735_BLACK);      
//if (zz1 == 1) printText("1 - Monday", ST7735_RED,10,80,1);  // Temperature Static Text
//if (zz1 == 2) printText("2 - Tuesday", ST7735_RED,10,80,1);  // Temperature Static Text
//if (zz1 == 3) printText("3 - Wednesday", ST7735_RED,10,80,1);  // Temperature Static Text
//if (zz1 == 4) printText("4 - Thursday", ST7735_RED,10,80,1);  // Temperature Static Text
//if (zz1 == 5) printText("5 - Friday", ST7735_RED,10,80,1);  // Temperature Static Text
//if (zz1 == 6) printText("6 - Saturday", ST7735_RED,10,80,1);  // Temperature Static Text
//if (zz1 == 0) printText("0 - Sunday", ST7735_RED,10,80,1);  // Temperature Static Text  
    delay(50);
     
if (digitalRead(plus) == LOW)
    {
    zz1 = zz1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    zz1 = zz1-1;
    delay(150);
    }    
if (zz1 > 6) zz1 = 0;
if (zz1 < 0) zz1 = 6;       
     
}  // end loop nivel = 3 (change the day of the week)

if (nivel == 4)   // change year
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
    
  }    


dtostrf(yy1,3, 0, chartemp); 
 u8g.setColorIndex(3);u8g.setFont(u8g_font_5x8);
  u8g.drawStr(60,50,"Set-An");
   u8g.drawStr(100,50,chartemp);
    
      delay(50);
    

     
if (digitalRead(plus) == LOW)
    {
    yy1 = yy1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    yy1 = yy1-1;
    delay(150);
    }    
if (yy1 > 49) yy1 = 30;
if (yy1 < 16) yy1 = 17;       
     
}  // end loop stare = 4 (change the year)


if (nivel == 5)   // change mounth
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
 
  }    

dtostrf(ll1,3, 0, chartemp); 
 u8g.setColorIndex(3);u8g.setFont(u8g_font_5x8);
  u8g.drawStr(60,50,"Set_luna");
   u8g.drawStr(100,50,chartemp);
    
      delay(50);
    
     
if (digitalRead(plus) == LOW)
    {
    ll1 = ll1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    ll1 = ll1-1;
    delay(150);
    }    
   
if (ll1 > 12) ll1 = 1;
if (ll1 < 1) ll1 = 12;       
     
}  // end loop stare = 5 (change the day as data)

if (nivel == 6)   // change day as data
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);

  }    


dtostrf(dd1,3, 0, chartemp); 
 u8g.setColorIndex(3);u8g.setFont(u8g_font_5x8);
  u8g.drawStr(60,50,"Set_data");
   u8g.drawStr(100,50,chartemp);
   
      delay(50);
    
 
if (digitalRead(plus) == LOW)
    {
    dd1 = dd1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    dd1 = dd1-1;
    delay(150);
    }    
    
 // Dawn & Dusk controller. http://andydoz.blogspot.ro/2014_08_01_archive.html
 if (ll == 4 || ll == 5 || ll == 9 || ll == 11) { //30 days hath September, April June and November
    maxday = 30;
  }
  else {
  maxday = 31; //... all the others have 31
  }
  if (ll ==2 && yy % 4 ==0) { //... Except February alone, and that has 28 days clear, and 29 in a leap year.
    maxday = 29;
  }
  if (ll ==2 && ll % 4 !=0) {
    maxday = 28;
  }

if (dd1 > maxday) dd1 = 1;
if (dd1 < 1) dd1 = maxday;       
     
}  // end loop nivel = 6 (change the day as data)
if (nivel >=7)  // readfy to return to main loop
 {
  //setDS3231time(0, mm1, hh1, zz1, dd1, ll1, yy1);
   // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  RTC.adjust(DateTime(yy1, ll1, dd1, hh1, mm1, 0));
 
  
  nivel = 0;  

  initial = 1;
   
 }
 }



void setup(void) {
  Serial.begin(9600);
  // If you want to set the aref to something other than 5v
 // analogReference(EXTERNAL);
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
    
  }
pinMode(meniu, INPUT); 
pinMode(plus, INPUT); 
pinMode(minus, INPUT); 
digitalWrite(meniu, HIGH);  // put inputs in high state (when push is to ground -> low state)
digitalWrite(plus, HIGH);
digitalWrite(minus, HIGH);
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
}

