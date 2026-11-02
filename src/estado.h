// estado.h - variaveis globais do programa (camera, interacoes e animacoes)
// Definidas em estado.cpp; os demais modulos so enxergam as declaracoes "extern".
#ifndef ESTADO_H
#define ESTADO_H

// ---- camera ----
extern float camX, camY, camZ;
extern float dirX, dirY, dirZ;       // vetor de direcao do olhar
extern float angulo, pitch;          // giro horizontal e vertical
extern int   ultimoMouseX, ultimoMouseY;
extern bool  mousePressionado;

// ---- interacoes (estado atual) ----
extern float anguloCadeira;
extern float gavetaX;
extern bool  gavetaAberta;
extern bool  luzLampada;
extern bool  luzAbajur;
extern bool  portaAberta;
extern float anguloPorta;
extern bool  tvLigada;
extern bool  monitorLigado;
extern bool  cortinaAberta;

// ---- animacao suave: o teclado muda o ALVO e o timer aproxima o valor atual ----
extern float anguloPortaAlvo;
extern float gavetaAlvo;
extern float cortinaPos;             // 0 = fechada, 1 = aberta
extern float cortinaAlvo;
extern float anguloCadeiraAlvo;
extern bool  mostraHUD;
extern bool  portaRackAberta[3];
extern float anguloPortaRack[3];
extern float anguloPortaRackAlvo[3];
extern bool  portaArmarioAberta[2];
extern float anguloPortaArmario[2];
extern float anguloPortaArmarioAlvo[2];

#endif
