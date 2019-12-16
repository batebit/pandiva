int buttonPins[] = {
  22,23,24,25,26,27,28,29,30,31,32,33};
  //int piezoPins[] = {A10,A11,A12,A13,A14,A15};

void setup(){
  for(int i = 0; i < 12; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
}

void loop(){
  char divisor = ';';
//  for(int i = 0; i < 6; i++){
//    Serial.print(analogRead(piezoPins[i]));
//    Serial.print(divisor);
//  }
  for(int i = 0; i < 12; i++){
    int estadoBotao = digitalRead(buttonPins[i]);
    Serial.print(estadoBotao);
    Serial.print(divisor);
  }
  Serial.println(" => FIM");
  //Serial.println("\t");
  delay(10); 
}



