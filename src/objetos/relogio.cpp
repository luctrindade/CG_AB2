// relogio.cpp - relogio de parede
#include "comum.h"

void desenhaRelogio() {
    glPushMatrix();
        glTranslatef(7.9f, 4.0f, -4.3f); // Centralizado na parede do fundo
        glRotatef(-90.0f,.0f,1.0f,.0f);
        // Borda do relogio
        matPlastico(0.1f, 0.1f, 0.1f);
        gluCylinder(qTex, 0.6, 0.6, 0.1, 32, 1);
        
        // Fundo (Mostrador)
        glTranslatef(0.0f, 0.0f, 0.05f);
        matTecido(0.9f, 0.9f, 0.9f);
        gluDisk(qTex, 0.0, 0.6, 32, 1);
        
        // Ponteiros
        float tempo = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        glTranslatef(0.0f, 0.0f, 0.01f);
        matMetal(0.1f, 0.1f, 0.1f);
        
        glPushMatrix(); // Minutos
            glRotatef(-tempo * 50.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 0.2f, 0.0f);
            glScalef(0.02f, 0.4f, 0.02f);
            glutSolidCube(1.0f);
        glPopMatrix();
        
        glPushMatrix(); // Horas
            glRotatef(-tempo * 5.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 0.15f, 0.0f);
            glScalef(0.03f, 0.3f, 0.02f);
            glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix();
}
