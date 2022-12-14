// Include required libraries
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>

// Create instances
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
MFRC522 mfrc522(10, 9); // MFRC522 mfrc522(SS_PIN, RST_PIN)
Servo sg90;
// Initialize Pins for led's, servo and buzzer
// Blue LED is connected to 5V

constexpr uint8_t greenLed = 8;
constexpr uint8_t redLed = 7;
constexpr uint8_t servoPin = 6;
constexpr uint8_t buzzerPin = 5;

char initial_password[4] = {'1', '2', '3', '4'};  // Variable to store initial password
String tagUID = "05 44 0F 22";  // String to store UID of tag. Change it with your tag's UID
char password[4];   // Variable to store users password
boolean RFIDMode = true; // boolean to change modes
char key_pressed = 0; // Variable to store incoming keys
uint8_t i = 0;  // Variable used for counter
// defining how many rows and columns our keypad have
const byte rows = 4;
const byte columns = 4;
// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Initializing pins for keypad
byte row_pins[rows] = {A0, A1, A2, A3};
byte column_pins[columns] = {5, 6, 7};
unsigned long time;

// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);
void setup() {
  // Arduino Pin configuration
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
  sg90.attach(servoPin);  //Declare pin 8 for servo
  sg90.write(0); // Set initial position at 90 degrees
  lcd.begin(16, 2);   // LCD screen
  //lcd.backlight();
  SPI.begin();      // Init SPI bus
  Serial.begin(9600);
  mfrc522.PCD_Init();   // Init MFRC522
  lcd.clear(); // Clear LCD screen
}
//==========================================================================
void loop() {
  // System will first look for mode
  if (RFIDMode == true) {
    lcd.setCursor(0, 0);
    lcd.print("   Door Lock");
    lcd.setCursor(0, 1);
    lcd.print(" Scan Your Tag ");
    Serial.println("Scan Tag");
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    //Reading from the card
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();
    //Checking the card
    if (tag.substring(1) == tagUID)
    {
      Serial.println("Tag matched");
      time = millis();
      // If UID of tag is matched.
      lcd.clear();
      lcd.print("Tag Matched");
      digitalWrite(greenLed, HIGH);
      delay(3000);
      digitalWrite(greenLed, LOW);
      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0, 1);
      RFIDMode = false; // Make RFID mode false
    }
    else
    {
      // If UID of tag is not matched.
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Tag Shown");
      lcd.setCursor(0, 1);
      lcd.print("Access Denied");
      digitalWrite(buzzerPin, LOW);
      digitalWrite(redLed, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(redLed, LOW);
      lcd.clear();
    }
  }
  // If RFID mode is false, it will look for keys from keypad
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey(); // Storing keys
    if (key_pressed)
    {
      password[i++] = key_pressed; // Storing in password variable
      lcd.print("*");
    }
    if (i == 4) // If 4 keys are completed
    {
      delay(200);
      if (!(strncmp(password, initial_password, 4))) // If password is matched
      {
        lcd.clear();
        lcd.print("Pass Accepted");
        Serial.println("Password accepted");
        sg90.write(90); // Door Opened
        digitalWrite(greenLed, HIGH);
        delay(3000);
        digitalWrite(greenLed, LOW);
        sg90.write(0); // Door Closed
        lcd.clear();
        i = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else    // If password is not matched
      {
        lcd.clear();
        lcd.print("Wrong Password");
        Serial.print("Wrong password");
        digitalWrite(buzzerPin, LOW);
        digitalWrite(redLed, HIGH);
        delay(3000);
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(redLed, LOW);
        lcd.clear();
        i = 0;
        RFIDMode = true;  // Make RFID mode true
      }
    }
  }
}
