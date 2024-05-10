#include <ACS712.h>
#include <SD.h>
#include <SPI.h>
#include "DHT.h"

#define acs712 A0

#define CS_pin 10 //Definição do pino de entrada do MicroSD Card Adapter

#define pinmotor 5 //Definição do pino do motor
#define maxvelocidade 255 //Definição da velocidade maxima
#define vel 0 //Definição da velocidade mínima
#define saltosdavelocidade 5 


float velocidade = 0;
int i;

int valor =0;
const int sensorValue =A0;
float Tempo=00;

float voltagem = 0.0; //Criação da variável voltagem global
float corrente = 0.0; //Criação da variável corrente global
float potencia = 0.0; //Criação da variável potencia global
float tensaoDC = 7.2; //Criação da variável tensao global
float adc =0;

 //Criação das variàveis para medir o tempo percorrido

File sd_file; //Criação de uma variável sd_file

void setup() {
  Serial.begin(9600);  //Inicizalização do módulo serial
  
  pinMode(acs712, INPUT);

  pinMode(pinmotor, OUTPUT); //Identificação do pino digital do motor como OUTPUT

  if (SD.begin(10)) { //Verificação da inicialização correta do SD
    Serial.println("SD card is initialized. Ready to go");
  } else{
    Serial.println("Failed loading"); //Falha se o SD não inicializar
    return;
  }
  File sd_file = SD.open("corrente.csv", FILE_WRITE); //Abrindo o SD para escrita

  //Impressão do cabeçalho do arquivo SD
  if (sd_file) { 
    sd_file.print("Tempo");
    sd_file.print(",");
    sd_file.print("Corrente");
    sd_file.println();
  }
  sd_file.close();

}

void loop() {
  //Essa série de While foi um código que o pessoal da eletrônica tentou reproduzir para controlar a velocidade do motor.
  //Minha ideia foi que eu chamasse a função senddata() toda vez que um laço desse while fosse chamado também e ficasse um tempo
  //determinado coletando as correntes do circuito, foi por isso também que se você notar não existe delays na função loop
  //e sim delays dentro do senddata()
  //Os valores de quanto tempo o laço irá durar, assim como o tempo do delay que será padronizado para a função senddata irá
  //depender de como a eletrônica está pensando em realizar esse controle.
  analogWrite(pinmotor, velocidade);
  
  if(velocidade == 255){
    senddata(Tempo);
    Tempo  = Tempo+0.25;
  } else{
    velocidade = velocidade + saltosdavelocidade;
    senddata(Tempo);
    Tempo = Tempo+0.25;
  }
}


void senddata(float Tempo){ //Função para leitura do sensor ACS712 e para Armazenamento no SD
  float tempoatual = 0.0; // Armazenamento do tempo atual
  File sd_file = SD.open("corrente.csv", FILE_WRITE); //Abrindo o sd_file para escrita
  float MediaCorrente = 0.0; // Variável para armazenar a média das correntes
  float Medidas = 0.0; //Variável para Realizar a divisão das médias
  int segundos = Tempo; 
  // For com 250 milisegundos de duração, com 25 medidas feitas, com as medias dessas medidas
  for(float seconds =0.0; seconds<0.250; seconds = seconds+0.01){
    
    float adc = analogRead(A0);

    voltagem = (5.000/1023.000)*(adc); // Transformação pra voltagem
    voltagem = voltagem -2.5; //Exclui os valores negativos

    //voltagem = (analogRead(A0) - (510))*(5.0/(1023.0*0.185)); //Leitura da porta analógica A0 e correções

    corrente = ((voltagem*1000)/0.185); //Cálculo da corrente

    MediaCorrente = corrente +MediaCorrente; //Somatório de todas iterações feitas dentro deste for

    Medidas = voltagem + Medidas;

    delay(10);
  
  }
  tempoatual=0.250 + Tempo; //Atualização do tempo atual

  MediaCorrente = MediaCorrente/25; //Dividindo os valores obtidos da média
  Medidas =  Medidas/25;

  Serial.print("Corrente: "); //Printando a corrente no Serial
  Serial.println(MediaCorrente);

  //Serial.print("Voltagem: ");
  //Serial.print(Medidas);

  sd_file.print(tempoatual); //Printando o tempo atual no SD
  sd_file.print(",");
  sd_file.print(MediaCorrente); //Printando a MédiaCorrente no SD
  sd_file.println();

  //Serial.print("Tempo (s): "); //Printando o tempo no Serial
  //Serial.print(tempoatual);
  //Serial.print(",  ");
    // Serial.print(corrente);

  Serial.println();

  sd_file.flush();
    
  sd_file.close();
}