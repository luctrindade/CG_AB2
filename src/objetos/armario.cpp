// armario.cpp - armario
#include "comum.h"

void desenhaArmario() {
    glPushMatrix();
        // Posicionado na parede da esquerda, mais próximo da porta 
        glTranslatef(-7.2f, 0.0f, 3.0f);

        // Corpo do Armário 
        matMadeira(.4f,.25f,.15f);
        glPushMatrix();
            glTranslatef(0.0f, 2.5f, 0.0f);
            caixaTex(texMadeira,1.5f, 5.0f, 3.0f); // 1.5m prof, 5m alt, 3m larg
        glPopMatrix();

        // Portas do Armário 
        matMadeira(.45f,.3f,.2f);
        
        // Porta Esquerda
        glPushMatrix();
            glTranslatef(0.76f, 2.5f, -0.75f);
            caixaTex(texMadeira,0.05f, 4.8f, 1.4f);
        glPopMatrix();
        
        // Porta Direita
        glPushMatrix();
            glTranslatef(0.76f, 2.5f, 0.75f);
            caixaTex(texMadeira,0.05f, 4.8f, 1.4f);
        glPopMatrix();

        // Puxadores 
        matMetal(0.7f, 0.7f, 0.7f);
        
        // Puxador Esquerdo
        glPushMatrix();
            glTranslatef(0.8f, 2.5f, -0.1f);
            glScalef(0.05f, 0.6f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
        
        // Puxador Direito
        glPushMatrix();
            glTranslatef(0.8f, 2.5f, 0.1f);
            glScalef(0.05f, 0.6f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}
