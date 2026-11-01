// monitor.cpp - monitor (tela com textura quando ligado)
#include "comum.h"

void desenhaMonitor(){
    glPushMatrix();
        glTranslatef(.0f,2.1f,-6.0f);
        matPlastico(0.2f, 0.2f, 0.2f);
        //apoio
        glPushMatrix();
            glScalef(1.0f, 0.1f, 0.8f);
            glutSolidCube(1.0);
        glPopMatrix();
        //haste vertical
        glPushMatrix();
            glTranslatef(0.0f, 0.4f, -0.2f);
            glScalef(0.2f, 0.8f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
        //tela principal
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            
            // Moldura Traseira 
            matPlastico(0.15f, 0.15f, 0.15f);
            glPushMatrix();
                glScalef(2.0f, 1.2f, 0.2f);
                glutSolidCube(1.0);
            glPopMatrix();

            // Display / Vidro da Tela
            if (!monitorLigado) {
                // Monitor Desligado
                matVidro(0.1f, 0.1f, 0.12f);
                glPushMatrix();
                    glTranslatef(0.0f, 0.0f, 0.11f); // Desloca para frente 
                    glScalef(1.9f, 1.1f, 0.02f);
                    glutSolidCube(1.0);
                glPopMatrix();
            } else {
            matVidro(1, 1, 1);
            float emissaoMonitor[] = {0.6f, 0.6f, 0.6f, 1.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, emissaoMonitor);
            telaMonitor();
            float semEmissao[] = {0, 0, 0, 1};
            glMaterialfv(GL_FRONT, GL_EMISSION, semEmissao);
        }
        glPopMatrix();
    glPopMatrix();
}

// Tela do monitor 
void telaMonitor() {
    planoTex(texMonitor, -0.95f,-0.55f,0.121f,  1.9f,0,0,  0,1.1f,0,  0,0,1,  1,1, 4,4);
}
