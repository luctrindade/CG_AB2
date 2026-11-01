// animacao.h - interpolacao suave de porta, gaveta, cortina e cadeira
#ifndef ANIMACAO_H
#define ANIMACAO_H

float aproxima(float a, float alvo, float vel);
void atualiza(int);
void iniciaAnimacao();      // agenda o primeiro tick do timer

#endif
