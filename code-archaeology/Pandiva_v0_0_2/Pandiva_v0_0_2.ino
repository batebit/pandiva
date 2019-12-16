/*
  Batebit Artesania Digital
 Pandiva v0.0.2
 Adicionando MIDI
 */

#include <MIDI.h>

boolean lightStatus = false;
boolean lockButton1 = false;
boolean lockTurnButton1 = true;
boolean lockButton2 = false;
boolean lockTurnButton2 = true;
boolean lockButton3 = false;
boolean lockTurnButton3 = true;
int frequencia = 0;
int ultimaNota1 = 0;
int ultimaNota2 = 0;
int ultimaNota3 = 0;

int pinButton1 = 5;
int pinButton2 = 6;
int pinButton3 = 4;
int audioPin = 9;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  MIDI.begin();          
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(pinButton1, INPUT_PULLUP);
  pinMode(pinButton2, INPUT_PULLUP);
  pinMode(pinButton3, INPUT_PULLUP);
  tone(audioPin,440);
  delay(50);
  noTone(audioPin);
  MIDI.sendNoteOn(100,127,1);  
  delay(100);		      
  MIDI.sendNoteOff(100,0,1);
}

void loop() {

  int vara = analogRead(A0);

  int button1 = 1023-(int(digitalRead(pinButton1))*1023);
  int button2 = 1023-(int(digitalRead(pinButton2))*1023);
  int button3 = 1023-(int(digitalRead(pinButton3))*1023);

  vara = constrain(vara, 0, 1023);
  frequencia = map(vara, 0, 1023, 30, 80);
  frequencia = int(frequencia);

  //  if(button1 > 512){
  //    tone(audioPin, frequencia);
  //    lockButton1 = true;
  //  } 
  //  else {
  //    lockButton1 = false;
  //  }
  //
  //  if(button2 > 512){
  //    tone(audioPin, (frequencia*3)/2);
  //    lockButton2 = true;
  //  } 
  //  else {
  //    lockButton2 = false;
  //  }
  //
  //  if(button3 > 512){
  //    tone(audioPin, (frequencia*5)/4);
  //    lockButton3 = true;
  //  } 
  //  else {
  //    lockButton3 = false;
  //  }
  //
  //  if(!lockButton1 && !lockButton2 && !lockButton3){
  //    noTone(audioPin);
  //  }

  if(button1 > 100 && !lockButton1){
    //tone(8, frequencia);
    lockButton1 = true;
    lockTurnButton1 = false;
    ultimaNota1 = frequencia;
    MIDI.sendNoteOn(ultimaNota1,127,1);
  }

  if(button1 < 20 && !lockTurnButton1){
    //noTone(8);
    lockButton1 = false;
    lockTurnButton1 = true;
    MIDI.sendNoteOff(ultimaNota1,127,1);
    ultimaNota1 = 0;
  }

  if(button2 > 100 && !lockButton2){
    //tone(8, (frequencia*3)/2);
    lockButton2 = true;
    lockTurnButton2 = false;
    ultimaNota2 = frequencia + 4;
    MIDI.sendNoteOn(ultimaNota2,127,1);
  }

  if(button2 < 20 && !lockTurnButton2){
    //noTone(8);
    lockButton2 = false;
    lockTurnButton2 = true;
    MIDI.sendNoteOff(ultimaNota2,127,1);
    ultimaNota2 = 0;
  }

  if(button3 > 100 && !lockButton3){
    //tone(8, (frequencia*5)/4);
    lockButton3 = true;
    lockTurnButton3 = false;
    ultimaNota3 = frequencia + 7;
    MIDI.sendNoteOn(ultimaNota3,127,1);
  }

  if(button3 < 20 && !lockTurnButton3){
    //noTone(8);
    lockButton3 = false;
    lockTurnButton3 = true;
    MIDI.sendNoteOff(ultimaNota3,127,1);
    ultimaNota3 = 0;
  }

  //digitalWrite(13, lightStatus);
  char divisor = ';';
  Serial.print(button1);
  Serial.print(divisor);
  Serial.print(button2);
  Serial.print(divisor);
  Serial.print(button3);
  Serial.print(divisor);
  Serial.println(vara);
  //Serial.println("\t");
  delay(10);        // delay in between reads for stability
}







