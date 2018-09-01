#include <Arduino.h>

#include "LapisServo.h"



LapisServo::LapisServo(Adafruit_PWMServoDriver pwm, int id, int min, int max, float anguloMin, float anguloMax) {
  _pwm = pwm;
  _id = id;
  _min = min;
  _max = max;
  _estado = 'P';
  _incremento = 1;
  _angulo = 0;
  _anguloMin = anguloMin;
  _anguloMax = anguloMax;
}

void LapisServo::atualizar() {
  if (_estado == 'D') {
    setAngulo(_angulo - _incremento);
  } else if (_estado == 'E') {
    setAngulo(_angulo + _incremento);
  } else if (_estado == 'P') {
    //n deve precisar fazer nada aqui....
    //setAngulo(_angulo);
  }
}

void LapisServo::setAngulo(float angulo) {
  Serial.print("Servo: ");
  Serial.print(_id);

  if (angulo > _anguloMin && angulo < _anguloMax)
  {
    Serial.print("  -> ");
    Serial.println(angulo);

    _angulo = angulo;

    int pulso = map(angulo, _anguloMin, _anguloMax, _min, _max);

    _pwm.setPWM(_id, 0, pulso);
  } else {
    Serial.print("  -> ");
    Serial.println(_angulo);
  }
}

void LapisServo::mover(char c) {
#ifdef _DEBUG
  Serial.print("Servo: ");
  Serial.print(_id);
#endif
  if (c == '0') {
#ifdef _DEBUG
    Serial.println(" Nao teve mudança.");
#endif
    return;
  } else if (c == 'D') {
    if (_estado == 'D') {
      // parar
#ifdef _DEBUG
      Serial.println(" Parou.");
#endif
      _estado = 'P';
    } else {
#ifdef _DEBUG
      Serial.println(" Movendo para direita.");
#endif
      _estado = 'D';
    }
  } else if (c == 'E') {
    if (_estado == 'E') {
#ifdef _DEBUG
      Serial.println(" Parou.");
#endif
      _estado = 'P';
    } else {
#ifdef _DEBUG
      Serial.println(" Movendo para esquerda.");
#endif
      _estado = 'E';
    }
  } else {
#ifdef _DEBUG
    Serial.println(" ERRO!.");
#endif
    return;
  }
}
