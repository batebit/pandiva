/*
  Batebit Artesania Digital
 Pandiva v0.0.4
 Multiplos Botoes
 */

#include <MIDI.h>



int escala[] = {
  48,50,52,55,57,60,62,64,67,69,72,74,76,79,81};

int acordes1[] = {};
int acordes2[] = {};
int acordes3[] = {};

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

const int MELODIC = 0;
const int PERCUSSION = 1;
const int CHORDS = 2;
int mode = MELODIC;
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
    mode = (mode+1)%3;
    lockModeButton = true;
  }

  if(modeButtonState == HIGH){
    lockModeButton = false;
  }

  if(mode == MELODIC){
    digitalWrite(13, LOW);
  } 
  else if(mode == PERCUSSION) {
    analogWrite(13, 10);
  } 
  else if(mode == CHORDS){
    analogWrite(13, 255);
  }

  int vara = analogRead(A0);
  int value1 = analogRead(A12);
  int value2 = analogRead(A13);
  int value3 = analogRead(A14);
  int value4 = analogRead(A15);
  int soma = value1 + value2 + value3 + value4;
  int velocity = map(soma, 0, 450, 50, 127);
  velocity = constrain(velocity, 50, 127);

  for(int i = 0; i < 15; i++){
    buttonStates[i] = 1023-(int(digitalRead(buttonPins[i]))*1023);
    if(mode == MELODIC){
      // MELODIC MODE
      vara = constrain(vara, 0, 1023);
      if(buttonStates[i] > 100 && !locks[i]){
        //tone(8, frequencia);
        locks[i] = true;
        locksTurn[i] = false;
        ultimaNota[i] = calculateToSend(i, vara);
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
    else if(mode == PERCUSSION) {
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
    else if(mode == CHORDS){
      // CHORDS MODE
      if(buttonStates[i] > 100 && !locks[i]){
        //tone(8, frequencia);
        locks[i] = true;
        locksTurn[i] = false;
        ultimaNota[i] = calculateToSend(i, vara);
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

  if(mode == PERCUSSION){
    if(abs(lastVara - vara) > 1){
      int ccToSend = map(vara, 0, 1023, 0, 127);
      MIDI.sendControlChange(1, ccToSend, 1);  
    }
  }

  lastVara = vara;
  delay(10);
}

int calculateToSend(int index, int vara){
  int result = 0;
  if(mode == MELODIC){
    int indiceEscala = map(vara, 0, 1023, 0, 14);
    indiceEscala = int(indiceEscala);
    indiceEscala = constrain(indiceEscala, 0, 14);
    int notaMIDI = escala[indiceEscala];
    if(index <= 5){
      result = notaMIDI;
    } 
    else if(index <= 10){
      result = notaMIDI + 4;
    } 
    else if(index <= 15){
      result = notaMIDI + 7;
    }
  } 
  else if(mode == PERCUSSION) {
    if(index <= 5){
      result = 60;
    } 
    else if(index <= 10){
      result = 61;
    } 
    else if(index <= 15){
      result = 62;
    }
  } 
  else if(mode == CHORDS){
    
    int indiceAcordes = map(vara, 0, 1023, 0, 14);
    indiceAcordes = int(indiceAcordes);
    indiceAcordes = constrain(indiceAcordes, 0, 14);
    int nota1 = acordes1[indiceAcordes];
    int nota2 = acordes2[indiceAcordes];
    int nota3 = acordes3[indiceAcordes];
    if(index <= 5){
      result = 60;
    } 
    else if(index <= 10){
      result = 61;
    } 
    else if(index <= 15){
      result = 62;
    }
  }
  return result;
}


















