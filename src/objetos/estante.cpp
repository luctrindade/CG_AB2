// estante.cpp - estante com livros
#include "comum.h"

void desenhaEstante() {
    glPushMatrix();
        //parede esquerda
        glTranslatef(-6.45f, 0.0f, -7.45f);
        //glRotatef(90.0,.0f,1.0f,.0f); 
        matMadeira(0.5f, 0.3f, 0.15f);

        // Laterais
        glPushMatrix();
            glTranslatef(-1.0f, 2.0f, 0.0f);
            caixaTex(texMadeira,0.1f, 4.0f, 1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0f, 2.0f, 0.0f);
            caixaTex(texMadeira,0.1f, 4.0f, 1.0f);

        glPopMatrix();

        // Topo e Base
        glPushMatrix();
            glTranslatef(0.0f, 4.0f, 0.0f);
            caixaTex(texMadeira,2.1f, 0.1f, 1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 0.05f, 0.0f);
            caixaTex(texMadeira,2.1f, 0.1f, 1.0f);
        glPopMatrix();

        // Prateleiras
        for(float y = 1.0f; y <= 3.0f; y += 1.0f) {
            glPushMatrix();
                glTranslatef(0.0f, y, 0.0f);
                caixaTex(texMadeira,2.0f, 0.1f, 0.9f);

            glPopMatrix();
        }

        // Prateleira de baixo y = 1
        // Livros deitados 
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texLivro);

        glPushMatrix();
            matTecido(0.8f,0.5f,0.1f);
            glTranslatef(0.5f, 1.1f, 0.0f);
            glScalef(0.6f, 0.15f, 0.8f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(0.2f,0.6f,0.6f);
            glTranslatef(0.5f, 1.25f, 0.0f);
            glScalef(0.55f, 0.15f, 0.75f);
            cuboTex();
        glPopMatrix();
        // Livros em pe
        glPushMatrix();
            matTecido(.5f,.1f,.5f);
            glTranslatef(-0.6f, 1.3f, 0.0f);
            glScalef(0.15f, 0.6f, 0.7f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.9f,.8f,.1f);
            glTranslatef(-0.4f, 1.3f, 0.0f);
            glScalef(0.2f, 0.55f, 0.7f);
            cuboTex();
        glPopMatrix();

        // prateleira do meio y =2 
        glPushMatrix();
            matTecido(.8f,.1f,.1f);
            glTranslatef(-0.5f, 2.3f, 0.0f);
            glScalef(0.2f, 0.5f, 0.7f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.1f,.2f,.8f);
            glTranslatef(-0.2f, 2.3f, 0.0f);
            glScalef(0.15f, 0.5f, 0.7f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.1f,.7f,.2f);
            glTranslatef(.01f, 2.3f, 0.0f);
            glRotatef(15.0f, 0.0f, 0.0f, 1.0f); // Inclinacao
            glScalef(0.15f, 0.5f, 0.7f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.4f,.4f,.4f);
            glTranslatef(0.5f, 2.3f, 0.0f);
            glScalef(0.18f, 0.55f, 0.7f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.7f,.3f,.5f);
            glTranslatef(0.7f, 2.3f, 0.0f);
            glScalef(0.15f, 0.5f, 0.7f);
            cuboTex();
        glPopMatrix();

        // prateleira de cima y = 3
        glPushMatrix();
            matTecido(.1f,.1f,.5f);
            glTranslatef(-0.7f, 3.25f, 0.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.1f,.1f,.5f);
            glTranslatef(-0.55f, 3.25f, 0.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.6f,.1f,.2f);
            glTranslatef(-0.4f, 3.25f, 0.0f);
            glScalef(0.15f, 0.45f, 0.65f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.3f,.5f,.2f);
            glTranslatef(-0.17f, 3.2f, 0.0f);
            glRotatef(25.0f, 0.0f, 0.0f, 1.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            cuboTex();
        glPopMatrix();

    glPopMatrix();
}
