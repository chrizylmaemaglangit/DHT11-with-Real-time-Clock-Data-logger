#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>
#include "DHT.h"
#include <SD.h>
#include <SPI.h>
//#include <Servo.h>
//Servo s;
#define DHTPIN 2
#define DHTTYPE DHT11
#define I2C_ADDR 0x27 

virtuabotixRTC myRTC(6, 7, 8); //Initialize rtc

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x3F, 16 column and 2 rows
 
DHT dht(DHTPIN, DHTTYPE);


File myFile; //--> File Initialization
const int chipSelect = 4; //--> chipSelect (CS) Pin


void setup()
{
  Serial.begin(9600);
//  s.attach(9);
  Serial.println("Initializing RTC DS1302...");
  delay(100);

  //----------------------------------------------Initializing RTC DS1302 (secs, mins, hrs, weekday(wednesday), day, month, year) ;lcd= date & hr, minutes 
 myRTC.setDS1302Time(18, 44, 12, 3, 8, 3, 2023);

 
  Serial.println("Start DHT11");
  dht.begin();
  delay(100);
  Serial.println();
  //----------------------------------------------
   //----------------------------------------------Initializing Micro SD Card
  Serial.println("Initializing Micro SD Card...");
  delay(100);

  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    while (1);
  }

  Serial.println("Successfully Initializing Micro SD Card");
  Serial.println();
  delay(100);

  lcd.init();      // initialize the lcd
  lcd.backlight(); // open the backlight 
  //----------------------------------------------

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //___________________________________________________________________________________Millis to update the Date, Time, Temperature and Humidity

    myRTC.updateTime(); //--> Get or update the Date and Time
    delay(1000);
    //----------------------------------------------Get or update Temperature and Humidity Values
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    //----------------------------------------------
    lcd.clear();
    //----------------------------------------------Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read Data from DHT sensor!");
      lcd.setCursor(0,0);
      lcd.print("Failed");
      delay(500);
      return;
    }else{
    //----------------------------------------------

    //___________________________________________________________________________________Conditions for writing data to a Micro SD Card
    // Data Date, Time, Temperature and Humidity will be written to the Micro SD Card if Seconds are worth 0,5,10,15,20 and so on
      Serial.print("Current Date/Time: ");
      //----------------------------------------------Printing Date and Time to Serial Monitor
      Serial.print(myRTC.dayofmonth);
      lcd.setCursor(0,0);
      lcd.print(myRTC.dayofmonth);
      Serial.print('/');
      lcd.setCursor(2,0);
      lcd.print("/");
      Serial.print(myRTC.month);
      lcd.setCursor(3,0);
      lcd.print(myRTC.month);
      Serial.print('/');
      lcd.setCursor(4,0);
      lcd.print("/");
      Serial.print(myRTC.year);
      lcd.setCursor(5,0);
      lcd.print(myRTC.year);
     
      Serial.print(" ");   
      Serial.print(myRTC.hours);
      lcd.setCursor(10,0);
      lcd.print(myRTC.hours);
      Serial.print(':');
      lcd.setCursor(12,0);
      lcd.print(":");
      Serial.print(myRTC.minutes);
      lcd.setCursor(13,0);
      lcd.print(myRTC.minutes);
      Serial.print(':');
//      lcd.setCursor(14,0);
//      lcd.print(":");
      Serial.print(myRTC.seconds);
//      lcd.setCursor(14,0);
//      lcd.print(myRTC.minutes);
     
      //----------------------------------------------

      //----------------------------------------------Printing Temperature and Humidity Values to Serial Monitor

      Serial.print(" Temperature : ");
      lcd.setCursor(0,1);
      lcd.print("T:");
      Serial.print(t);
      lcd.print(t);
      lcd.print((char)223);
      lcd.print("C");

      Serial.print(" Humidity : ");
      lcd.setCursor(8,1);
      lcd.print("H:");
      Serial.println(h);
      lcd.print(h);
      lcd.print("%");
      Serial.println();
      //----------------------------------------------
//      if(t >= 30)
//      {
//        s.write(90);
//       }else{
//        s.write(0);
//        }
      //___________________________________________________________________________________Write to Micro SD Card
      //----------------------------------------------Opening files that have been specified on the Micro SD Card
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      myFile = SD.open("datalog.txt", FILE_WRITE);
      //----------------------------------------------
    
      //----------------------------------------------Writing Data to the specified file on the Micro SD Card
      // if the file opened okay, write to it:
      if (myFile) {
        Serial.println("Writing to datalog.txt...");
        
        //----------------------------------------------Writing Date Data to Micro SD Card
        myFile.print("Date:");
        myFile.print(myRTC.dayofmonth);
        myFile.print("-");
        myFile.print(myRTC.month);
        myFile.print("-");
        myFile.print(myRTC.year);
        //----------------------------------------------
        
        myFile.print(", "); //--> Write the delimiter between the data in the form of a comma character to the Micro SD Card

        //----------------------------------------------Writing Time Data to Micro SD Card
        myFile.print("Time:");
        myFile.print(myRTC.hours);
        myFile.print(":");
        myFile.print(myRTC.minutes);
        myFile.print(":");
        myFile.print(myRTC.seconds);
        //----------------------------------------------
        myFile.print(", ");
        myFile.print("T:");       
        myFile.print(t); //--> Writing Temperature Data to Micro SD Card
        myFile.print(", ");
        myFile.print("H:");
        myFile.println(h); //--> Writing Humidity Data to Micro SD Card
          
        myFile.close(); //--> close the file
        
        Serial.println("Successfully writing Data to Micro SD Card");
        Serial.println("----");
        Serial.println();
      } else {      
        Serial.println("Error opening datalog.txt"); //--> if the file didn't open, print an error
      }
      //----------------------------------------------
      //___________________________________________________________________________________
    delay(3000);
    //___________________________________________________________________________________
  }
}

