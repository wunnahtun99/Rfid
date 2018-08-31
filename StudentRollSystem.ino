/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

// replace with your channel’s thingspeak API key,
String apiKey = "Y3RIFU93Q3H3EBE4";
const char* ssid = "LOL";
const char* password = "wunnahtun";

const char* server = "api.thingspeak.com";


WiFiClient client;


constexpr uint8_t RST_PIN = 5;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;         // Configurable, see typical pin layout above
int a=0,b=0,c=0,d=0,temp=0;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
 
delay(10);
SPI.begin();      // Initiate  SPI bus
mfrc522.PCD_Init();   // Initiate MFRC522
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
}

void loop() {


  unsigned long now = millis();

    long m=now/(1000*60);
    if(m>60){
    m=m-60;
    }
    if(m!=temp){
      Serial.println(m);
      temp=m;
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
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "DD DD 3E D5") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    if(m>0&&15>m){
      a=a+1;
    }

    if(m>15 && 30>m){
      b=b+1;
    }
   if(m>30 && 45>m){
      c=c+1;
    }
   if(m>45 && 59>m){
      d=d+1;
    }


    
  }
  else {
    Serial.println("Access Denied");
  }
  
  
  if (client.connect(server,80)) { // "184.106.153.149" or api.thingspeak.com
String postStr = apiKey;
postStr +="&field1=";
postStr += String(4);
postStr +="&field2=";
postStr += String(4);
postStr +="&field3=";
postStr += String(4);
postStr +="&field4=";
postStr += String(4);
postStr += "\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

Serial.print("A: ");
Serial.print(a);
Serial.print(" b: ");
Serial.print(b);
Serial.print("c: ");
Serial.print(c);
Serial.print(" d: ");
Serial.print(d);
Serial.println("% send to Thingspeak");
}
client.stop();

Serial.println("Waiting…");
// thingspeak needs minimum 15 sec delay between updates
delay(1000);
  
  
  }
