#include <SoftwareSerial.h>

SoftwareSerial BTserial(10, 11); // RX | TX

int sensorPin = A0;
int led = 5;
int bpm = 0;
int info;

void setup() {

  BTserial.begin(38400); 
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  BTserial.println("Send command...");
  pinMode(led, OUTPUT);

}

void loop() {

  bpm = analogRead(sensorPin);
  
  //IMPORTANT: The complete String has to be of the Form: 1234,1234,1234,1234;
  
  //(every Value has to be seperated through a comma (',') and the message has to
  
  //end with a semikolon (';'))
  if (BTserial.available()) {
    info = BTserial.read();
    Serial.print("Data received: ");
    Serial.println();
     if(info == '1')
     {
        digitalWrite(led, 1);
        Serial.println("LED on!");
        BTserial.println("LED on!");
     }
     
  }
  else if(info == '0')
  {
       digitalWrite(led,0);
       Serial.println("LED Off!");
       BTserial.println("LED  On D13 Off ! ");

  }
  else
  {;}
  
  BTserial.print("1234 test");
  BTserial.print(",");
  
  BTserial.print(bpm);
  BTserial.print(";");
  
  //message to the receiving device
  
  delay(20);

}
