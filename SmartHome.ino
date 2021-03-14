#include <Keypad.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,8,9,10,11);

//pins and leds
int sensor = A0;
int redLed = A1;
int greenLed = A2;
int buzzer = A3;

//variables and constatns
int temperature;
const byte rows = 4;
const byte columns = 4;
int i=0,size=0,j=0,var,wrongCode;
char keypressed;

//keypad matrix
char keypadMatrix [rows][columns] = { {'1','2','3','A'},
									  {'4','5','6','B'},
  									  {'7','8','9','C'},
  									  {'*','0','#','D'},
                                    };
byte rowsPins[rows] = {2,3,A4,A5};
byte columnsPins[columns] = {4,5,6,7};
char code[] = {'1','B','7','C'};

//keypad object
Keypad keypad = Keypad(makeKeymap(keypadMatrix),rowsPins,columnsPins,rows,columns);

void setup()
{
  pinMode(sensor,INPUT);
  pinMode(redLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(A5,INPUT);
  pinMode(A4,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  
  Serial.begin(9600);
  lcd.begin(16,2);
}

void loop()
{lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Enter code:");
 ReadKeypad();
 
 while(size == sizeof(code) && keypressed != '#'){
   keypressed = keypad.getKey();
   var = 1;
   ReadSensor();
   if(size == sizeof(code)) {
     if(temperature > 45) {
 		AlarmOn();
     } else {
       AlarmOff();
   }
  }
 }
 
 if(size != sizeof(code)) {
   wrongCode = 1;
 }
 
 if(var) {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("ENTER CODE:");
   ReadKeypad();
   if(size == sizeof(code)) {
     ShutDownAlarm();
     wrongCode = 0;
   }
   var = 0;
 }
 
 if(wrongCode) {
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Wrong code");
 lcd.setCursor(0,1);
 lcd.print("Try again");
 delay(2000);
 }
}

void ReadSensor() 
{ int t = analogRead(sensor);
  temperature = map(t,20,358,-40,125);
}

void AlarmOn()
{
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  tone(buzzer,1000,10000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DANGER");
  lcd.setCursor(0,1);
  lcd.print("EVACUATE");
  delay(200);
}

void AlarmOff()
{digitalWrite(greenLed, HIGH);
 digitalWrite(redLed, LOW);
 noTone(buzzer);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Temperature: ");
 lcd.print(temperature);
 lcd.setCursor(0,1);
 lcd.print("degrees celsius");
 delay(200);
}

void ReadKeypad(){          
       i=0;            //all variables set to 0          
       size=0;
       j=0; 
                                     
     while(keypressed != 'A'){                                   //The user press A to confirm the code otherwise he can keep typing
           keypressed = keypad.getKey();                         
             if(keypressed != NO_KEY && keypressed != 'A' ){     //If the char typed isn't A and neither "nothing"
              lcd.setCursor(j,1);                                //This to write "*" on the LCD whenever a key is pressed it's position is controlled by j
              lcd.print("*");
              j++;
            if(keypressed == code[i] && i<sizeof(code)){         //if the char typed is correct a and code increments to verify the next caracter
                 size++;                                              
                 i++;
                 }
            else
                size--;                                          //if the character typed is wrong a decrements and cannot equal the size of code []
            }
            }
    keypressed = NO_KEY;

}

void ShutDownAlarm(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SYSTEM OFF");
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  noTone(buzzer);
  delay(2000);
}