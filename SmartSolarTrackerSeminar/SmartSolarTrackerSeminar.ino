#include <Servo.h>    // aus der Librarie von arduino

//Pins Photowiderstände 1-4
const int Lsp1 = A1;
const int Lsp2 = A2;
const int Lsp3 = A3;
const int Lsp4 = A4;

int lsv1;
int lsv2;
int lsv3;
int lsv4;

Servo S1;
Servo S2;

int akt_Pos1 = 90;
int akt_Pos2 = 90;

int new_Pos1 = 0;
int new_Pos2 = 0;

int Wert1 = 0;
int Wert2 = 0;

// All values belong to the dust sensor 
// vergleiche (https://circuitdigest.com/microcontroller-projects/interfacing-dust-sensor-with-arduino)
// Pin Staubsensor (A0)
const int measurePin = A0;

//Connect 3 LED driver pins of dust sensor to Arduino D2 
const int ledPower = 7;
const int led1 = A5;

//time required to sample signal coming out (dust sensor)
int samplingTime = 280;

int deltaTime = 40;

int sleepTime = 9680;

float voMeasured = 0;

float calcVoltage = 0;

float dustDensity = 0;
 
int Regelwert = 230; // Regulerer Wert der Staubdichte der Akzeptabel ist.


void setup() {
  // put your setup code here, to run once:

  //Servo-Motoren
  S1.attach(8);
  S1.write(akt_Pos1);
  S2.attach(9);
  S2.write(akt_Pos2);

  //LED for dust sensor
  pinMode(ledPower,OUTPUT);
  pinMode(led1,OUTPUT);
  Serial.begin(9600);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  int Toll1 = (5); // akzeptable Abweichung
  int Toll2 = (-5); // akzeptable -Abweichung

  // 2. Schritt: Auslesen der LDR
  lsv1 = analogRead(Lsp1);
  lsv2 = analogRead(Lsp2);
  lsv3 = analogRead(Lsp3);
  lsv4 = analogRead(Lsp4);

  // 2. Schritt
  int a = lsv1 + lsv3; // Linke sensoren 
  int b = lsv2 + lsv4;// Rechte sensoren
  int c = lsv1 + lsv2;// Obere sensoren
  int d = lsv3 + lsv4;// Untere sensoren
  Serial.println(Wert2);
  // differenz der betrage der sensoren
  Wert1 = a - b; //Horizontaller Wert
  Wert2 = c - d; // Wertikaller Wert

  //Funktion Wer. bewegung
  if (Wert2 > Toll1 && akt_Pos2 < 180){// rechter wert grözer + linker wert größer - 
    new_Pos2 = akt_Pos2 + 5;
    S2.write(new_Pos2);
    akt_Pos2 = new_Pos2;
    delay(1000);}
  else{
      if(Wert2 < Toll2 && akt_Pos1 > 0 ){
        new_Pos2 = akt_Pos2 - 5;
        S2.write(new_Pos2);
        akt_Pos2 = new_Pos2;
      }
      delay(1000);
    }
      // Funktion Hor. bewegung
    if(Wert1 > Toll1 && akt_Pos1 < 180){
        new_Pos1 = akt_Pos1 + 5;
        S1.write(new_Pos1);
        akt_Pos1 = new_Pos1;
        delay(1000);
      }
    else{
        if (Wert1 < Toll2 && akt_Pos1 > 0){
          new_Pos1 = akt_Pos1 - 5;
          S1.write(new_Pos1);
          akt_Pos1 = new_Pos1;
        }
        delay(1000);
      }
  
  // Dust sensor 
  digitalWrite(ledPower,LOW); // Power der LED im staub sensor

  delayMicroseconds(samplingTime); 

 voMeasured = analogRead(measurePin); // liest den staub anteil

 delayMicroseconds(deltaTime);

 digitalWrite(ledPower,HIGH); // schaltet die LED im staubsensor aus

 delayMicroseconds(sleepTime);

 // 0 - 5V mappes to 0 - 1023 integer values

 // stehlt strom wieder her

 calcVoltage = voMeasured * (5.0 / 1024.0);

 // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/

 dustDensity = 170 * calcVoltage - 0.1;
 if (dustDensity > Regelwert){     // Überprüft ob der staub wehrt höher ist als normal 
  digitalWrite(led1, HIGH);
 }

}
