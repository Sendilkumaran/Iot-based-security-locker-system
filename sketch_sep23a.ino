#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Keypad.h>

const String password_1 = "1A3B";    // change your password here
String input_password;

int angle = 0; // the current angle of servo motor
unsigned long lastTime;

int state;


const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
const int buzzerpin=11;//declaring buzzer pin
const int servopin=10;//declaring servo pin
LiquidCrystal_I2C lcd(0x27,20,4);
Servo Servo1;
 

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9, 8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


void setup()
{ 
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  Servo1.attach(servopin);
  Servo1.write(0);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("Enter password:") ;
  lcd.print("Enter password:");
}

void loop()
{ 
  while (true){
    
    char key = myKeypad.getKey();
    
    if (key) {
  
      if (key == '*') {
        input_password = ""; // reset the input password
      } 
      else if (key == '#') {
        if (input_password == password_1 ) {
          lcd.clear();
          lcd.print("Locker Opened");
          angle = 90;
          Servo1.write(angle);
          lastTime = millis();
        } 
        else {
          digitalWrite(buzzerpin,HIGH); 
          delay(1000); 
          digitalWrite(buzzerpin,LOW);
          lcd.clear();
          lcd.print("Wrong Password");
          delay(2000);
          lcd.clear();
          lcd.print("Enter password:");
        }
  
        input_password = ""; // reset the input password
      } 
      
      else {
        input_password += key; // append new character to input password string
      }
    }
  
    if (angle == 90 && (millis() - lastTime) > 5000) { // 5 seconds
      angle = 0;
      Servo1.write(angle);
      lcd.clear();
      lcd.print("Locker Closed");
      delay(2000);
      lcd.clear();
      lcd.print("Enter password:");
    }

    if (Serial.available()){
      break;
    }
  }

  int state = Serial.parseInt();
  if (state==1){
    Servo1.write(90);
    lcd.clear();
    lcd.print("Locker Opened");   
  }
    
  if (state==2){
    Servo1.write(0);
    lcd.clear();
    lcd.print("Locker Closed");
  }
}
