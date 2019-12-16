/*
  Batebit Artesania Digital
 Pandiva v0.0.1
 */

boolean lightStatus = false;
boolean lockButton1 = false;
boolean lockTurnButton1 = true;
boolean lockButton2 = false;
boolean lockTurnButton2 = true;
boolean lockButton3 = false;
boolean lockTurnButton3 = true;
int frequencia = 0;

int pinButton1 = 5;
int pinButton2 = 6;
int pinButton3 = 4;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(pinButton1, INPUT_PULLUP);
  pinMode(pinButton2, INPUT_PULLUP);
  pinMode(pinButton3, INPUT_PULLUP);
  tone(8,440);
  delay(50);
  noTone(8);
}

void loop() {

  int vara = analogRead(A0);

  int button1 = 1023-(int(digitalRead(pinButton1))*1023);
  int button2 = 1023-(int(digitalRead(pinButton2))*1023);
  int button3 = 1023-(int(digitalRead(pinButton3))*1023);

  frequencia = map(vara, 0, 1023, 200, 3000);

  if(button1 > 512){
    tone(8, frequencia);
    lockButton1 = true;
  } else {
    lockButton1 = false;
  }
  
  if(button2 > 512){
    tone(8, (frequencia*3)/2);
    lockButton2 = true;
  } else {
    lockButton2 = false;
  }
  
  if(button3 > 512){
    tone(8, (frequencia*5)/4);
    lockButton3 = true;
  } else {
    lockButton3 = false;
  }
  
  if(!lockButton1 && !lockButton2 && !lockButton3){
    noTone(8);
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





