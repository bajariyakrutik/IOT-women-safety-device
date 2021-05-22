#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//Creates new instance for SIM900D Module
SoftwareSerial SIM900(7, 8);

//Creates a new instance of the TinyGPS object for GPS Module
TinyGPS gps;  

int buzz = 13, inPin = 7;
bool newData = false;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//Creates a new instance of the Liquid Crystal LCD Display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  
  //Buzzer pin initialization
  pinMode(buzz, OUTPUT);

  //Switch pin initialization
  pinMode(inPin, INPUT);

  //LCD initial display
  lcd.setCursor(2,0);
  lcd.print("Women Safety");
  lcd.setCursor(5,1);
  lcd.print("Device");
  delay(1000);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Starting");
  lcd.setCursor(3,1);
  lcd.print("GPS + GSM");

  //GSM Initializing
  Serial.begin(9600);
  SIM900.begin(9600);

  //GPS Initializing
  while (Serial.available())
  {
    char c = Serial.read();
    Serial.print(c);
    if (gps.encode(c)) 
      newData = true;  
  }
  
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("GPS and GSM");
  lcd.setCursor(3,1);
  lcd.print("Started...");
  delay(500);
  lcd.clear();
}

void loop()
{
  bool val = digitalRead(inPin);
  
  if(!val){
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Woman NOT Safe");

    // Buzzer Tone and setting
    tone(buzz, 1000);

    //Running main program
    if(newData){
      float flat, flon;
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);   
      SIM900.print("AT+CMGF=1\r");  //Sets the GSM Module in Text Mode
      delay(100);
      SIM900.println("AT + CMGS = \"+91XXXXXXXXXX\"");    // recipient's mobile number with country code
      delay(100);
      SIM900.print("Latitude = ");
      SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      SIM900.print(" Longitude = ");
      SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
      delay(100);
      SIM900.print("\rI am in Danger!!");
      SIM900.print("\rPlease help me.\r");
      String SMS ="Google MAP link: https://www.google.com/maps/@"+String(flat)+","+String(flon)+",17z";
      SIM900.println(SMS);
      delay(100);
      SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
      delay(1000);
    }
    noTone(buzz);
  }
  else{
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Woman is Safe");
    delay(200);
  }
}
