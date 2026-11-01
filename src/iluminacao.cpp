// iluminacao.cpp - LIGHT0 (teto, pontual), LIGHT1 (abajur, spot) e LIGHT2 (brilho da TV)
// A posicao do spot do abajur e definida dentro de desenhaCriadoMudo() (herda a hierarquia do movel).
#include <GL/glut.h>
#include "iluminacao.h"

#ifndef GL_SEPARATE_SPECULAR_COLOR
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SEPARATE_SPECULAR_COLOR   0x81FA
#endif

void initIluminacao() {
    //ativando iluminacao e cores
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //luz principal (0)
    float luzAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    float luzDifusa[]   = {0.6f, 0.6f, 0.6f, 1.0f}; 
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glEnable(GL_LIGHT0); 

    // Luz 1: Abajur (Amarelada)
    float luzAmbiente1[] = {0.2f, 0.2f, 0.0f, 1.0f}; 
    float luzDifusa1[]   = {1.0f, 0.9f, 0.2f, 1.0f}; 
    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa1);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);
    //glEnable(GL_LIGHT1);
    // Luz 0 (teto)
    float espec0[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, espec0);

    // Luz 1 (abajur)
    float espec1[] = {1.0f, 0.9f, 0.4f, 1.0f};
    glLightfv(GL_LIGHT1, GL_SPECULAR, espec1);

    // Brilho calculado com a posição real da câmera 
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // Luz ambiente global baixinha, para o quarto não ficar preto com a luz apagada
    float ambGlobal[] = {0.08f, 0.08f, 0.08f, 1.0f};
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGlobal);

    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);  // brilho sobre a textura

    // Luz 2: brilho azulado da TV (ligada junto com a TV)
    float amb[] = {0, 0, 0, 1}, dif[] = {0.25f, 0.4f, 0.9f, 1}, esp[] = {0.2f, 0.3f, 0.6f, 1};
    glLightfv(GL_LIGHT2, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, esp);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.10f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.05f);
}

void posicionaLuzes() {
    float posicaoLuzTeto[] = {0.0f, 5.4f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuzTeto);
    float posLuzTV[] = {5.5f, 2.9f, 6.2f, 1.0f};
    glLightfv(GL_LIGHT2, GL_POSITION, posLuzTV);
}
