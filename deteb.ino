#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "LapisServo.h"

#define _DEBUG
#define SERIALCOM

#ifdef SERIALCOM
#include "SerialCom.h"
#else
#include "WifiCom.h"
#endif


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


// calcular para cada servo o minimo e maximo
#define SERVOMIN  143 // this is the 'minimum' pulse length count (out of 4096) - 1msec/20msec => 4096/20
#define SERVOMAX  491 // this is the 'maximum' pulse length count (out of 4096) - 2/20 => 4096/10


#define QTD_SERVO 3 // Mudar dependendo da quantidade de servo


SerialCom com = SerialCom();

// variaveis para comunicação serial
String inputString = "";
bool stringComplete = false;

// variaveis de temporização
unsigned long ultimaAtualizacao = 0;
const long intervalo = 100;            // em ms

// Criar cada servo individualmente
LapisServo listaServos[QTD_SERVO] = {
  // (PWM, pulse_min, pulse_max, inicial, min, max, passo)
  LapisServo(pwm, 0, 176, 359, 0, 0, 120, 1), // garra
  LapisServo(pwm, 1, 126, 495, 0, 0, 120, 5), // pulso
  LapisServo(pwm, 2, 126, 495, 0, 0, 120, 5), //
  //LapisServo(pwm, 3, 126, 495, 0, 0, 120, 5), // 
  //LapisServo(pwm, 4, 126, 495, 0, 0, 120, 5), // 
  //LapisServo(pwm, 5, 126, 495, 0, 0, 120, 5), // base
};


void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando Programa...");
  Serial.println("DETEB 2018");
  Serial.println("LAPIS PEB / COPPE / UFRJ");

  pwm.begin();

  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates - servo usado funciona com 50Hz

  //Wire.setClock(400000); // para aumentar a velocidade de envio (i2c) para o controlador (pesquisar mais)

  delay(10);

  reiniciarServos();

  Serial.println("Inicialização concluida");
}

void loop() {
  unsigned long dt = millis();

  lerSerial();

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
    //Serial.println("----------");
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
  Serial.print(str);
#endif

  if (tamanho == 2) { // RESET
    if (str[0] == 'R') {
      reiniciarServos();
      return true;
    }
  }
  else if (tamanho - 1 == QTD_SERVO * 2) { // Comando
    for (int i = 0; i < QTD_SERVO * 2; i++) {
      char c1 = str[(i * 2)];
      char c2 = str[(i * 2) + 1];

      if (c1 == '0') {
        listaServos[i].mover(c2);
      } else {
        listaServos[i].mover(c1);
      }
    }

    return true;
  } else {
    Serial.println("Tamanho da string recebida diferente do esperado.");
    return false;
  }
}

void reiniciarServos() {
  for (int i = 0; i < QTD_SERVO; ++i) {
    listaServos[i].reiniciar();
  }
}


void lerSerial() {
  while (com.available()) {

    char inChar = (char)com.read();
    inputString += inChar;

    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
