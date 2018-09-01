#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "LapisServo.h"

#define _DEBUG


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


// calcular para cada servo o minimo e maximo
#define SERVOMIN  143 // this is the 'minimum' pulse length count (out of 4096) - 1msec/20msec = 4096/20
#define SERVOMAX  491 // this is the 'maximum' pulse length count (out of 4096) - 2/20 = 4096/10

#define QTD_SERVO 3



// variaveis para comunicação serial
String inputString = "";
bool stringComplete = false;

// variaveis de temporização
unsigned long ultimaAtualizacao = 0;
const long intervalo = 1000;            // em ms

// Criar cada servo individualmente
LapisServo listaServos[QTD_SERVO] = {
  LapisServo(pwm, 1, SERVOMIN, SERVOMAX, -60, 60),
  LapisServo(pwm, 2, SERVOMIN, SERVOMAX, -60, 60),
  LapisServo(pwm, 3, SERVOMIN, SERVOMAX, -60, 60),
};


void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando Programa...");
  Serial.println("DETEB 2018");
  Serial.println("LAPIS PEB/COPPE/UFRJ");

  pwm.begin();

  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates - servo usado funciona com 50Hz

  //Wire.setClock(400000); // para aumentar a velocidade de envio (i2c) para o controlador (pesquisar mais)

  delay(10);

  setAnguloInicial(0);

  Serial.println("Inicialização concluida");
}

void loop() {
  unsigned long dt = millis();

  // Caso a string tenha sido lida completamente, limpa-la e processa-la
  if (stringComplete) {
    stringComplete = false;
    processString(inputString);
    inputString = "";
  }

  // executar a cada 'intervalo' de tempo
  if (dt - ultimaAtualizacao >= intervalo) {
    ultimaAtualizacao = dt;

    // atualizar a posicao de cada servo
    for (int i = 0; i < QTD_SERVO; ++i) {
      listaServos[i].atualizar();
    }
#ifdef _DEBUG
    Serial.println("---------");
#endif
  }
}

/* Processa a string recebia, a principio uma sequencia de
   QTD_SERVOS caracteres: D; E; 0.
*/
bool processString(String str) {
  int tamanho = str.length();

#ifdef _DEBUG
  Serial.print("String recebida: ");
  Serial.println(str);

  Serial.print("Tamanho da string recebida: ");
  Serial.println(tamanho);
#endif

  if (tamanho - 1 != QTD_SERVO) {
    Serial.println("Tamanho da string recebida diferente do esperado.");
    return false;
  }

  for (int i = 0; i < QTD_SERVO; ++i) {
    char c = str[i];

    listaServos[i].mover(c);
  }

  return true;
}

void setAnguloInicial(float angulo) {
  for (int i = 0; i < QTD_SERVO; ++i) {
    listaServos[i].setAngulo(angulo);
  }
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
