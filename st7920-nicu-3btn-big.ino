/*
  http://bentommye.blogspot.com/2013/10/analog-lcd-clock-arduino-lcd.html
  // Dawn & Dusk controller. http://andydoz.blogspot.ro/2014_08_01_archive.html
  // 16th August 2014 - (C) A.G.Doswell 2014
*/

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 22 on the Arduino Due
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
//#include <Encoder.h> // from http://www.pjrc.com/teensy/td_libs_Encoder.html
//Encoder knob(10, 11); //encoder connected to pins 2 and 3 (and ground)
//#define meniu 9 
#define minus 11
#define plus 10
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif
RTC_DS1307 rtc;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeek[7][12] = {" Duminica", " Luni", " Marti", " Miercuri", "   Joi", " Vineri", " Sambata"};

#include "U8glib.h"
// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device
//  U8GLIB_ST7920_128X64_1X u8g(13, 11, 10); // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10
// se orginal code for your LCD!!!!!!
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 13, 2, 3, A5, 12);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
             //U8GLIB_ST7920_128X64_1X u8g(7, 6, 5 ,8); //Enable, RW, RS, RESET

U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7);
int X = 0;
int Y = 0;
int I = 0;
int X2 = 0;
int Y2 = 0;
int X3 = 0;
int Y3 = 0;
int Timer = 0;
int Minutt = 0;
int Sekund = 0;
float Vinkel = 0;
int an = 0;
int luna = 0;
int zi = 0;
int zis = 0;

int ScreenWith = 128;
int ScreenWithC = 64;
int ScreenHeight = 64;
int ScreenHeightC = 32;
int yArray[128];
int deics = ScreenWith/4;
int deics2 = 0;

//the variables provide the holding values for the set clock routine
int setyeartemp; 
int setmonthtemp;
int setdaytemp;
int setoretemp;
int setzitemp;
int setminstemp;
int setsecs = 0;
int maxday; // maximum number of days in the given month
int TimeMins; // number of seconds since midnight
int TimerMode = 2; //mode 0=Off 1=On 2=Auto
int TimeOut = 10;
int TimeOutCounter;

// These variables are for the push button routine
int buttonstate = 0; //flag to see if the button has been pressed, used internal on the subroutine only
int pushlengthset = 1500; // value for a long push in mS
int pushlength = pushlengthset; // set default pushlength
int pushstart = 0;// sets default push value for the button going low
int pushstop = 0;// sets the default value for when the button goes back high

int knobval; // value for the rotation of the knob
boolean buttonflag = false; // default value for the button flag
#define buton 9
//char s[3] = "";
char temperatureChar[10];
float temperature = 0;
float previousTemperature = 0;
float temperatura, temperatura0;
#include <stdlib.h>
char tmp_string[8];

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here 
 // u8g.drawRFrame(0,0,128,64,3);
 u8g.drawRFrame(0,0,128,64,3);
 u8g.drawRFrame(2,2,124,28,2);
 u8g.drawRFrame(2,31,55,15,2);
  u8g.drawRFrame(58,31,68,15,2);
 // u8g.drawRFrame(58,31,68,15,2);
 //u8g.drawCircle(ScreenWithC-deics,ScreenHeightC,31);
//  u8g.drawCircle(10,ScreenHeightC,52);
 // u8g.setFont(u8g_font_6x10);
 // u8g.setFont(u8g_font_5x7);
  //u8g.setFont(u8g_font_osb21); 
 //   TegnKlokkeBg();
   DateTime now = rtc.now();
 //   TegnViser(Timer-1, 12.0, 15); // Omdreinig, Omdreiningstall / omdreining, radius 
  //  TegnViser(Minutt-5, 60.0, 24); // Omdreinig, Omdreiningstall / omdreining, radius  
   // TegnViser(Sekund, 60-deics, 27); // Omdreinig, Omdreiningstall / omdreining, radius
  //  TegnViser(Sekund-6, 60.0, 27);
 u8g.setPrintPos(2*deics + deics2-1, 42); u8g.setFont(u8g_font_6x10);
if (zi < 10) u8g.print("0");
u8g.print(zi);
u8g.print("-");
if (luna < 10) u8g.print("0");
u8g.print(luna);
u8g.print("-");
//u8g.print(an-2000);
u8g.print(an);

// u8g.setFont(u8g_font_6x10);
//u8g.setPrintPos(1,60);u8g.print("Temperatura");
sensors.requestTemperatures(); temperature = sensors.getTempCByIndex(0); 
 float temperatureC = temperature;
  String thisTemp1 = String(temperatureC) + " C";
  u8g.drawCircle(86, 52, 2);
   // printing output as follows used less program storage space
   u8g.setFont(u8g_font_9x18B);
  const char* thisTemp = (const char*) thisTemp1.c_str();
   //u8g.setFont(u8g_font_fur17); 
   u8g.drawStr(36,60,thisTemp);

   
u8g.setPrintPos(4, 42);
//u8g.setScale2x2();
u8g.setFont(u8g_font_6x10);
//u8g.setFont(u8g_font_timB24n);
u8g.print(daysOfTheWeek[zis]);
//u8g.undoScale();

//u8g.setPrintPos(2*deics + deics2, 10); 
u8g.setPrintPos(4, 26); 
//u8g.setFont(u8g_font_fur17);
//u8g.setFont(u8g_font_9x18B);
u8g.setFont(u8g_font_fub20n);
//u8g.setFont(u8g_font_timB24n);
if (Timer < 10) u8g.print(" ");
u8g.print(Timer);
u8g.print(":");
if (Minutt < 10) u8g.print("0");
u8g.print(Minutt);
u8g.print(":");
if (Sekund < 10) u8g.print("0");
u8g.print(Sekund);
}

/*void TegnViser(float Omdreining, float forhold, int Radius) {
    Vinkel = Omdreining * 2.0 * 3.1415 / forhold - 1,5707; // 12 timer blir til 2Pi  
    X2 = ScreenWithC -deics + Radius * cos(Vinkel);
    Y2 = ScreenHeightC + Radius * sin(Vinkel);
    u8g.drawLine(ScreenWithC-deics, ScreenHeightC, X2, Y2);
}
*/
/*
void TegnKlokkeBg() { // draw clock background
    //u8g.drawCircle(ScreenWithC, ScreenHeightC, 30);
    u8g.drawCircle(ScreenWithC-deics, ScreenHeightC, 1);
  
    u8g.setFont(u8g_font_unifont);
    u8g.setFontPosTop();
    u8g.drawStr(55-deics, 4, "12");
    u8g.drawStr(83-deics, 25, "3");
    u8g.drawStr(60-deics, 45, "6");
    u8g.drawStr(39-deics, 25, "9");
  
    for(int TimeStrek = 0; TimeStrek<12; TimeStrek++) { // draw time lines
      Vinkel = TimeStrek / 12.0 * 2 * 3.1415;
      X2 = ScreenWithC-deics + 30 * cos(Vinkel);
      Y2 = ScreenHeightC + 30 * sin(Vinkel);
      X3 = ScreenWithC-deics + 28 * cos(Vinkel);
      Y3 = ScreenHeightC + 28 * sin(Vinkel);
      u8g.drawLine(X2, Y2, X3, Y3);
    }

}
*/

void setup(void) {

  // flip screen, if required
  // u8g.setRot180();

  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 )
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

pinMode(buton,INPUT);//push button on encoder connected to A0 (and GND)
digitalWrite(buton,HIGH); //Pull button high
pinMode(plus, INPUT); 
pinMode(minus, INPUT); 
digitalWrite(plus, HIGH);
digitalWrite(minus, HIGH);
}

void loop(void) {

 DateTime now = rtc.now();
 Timer = now.hour(), DEC;
 Minutt = now.minute(), DEC;
 Sekund = now.second(), DEC;
    Serial.print(Timer);
    Serial.print(':');
    Serial.print(Minutt);
    Serial.print(':');
    Serial.print(Sekund);
    Serial.print(" - ");
    an = now.year(), DEC;
    Serial.print(an);
    Serial.print('/');
    luna = now.month(), DEC;
    Serial.print(luna);
    Serial.print('/');
    zi = now.day(), DEC;
    Serial.print(zi);
    Serial.print(" (");
   zis = now.dayOfTheWeek();
   Serial.print(daysOfTheWeek[zis]);
    Serial.println(") ");
  
  // picture loop
  u8g.firstPage(); 
  do {
    draw();
  } while( u8g.nextPage() );
  // rebuild the picture after some delay
  delay(100);

 pushlength = pushlengthset;
 pushlength = getpushlength();
 delay (10);
    
 if (pushlength < pushlengthset) 
 {
  //ShortPush ();   
   Serial.println("short push");
    // picture loop
  u8g.firstPage(); 
  do {
    draw1();
  } while( u8g.nextPage() );
  // rebuild the picture after some delay
  delay(1000);

 }
  //This runs the setclock routine if the knob is pushed for a long time
       if (pushlength > pushlengthset) {
  //       lcd.clear();
  Serial.println("long push");
         DateTime now = rtc.now();
         setyeartemp=now.year(),DEC;
         setmonthtemp=now.month(),DEC;
         setdaytemp=now.day(),DEC;
         setoretemp=now.hour(),DEC;
         setminstemp=now.minute(),DEC;
         setzitemp = now.dayOfTheWeek();
         setclock();
         pushlength = pushlengthset;
       };
//}

  
} // end main loop


// subroutine to return the length of the button push.
int getpushlength() 
{
  buttonstate = digitalRead(buton);  
       if(buttonstate == LOW && buttonflag==false) {     
              pushstart = millis();
              buttonflag = true;
          };     
       if (buttonstate == HIGH && buttonflag==true) {
         pushstop = millis ();
         pushlength = pushstop - pushstart;
         buttonflag = false;
       };
        Serial.println("_");
       return pushlength;
}

void draw1(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 10, 32, "Thx to niq_ro!");
}


//sets the clock
void setclock (){
   u8g.setFont(u8g_font_unifont);
   setyear ();
   setmonth ();
   setday ();
   setzi();
   setore ();
   setmins (); 
   rtc.adjust(DateTime(setyeartemp,setmonthtemp,setdaytemp,setoretemp,setminstemp,setsecs));
   delay (1000); 
}

// The following subroutines set the individual clock parameters
int setyear () {
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setyeartemp;
    }
   /* knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) { //bit of software de-bounce
      knobval = -1;
      delay (50);
    }
    if (knobval > 1) {
      knobval = 1;
      delay (50);
    }
    setyeartemp=setyeartemp + knobval;
    */
    if (digitalRead(plus)==LOW)
    {
      setyeartemp=setyeartemp + 1;
    delay(50);
    }
    if (digitalRead(minus)==LOW)
    {
      setyeartemp=setyeartemp - 1;
    delay(50);
    }
    
    if (setyeartemp < 2018) { //Year can't be older than currently, it's not a time machine.
      setyeartemp = 2018;
    }
//itoa(setyeartemp - 2000, tmp_string, 10); // https://www.avrfreaks.net/forum/how-do-i-print-variable-u8glib
itoa(setyeartemp, tmp_string, 10); // https://www.avrfreaks.net/forum/how-do-i-print-variable-u8glib
  u8g.firstPage(); 
  do {
  u8g.drawStr( 0, 20, "Set Year");
//  u8g.drawStr( 0, 40, "20"); 
  u8g.drawStr(25, 40, tmp_string);
  } while( u8g.nextPage() );  
    setyear();
}

int setmonth () {
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setmonthtemp;
    }

 //   lcd.setCursor (0,1);
/*    knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    setmonthtemp=setmonthtemp + knobval;
    if (setmonthtemp < 1) {// month must be between 1 and 12
      setmonthtemp = 1;
    }
    */
     if (digitalRead(plus)==LOW)
    {
      setmonthtemp=setmonthtemp + 1;
    delay(50);
    }
    if (digitalRead(minus)==LOW)
    {
      setmonthtemp=setmonthtemp - 1;
    delay(50);
    }
    if (setmonthtemp > 12) {
      setmonthtemp=12;
    }
    itoa(setmonthtemp, tmp_string, 10);
    u8g.firstPage(); 
  do {
  u8g.drawStr( 0, 20, "Set Month");
  u8g.drawStr(25, 40, tmp_string);
  } while( u8g.nextPage() ); 
    setmonth();
}


int setday () {
  if (setmonthtemp == 4 || setmonthtemp == 5 || setmonthtemp == 9 || setmonthtemp == 11) { //30 days hath September, April June and November
    maxday = 30;
  }
  else {
  maxday = 31; //... all the others have 31
  }
  if (setmonthtemp ==2 && setyeartemp % 4 ==0) { //... Except February alone, and that has 28 days clear, and 29 in a leap year.
    maxday = 29;
  }
  if (setmonthtemp ==2 && setyeartemp % 4 !=0) {
    maxday = 28;
  }
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setdaytemp;
    }
 /*   knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    setdaytemp = setdaytemp+ knobval;
    if (setdaytemp < 1) {
      setdaytemp = 1;
    }
    */
     if (digitalRead(plus)==LOW)
    {
      setdaytemp=setdaytemp + 1;
    delay(50);
    }
    if (digitalRead(minus)==LOW)
    {
      setdaytemp=setdaytemp - 1;
    delay(50);
    }
    if (setdaytemp > maxday) {
      setdaytemp = maxday;
    }
  itoa(setdaytemp, tmp_string, 10);  
    u8g.firstPage(); 
  do {
  u8g.drawStr( 0, 20, "Set Day");
u8g.drawStr(25, 40, tmp_string);
  } while( u8g.nextPage() ); 
    setday();
}


int setzi () {
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setzitemp;
    }
 /*   knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    setzitemp=setzitemp + knobval;
    */
    if (digitalRead(plus)==LOW)
    {
      setzitemp=setzitemp + 1;
    delay(50);
    }
    if (digitalRead(minus)==LOW)
    {
      setzitemp=setzitemp - 1;
    delay(50);
    }
    if (setzitemp < 0) {// month must be between 0 and 6
      setzitemp = 0;
    }
    if (setzitemp > 6) {
      setzitemp=6;
    }
    itoa(setzitemp, tmp_string, 10);
    u8g.firstPage(); 
  do {
  u8g.drawStr( 0, 20, "Set Day of Week");
  u8g.drawStr(25, 40, tmp_string);
  u8g.drawStr(0,60, daysOfTheWeek[setzitemp]);
  } while( u8g.nextPage() ); 
    setzi();
}

int setore () {
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setoretemp;
    }
 /*   knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
      delay (50);
    }
    if (knobval > 1) {
      knobval = 1;
      delay (50);
    }
    setoretemp=setoretemp + knobval;
    */
     if (digitalRead(plus)==LOW)
    {
      setoretemp=setoretemp + 1;
    delay(50);
    }
    if (digitalRead(minus)==LOW)
    {
      setoretemp=setoretemp - 1;
    delay(50);
    }
    if (setoretemp < 0) {
      setoretemp = 0;
    }
    if (setoretemp > 23) {
      setoretemp=23;
    }
   itoa(setoretemp, tmp_string, 10);  
    u8g.firstPage(); 
  do {
  u8g.drawStr( 0, 20, "Set Hour");
u8g.drawStr(25, 40, tmp_string);
  } while( u8g.nextPage() ); 
    setore();
}

int setmins () {
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setminstemp;
    }
 /*   knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
      delay (50);
    }
    if (knobval > 1) {
      knobval = 1;
      delay (50);
    }
    setminstemp=setminstemp + knobval;
    */
     if (digitalRead(plus)==LOW)
    {
      setminstemp=setminstemp + 1;
    delay(50);
    }
    if (digitalRead(minus)==LOW)
    {
      setminstemp=setminstemp - 1;
    delay(50);
    }
    if (setminstemp < 0) {
      setminstemp = 0;
    }
    if (setminstemp > 59) {
      setminstemp=59;
    }
   itoa(setminstemp, tmp_string, 10);  
    u8g.firstPage(); 
  do {
  u8g.drawStr( 0, 20, "Set Minutes");
u8g.drawStr(25, 40, tmp_string);
  } while( u8g.nextPage() ); 
    setmins();
}
// see https://arduino.cz/tutorial-uzivani-hodin-realneho-casu-ds1307-a-ds3231-s-arduinem/
// http://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-ds3231-real-time-clock-modules-with-arduino/

