// acessorios.cpp - teclado, mouse e caneca sobre a mesa
#include "comum.h"

void desenhaPeriferico(GLuint texTopo, float w, float h, float d) {
    float x = w/2.0f, y = h/2.0f, z = d/2.0f;
    
    // Topo 
    matPlastico(1.0f, 1.0f, 1.0f);
    planoTex(texTopo,  x, y,-z, -w,0,0,  0,0,d,  0,1,0,  1,1, 1,1);
    
    matPlastico(0.1f, 0.1f, 0.1f);
    planoTex(0, -x,-y,-z,  w,0,0,  0,0,d,  0,-1,0, 1,1, 1,1); // Fundo
    planoTex(0, -x,-y, z,  w,0,0,  0,h,0,  0,0,1,  1,1, 1,1); // Frente
    planoTex(0, -x,-y,-z,  w,0,0,  0,h,0,  0,0,-1, 1,1, 1,1); // Trás
    planoTex(0,  x,-y,-z,  0,0,d,  0,h,0,  1,0,0,  1,1, 1,1); // Direita
    planoTex(0, -x,-y,-z,  0,0,d,  0,h,0, -1,0,0,  1,1, 1,1); // Esquerda
}

void desenhaAcessorio(){
    // Teclado
    glPushMatrix();
        //encostar na mesa
        glTranslatef(0.0f, 2.11f, -5.2f); 
        
        // Inclinação ergonómica de no eixo X
        glRotatef(7.0f, 1.0f, 0.0f, 0.0f);

        // Corpo principal 
        glPushMatrix();
            glTranslatef(0.0f, 0.05f, -0.05f); 
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            desenhaPeriferico(texTeclado, 1.2f, 0.06f, 0.35f);
        glPopMatrix();

        // Descanso de pulso frontal 
        glPushMatrix();
            glTranslatef(0.0f, 0.02f, 0.15f); 
            matPlastico(0.12f, 0.12f, 0.12f);
            glScalef(1.2f, 0.04f, 0.12f);
            glutSolidCube(1.0f);
        glPopMatrix();
        
        // Pezinhos de elevação traseiros
        matPlastico(0.05f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(-0.5f, 0.02f, -0.2f);
            glScalef(0.08f, 0.06f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.5f, 0.02f, -0.2f);
            glScalef(0.08f, 0.06f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix();

    // (Mouse)
    glPushMatrix();
        glTranslatef(0.8f, 2.1f, -5.2f); 

        // Corpo curvo do mouse
        matPlastico(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            // Transforma a esfera
            glScalef(0.12f, 0.06f, 0.18f); 
            esferaTex(texListras, 1.0f);     
        glPopMatrix();

        // Scroll
        glPushMatrix();
            glTranslatef(0.0f, 0.06f, -0.12f); 
            matMetal(0.1f, 0.1f, 0.1f);        
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glutSolidTorus(0.012, 0.020, 15, 15); 
        glPopMatrix();
    glPopMatrix();

    // Caneca 
    glPushMatrix();
        glTranslatef(-1.0f, 2.13f, -5.5f);
        glRotatef(-90.0f, 1, 0, 0);
        matCeramica(1, 1, 1);
        cilindroTex(texCaneca, 0.15f, 0.15f, 0.3f);
    glPopMatrix();
}
