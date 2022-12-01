#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define RST_PIN         D0           // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

#define FIREBASE_HOST "esp8266final-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "1Yk2lx09gDtQGpu2gBXqYmPY8SzNyluWYEKeoD2F" //Your Firebase Database Secret goes here

#define WIFI_SSID "OPPOA5s"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "oppokeyboard"

const long utcOffsetInSeconds = 18000;
//----------------------------------------

//----------------------------------------Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

FirebaseData firebaseData;

String value="";
String DBnm="Detected";
String HD="VehcileNo";
String TD="VehcileStatus";
String T="Time";
String R="Valid";
String F="Theft vehicle";
int cnt=0;
//*****************************************************************************************//
void setup() {
Serial.begin(9600);                                           // Initialize serial communications with the PC
SPI.begin();                                                  // Init SPI bus
mfrc522.PCD_Init();                                              // Init MFRC522 card
Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}
//*****************************************************************************************//
void loop() {
    timeClient.update();
    String hr, mn, sc;
    if (timeClient.getHours() < 10) {
      hr = "0" + String(timeClient.getHours());
    }
    else {
      hr = String(timeClient.getHours());
    }
    
    if (timeClient.getMinutes() < 10) {
      mn = "0" + String(timeClient.getMinutes());
    }
    else {
      mn = String(timeClient.getMinutes());
    }
  
    if (timeClient.getSeconds() < 10) {
      sc = "0" + String(timeClient.getSeconds());
    }
    else {
      sc = String(timeClient.getSeconds());
    }
    
    String TimeNow = hr + ":" + mn + ":" + sc;
    Serial.print(TimeNow);
// Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
byte block;
byte len;
//-------------------------------------------
// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
if ( ! mfrc522.PICC_IsNewCardPresent()) {
return;
}
// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial()) {
return;
}
//Serial.println(F("**Card Detected:**"));
byte buffer1[18];
block = 4;
len = 18;
status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
if (status != MFRC522::STATUS_OK) {
Serial.print(F("Authentication failed: "));
Serial.println(mfrc522.GetStatusCodeName(status));
return;
}
status = mfrc522.MIFARE_Read(block, buffer1, &len);
if (status != MFRC522::STATUS_OK) {
Serial.print(F("Reading failed: "));
Serial.println(mfrc522.GetStatusCodeName(status));
return;
}

for (uint8_t i = 0; i < 16; i++)
{
value += (char)buffer1[i];
}
value.trim();
if(value!=""){
  cnt++;
}
Serial.print(value);
Serial.println(F("\n"));

mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1(); 
if(value=="KA-02-E-6854")
{
Serial.println(F("THEFT VEHICLE DETECTED"));  
//Serial.println(F("\n"));          
}
else
{
Serial.println(F("VALID VEHICLE"));
//Serial.println(F("\n"));
}
//Serial.println(F("\n"));
delay(1000);
if(value!=""){
String DBaddH = DBnm + "/" + cnt + "/" + HD; //--> Creating a Database path
    if(Firebase.setString(firebaseData,DBaddH,value)){ //--> Command or code for sending Humidity data in the form of a String data type to the Firebase Realtime Database.
           //Serial.println("Value Uploaded Successfully");
           delay(1000);
     }
     
else {        
       Serial.println(firebaseData.errorReason());
      }
 if(value=="KA-02-E-6854")
 {
    String DBaddT = DBnm + "/" + cnt + "/" + TD; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,F)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     }
           else {        
       Serial.println(firebaseData.errorReason());
        }
 }
 else
        {
          String DBaddT = DBnm + "/" + cnt + "/" + TD; //--> Creating a Database path
          if(Firebase.setString(firebaseData,DBaddT,R)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     }
           else {        
       Serial.println(firebaseData.errorReason());
      }
        }

    String DBaddT = DBnm + "/" + cnt + "/" + T; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,TimeNow)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     } 
else {        
    Serial.println(firebaseData.errorReason());
  }
} 
  value.clear();  
}


***********************************************************************


#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

Servo servo;

//#define BUZZER_pin      D2
#define RST_PIN         D0           // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

#define FIREBASE_HOST "esp8266final-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "1Yk2lx09gDtQGpu2gBXqYmPY8SzNyluWYEKeoD2F" //Your Firebase Database Secret goes here

#define WIFI_SSID "V2037"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "ashishMrb"

const int irPin = D2;
const int moto = D1;
const long utcOffsetInSeconds = 19800;
//----------------------------------------

//----------------------------------------Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

FirebaseData firebaseData;


String value="";
String DBnm="Detected";
String HD="VehicleNo";
String TD="VehicleStatus";
String T="Time";
String R="Valid";
String F="Theft vehicle";
String loc="Chalukya Circle";
int buzzer = 2;

//int cnt=0;
//*****************************************************************************************//
void setup() {
pinMode(irPin, INPUT); // initialize digital pin 2 as an input 
pinMode(moto, OUTPUT); //for LED as output
servo.attach(5);  //D4
servo.write(90);
delay(2000);
Serial.begin(9600);                                           // Initialize serial communications with the PC
pinMode(buzzer, OUTPUT);
SPI.begin();                                                  // Init SPI bus
mfrc522.PCD_Init();                                              // Init MFRC522 card
Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}
//*****************************************************************************************//
void loop() {
    timeClient.update();
    String hr, mn, sc;
    if (timeClient.getHours() < 10) {
      hr = "0" + String(timeClient.getHours());
    }
    else {
      hr = String(timeClient.getHours());
    }
    
    if (timeClient.getMinutes() < 10) {
      mn = "0" + String(timeClient.getMinutes());
    }
    else {
      mn = String(timeClient.getMinutes());
    }
  
    if (timeClient.getSeconds() < 10) {
      sc = "0" + String(timeClient.getSeconds());
    }
    else {
      sc = String(timeClient.getSeconds());
    }
    
    String TimeNow = hr + ":" + mn + ":" + sc;
   // Serial.print(TimeNow);
// Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
byte block;
byte len;
//-------------------------------------------
// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
if ( ! mfrc522.PICC_IsNewCardPresent()) {
return;
}
// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial()) {
return;
}
Serial.println(F("**vehicle Detected:**"));
byte buffer1[18];
block = 4;
len = 18;
status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
if (status != MFRC522::STATUS_OK) {
Serial.print(F("Authentication failed: "));
Serial.println(mfrc522.GetStatusCodeName(status));
return;
}
status = mfrc522.MIFARE_Read(block, buffer1, &len);
if (status != MFRC522::STATUS_OK) {
Serial.print(F("Reading failed: "));
Serial.println(mfrc522.GetStatusCodeName(status));
return;
}

for (uint8_t i = 0; i < 16; i++)
{
value += (char)buffer1[i];
}
value.trim();
//if(value!=""){
////  cnt++;
//}
//Serial.print(value);
//Serial.println(F("\n"));

mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1(); 
//if(value=="KA-02-E-6854")
//{
//Serial.println(F("THEFT VEHICLE DETECTED"));  
//Serial.println(F("\n"));          
//}
//else
//{
//Serial.println(F("VALID VEHICLE"));
//Serial.println(F("\n"));
//}
////Serial.println(F("\n"));
//delay(1000);


 int valu = digitalRead(4);
//  Serial.println(F(val)); 
 if (valu == 0)
 {          
   int valu = digitalRead(irPin);
  //Serial.println(irValue);
  if(value=="KA-02-E-6854")
  {
    servo.write(0);
    //digitalWrite(buzzerPin, HIGH);
  }
  else
  {
    servo.write(90);
    //digitalWrite(buzzerPin, LOW);
  }
 // delay(10);
//           //delay(1000);   
//          } 
// 
// if (valu==1)
//     { 
//           servo.write(90);
//          // delay(1000);
//           }
 }
// else
// {
//  servo.write(90);
// }
 
if(value!=""){

  String DBaddT = DBnm + "/" + T; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,TimeNow)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     } 
else {        
    Serial.println(firebaseData.errorReason());
  }


String DBaddH = DBnm + "/" + HD; //--> Creating a Database path
    if(Firebase.setString(firebaseData,DBaddH,value)){ //--> Command or code for sending Humidity data in the form of a String data type to the Firebase Realtime Database.
           Serial.println("Value Uploaded Successfully");
           delay(1000);
     }
     
else {        
       Serial.println(firebaseData.errorReason());
      }
 if(value=="KA-02-E-6854")
 {
    String DBaddT = DBnm + "/" + TD; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,F)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
//           delay(1000);
//           digitalWrite(buzzer, HIGH);
//           delay(200);
//           digitalWrite(buzzer, LOW);
//           delay(200); 
           
     }
           else {        
       Serial.println(firebaseData.errorReason());
        }
 }
 
 else
        {
          String DBaddT = DBnm + "/" + TD; //--> Creating a Database path
          if(Firebase.setString(firebaseData,DBaddT,R)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     }
           else {        
       Serial.println(firebaseData.errorReason());
      }
        }
         DBaddT = DBnm + "/" + "Location"; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,loc)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     } 
else {        
    Serial.println(firebaseData.errorReason());
  }

   
  value.clear(); 
} 
}

*****************************************************************************************************





#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

Servo servo;

//#define BUZZER_pin      D2
#define RST_PIN         D0           // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

#define FIREBASE_HOST "esp8266final-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "1Yk2lx09gDtQGpu2gBXqYmPY8SzNyluWYEKeoD2F" //Your Firebase Database Secret goes here

#define WIFI_SSID "V2037"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "ashishMrb"

const int irPin = D2;
const int moto = D1;
const long utcOffsetInSeconds = 19800;
//----------------------------------------

//----------------------------------------Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

FirebaseData firebaseData;


String value="";
String DBnm="Detected";
String HD="VehicleNo";
String TD="VehicleStatus";
String T="Time";
String R="Valid";
String F="Theft vehicle";
String loc="Chalukya Circle";
int buzzer = 2;

//int cnt=0;
//*****************************************************************************************//
void setup() {
pinMode(irPin, INPUT); // initialize digital pin 2 as an input 
pinMode(moto, OUTPUT); //for LED as output
servo.attach(5);  //D4
servo.write(90);
delay(2000);
Serial.begin(9600);                                           // Initialize serial communications with the PC
pinMode(buzzer, OUTPUT);
SPI.begin();                                                  // Init SPI bus
mfrc522.PCD_Init();                                              // Init MFRC522 card
Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}
//*****************************************************************************************//
void loop() {
    timeClient.update();
    String hr, mn, sc;
    if (timeClient.getHours() < 10) {
      hr = "0" + String(timeClient.getHours());
    }
    else {
      hr = String(timeClient.getHours());
    }
    
    if (timeClient.getMinutes() < 10) {
      mn = "0" + String(timeClient.getMinutes());
    }
    else {
      mn = String(timeClient.getMinutes());
    }
  
    if (timeClient.getSeconds() < 10) {
      sc = "0" + String(timeClient.getSeconds());
    }
    else {
      sc = String(timeClient.getSeconds());
    }
    
    String TimeNow = hr + ":" + mn + ":" + sc;
   // Serial.print(TimeNow);
// Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
byte block;
byte len;
//-------------------------------------------
// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
if ( ! mfrc522.PICC_IsNewCardPresent()) {
return;
}
// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial()) {
return;
}
Serial.println(F("**vehicle Detected:**"));
byte buffer1[18];
block = 4;
len = 18;
status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
if (status != MFRC522::STATUS_OK) {
Serial.print(F("Authentication failed: "));
Serial.println(mfrc522.GetStatusCodeName(status));
return;
}
status = mfrc522.MIFARE_Read(block, buffer1, &len);
if (status != MFRC522::STATUS_OK) {
Serial.print(F("Reading failed: "));
Serial.println(mfrc522.GetStatusCodeName(status));
return;
}

for (uint8_t i = 0; i < 16; i++)
{
value += (char)buffer1[i];
}
value.trim();
//if(value!=""){
////  cnt++;
//}
//Serial.print(value);
//Serial.println(F("\n"));

mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1(); 
//if(value=="KA-02-E-6854")
//{
//Serial.println(F("THEFT VEHICLE DETECTED"));  
//Serial.println(F("\n"));          
//}
//else
//{
//Serial.println(F("VALID VEHICLE"));
//Serial.println(F("\n"));
//}
////Serial.println(F("\n"));
//delay(1000);


 int valu = digitalRead(4);
//  Serial.println(F(val)); 
 if (valu == 0)
 {          
   int valu = digitalRead(irPin);
  //Serial.println(irValue);
  if(value=="KA-02-E-6854")
  {
    servo.write(0);
           delay(1000);
           digitalWrite(buzzer, HIGH);
           delay(200);
           digitalWrite(buzzer, LOW);
           delay(200); 
    //digitalWrite(buzzerPin, HIGH);
  }
 }
  else
  {
    servo.write(90);
    //digitalWrite(buzzerPin, LOW);
  }
 // delay(10);
//           //delay(1000);   
//          } 
// 
// if (valu==1)
//     { 
//           servo.write(90);
//          // delay(1000);
//           }
 //}
// else
// {
//  servo.write(90);
// }
 
if(value!=""){

  String DBaddT = DBnm + "/" + T; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,TimeNow)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     } 
else {        
    Serial.println(firebaseData.errorReason());
  }


String DBaddH = DBnm + "/" + HD; //--> Creating a Database path
    if(Firebase.setString(firebaseData,DBaddH,value)){ //--> Command or code for sending Humidity data in the form of a String data type to the Firebase Realtime Database.
           Serial.println("Value Uploaded Successfully");
           delay(1000);
     }
     
else {        
       Serial.println(firebaseData.errorReason());
      }
 if(value=="KA-02-E-6854")
 {
    String DBaddT = DBnm + "/" + TD; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,F)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.

           
     }
           else {        
       Serial.println(firebaseData.errorReason());
        }
 }
 
 else
        {
          String DBaddT = DBnm + "/" + TD; //--> Creating a Database path
          if(Firebase.setString(firebaseData,DBaddT,R)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     }
           else {        
       Serial.println(firebaseData.errorReason());
      }
        }
         DBaddT = DBnm + "/" + "Location"; //--> Creating a Database path
     if(Firebase.setString(firebaseData,DBaddT,loc)){ //--> Command or code for sending Temperature data in the form of a String data type to the Firebase Realtime Database.
           delay(1000);
     } 
else {        
    Serial.println(firebaseData.errorReason());
  }

   
  value.clear(); 
} 
}
