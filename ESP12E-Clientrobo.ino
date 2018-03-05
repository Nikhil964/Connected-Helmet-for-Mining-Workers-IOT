/*
 *  This sketch sends data via HTTP GET requests to api.thingspeak.com service.
 *
 *  You need to get API write Key at api.thingspeak.com  and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "LIBRARY-BLOCK";
const char* password = "";

int Alarm=D1;  //
void serverscript();


//const char* host = "192.168.0.187";
WiFiServer server(80);
String readString;


void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(Alarm, OUTPUT);
  // sets the pins as outputs:
 
    
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
  
  serverscript();
  delay(2000);
 
}
  
void serverscript(){
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

         
       if (readString.indexOf("ALERT") >0){
        digitalWrite(Alarm, HIGH);
                  
       }
        
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}

