// DO Pruebas de calibracion
// Usando multi UART #4


#include <SoftwareSerial.h>
// Definiendo pines de comunicacion RX y TX 
#define rx 2
#define tx 3 
// Se define la comunicacion serial UART
SoftwareSerial mySerial(rx, tx);

// Definiendo los pines de configuracion del MultiUART
int Pin_x = 5;
int Pin_y = 4;



void setup()
{
  mySerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
  delay(500);
  
  pinMode(Pin_x, OUTPUT);           //Set the digital pin as output.
  pinMode(Pin_y, OUTPUT);          //Set the digital pin as output.
  delay(500);
}
 
void loop()
{
  //after start up the program, you can using terminal to connect the serial of gprs shield,
  //if you input 't' in the terminal, the program will execute SendTextMessage(), it will show how to send a sms message,
  //if input 'd' in the terminal, it will execute DialVoiceCall(), etc.
 
  if (Serial.available()){
   open_channel();
   switch(Serial.read())
   {
     case 'p':
       TurnOnLed();
       break;
     case 'd':
       TurnOffLed();
       break;
     case 'a':
       ContinuousReadingModeOn();
       break;
     case 's':
       ContinuousReadingModeOff();
       break;
     case 'R':
       SingleReading();
       break;
     case 'C':
       CalibrationAtmos();
       break;
     case 'L':
       Calibration0();
       break;
   } 
  }
  if (mySerial.available())
    Serial.write(mySerial.read());
}
 
///SendTextMessage()
///this function is to send a sms message
void TurnOnLed()
{
  mySerial.print("L,1");
  mySerial.print("\r");
  delay(100);
  ShowSerialData();
}
 
void TurnOffLed()
{
  mySerial.print("L,0");
  mySerial.print("\r");
  delay(100);
  ShowSerialData(); 
}
 
void ContinuousReadingModeOn()
{
  mySerial.print("C,1");
  mySerial.print("\r");
  delay(100);
  ShowSerialData();
}
 
void ContinuousReadingModeOff()
{
  mySerial.print("C,0");
  mySerial.print("\r");
  delay(100);
  ShowSerialData();
}

void SingleReading(){
  mySerial.print("R");
  mySerial.print("\r");
  delay(100);
  ShowSerialData();
}

void CalibrationAtmos(){
  mySerial.print("Cal");
  mySerial.print("\r");
  delay(100);
  ShowSerialData();
}

void Calibration0(){
  mySerial.print("Cal,0");
  mySerial.print("\r");
  delay(100);
  ShowSerialData();
}
 
void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}

void open_channel(){
   switch ('2') {
     case '1':
       digitalWrite(Pin_x, LOW);
       digitalWrite(Pin_y, LOW);
     break;
     case '2':  // Aqui esta conectado el circuito de OD
       digitalWrite(Pin_x, LOW);
       digitalWrite(Pin_y, HIGH);
     break;
     case '3':
       digitalWrite(Pin_x, HIGH);
       digitalWrite(Pin_y, LOW);
     break;
     case '4':
       digitalWrite(Pin_x, HIGH);
       digitalWrite(Pin_y, HIGH);
     break;
   }
 } 

