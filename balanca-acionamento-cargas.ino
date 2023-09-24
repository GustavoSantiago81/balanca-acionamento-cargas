#include "HX711.h"                    // Biblioteca HX711 
#define DOUT  A0                      // HX711 DATA OUT = pino A0 do Arduino 
#define CLK  A1                       // HX711 SCK IN = pino A1 do Arduino 
HX711 balanca;          // define instancia balança HX711
float calibration_factor = 36040;     // fator de calibração para teste inicial
float peso = 0;
int saida = 4;

void setup()
{
  pinMode (saida, OUTPUT);
  Serial.begin(9600);            // monitor serial 9600 Bps
  balanca.begin(DOUT, CLK);      // inicializa a balança
  Serial.println();              // salta uma linha
  Serial.println("Pressione + ou A,S,D,F para aumentar Fator de Calibração por 10,100,1000,10000 respectivamente");
  Serial.println("Pressione - ou Z,X,C,V para diminuir Fator de Calibração por 10,100,1000,10000 respectivamente");
  Serial.println("Após leitura correta do peso, pressione t para TARA(zerar) ");
  delay(1000);
  balanca.set_scale();                                             // configura a escala da Balança
  zeraBalanca ();                                                  // zera a Balança
}
void zeraBalanca ()
{
  Serial.println();                                               // salta uma linha
  balanca.tare();                                                 // zera a Balança
  Serial.println("Balanca Zerada ");
}
void loop()
{
  balanca.set_scale(calibration_factor);                     // ajusta fator de calibração
  peso = balanca.get_units();


  Serial.print("Peso: ");                                    // imprime no monitor serial
  Serial.print(balanca.get_units(), 3);                      // imprime peso da balança com 3 casas decimais

  Serial.print(" kg");
  Serial.print("      Fator de Calibracao: ");               // imprime no monitor serial
  Serial.println(calibration_factor);                        // imprime fator de calibração

  delay(500) ;                                               // atraso de 0,5 segundo

  if (Serial.available())                                    // reconhece letra para ajuste do fator de calibração
  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')                // a = aumenta 10
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')           // z = diminui 10
      calibration_factor -= 10;
    else if (temp == 's')                          // s = aumenta 100
      calibration_factor += 100;
    else if (temp == 'x')                          // x = diminui 100
      calibration_factor -= 100;
    else if (temp == 'd')                          // d = aumenta 1000
      calibration_factor += 1000;
    else if (temp == 'c')                          // c = diminui 1000
      calibration_factor -= 1000;
    else if (temp == 'f')                          // f = aumenta 10000
      calibration_factor += 10000;
    else if (temp == 'v')                          // v = dimuni 10000
      calibration_factor -= 10000;
    else if (temp == 't') zeraBalanca ();          // t = zera a Balança

  }

  if (peso >= 25.000)
  {
    digitalWrite (saida, 1);
  }
  else
  {
    digitalWrite (saida, 0);
  }
}
