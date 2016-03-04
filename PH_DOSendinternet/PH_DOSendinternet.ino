//--------------------------------------------WAPOSAT----------------------------------------------------
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>         //Include the software serial library 
#define rx 2                        //define what pin rx is going to be
#define tx 3                        //define what pin tx is going to be
#include <stdio.h>

//---------------------------comunicacion serial con sensor-------------------------------------------
SoftwareSerial myserial(rx, tx);    //define how the soft serial port is going to work.
int Pin_x = 5;                      //Arduino pin 5 to control pin X
int Pin_y = 4;                      //Arduino pin 4 to control pin Y

//---------------------------datos de sensor----------------------------------------------------------
char sensordata1[30];  //A 30 byte character array to hold incoming data from the sensors
byte sensor_bytes_received1=0;       //We need to know how many characters bytes have been received

char sensordata2[30];  //A 30 byte character array to hold incoming data from the sensors
byte sensor_bytes_received2=0;       //We need to know how many characters bytes have been received

//----------------------------configuracion shell ethernet----------------------------------------------------------
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 172, 16, 13, 177 };
byte gateway[] = { 172, 16, 13, 2 };
//byte server[] = { 45, 55, 150, 245 }; // IP Publico del servidor WAPOSAT
byte server[] = { 172, 16, 13, 15 }; // IPServidor LAN


boolean lastConnected = false;                 // state of the connection last time through the main loop
EthernetClient client;

//----------------------------------------------------------------------------------------------------
void setup()
{
//-----------------configuracion de serial pc---------------------------------------------------------  
  //Ethernet.begin(mac); // inicializa ethernet shield para un DHCP
  Ethernet.begin(mac, ip, dns, gateway); // inicializa ethernet shield
  Serial.begin(9600);
//-----------------configuracion de serial sensor-----------------------------------------------------
  pinMode(Pin_x, OUTPUT);           //Set the digital pin as output.
  pinMode(Pin_y, OUTPUT);          //Set the digital pin as output.
  myserial.begin(9600);             //Set the soft serial port to 9600

  delay(1000); // espera 1 segundo despues de inicializar

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

//----------------funcion de coneccion al canal 1------------------------------------------------------------------
void open_channel1(){        //This function controls what UART port is opened.                                                      //If *channel==1 then we open channel 1     
         myserial.flush();
         digitalWrite(Pin_x, LOW);                   //Pin_x and pin_y control what channel opens 
         digitalWrite(Pin_y, LOW);                   //Pin_x and pin_y control what channel opens 
      }
void open_channel2(){
         myserial.flush();
         digitalWrite(Pin_x, LOW);                   //Pin_x and pin_y control what channel opens 
         digitalWrite(Pin_y, HIGH);                   //Pin_x and pin_y control what channel opens 
      }
//---------------------------------------------------------------------------------------------------------------------

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    LecturaSensores();
    // send the HTTP PUT request:
    //client.print("GET /index/Template/InsertData2.php?equipo=5&sensor1=1&sensor2=3&valor1="); // Envia los datos utilizando GET
    client.print("GET /WAPOSAT2/Template/InsertData2.php?equipo=5&sensor1=1&sensor2=3&valor1="); // Envia los datos LAN
    client.print(sensordata1);
    client.print("&valor2=");
    client.print(sensordata2);
    client.println(" HTTP/1.0");
    client.println();
    
    myserial.flush();
    // note the time that the connection was made:
    //lastConnectionTime = millis()/1000;
    delay(1000);
  } 
  else {
    // if you couldn't make a connection:
//    contDisconnecting = contDisconnecting+1;
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

void LecturaSensores() {
  //-----------------------sensor de pH------------------------------------------------------------------  
      open_channel1();                            //Call the function "open_channel" to open the correct data path
      delay(1000);
      myserial.print('r');                      //Send the command from the computer to the Atlas Scientific device using the softserial port                                     
      myserial.print("\r");
       
      sensor_bytes_received1=myserial.readBytesUntil(13,sensordata1,30); //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received 
      sensordata1[sensor_bytes_received1]=0;            //we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
      Serial.println(sensordata1);
       
  //-----------------------sensor de DO------------------------------------------------------------------  
      open_channel2();                            //Call the function "open_channel" to open the correct data path
      delay(1000);
      myserial.print('r');                      //Send the command from the computer to the Atlas Scientific device using the softserial port                                     
      myserial.print("\r");
       
      sensor_bytes_received2=myserial.readBytesUntil(13,sensordata2,30); //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received 
      sensordata2[sensor_bytes_received2]=0;            //we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
      Serial.println(sensordata2);
  
}
