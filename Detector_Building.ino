int num = 5;
int THERMISTORPIN = A0;
int THERMISTORNOMINAL = 10000;
int TEMPERATURENOMINAL = 25;
int BCOEFFICIENT = 3950;
int SERIESRESISTOR = 10000;

int samples[5];
 
void setup(void) {
  Serial.begin(9600);
  // connect AREF to 3.3V and use that as VCC, less noisy!
  analogReference(EXTERNAL);
}
 
void loop(void) {
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
 
  Serial.print("Average analog reading "); 
  Serial.println(average);
  
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
  
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
 
  delay(1000);
}
