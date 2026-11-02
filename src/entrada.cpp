// entrada.cpp - mouse (camera) e teclado (movimento e interacoes)
#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "entrada.h"
#include "estado.h"
#include "colisao.h"

void gerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressionado = true;
            ultimoMouseX = x; // Grava onde o clique começou
            ultimoMouseY = y;
        } else if (state == GLUT_UP) {
            mousePressionado = false; // Soltou o clique, trava a câmera
        }
    }
}

void movimentoMouse(int x, int y) {
    if (!mousePressionado) return; // Só move a câmera se estiver segurando o clique

    float deltaX = x - ultimoMouseX;
    float deltaY = y - ultimoMouseY;

    ultimoMouseX = x;
    ultimoMouseY = y;

    float sensibilidade = 0.002f; 
    angulo += deltaX * sensibilidade; 
    pitch -= deltaY * sensibilidade;  

    if (pitch > 1.5f) pitch = 1.5f;
    if (pitch < -1.5f) pitch = -1.5f;

    dirX = cos(pitch) * cos(angulo);
    dirY = sin(pitch);
    dirZ = cos(pitch) * sin(angulo);

    glutPostRedisplay();
}

void teclado(unsigned char key, int x_mouse, int y_mouse){
    float len = sqrt(dirX*dirX + dirZ*dirZ);
    float fx = dirX / len, fz = dirZ / len;   // sem perder velocidade ao olhar para cima/baixo
    float velocidade = 0.2f;
    switch(tolower(key)){
        case 'w': mover( fx*velocidade,  fz*velocidade); break;
        case 's': mover(-fx*velocidade, -fz*velocidade); break;
        case 'd': mover(-fz*velocidade,  fx*velocidade); break;
        case 'a': mover( fz*velocidade, -fx*velocidade); break;
        case 'h': mostraHUD = !mostraHUD; break;
        case 'r': {
            float dist = sqrt(pow(camX, 2) + pow(camZ + 4.0f, 2));
            if (dist < 2.5f) anguloCadeiraAlvo += 45.0f;
            break;
        }
        case 'e': {
            float distGaveta  = sqrt(pow(camX - 3.0f, 2) + pow(camZ + 6.0f, 2));
            float distPorta   = sqrt(pow(camX + 5.5f, 2) + pow(camZ - 8.0f, 2));
            float distTV      = sqrt(pow(camX - 5.5f, 2) + pow(camZ - 7.0f, 2));
            float distMonitor = sqrt(pow(camX, 2) + pow(camZ + 6.0f, 2));

            float minDist = 999.0f;
            int objID = -1;

            if (distGaveta < 3.0f && distGaveta < minDist)   { minDist = distGaveta;  objID = 0; }
            if (distPorta < 4.0f && distPorta < minDist)     { minDist = distPorta;   objID = 1; }
            if (distTV < 4.0f && distTV < minDist)           { minDist = distTV;      objID = 2; }
            if (distMonitor < 3.5f && distMonitor < minDist) { minDist = distMonitor; objID = 3; }

            if (objID == 0) {
                gavetaAberta = !gavetaAberta;
                gavetaAlvo = gavetaAberta ? 1.0f : 0.0f;
            } else if (objID == 1) {
                portaAberta = !portaAberta;
                anguloPortaAlvo = portaAberta ? 90.0f : 0.0f;
            } else if (objID == 2) {
                tvLigada = !tvLigada;
                if (tvLigada) glEnable(GL_LIGHT2); else glDisable(GL_LIGHT2);
            } else if (objID == 3) {
                monitorLigado = !monitorLigado;
            }
            break;
        }
        case 'f':{
            float distLampada = sqrt(pow(camX - (3.0f), 2) + pow(camZ - (-6.0f), 2));
            
            if (distLampada < 3.0f) {
                // Se estiver perto, controla o abajur
                luzAbajur = !luzAbajur;
                if(luzAbajur) glEnable(GL_LIGHT1);
                else glDisable(GL_LIGHT1);
            } else {
                // Se estiver longe, controla a luz do teto
                luzLampada = !luzLampada;
                if(luzLampada) glEnable(GL_LIGHT0);
                else glDisable(GL_LIGHT0);
            }
            break;
        }
        case 'c':{
            float distJanela = sqrt(pow(camX - 4.0f, 2) + pow(camZ - (-7.9f), 2));
            if (distJanela < 4.0f) {
                cortinaAberta = !cortinaAberta;
                cortinaAlvo = cortinaAberta ? 1.0f : 0.0f;
            }
            break;
        }
        case '1':
        case '2':
        case '3': {
            float distRack = sqrt(pow(camX - 5.5f, 2) + pow(camZ - 7.0f, 2));
            if (distRack < 4.5f) {
                int i = key - '1'; // Converte '1','2','3' para índice 0, 1 ou 2
                portaRackAberta[i] = !portaRackAberta[i];
                anguloPortaRackAlvo[i] = portaRackAberta[i] ? 90.0f : 0.0f;
            }
            break;
        }
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}
