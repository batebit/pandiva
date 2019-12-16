/*
  Batebit Artesania Digital
 Pandiva v0.0.7
 Multiplos Botoes
 Tres modos de funcionamento
 Adicionar movimento de vara glissando
 Com LUZ na ponta da vara
 */

#include <MIDI.h>

int red = 5;
int green = 6;
int blue = 7;

int escala[] = {
  48,50,52,55,57,60,62,64,67,69,72,74,76,79,81};
int escalaLuz[] = {
  1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};

//int acordes1[] = {
//  48,50,52,53,55,57,59};
//int acordes2[] = {
//  52,53,55,57,59,60,62};
//int acordes3[] = {
//  55,57,59,60,62,64,65};
//int acordesLuz[] ={
//  1,0,1,0,1,0,1};

//int acordes = 9;
//int acordes1[] = {
//  48,48,50,53,52,55,57,55,59};
//int acordes2[] = {
//  53,52,55,57,55,59,60,60,62};
//int acordes3[] = {
//  57,55,59,60,60,62,65,64,67};
//int acordesLuz[] ={
//  1,0,1,0,1,0,1,0,1};

int acordes = 6;
int acordes1[] = {
  48,48,50,53,52,55};
int acordes2[] = {
  53,52,55,57,55,59};
int acordes3[] = {
  57,55,59,60,60,62};
int acordesLuz[] ={
  1,0,1,0,1,0};

int buttonPins[] = {
  22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};


boolean locks[] = {
  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
boolean locksTurn[] = {
  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

int buttonStates[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int lastVelocity[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int lastVelocity2[] = {
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
  //Serial.begin(115200);
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
  if(abs(lastVara - vara) < 10){
    vara = lastVara;
  }
  int value1 = analogRead(A12);
  int value2 = analogRead(A13);
  int value3 = analogRead(A14);
  int value4 = analogRead(A15);
  int soma = value1 + value2 + value3 + value4;
  int velocity = map(soma, 0, 450, 65, 127);
  velocity = constrain(velocity, 65, 127);

  int color = map(vara, 0, 1023, 0, 255);
  //analogWrite(red, 156);
  //analogWrite(green, 119);
  //analogWrite(blue, 101);

  for(int i = 0; i < 15; i++){
    buttonStates[i] = 1023-(int(digitalRead(buttonPins[i]))*1023);
    if(mode == MELODIC){
      if(escalaLuz[int(map(vara, 0, 1023, 0, 15))] == 1){
        analogWrite(red, 156);
        analogWrite(green, 119);
        analogWrite(blue, 101);
      } 
      else {
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        digitalWrite(blue, LOW);
      }
      // MELODIC MODE
      vara = constrain(vara, 0, 1023);
      if(buttonStates[i] > 100 && !locks[i]){
        //tone(8, frequencia);
        locks[i] = true;
        locksTurn[i] = false;
        ultimaNota[i] = calculateToSend(i, vara);
        lastVelocity[i] = velocity;
        MIDI.sendNoteOn(ultimaNota[i],velocity,1);
      }

      if(buttonStates[i] < 20 && !locksTurn[i]){
        //noTone(8);
        locks[i] = false;
        locksTurn[i] = true;
        MIDI.sendNoteOff(ultimaNota[i],velocity,1);
        ultimaNota[i] = 0;
      }
      // SE LIGAR AQUI
      if(buttonStates[i] > 100 && locks[i]){
        if(abs(lastVara - vara) > 2){
          MIDI.sendNoteOff(ultimaNota[i],velocity,1);
          ultimaNota[i] = calculateToSend(i, vara);
          MIDI.sendNoteOn(ultimaNota[i],55,1);
        }
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
      if(acordesLuz[int(map(vara, 0, 1023, 0, 5))] == 1){
        analogWrite(red, 156);
        analogWrite(green, 119);
        analogWrite(blue, 101);
      } 
      else {
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        digitalWrite(blue, LOW);
      }
      
      // CHORDS MODE
      if(buttonStates[i] > 100 && !locks[i]){
        //tone(8, frequencia);
        locks[i] = true;
        locksTurn[i] = false;
        ultimaNota[i] = calculateToSend(i, vara);
        lastVelocity[i] = velocity;
        MIDI.sendNoteOn(ultimaNota[i],velocity,1);
      }
      if(buttonStates[i] < 20 && !locksTurn[i]){
        //noTone(8);
        locks[i] = false;
        locksTurn[i] = true;
        MIDI.sendNoteOff(ultimaNota[i],velocity,1);
        ultimaNota[i] = 0;
      }

      if(buttonStates[i] > 100 && locks[i]){
        if(abs(lastVara - vara) > 3){
          lastVara = vara;
          vara = analogRead(A0);
          MIDI.sendNoteOff(ultimaNota[i],0,1);
          ultimaNota[i] = calculateToSend(i, vara);
          MIDI.sendNoteOn(ultimaNota[i],55,1);
        }
      }

    }
  }

  // PENSANDO OS MOVIMENTOS DE VARA
  if(mode == MELODIC){

  } 
  else if(mode == PERCUSSION) {
    if(abs(lastVara - vara) > 1){
      int ccToSend = map(vara, 0, 1023, 0, 127);
      MIDI.sendControlChange(1, ccToSend, 1);  
    }
  } 
  else if(mode == CHORDS){

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

    int indiceAcordes = map(vara, 0, 1023, 0, acordes-1);
    indiceAcordes = int(indiceAcordes);
    indiceAcordes = constrain(indiceAcordes, 0, acordes-1);
    int nota1 = acordes1[indiceAcordes];
    int nota2 = acordes2[indiceAcordes];
    int nota3 = acordes3[indiceAcordes];
    if(index <= 5){
      result = nota1;
    } 
    else if(index <= 10){
      result = nota2;
    } 
    else if(index <= 15){
      result = nota3;
    }
  }
  return result;
}

void setLED(int hue, int l){
  int col[3] = {
    0,0,0      };
  getRGB(hue, 255, l, col);
  ledWrite(col[0], col[1], col[2]);
}

void getRGB(int hue, int sat, int val, int colors[3]) {
  // hue: 0-259, sat: 0-255, val (lightness): 0-255
  int r, g, b, base;

  if (sat == 0) { // Achromatic color (gray).
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;
  } 
  else  {
    base = ((255 - sat) * val)>>8;
    switch(hue/60) {
    case 0:
      r = val;
      g = (((val-base)*hue)/60)+base;
      b = base;
      break;
    case 1:
      r = (((val-base)*(60-(hue%60)))/60)+base;
      g = val;
      b = base;
      break;
    case 2:
      r = base;
      g = val;
      b = (((val-base)*(hue%60))/60)+base;
      break;
    case 3:
      r = base;
      g = (((val-base)*(60-(hue%60)))/60)+base;
      b = val;
      break;
    case 4:
      r = (((val-base)*(hue%60))/60)+base;
      g = base;
      b = val;
      break;
    case 5:
      r = val;
      g = base;
      b = (((val-base)*(60-(hue%60)))/60)+base;
      break;
    }
    colors[0]=r;
    colors[1]=g;
    colors[2]=b;
  }
}

void ledWrite(int r, int g, int b){
  analogWrite(red, 255-r);
  analogWrite(green, 255-g);
  analogWrite(blue, 255-b);
}





















