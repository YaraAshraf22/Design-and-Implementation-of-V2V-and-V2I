#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


//Node_MCU as STA + AP Credentials 
const char* ssid = "NodeMCU_V2";  
const char* password = "12345678";
// Main Node_MCU AP
const char* AP_SSID = "NodeMCU_V1";
const char* AP_Password = "12345678";

IPAddress AP_STA_IP(192,168,1,10);
IPAddress gateway(192,168,1,10);
IPAddress subnet(255,255,255,0);

IPAddress AP_IP(192,168,1,1);
IPAddress broadcast_IP(192,168,1,255);

//Set UDP
#define UDP_Rx_PORT 2000
#define UDP_Tx_PORT_Broadcast 2010
#define UDP_Tx_PORT_Main_Unit 1024
WiFiUDP UDP;

char Incoming_Packet[255];
char message[]="Slow Down, Acident Ahead at Coordinates: "; 
char location[255];
char latitude[16];
char longitude[16];

int n_message=0;

void setup(void)
{
Serial.begin(9600);
Serial.println();

WiFi.mode(WIFI_AP_STA); //Set as AP+STA
WiFi.softAPConfig(AP_STA_IP, gateway, subnet);
WiFi.softAP(ssid, password);
WiFi.begin(AP_SSID, AP_Password);       //Connect to AP
WiFi.config(AP_STA_IP, gateway, subnet);


while (WiFi.status() != WL_CONNECTED)   //loop terminated if STA connected to AP
{
delay(500);
}

Serial.write("Connected");
Serial.write('/');




//Start UDP
UDP.begin(UDP_Rx_PORT);
}


void loop() 
{
  
// Wait for Incoming UDP Packet  
int packetSize = UDP.parsePacket();
if (packetSize)
{
  int len = UDP.read(Incoming_Packet, 255);
  if (len > 0)
  {
  Incoming_Packet[len] = 0;
  }
  if (Incoming_Packet[0]=='%')
  {
    Serial.write(Incoming_Packet);
  }
  else
  {
    n_message++;
    
    Serial.write(Incoming_Packet);
    Serial.write('/');

    delay(50);
    UDP.beginPacket(broadcast_IP, UDP_Tx_PORT_Broadcast);
    UDP.write(Incoming_Packet);
    UDP.endPacket();
    
   }
    
    
  }
  
}




/*switch(n_message)
    {
      case 1:
    {
     UDP.beginPacket(broadcast_IP, UDP_Tx_PORT_Broadcast);
     UDP.write(message);
     UDP.endPacket();
      break;
    }
    case 2:
    {
     //rebroadcast to behind surrounding vehicles
     UDP.beginPacket(broadcast_IP, UDP_Tx_PORT_Broadcast);
     UDP.write("Latitude:");
     UDP.endPacket();

     delay(10);
     
     
     UDP.beginPacket(broadcast_IP, UDP_Tx_PORT_Broadcast);
     UDP.write(Incoming_Packet);
     UDP.endPacket();

      break;
    }
    case 3:
    {
      n_message=0;
     
     //rebroadcast to behind surrounding vehicles
     UDP.beginPacket(broadcast_IP, UDP_Tx_PORT_Broadcast);
     UDP.write("Longitude");
     UDP.endPacket();
     
     delay(10);
     
     UDP.beginPacket(broadcast_IP, UDP_Tx_PORT_Broadcast);
     UDP.write(Incoming_Packet);
     UDP.endPacket();
     break;
    }
    }*/
