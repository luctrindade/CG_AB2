// quadros.cpp - quadro da parede do fundo e quadros da parede esquerda
#include "comum.h"

void desenhaQuadro() {
    glPushMatrix();
        // lateral esquerda
        glTranslatef(-3.5f, 3.5f, -7.9f);
        
        glRotatef(90.0,.0f,1.0f,.0f);

        // tela do Quadro 
        glPushMatrix();
            glColor3f(0.9f, 0.9f, 0.9f);
            glScalef(0.05f, 2.0f, 3.0f); 
            glutSolidCube(1.0f);
        glPopMatrix();
        matTecido(1, 1, 1);
        planoTex(texQuadroPaisagem, -0.03f,-1.0f,-1.5f,  0,0,3.0f,  0,2.0f,0,  -1,0,0,  1,1, 6,4);
        // moldura escura
        matMadeira(0.2f, 0.1f, 0.1f);
        
        // topo e Base da Moldura
        glPushMatrix();
            glTranslatef(0.0f, 1.05f, 0.0f);
            caixaTex(texMadeira,0.1f, 0.1f, 3.2f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, -1.05f, 0.0f);
            caixaTex(texMadeira,0.1f, 0.1f, 3.2f);
        glPopMatrix();

        // lateral moldura 
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -1.55f);
            caixaTex(texMadeira,0.1f, 2.0f, 0.1f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.55f);
            caixaTex(texMadeira,0.1f, 2.0f, 0.1f);
        glPopMatrix();

    glPopMatrix();
}

void desenhaQuadrosEsquerda() {
    glPushMatrix();
        // Encostados na parede da esquerda 
        glTranslatef(-7.9f, 3.5f, -2.0f); 
        // Gira 90 graus para colar na parede lateral
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 

        // Função lambda/interna rápida para desenhar um quadro genérico
        auto desenhaUmQuadro = [](GLuint tex) {
            // Tela Branca
            glPushMatrix();
                matTecido(0.9f, 0.9f, 0.9f);
                glScalef(1.5f, 2.0f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
            matTecido(1, 1, 1);
            planoTex(tex, -0.75f,-1.0f,0.03f,  1.5f,0,0,  0,2.0f,0,  0,0,1,  1,1, 4,4);
            // Moldura Escura
            matMadeira(0.2f, 0.1f, 0.1f);
            glPushMatrix(); glTranslatef(0.0f, 1.05f, 0.0f); caixaTex(texMadeira,1.6f, 0.1f, 0.1f); glPopMatrix();
            glPushMatrix(); glTranslatef(0.0f, -1.05f, 0.0f); caixaTex(texMadeira,1.6f, 0.1f, 0.1f); glPopMatrix();
            glPushMatrix(); glTranslatef(-0.75f, 0.0f, 0.0f); caixaTex(texMadeira,0.1f, 2.0f, 0.1f); glPopMatrix();
            glPushMatrix(); glTranslatef(0.75f, 0.0f, 0.0f); caixaTex(texMadeira,0.1f, 2.0f, 0.1f); glPopMatrix();
        };
        
        // Quadro 1
        glPushMatrix();
            glTranslatef(-1.5f, 0.0f, 0.0f); 
            desenhaUmQuadro(texQuadroAbstrato);
        glPopMatrix();

        // Quadro 2 
        glPushMatrix();
            glTranslatef(1.5f, 0.5f, 0.0f); 
            desenhaUmQuadro(texQuadroPaisagem);
        glPopMatrix();

    glPopMatrix();
}
