// cama.cpp - cama, colchao, colcha e travesseiro
#include "comum.h"

static void desenhaColcha() {
    matTecido(1, 1, 1);
    planoTex(texColcha, -1.4f,1.205f,-2.9f,  2.8f,0,0,  0,0,5.8f,  0,1,0,  3,6, 4,8);
}

void desenhaCama(){
    // CAMA
    glPushMatrix();
        glTranslatef(5.5f, 0.0f, -4.0f);
        
        // base da cama
        glPushMatrix();
            glTranslatef(0.0f, 0.4f, 0.0f);
            matMadeira(0.5f, 0.3f, 0.2f);
            caixaTex(texMadeira, 3.0f, 0.8f, 6.0f);
        glPopMatrix();
        
        // colchao
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            matTecido(0.3f, 0.4f, 0.7f);
            caixaTex(texCama,2.8f, 0.4f, 5.8f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        desenhaColcha();
        // travesseiro
        glPushMatrix(); // 
            glTranslatef(0.0f, 1.3f, -2.0f); 
            glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
            matTecido(1, 1, 1);
            caixaTex(texAlgodao, 1.5f, 0.2f, 1.0f);
        glPopMatrix();
    glPopMatrix();
}
