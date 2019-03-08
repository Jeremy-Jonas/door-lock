#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(2, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, INPUT_PULLUP);

}
void loop() {
  int pin8 = digitalRead(8);
  if (pin8 == LOW) {
    digitalWrite(4, HIGH);
    delay(10000);
    digitalWrite(4, LOW);
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
  String content= "";
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
  if (content.substring(1) == "** ** ** **") //change here the UID of the card that you want to give access
  {
    Serial.println("Authorised access");
    Serial.println();
    digitalWrite(2, HIGH);
    delay(3000);
    digitalWrite(2, LOW);
  }

  else if (content.substring(1) == "** ** ** **")
  {
    Serial.println("Authorised access");
    Serial.println();
    digitalWrite(2, HIGH);
    delay(3000);
    digitalWrite(2, LOW);
  }
 
  else   {
    Serial.println(" Access denied");
    digitalWrite(7, HIGH);
    delay(3000);
    
  }
  
} 