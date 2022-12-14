/*

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 13

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celcius=0;
float Fahrenheit=0;

int led = 1;
int state = 0;
int buttonState = -1;
int dial = -1;
double temp = -1;
double setTemp = -1;
double minTemp = 100; // temps in F
double maxTemp = 250; // temps in F
float C = 0;
float F = 0;
bool start = false;
int TIME = 642000; //642000/12oz. //10000;  //3750; // flow rate is 8L/min, 3750 ms for 500mL

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int potPin = A1;
int startButton = 8;
int tempProbe = 13;
int pump = 9;
int relay = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup(void) {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(startButton, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(pump, OUTPUT);
  
  //pinMode(potPin, INPUT);
  
  // connect AREF to 3.3V and use that as VCC, less noisy!
  analogReference(EXTERNAL);
}
 
void loop(void) {

  //digitalWrite(pump, HIGH);
  
  buttonState = digitalRead(startButton);
  dial = analogRead(potPin);
  //Serial.println(analogRead(A0));

  // =========================== Get temp data ======================
  
  sensors.requestTemperatures(); 
  C = sensors.getTempCByIndex(0);
  F = sensors.toFahrenheit(C);
  Serial.print(" C:  ");
  Serial.print(C);
  Serial.print(" F:  ");
  Serial.println(F);
  
  // =========================== Print data ======================
  
  if (buttonState == HIGH) // button only used for set temp and progression to second step
  {
    if(state == 0) // record user set temp for next stage in coffee making
    {
      //setTemp = map(dial, 0, 1023, minTemp, maxTemp); // define bounds of coffee temps
      setTemp = 200; // in F
      state++;
      // begin heating sequence
      
    }
    if(state == 3)
      state = 0;

  }
    
    
    //Serial.println("Button state: " + buttonState);

//  Serial.println();
//  Serial.print("Button state: ");
//  Serial.println(buttonState);
    
//  Serial.print("Temp: ");
//  Serial.println(temp);
    Serial.print("State: ");
    Serial.println(state);
    //Serial.println(dial);

  // =========================== UI Code ======================
  
  switch(state)
  {
    
    case 0:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Set Temperature: "); 

      setTemp = 140;

      Serial.print("Please set desired coffee temperature: ");
      temp = map(dial, 0, 1023, minTemp, maxTemp);
      Serial.println(temp);
      
      lcd.setCursor(1, 1);
      lcd.print("Temp: ");
      lcd.print(setTemp);
      
      break;
      
    case 1:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Heating...");
      Serial.println("Heating beverage...");

      lcd.setCursor(1, 1);
      String result = "Temp: ";
      result += F;
      result += " F";

      lcd.print(result);
      
//      lcd.print("Temp: ");
//      lcd.print(C);
//      lcd.print(" C, ");
//      lcd.print(F);
//      lcd.print(" F");
      
      // heat until temp reached
      if(F >= setTemp)
      { 
          Serial.println("========= TEMPERATURE REACHED =========");
          digitalWrite(relay, HIGH); // turn heater off
          state = 2; // turn pump on

          // ================ turn pump on ==================
          
          Serial.println("sdlfkajdlsifjselfij");
          //lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Preparing...");
          Serial.println("Preparing coffee...");

          double timer = millis();
          double current;
          for (int i = 0; i < TIME; i += (millis() - timer))
          {
            current = millis();
            lcd.setCursor(2, 0);
            lcd.print((TIME - i) / 1000);
            lcd.print(" seconds left");
            Serial.println(TIME - i);
            delay(1);           
          }

          
          // run pump for time
          digitalWrite(pump, HIGH);
          Serial.println(TIME);
          delay(TIME);
          digitalWrite(pump, LOW);
          state++;

          // ====================== enjoy coffee ======================

          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Enjoy coffee!");
          Serial.println("Enjoy beverage!");
          //break;
      }
      
      break;
      
    case 2:
      Serial.println("sdlfkajdlsifjselfij");
      //lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Preparing...");
      Serial.println("Preparing coffee...");
      
      // run pump for time
      digitalWrite(pump, HIGH);
      Serial.println(TIME);
      //delay(TIME);
      digitalWrite(pump, LOW);
      state++;
      break;
      
    case 3:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Enjoy coffee!");
      Serial.println("Enjoy beverage!");
      //break;
      
    default:
      Serial.println("++++++++++++++");
      break;
  }
 
  delay(10);
}
