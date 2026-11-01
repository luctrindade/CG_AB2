// sala.cpp - chao, paredes, teto e tapete
#include "comum.h"

// ======================= SALA (chao, paredes, teto) =======================
void desenhaSala() {
    // Chao de madeira envernizada
    material(1, 1, 1, 0.35f, 30.0f);
    //planoTex(texChao, -8,0,-8,  16,0,0,  0,0,16,  0,1,0,  4,4, 16,16);
    planoBump(texChao, -8,0,-8,  16,0,0,  0,0,16,  0,1,0,  4,4, 50,50, 0.3f);
    
    // Paredes
    matParede(1, 1, 1);
    planoTex(texParede, -8,0,-8,  16,0,0,  0,6,0,  0,0,1,   4,1.5f, 16,6);    // frente
    planoTex(texParede, -8,0,-8,  0,0,16,  0,6,0,  1,0,0,   4,1.5f, 16,6);    // esquerda
    planoTex(texParede,  8,0,-8,  0,0,16,  0,6,0, -1,0,0,   4,1.5f, 16,6);    // direita
    // parede de tras, com vao da porta
    planoTex(texParede, -8,0,8,    1.5f,0,0, 0,6,0, 0,0,-1,  0.4f,1.5f, 2,6);
    planoTex(texParede, -6.5f,4,8, 2,0,0,    0,2,0, 0,0,-1,  0.5f,0.5f, 2,2);
    planoTex(texParede, -4.5f,0,8, 12.5f,0,0,0,6,0, 0,0,-1,  3,1.5f, 12,6);

    // Teto
    matParede(0.9f, 0.9f, 0.9f);
    planoTex(texParede, -8,6,-8,  16,0,0,  0,0,16,  0,-1,0,  4,4, 16,16);
}

// ======================= TAPETE, COLCHA, TELAS =======================
void desenhaTapete() {
    matTecido(1, 1, 1);
    planoTex(texTapete, -2.5f,0.02f,-4.8f,  5,0,0,  0,0,4,  0,1,0,  1,1, 4,4);
}
