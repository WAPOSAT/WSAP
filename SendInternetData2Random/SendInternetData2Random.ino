//--------------------------------------------WAPOSAT----------------------------------------------------
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>         //Include the software serial library 

//----------------------------configuracion shell ethernet----------------------------------------------------------
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 172, 16, 13, 177 };
byte gateway[] = { 172, 16, 13, 254 };
byte server[] = { 45, 55, 150, 245 }; // IP Publico del servidor WAPOSAT
//byte server[] = { 172, 16, 13, 1 }; // IPServidor LAN

boolean lastConnected = false;                 // state of the connection last time through the main loop
EthernetClient client;


//----------------------------------------------------------------------------------------------------
void setup()
{
//-----------------configuracion de serial pc---------------------------------------------------------  
  //Ethernet.begin(mac); // inicializa ethernet shield para un DHCP
  Ethernet.begin(mac, ip, dns, gateway); // inicializa ethernet shield
  Serial.begin(9600);
}

void loop()
{
        
//-----------------------conectando al servidor--------------------------------------------------------  
  //Serial.println("Conectando...");
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
 
  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  //if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
  if(!client.connected()) {  
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("GET /index/Template/InsertData2.1.php?equipo=1&sensor1=1&sensor2=2"); // Envia los datos utilizando GET
    client.println(" HTTP/1.0");
    client.println();
    delay(120000);
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

