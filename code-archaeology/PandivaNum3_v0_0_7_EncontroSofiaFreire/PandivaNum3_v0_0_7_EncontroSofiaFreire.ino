#include <MIDI.h>
#include <Bounce2.h>

MIDI_CREATE_DEFAULT_INSTANCE();

/*
  Batebit Artesania Digital
  Pandiva #3
  Usando os botões Sanwa OBSF-30
  Tres modos de funcionamento
  Versão feita para o Jardim Muzak
  Trabalhando com eventos de teclado
*/

const size_t EXCITATION_BUTTONS_SIZE = 12;

int notasToAbletonLiveDrumRack[36][4] = {
  //PRETO (grave), MARROM (médio grave, médio agudo), BRANCO (agudo)
  //======= MODE 1
  {43, 55, 58, 62}, // xxxx
  {41, 53, 57, 62}, // Oxxx
  {39, 51, 55, 62}, // xOxx
  {38, 50, 57, 62}, // xxOx
  {36, 51, 55, 62}, // xxxO
  {36, 51, 56, 62}, // OOxx
  {36, 51, 57, 62}, // xOOx
  {36, 51, 56, 62}, // xxOO
  {38, 50, 57, 62}, // OxOx
  {38, 50, 57, 62}, // xOxO
  {38, 50, 58, 63}, // OOOx
  {38, 50, 58, 63}, // xOOO
  //======= MODE 2
  {55, 67, 70, 74},
  {53, 65, 69, 74},
  {51, 63, 67, 74},
  {50, 62, 69, 74},
  {48, 63, 67, 74},
  {48, 63, 68, 74},
  {48, 63, 69, 74},
  {48, 63, 68, 74},
  {50, 62, 69, 74},
  {50, 62, 69, 74},
  {50, 62, 70, 75},
  {50, 62, 70, 75},
  //======= MODE 3
  {43, 55, 58, 62}, // xxxx
  {41, 53, 57, 62}, // Oxxx
  {39, 51, 55, 62}, // xOxx
  {38, 50, 57, 62}, // xxOx
  {36, 51, 55, 62}, // xxxO
  {36, 51, 56, 62}, // OOxx
  {36, 51, 57, 62}, // xOOx
  {36, 51, 56, 62}, // xxOO
  {38, 50, 57, 62},
  {38, 50, 57, 62},
  {38, 50, 58, 63},
  {38, 50, 58, 63},
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

int ultimaGroupNote[4] = {
  0, 0, 0, 0
};

int group[] = {
      0, 0, 0, 0, 1, 2, 2, 1, 3, 3, 3, 3
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
Bounce s1 = Bounce();
Bounce s2 = Bounce();
Bounce s3 = Bounce();
Bounce s4 = Bounce();
Bounce* buttons[4] = {&s1, &s2, &s3, &s4};

Bounce excitationButtons[EXCITATION_BUTTONS_SIZE] = {
  Bounce(), Bounce(), Bounce(), Bounce(),
  Bounce(), Bounce(), Bounce(), Bounce(),
  Bounce(), Bounce(), Bounce(), Bounce()
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

boolean readingPots = true;
int controlNumber1 = 1;
int controlNumber2 = 11;
int ccThreshold = 2;
int potValueThreshold = 4;
int lastCCMIDI1 = 0;
int lastCCMIDI2 = 0;
int lastPotValue1 = 0;
int lastPotValue2 = 0;

void setup() {
  MIDI.begin();
  //Serial.begin(115200);
  for (int i = 0; i < quantButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    excitationButtons[i].attach(buttonPins[i]);
    excitationButtons[i].interval(10);
  }
  pinMode(ledConfig, OUTPUT); //LED CONFIG
  pinMode(pinButtonConfig, INPUT_PULLUP); //BUTTON CONFIG


  // ===== SELECTORS =====
  for (int i = 0; i < quantSelectors; i++) {
    pinMode(selectorLedPins[i], OUTPUT);
    pinMode(selectorPins[i], INPUT_PULLUP);
    buttons[i]->attach(selectorPins[i]);
    buttons[i]->interval(5); // interval in ms
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

  //buttons[i]->update();

  //==== READING POTS and SENDING CCs ====
  if (readingPots == true) {
    int potValue1 = analogRead(A0);
    int potValue2 = analogRead(A1);
    int ccMIDI1 = map(potValue1, 0, 1023, 0, 127);
    int ccMIDI2 = map(potValue2, 0, 1023, 0, 127);
    //  if(abs(lastCCMIDI1 - ccMIDI1) >= ccThreshold){
    if (abs(lastPotValue1 - potValue1) >= potValueThreshold) {
      MIDI.sendControlChange(controlNumber1, ccMIDI1, 1);
    }
    if (abs(lastPotValue2 - potValue2) >= potValueThreshold) {
      MIDI.sendControlChange(controlNumber2, ccMIDI2, 1);
    }
    lastCCMIDI1 = ccMIDI1;
    lastCCMIDI2 = ccMIDI2;
    lastPotValue1 = potValue1;
    lastPotValue2 = potValue2;
  }
  //==== =========================== ====

  int velocity = 100;

  if (mode == MELODIC) {
    
    int notas[12][4] =
    { {43, 55, 58, 62}, // xxxx
      {41, 53, 57, 62}, // Oxxx
      {39, 51, 55, 62}, // xOxx
      {38, 50, 57, 62}, // xxOx
      {36, 51, 55, 62}, // xxxO
      {36, 51, 56, 62}, // OOxx
      {36, 51, 57, 62}, // xOOx
      {36, 51, 56, 62}, // xxOO
      {38, 50, 57, 62}, // OxOx
      {38, 50, 57, 62}, // xOxO
      {38, 50, 58, 63}, // OOOx
      {38, 50, 58, 63}  // xOOO
    };
    for (int i = 0; i < quantButtons; i++) {
      excitationButtons[i].update();
      if (excitationButtons[i].fell()) {
        if (isGroupPressed(i) == false) {
          ultimaGroupNote[group[i]] = calculateToSend_Gira(i, selector);
          lastVelocity[i] = velocity;
          MIDI.sendNoteOn(ultimaGroupNote[group[i]] + 12, velocity, 1);
        }
        locks[i] = true;
        locksTurn[i] = false;
      }
      if (excitationButtons[i].rose()) {
        locks[i] = false;
        locksTurn[i] = true;
        if (isGroupPressed(i) == false) {
          MIDI.sendNoteOff(ultimaGroupNote[group[i]] + 12, velocity, 1);
          //ultimaNota[i] = 0;
        }
      }
    }
  } else if (mode == PERCUSSION) {
    int solB[21] = {54, 56, 58, 59, 61, 63, 65, 66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85, 87, 89};
    int paraSeletores[4] = {0, 1, 3, 5}; // (1,2,4,6);
    int converteBotaoGrau_afinacao[3][12] = {
      {7, 1, 0, 6, 9, 3, 2, 8, 11, 5, 4, 13},
      {7, 1, 0, 6, 9, 3, 2, 2, 11, 5, 4, 4},
      {7, 1, 0, 6, 9, 2, 2, 2, 11, 6, 4, 4}
    };
    int umaDasTresOpcoesDeAfinacaoDeJoao = 0;

    for (int i = 0; i < quantButtons; i++) {
      excitationButtons[i].update();
      if (excitationButtons[i].fell()) {
        int seletorConstrained = constrain(selector, 0, 3);
        int nota = solB[paraSeletores[seletorConstrained] + converteBotaoGrau_afinacao[umaDasTresOpcoesDeAfinacaoDeJoao][i]];
        MIDI.sendNoteOn(nota, velocity, 1);
        ultimaNota[i] = nota;
      }

      if (excitationButtons[i].rose()) {
        MIDI.sendNoteOff(ultimaNota[i], velocity, 1);
      }
    }

  } else if (mode == CHORDS) {

  }


  delay(10);
}


//int calculateToSend(int index, int selector) {
//  //MIDI.sendControlChange(1, selector, 1);
//  int result = 0;
//  if (index <= 3) {
//    result = notasToAbletonLiveDrumRack[selector + mode * 8][0];
//  }
//  else if (index <= 7) {
//    result = notasToAbletonLiveDrumRack[selector + mode * 8][1];
//  }
//  else if (index <= 11) {
//    result = notasToAbletonLiveDrumRack[selector + mode * 8][2];
//  }
//  return result;
//}

int calculateToSend_Gira(int index, int selector) {
  //MIDI.sendControlChange(1, selector, 1);
  int result = 0;
  if (index <= 3) {
    //Era assim: result = notasToAbletonLiveDrumRack[selector + mode * 12][0];
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][0];
  }
  else if ((index == 4) || (index == 7)) {
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][1];
  }
  else if ((index == 5) || (index == 6)) {
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][2];
  }
  else if (index <= 11) {
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][3];
  }
  if (isGroupPressed(index)) {
    result = ultimaGroupNote[group[index]];
  }
  return result;
}

int calculateToSend_AurorasFlamboyants(int index, int selector) {
  //MIDI.sendControlChange(1, selector, 1);
  int result = 0;
  if (index <= 3) {
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][0];
  }
  else if ((index == 4) || (index == 7)) {
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][1];
  }
  else if ((index == 5) || (index == 6)) {
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][2];
  }
  else if (index <= 11) {
    result = notasToAbletonLiveDrumRack[selector + 0 * 12][3];
  }
  if (isGroupPressed(index)) {
    result = ultimaGroupNote[group[index]];
  }
  return result;
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
    case 5:
      result = 8;
      break;
    case 10:
      result = 9;
      break;
    case 7:
      result = 10;
      break;
    case 14:
      result = 11;
      break;
    default:
      result = 0;
  }

  return result;
}

boolean isGroupPressed(int index) {
  int currentGroup = group[index];
  boolean result = false;
  for (int i = 0; i < EXCITATION_BUTTONS_SIZE; i++) {
    if (group[i] == currentGroup) {
      result = result || locks[i];
    }
  }
  return result;
}


