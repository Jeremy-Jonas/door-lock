#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

//Define some keywords (purely to make the code more readable to humans)
#define pressed LOW
#define notPressed HIGH
#define on HIGH
#define off LOW

//define the inputs
#define callButton digitalRead(8)
#define internalButton digitalRead(6)

//define the outputs
#define doorStrike 2
#define notificationLED 4
#define resetRelay 5
#define unauthorisedLED 7

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

unsigned int pressTime = 0;   //create an integer variable to be incremented while the button is pressed

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(doorStrike, OUTPUT);
  pinMode(unauthorisedLED, OUTPUT);
  pinMode(notificationLED, OUTPUT);
  pinMode(8, INPUT_PULLUP); //have to use the pin numbers when initalising inputs due to using "digitalRead(x)" in the definitions
  pinMode(6, INPUT_PULLUP);

}
void loop() {
  if (callButton == pressed && pressTime < 150) { //when the call button is pressed start incrementing pressTime until either the button is released or 15 seconds elapses
    pressTime++;
    delay(100); //a delay of 100 means it checks the status of the button every tenth of a second, and means a value of 150 in pressTime = 15 seconds
  } else if (callButton == pressed && pressTime >= 150) { //if the call button has been pressed and held for 15 seconds turn on the reset relay
    digitalWrite(resetRelay, on);
    //program will end here in this case as the arduino resets.
  } else if (callButton == notPressed && pressTime != 0) { //if the call button is released before 15 seconds, turn on the internal "call notification" LED
    digitalWrite(notificationLED, on);
    pressTime = 0;
    delay(100);
  }

  if (internalButton == pressed) {
    digitalWrite(notificationLED, off);
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "** ** ** **") //replace the ** with the UID of the card that you want to give access
  {
    Serial.println("Authorised access"); //send "authorised access" message to the serial monitor (if arduino is connected to PC)
    Serial.println();
    digitalWrite(doorStrike, on); //activate the relay which drives the electronic doorstrike
    delay(3000);
    digitalWrite(doorStrike, off); //deactivate the relay which drives the electronic doorstrike
  }

  else if (content.substring(1) == "** ** ** **") //add a second UID here if you want to use more than one card
  {
    Serial.println("Authorised access");
    Serial.println();
    digitalWrite(doorStrike, on);
    delay(3000);
    digitalWrite(doorStrike, off);
  }

  else   {
    Serial.println("Access denied");
    digitalWrite(unauthorisedLED, on); //illuminate led on pin 7 to indicate an access attempt was made with unauthorised card
  }

}
