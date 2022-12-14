#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int r = 10;
int g = 6;
int b = 9;

int num = 5;
float temp;
int THERMISTORPIN = A0;
float THERMISTORNOMINAL = 112000; //resistance at tempnominal C
float TEMPERATURENOMINAL = 22.4;   //temp for thermnominal resistance
int BCOEFFICIENT = 3950;
float SERIESRESISTOR = 98600;

int samples[5];

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

  analogReference(EXTERNAL);
}

void loop() {
  
  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< num; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< num; i++) {
     average += samples[i];
  }
  average /= num;
 
  //Serial.print("Average analog reading "); 
  //Serial.println(average);
  
  // convert the value to resistance
  average = (1023 / average) - 1;
  average = SERIESRESISTOR / average;
 
  Serial.print("Thermistor resistance "); 
  Serial.println(average);

  float steinhart;
  
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  //if(steinhart <= 14 && steinhart >= 10)
    //steinhart -= 0.275;
  //if(steinhart < 10)
    //steinhart -= 0.5;
    
//  steinhart = log(average / THERMISTORNOMINAL) * pow((TEMPERATURENOMINAL + 273.15), 2); // (ln(R/Ro)) * (To)^2
//  steinhart = steinhart / (BCOEFFICIENT + (TEMPERATURENOMINAL + 273.15) * log(average / THERMISTORNOMINAL)) + TEMPERATURENOMINAL;

  float voltage = analogRead(THERMISTORPIN) * (5.0 / 1023.0);

  //float t = (BCOEFFICIENT*TEMPERATURENOMINAL)/(BCOEFFICIENT + TEMPERATURENOMINAL*log(SERIESRESISTOR/(THERMISTORNOMINAL*(5/voltage) - 1)));
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
  //Serial.println(t);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(steinhart);
  
  Serial.print("Voltage: ");
  Serial.println(voltage);
  Serial.println();
  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.print(voltage);
  
  temp = steinhart;
  
  if(temp <= 20)
  {
    analogWrite(b, 255);
    analogWrite(g, 0);
    analogWrite(r, 0);
    //Serial.println("Blue");
  }
  else if(temp > 20 && temp <=40)
  {
    analogWrite(g, 255);
    analogWrite(b, 0);
    analogWrite(r, 0);
    //Serial.println("Green");
  }
  else
  {
    analogWrite(r, 255);
    analogWrite(g, 0);
    analogWrite(b, 0);
    //Serial.println("Red"); 
  }
  
  delay(1000);
  // put your main code here, to run repeatedly:

}
