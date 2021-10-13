// INCLUSÃO DE BIBLIOTECAS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
// DEFINIÇÕES
#define endereco  0x27 // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2
#define pinSensorD 13
#define verde 6
#define amarelo 7
#define vermelho 12

const byte ROWS = 4;
const byte COLS = 4;

char keys [ROWS] [COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad teclado = Keypad (makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

float leituraSensor;
float leituraAnterior;
int valor_umidade;
String leitura_teclas;

void setup() {
  lcd.init(); // INICIA A COMUNICAÇÃO COM O DISPLAY
  lcd.backlight(); // LIGA A ILUMINAÇÃO DO DISPLAY
  pinMode(vermelho, OUTPUT);  //vermelho
  pinMode(amarelo, OUTPUT);  //amarelo
  pinMode(verde, OUTPUT);  //verde
  pinMode(pinSensorD, INPUT); //entrada digital do sensor
  
  Serial.begin(9200);
  config_umidade();
}
void loop() {
  //digitalRead(pinSensorD);
  //analogRead(A0);
  leituraSensor = analogRead(A0);
  Serial.println(leituraSensor);
  leituraSensor = leituraSensor*100.;
  leituraSensor = (leituraSensor/1023);
  leituraSensor = 100 - leituraSensor;
 
  if (leituraSensor <= valor_umidade) {
     //No estado seco
     digitalWrite(vermelho, HIGH);  //vermelho
     digitalWrite(amarelo, LOW);   //Amarelo
     digitalWrite(verde, LOW);   //verde
     delay(500);
     
     //Ao entrar no estado seco  
     while (leituraSensor <= valor_umidade) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Umidade do solo: ");
        lcd.setCursor(0,1);
        lcd.print(leituraSensor);
        lcd.print("% - Baixo");
        delay(5000);
        
       // digitalWrite(6, HIGH);   //rele / válvula / solenoide / bomba
        digitalWrite(vermelho, LOW);  //vermelho
        digitalWrite(amarelo, HIGH);  //amarelo
        
        lcd.clear();
        lcd.print("Regando...");
        delay(2000); 
        
        //digitalWrite(6, LOW);   //rele / válvula / solenoide / bomba         
        lcd.clear();
        lcd.print("Verificando...");
        delay(3000);
        digitalWrite(amarelo, LOW);
        digitalWrite(vermelho, HIGH);
        
        Serial.println(analogRead(A0));
        leituraSensor = analogRead(A0);
        leituraSensor = leituraSensor*100.;
        leituraSensor = (leituraSensor/1023);
        leituraSensor = 100 - leituraSensor;
    }
  } 
  else {
     //No estado úmido
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Umidade do solo: ");
     lcd.setCursor(0,1);
     lcd.print(leituraSensor);
     lcd.print("% - Alto");
     
    // digitalWrite(6, LOW);   //rele / válvula / solenoide / bomba       
     digitalWrite(vermelho, LOW);   //vermelho
     digitalWrite(amarelo, LOW);   //Amarelo
     digitalWrite(verde, HIGH);  //verde
     delay(3000);     
  }
  leituraAnterior = leituraSensor;
}

void config_umidade(){
  lcd.setCursor(0,0);
  lcd.print("1.Umidade padrao");
  lcd.setCursor(0,1);
  lcd.print("2.Define umidade");

  char opcao = teclado.waitForKey();
  if(opcao == '1'){
    lcd.clear();
    valores_padrao();
  }
  else if (opcao == '2'){
    lcd.clear();
    lcd.print(opcao);
    receber_umidade();
  }
  else{
    lcd.clear();
    lcd.print("Opcao nao existe");
    delay(2000);
    lcd.clear();
    config_umidade();
  }
}

void receber_umidade(){
  int cont=0;
  lcd.setCursor(0,0);
  lcd.print("Informe umidade");
  lcd.setCursor(0,1);  
  lcd.print("ideal da planta:");
  //delay(2500);
  
  while(cont<2){
      char digitado = teclado.waitForKey();  
       //caso a tecla C (clear) tenha sido pressionada
       if(digitado == 'C'){
          leitura_teclas = "";
          lcd.clear();
          cont = 0;
      }else{
         leitura_teclas += digitado; //concatena o que foi digitado
         lcd.clear();
         lcd.print(leitura_teclas);
         cont++;
      }
    }
    delay(600);
    valor_umidade = leitura_teclas.toInt();
}

int valores_padrao(){
    lcd.setCursor(0,0);
    lcd.print("Utilizando valor");
    lcd.setCursor(0,1);
    lcd.print("padrao...");
    delay(2000);
    valor_umidade = 30; 
}
