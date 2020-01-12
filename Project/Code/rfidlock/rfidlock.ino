
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>


#define RST_PIN   8
#define SS_PIN    9

String cardId = "";
int totalTags = 0;
byte readtag[5];
char* myTags[80] = {};
boolean drawerunlocked = false;
int proximitySensor;
boolean tagVerfied = false;
boolean verfiedTag = false;



MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo; // Servo motor
LiquidCrystal lcd(1, 2, 3, 4, 5, 6); //Parameters)


void setup() {

  SPI.begin();        // Initiating SPI bus
  mfrc522.PCD_Init(); //  Initiating MFRC522
  lcd.begin(15, 1);   // Initiating LCD screen
  myServo.attach(10);  // Initiating Servo motor

  myServo.write(20); //lock position of the server
  
 
  lcd.print("  Welcome John"); // Disply Welcome John on the screen 
  lcd.setCursor(0, 1);
  lcd.print("  Register Tag"); // Disply Register Tag on the screen
  
  // waiting for master card to be scanned
  while (!tagVerfied) {
    tagVerfied = getID();
    if ( tagVerfied == true) {
      cardId[totalTags] = strdup(cardId.c_str()); // Sets the master tag into position 0 in the array
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Tag Set");
      totalTags++;
    }
  }
  tagVerfied = false;
  printNormalModeMessage();
}

void loop() {
  int proximitySensor = analogRead(A1);
  // if the drawer is closed 
  if (proximitySensor > 180) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) { 
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   
      return;
    }
    cardId = "";
    
    // The MIFARE PICCs that we use have 4 byte UID
    for ( uint8_t i = 0; i < 4; i++) {  //
      readtag[i] = mfrc522.uid.uidByte[i];
      cardId.concat(String(mfrc522.uid.uidByte[i], HEX)); 
    }
    cardId.toUpperCase();
    mfrc522.PICC_HaltA(); 

    verfiedTag = false;
    
    // checks for master tag
    if (cardId == myTags[0]) {
      lcd.clear();
      lcd.print("    Setting");
      lcd.setCursor(0, 1);
      lcd.print(" Add/Remove Tag");
      while (!tagVerfied) {
        tagVerfied = getID();
        if ( tagVerfied == true) {
          for (int i = 0; i < 100; i++) {
            if (cardId == myTags[i]) {
              myTags[i] = "";
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("  Tag Removed ");
              printNormalModeMessage();
              return;
            }
          }
          cardId[totalTags] = strdup(cardId.c_str());
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   Tag Added!");
          printNormalModeMessage();
          totalTags++;
          return;
        }
      }
    }
    tagVerfied = false;
    
    // Checks if the tag scanned is registered 
    for (int i = 0; i < 100; i++) {
      if (cardId == myTags[i]) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Tag verfied");
        myServo.write(140); // Unlocks the drawer
        printNormalModeMessage();
        verfiedTag = true;
      }
    }
    if (verfiedTag == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Access Denied");
      printNormalModeMessage();
    }
  }

  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Drawer Unlocked");
    while (!drawerunlocked) {
      proximitySensor = analogRead(A0);
      if (proximitySensor > 200) {
        drawerunlocked = true;
      }
    }
    drawerunlocked = false;
    delay(500);
    myServo.write(20); // Locks the door
    printNormalModeMessage();
  }
}

uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {  
    return 0;
  }
  cardId = "";
  for ( uint8_t i = 0; i < 4; i++) {  
    readtag[i] = mfrc522.uid.uidByte[i];
    cardId.concat(String(mfrc522.uid.uidByte[i], HEX)); 
  }
  cardId.toUpperCase();
  mfrc522.PICC_HaltA(); 
  return 1;
}

void printNormalModeMessage() {
  delay(1500);
  lcd.clear();
  lcd.print("      John");
  lcd.setCursor(0, 1);
  lcd.print(" Scan Your Tag");
}
