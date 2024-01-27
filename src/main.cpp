#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Wonderland-2";
const char* password = "sustancias123";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 13);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);

// Motor Speed
int ENA = D2;
int IN1 = D7;
int IN2 = D8;

// Motor Dir
int ENB = D1;
int IN3 = D5;
int IN4 = D6;


void setup()
{

 pinMode (ENA, OUTPUT);
 pinMode (ENB, OUTPUT);
 pinMode (IN1, OUTPUT);
 pinMode (IN2, OUTPUT);
 pinMode (IN3, OUTPUT);
 pinMode (IN4, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  // Configures static IP address
if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
}

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    int speed = incomingPacket[1] - 48;
    int direction = incomingPacket[0] - 48;
    int reverse = incomingPacket[2] - 48;

   if(speed == 0){
     analogWrite(ENA, 0);
   } else {
     analogWrite(ENA, 50 + (speed * 25));
   }
    if(reverse == 1){
       digitalWrite (IN1, HIGH);
       digitalWrite (IN2, LOW);
    } else if(reverse == 0){
      digitalWrite (IN1, LOW);
      digitalWrite (IN2, HIGH);
    }

    if(direction == 2){
      analogWrite(ENB, 230);
       digitalWrite (IN3, HIGH);
       digitalWrite (IN4, LOW);
    } else if(direction == 1){
      analogWrite(ENB, 0);
       digitalWrite (IN3, LOW);
       digitalWrite (IN4, LOW);
    } else if(direction == 0){
      analogWrite(ENB, 230);
       digitalWrite (IN3, LOW);
       digitalWrite (IN4, HIGH);
    }
  }
}