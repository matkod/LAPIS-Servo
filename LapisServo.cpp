#include <Arduino.h>

#include "LapisServo.h"



LapisServo::LapisServo(Adafruit_PWMServoDriver pwm, int id, int min, int max, float posInicial, float anguloMin, float anguloMax, float incremento) {
  _pwm = pwm;
  _id = id;
  _min = min;
  _max = max;
  _estado = 'P';
  _incremento = incremento;
  _angulo = 0;
  _anguloMin = anguloMin;
  _anguloMax = anguloMax;
  _posInicial = posInicial;
}

void LapisServo::reiniciar() {
  setAngulo(_posInicial, true);
}

void LapisServo::atualizar() {
  if (_estado == 'D') {
    setAngulo(_angulo - _incremento);
  } else if (_estado == 'E') {
    setAngulo(_angulo + _incremento);
  } else if (_estado == 'P') {
    //n deve precisar fazer nada aqui....
  }
}

void LapisServo::setAngulo(float angulo, bool reset) {
  Serial.print("Servo: ");
  Serial.print(_id);

  if (angulo < _anguloMin) {
    angulo = _anguloMin;
    reset = true;
  } else if (angulo > _anguloMax) {
    angulo = _anguloMax;
    reset = true;
  }

  Serial.print("  -> ");
  Serial.println(angulo);

  _angulo = angulo;

  int pulso = map(_angulo, _anguloMin, _anguloMax, _min, _max);

  _pwm.setPWM(_id, 0, pulso);

  if (reset) {
    _estado = 'P';
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
