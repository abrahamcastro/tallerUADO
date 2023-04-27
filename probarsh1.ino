// https://script.google.com/macros/s/AKfycbx-Gj8MONr_AwO8hKtU8yiNHTaIrOcGIFZ6-30THhJbANZXgzWfybESPpoRPVTm0Gn-xw/exec
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

const char* ssid = "Red"; //--> Your wifi name or SSID.
const char* password = "password"; //--> Your wifi password.


//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.

String GAS_ID = "AKfycbx5QNN_Nve3TMfe5G9VVaMvMPHHEMxgJR39jdck2K3gM0Jy96ZqfloKTslN8LSUnItq"; //--> spreadsheet script ID

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);

  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    
  }
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  client.setInsecure();
}


void loop() {
   sendData(13); //--> Calls the sendData Subroutine
}

// Subroutine for sending data to Google Sheets
void sendData(int dato) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Processing data and sending data
  
  String url = "/macros/s/" + GAS_ID + "/exec?d="+String(dato);
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("dato enviado");
  //----------------------------------------

  //----------------------------------------Checking the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
} 
//==============================================================================
