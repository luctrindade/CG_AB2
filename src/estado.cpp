// estado.cpp - definicao das variaveis globais (valores iniciais)
#include "estado.h"

// ---- camera ----
float camX = .0f, camY = 3.0f, camZ = 6.0f;
//vetor de direcao do personagem
float dirX = 0.0f, dirZ = -1.0f;
float dirY = 0.0f;
//angulo de rotacao
float angulo = -1.57f;
float pitch = 0.0f;                      // Angulo vertical
int ultimoMouseX = 400, ultimoMouseY = 300;   // Centro da tela
bool mousePressionado = false;

// ---- interacoes ----
float anguloCadeira = 0.0f;
float gavetaX = .0f;
bool gavetaAberta = false;
bool luzLampada = true;
bool luzAbajur = false;
bool portaAberta = false;
float anguloPorta = .0f;
bool tvLigada = false;
bool monitorLigado = false;
bool cortinaAberta = false;

// ---- animacao suave ----
float anguloPortaAlvo   = 0.0f;
float gavetaAlvo        = 0.0f;
float cortinaPos        = 0.0f;
float cortinaAlvo       = 0.0f;
float anguloCadeiraAlvo = 0.0f;
bool  mostraHUD         = true;
