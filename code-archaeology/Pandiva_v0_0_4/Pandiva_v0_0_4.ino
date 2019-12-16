/*
  Batebit Artesania Digital
 Pandiva v0.0.4
 Multiplos Botoes
 */

#include <MIDI.h>

int buttonPins[] = {
  22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};

boolean locks[] = {
  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
boolean locksTurn[] = {
  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

int buttonStates[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

boolean lightStatus = false;

int frequencia = 0;
int lastVara = 0;

int ultimaNota[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int audioPin = 9;

boolean melodicMode = true;
boolean lockModeButton = false;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  MIDI.begin();          
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  for(int i = 0; i < 15; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(2, INPUT_PULLUP);
  tone(audioPin,440);
  delay(50);
  noTone(audioPin);
  MIDI.sendNoteOn(100,127,1);  
  delay(100);		      
  MIDI.sendNoteOff(100,0,1);
}

void loop() {

  int modeButtonState = digitalRead(2);

  if(modeButtonState == LOW && !lockModeButton){
    melodicMode = !melodicMode;
    lockModeButton = true;
  }

  if(modeButtonState == HIGH){
    lockModeButton = false;
  }

  if(melodicMode){
    digitalWrite(13, LOW);
  } 
  else {
    digitalWrite(13, HIGH);
  }

  int vara = analogRead(A0);
  int value1 = analogRead(A12);
  int value2 = analogRead(A13);
  int value3 = analogRead(A14);
  int value4 = analogRead(A15);
  int soma = value1 + value2 + value3 + value4;
  int velocity = map(soma, 0, 500, 50, 127);
  velocity = constrain(velocity, 50, 127);

  for(int i = 0; i < 15; i++){
    buttonStates[i] = 1023-(int(digitalRead(buttonPins[i]))*1023);
    if(melodicMode){
      // MELODIC MODE
      vara = constrain(vara, 0, 1023);
      frequencia = map(vara, 0, 1023, 60, 100);
      frequencia = int(frequencia);
      if(buttonStates[i] > 100 && !locks[i]){
        //tone(8, frequencia);
        locks[i] = true;
        locksTurn[i] = false;
        ultimaNota[i] = calculateToSend(i, frequencia);
        MIDI.sendNoteOn(ultimaNota[i],velocity,1);
      }

      if(buttonStates[i] < 20 && !locksTurn[i]){
        //noTone(8);
        locks[i] = false;
        locksTurn[i] = true;
        MIDI.sendNoteOff(ultimaNota[i],velocity,1);
        ultimaNota[i] = 0;
      }
    } 
    else {
      // PERCUSSION MODE
      if(buttonStates[i] > 100 && !locks[i]){
        //tone(8, frequencia);
        locks[i] = true;
        locksTurn[i] = false;
        ultimaNota[i] = calculateToSend(i, 60);
        MIDI.sendNoteOn(ultimaNota[i],velocity,1);
      }
      if(buttonStates[i] < 20 && !locksTurn[i]){
        //noTone(8);
        locks[i] = false;
        locksTurn[i] = true;
        MIDI.sendNoteOff(ultimaNota[i],velocity,1);
        ultimaNota[i] = 0;
      }
    }
  }

  if(!melodicMode){
    if(abs(lastVara - vara) > 1){
      int ccToSend = map(vara, 0, 1023, 0, 127);
      MIDI.sendControlChange(1, ccToSend, 1);  
    }
  }

  lastVara = vara;
  delay(10);
}

int calculateToSend(int index, int freq){
  int result = 0;

  if(index <= 5){
    result = frequencia;
  } 
  else if(index <= 10){
    result = frequencia + 4;
  } 
  else if(index <= 15){
    result = frequencia + 7;
  }

  return result;
}












