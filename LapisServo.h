#ifndef LapisServo_h
#define LapisServo_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class LapisServo
{
  public:
    LapisServo(Adafruit_PWMServoDriver pwm, int id, int min, int max, float anguloMin=0, float anguloMax=120);
    void setAngulo(float angulo);
    void mover(char c);
    void atualizar();
  private:
    int _id;
    char _estado;
    Adafruit_PWMServoDriver _pwm;
    int _min;
    int _max;
    float _anguloMin;
    float _anguloMax;
    float _angulo;
    float _incremento;
};

#endif
