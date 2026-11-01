// janela.cpp - janela e cortinas transparentes (desenhadas por ultimo)
#include "comum.h"

void desenhaJanela() {
    glPushMatrix();
        //proximo a parede
        glTranslatef(4.0f, 3.5f, -7.9f);

        //vidro janela
        glPushMatrix();
            matVidro(1.0f, 1.0f, 1.0f); 
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texJanela); 
            
            glScalef(3.0f, 2.0f, 0.05f);
            cuboTex();
            
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //moldura topo
        matPlastico(0.9f, 0.9f, 0.9f);
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            
            caixaTex(texMoldura,3.2f, 0.1f, 0.1f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, -1.0f, 0.0f);
            caixaTex(texMoldura,3.2f, 0.1f, 0.1f);
            //glutSolidCube(1.0f);
        glPopMatrix();

        //moldura laterais 
        glPushMatrix();
            glTranslatef(-1.55f, 0.0f, 0.0f);
            caixaTex(texMoldura,0.1f, 2.0f, 0.1f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.55f, 0.0f, 0.0f);
            caixaTex(texMoldura,0.1f, 2.0f, 0.1f);
            //glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

// ======================= CORTINAS (transparentes, desenhadas por ultimo) =======================
void desenhaCortinas() {
    float esc = 1.6f + (0.3f - 1.6f) * cortinaPos;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glPushMatrix();
        glTranslatef(4.0f, 3.5f, -7.9f);
        glColor4f(0.8f, 0.7f, 0.5f, 0.7f);
        float zero[] = {0, 0, 0, 1};
        glMaterialfv(GL_FRONT, GL_SPECULAR, zero);

        glPushMatrix();   // esquerda
            glTranslatef(-1.6f + esc / 2.0f, 0.0f, 0.1f);
            caixaTex(texCortina,esc, 2.2f, 0.05f);
        glPopMatrix();
        glPushMatrix();   // direita
            glTranslatef(1.6f - esc / 2.0f, 0.0f, 0.1f);
            caixaTex(texCortina,esc, 2.2f, 0.05f);
        glPopMatrix();
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
