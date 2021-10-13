#define pinLDR A0
#define led 3
#include <Servo.h> 
Servo servo1; //cria um novo objeto servo
void setup() {
  //Sensor
   pinMode(3, OUTPUT);
   servo1.attach(9); //conecta o objeto servo1 ao pino 9
   servo1.write(18); //Altere para posicionar o mais perto do botão do teclado
   Serial.begin(9200);
}              
void loop() {  
  int valorLDR = analogRead(A0);
  if(valorLDR < 450){
    digitalWrite(led, HIGH);    
    servo1.write(30); // envia sinal para o servo apertar o botão
    delay(100);                                 
  }
  else{
    digitalWrite(led, LOW);
    servo1.write(18);   //Volta para posição inicial
  }
  Serial.println(valorLDR);
}
