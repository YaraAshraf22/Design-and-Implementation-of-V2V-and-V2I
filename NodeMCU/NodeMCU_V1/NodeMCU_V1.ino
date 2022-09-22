#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//V1 AP
const char* ssid = "NodeMCU_V1";  
const char* password = "12345678";  

IPAddress local_IP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress STA_NodeMCU(192,168,1,10);

//SET UDP
#define UDP_Rx_PORT 1024
#define UDP_Tx_PORT 2000
WiFiUDP UDP;

char packet[255];
char message[] = " Stop! Accident Ahead";


void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Configuring as SoftAP..."); 
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "AP Configured" : "Configuration Failed!");

  Serial.print("Setting Soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "AP Ready" : "Failed!");

  Serial.print("Soft-AP IP address: ");
  Serial.println(WiFi.softAPIP());

  //Begin listening to UDP port
  UDP.begin(UDP_Rx_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_Rx_PORT);

}

void loop() 
{

  char Rx_char=0;
  char avr_data[16]="";
  int index=0;
  

 if ( Serial.available() > 0 )
 {
   while (Serial.available() > 0 && index <16 )
   {
    delay(100);
    Rx_char=Serial.read();
    avr_data[index]=Rx_char;
    index++;
    avr_data[index]='\0'; 
    if (Rx_char == 'N' || Rx_char == 'E' || Rx_char == '!')
    {
      break;
    }
   }

  // Send UDP packet to NodeMCU 
  delay(100);

  UDP.beginPacket(STA_NodeMCU, UDP_Tx_PORT);
  UDP.write(avr_data);
  UDP.endPacket();    
 }

}
