#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

/*
  Batebit Artesania Digital
  Pandiva #3
  Usando os botões Sanwa OBSF-30
  Tres modos de funcionamento
  Versão feita para Abertura do Carnaval 2018
*/

int notasToAbletonLiveDrumRack[24][3] = {
  //PRETO, MARROM, BRANCO
  //======= MODE 1
  {36, 48, 60}, // xxxx
  {38, 50, 62}, // Oxxx
  {40, 52, 64}, // xOxx
  {41, 53, 65}, // xxOx
  {43, 55, 67}, // xxxO
  {45, 57, 69}, // OOxx
  {47, 59, 71}, // xOOx
  {48, 60, 72}, // xxOO
  //======= MODE 2
  {48, 53, 57},
  {48, 52, 55},
  {50, 55, 59},
  {53, 57, 60},
  {52, 55, 60},
  {55, 59, 62},
  {48, 53, 57},
  {48, 52, 55},
  //======= MODE 3
  {60, 65, 69},
  {60, 64, 67},
  {62, 67, 71},
  {65, 69, 72},
  {64, 67, 72},
  {67, 71, 74},
  {60, 65, 69},
  {60, 64, 67}
};

int buttonPins[] = {
  22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
};

boolean locks[] = {
  false, false, false, false, false, false, false, false, false, false, false, false
};
boolean locksTurn[] = {
  true, true, true, true, true, true, true, true, true, true, true, true
};

int buttonStates[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int lastVelocity[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int lastVelocity2[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int ultimaNota[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// ===== CONFIG OF THE SELECTORS =====
int quantSelectors = 4;
//int selectorPins[] = {11,10,9,8};
//int selectorLedPins[] = {7,6,5,4};
int selectorPins[] = {8, 9, 10, 11};
int selectorLedPins[] = {4, 5, 6, 7};
boolean locksSelector[] = {
  false, false, false, false
};
boolean locksTurnSelector[] = {
  true, true, true, true
};
int selectorStates[] = {
  0, 0, 0, 0
};
// ==========

int frequencia = 0;
int lastVara = 0 ;

const int MELODIC = 0;
const int PERCUSSION = 1;
const int CHORDS = 2;
int mode = MELODIC;
boolean lockModeButton = false;

int quantButtons = 12;
int ledConfig = 13;
int pinButtonConfig = 2;
int picoDaSoma = 3500;

void setup() {
  MIDI.begin();
  //Serial.begin(115200);
  for (int i = 0; i < quantButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(ledConfig, OUTPUT); //LED CONFIG
  pinMode(pinButtonConfig, INPUT_PULLUP); //BUTTON CONFIG


  // ===== SELECTORS =====
  for (int i = 0; i < quantSelectors; i++) {
    pinMode(selectorLedPins[i], OUTPUT);
    pinMode(selectorPins[i], INPUT_PULLUP);
  }
  // ==========

  //SEND NOTE DO DEBUG
  MIDI.sendNoteOn(100, 127, 1);
  delay(100);
  MIDI.sendNoteOff(100, 0, 1);
}

void loop() {

  int modeButtonState = digitalRead(pinButtonConfig);

  if (modeButtonState == LOW && !lockModeButton) {
    mode = (mode + 1) % 3;
    lockModeButton = true;
  }

  if (modeButtonState == HIGH) {
    lockModeButton = false;
  }

  if (mode == MELODIC) {
    digitalWrite(ledConfig, LOW);
  }
  else if (mode == PERCUSSION) {
    analogWrite(ledConfig, 10);
  }
  else if (mode == CHORDS) {
    analogWrite(ledConfig, 255);
  }

  int selector = readSelectors();

//  int value1 = analogRead(A10);
//  int value2 = analogRead(A11);
//  int value3 = analogRead(A12);
//  int value4 = analogRead(A13);
//  int value5 = analogRead(A14);
//  int value6 = analogRead(A15);
//  int soma = value1 + value2 + value3 + value4 + value5 + value6;
//  int velocity = map(soma, 0, picoDaSoma, 50, 127);
//  velocity = constrain(velocity, 50, 127);
  int velocity = 127;

  for (int i = 0; i < quantButtons; i++) {
    buttonStates[i] = 1023 - (int(digitalRead(buttonPins[i])) * 1023);
    if (buttonStates[i] > 100 && !locks[i]) {
      //tone(8, frequencia);
      locks[i] = true;
      locksTurn[i] = false;
      ultimaNota[i] = calculateToSend(i, selector);
      lastVelocity[i] = velocity;
      MIDI.sendNoteOn(ultimaNota[i], velocity, 1);
    }

    if (buttonStates[i] < 20 && !locksTurn[i]) {
      //noTone(8);
      locks[i] = false;
      locksTurn[i] = true;
      MIDI.sendNoteOff(ultimaNota[i], velocity, 1);
      ultimaNota[i] = 0;
    }
  }

  delay(10);
}


int calculateToSend(int index, int selector) {
  MIDI.sendControlChange(1, selector, 1);
  int result = 0;
  if (index <= 3) {
    result = notasToAbletonLiveDrumRack[selector + mode * 8][0];
  }
  else if (index <= 7) {
    result = notasToAbletonLiveDrumRack[selector + mode * 8][1];
  }
  else if (index <= 11) {
    result = notasToAbletonLiveDrumRack[selector + mode * 8][2];
  }
  return result;
}

int calculateVelocityByButtonsPressed(int index){
  
}

int readSelectors() {
  int result = 0;
  int somaAux = 0;

  if (digitalRead(selectorPins[0]) == LOW) {
    somaAux = somaAux + 1;
    digitalWrite(selectorLedPins[0], HIGH);
  }
  else {
    digitalWrite(selectorLedPins[0], LOW);
  }


  if (digitalRead(selectorPins[1]) == LOW) {
    somaAux = somaAux + 2;
    digitalWrite(selectorLedPins[1], HIGH);
  }
  else {
    digitalWrite(selectorLedPins[1], LOW);
  }


  if (digitalRead(selectorPins[2]) == LOW) {
    somaAux = somaAux + 4;
    digitalWrite(selectorLedPins[2], HIGH);
  }
  else {
    digitalWrite(selectorLedPins[2], LOW);
  }

  if (digitalRead(selectorPins[3]) == LOW) {
    somaAux = somaAux + 8;
    digitalWrite(selectorLedPins[3], HIGH);
  }
  else {
    digitalWrite(selectorLedPins[3], LOW);
  }

  switch (somaAux) {
    case 0:
      result = 0;
      break;
    case 1:
      result = 1;
      break;
    case 2:
      result = 2;
      break;
    case 4:
      result = 3;
      break;
    case 8:
      result = 4;
      break;
    case 3:
      result = 5;
      break;
    case 6:
      result = 6;
      break;
    case 12:
      result = 7;
      break;
    default:
      result = 0;
  }

  return result;
}

// ====== METODOS RENEGADOS
void setLED(int hue, int l) {
  int col[3] = {
    0, 0, 0
  };
  getRGB(hue, 255, l, col);
  ledWrite(col[0], col[1], col[2]);
}

void getRGB(int hue, int sat, int val, int colors[3]) {
  // hue: 0-259, sat: 0-255, val (lightness): 0-255
  int r, g, b, base;

  if (sat == 0) { // Achromatic color (gray).
    colors[0] = val;
    colors[1] = val;
    colors[2] = val;
  }
  else  {
    base = ((255 - sat) * val) >> 8;
    switch (hue / 60) {
      case 0:
        r = val;
        g = (((val - base) * hue) / 60) + base;
        b = base;
        break;
      case 1:
        r = (((val - base) * (60 - (hue % 60))) / 60) + base;
        g = val;
        b = base;
        break;
      case 2:
        r = base;
        g = val;
        b = (((val - base) * (hue % 60)) / 60) + base;
        break;
      case 3:
        r = base;
        g = (((val - base) * (60 - (hue % 60))) / 60) + base;
        b = val;
        break;
      case 4:
        r = (((val - base) * (hue % 60)) / 60) + base;
        g = base;
        b = val;
        break;
      case 5:
        r = val;
        g = base;
        b = (((val - base) * (60 - (hue % 60))) / 60) + base;
        break;
    }
    colors[0] = r;
    colors[1] = g;
    colors[2] = b;
  }
}

void ledWrite(int r, int g, int b) {
  //analogWrite(red, 255-r);
  //analogWrite(green, 255-g);
  //analogWrite(blue, 255-b);
}
