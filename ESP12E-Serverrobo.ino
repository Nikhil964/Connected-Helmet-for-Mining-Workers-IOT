/*
 *  This sketch sends data via HTTP GET requests to api.thingspeak.com service.
 *
 *  You need to get API write Key at api.thingspeak.com  and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include "DHT.h"    
#define DHTPIN  D2  //Pin to attach the DHT
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "VSES";
const char* password = "bridge@smart123v";

const char* host = "192.168.0.115";
void clientscript(String);
WiFiServer server(80);
String readString;
String Action;

void setup() {
  Serial.begin(115200);
  delay(10);

    pinMode(D1, OUTPUT);
 
    
   
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

int val = analogRead(A0);
int tempc = dht.readTemperature();    
  // Start the server
  server.begin();
  delay(1000);
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

// Check if a client has connected
  WiFiClient client = server.available();
 if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
        Serial.println(readString); //print to serial monitor for debuging
      
           delay(1);
        
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
             
        if (val>50&&tempc>25){           
        
      digitalWrite(D1, HIGH);
      Action = "ALERT";
      clientscript(Action);                  
                   }
             //clearing string for next read
            readString="";  
      }
      }
      }
      }
      }

void clientscript(String Action){
  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("Sending Message ");
  // This will send the request to the server
  
  client.print(String("GET ") + Action + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  //client.println("test123?results");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

