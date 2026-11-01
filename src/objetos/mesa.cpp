// mesa.cpp - mesa do computador
#include "comum.h"

void desenhaMesa(){
    glPushMatrix();
        glTranslatef(.0f,.0f,-6.0f);
        
        matMadeira(1.0f, 1.0f, 1.0f); 

        // Tampo 
        glPushMatrix();
            glTranslatef(.0f,2.0f,.0f);
            caixaTex(texMesa, 4.0f, 0.2f, 2.0f);  // aplica textura
        glPopMatrix();
        
        // Perna Fesq 
        glPushMatrix();
            glTranslatef(-1.8f, 1.0f, 0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
        
        // Perna Fdir
        glPushMatrix();
            glTranslatef(1.8f, 1.0f, 0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
        
        // Perna Tesq
        glPushMatrix();
            glTranslatef(-1.8f, 1.0f, -0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
        
        // Perna Tdir
        glPushMatrix();
            glTranslatef(1.8f, 1.0f, -0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
    glPopMatrix();
}
