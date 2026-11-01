// porta.cpp - porta com dobradica (abre com a tecla E)
#include "comum.h"

void desenhaPorta() {
    glPushMatrix();
        // Centraliza exatamente no buraco feito na parede
        glTranslatef(-5.5f, 2.0f, 8.0f); 
        
        // Moldura/Batentes fixos
        matMadeira(.3f,.15f,.05f);
        glPushMatrix(); // Batente Esquerdo
            glTranslatef(-1.05f, 0.0f, 0.0f);
            caixaTex(texMadeira,0.1f, 4.0f, 0.15f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix(); // Batente Direito
            glTranslatef(1.05f, 0.0f, 0.0f);
            caixaTex(texMadeira,0.1f, 4.0f, 0.15f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix(); // Batente Superior
            glTranslatef(0.0f, 2.05f, 0.0f);
            caixaTex(texMadeira,2.2f, 0.1f, 0.15f);
            //glutSolidCube(1.0f);
        glPopMatrix();

        // Dobradiça e Folha da Porta 
        glPushMatrix();
            // Joga o eixo de rotação na dobradiça esquerda 
            glTranslatef(-1.0f, 0.0f, 0.0f);
            glRotatef(anguloPorta, 0.0f, 1.0f, 0.0f);
            glTranslatef(1.0f, 0.0f, 0.0f); // Volta o desenho para o centro

            // Folha da porta
            glPushMatrix();
                matMadeira(.4f,.25f,.15f);
                caixaTex(texMadeira, 2.0f, 4.0f, 0.05f); 
            glPopMatrix();

            // Maçanetas
            matMetal(0.8f, 0.8f, 0.2f);
            glPushMatrix();
                glTranslatef(0.8f, 0.0f, 0.05f); // Lado de fora
                glutSolidSphere(0.08, 10, 10);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.8f, 0.0f, -0.05f); // Lado de dentro
                glutSolidSphere(0.08, 10, 10);
            glPopMatrix();
            
        glPopMatrix();
    glPopMatrix();
}
