#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 10, 11, 12}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7, 8}; // Connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

String passcode = "1234";
String inputCode = "";
int attemptCounter = 0;
const int maxAttempts = 3;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    if (key == '#') { // Check if the key is the confirmation key
      if (inputCode == passcode) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        delay(2000);
        attemptCounter = 0; // Reset attempt counter on success
        inputCode = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Password:");
      } else {
        attemptCounter++;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        delay(2000);
        if (attemptCounter >= maxAttempts) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Locked Out");
          while (1); // Lockout (infinite loop)
        }
        inputCode = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Password:");
      }
    } else if (key == '*') { // Clear input with '*'
      inputCode = "";
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear the second line
    } else {
      if (inputCode.length() < 4) {
        inputCode += key;
        lcd.setCursor(inputCode.length() - 1, 1);
        lcd.print(key);
      }
    }
  }
}
