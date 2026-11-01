// rack_tv.cpp - rack e televisao
#include "comum.h"

void desenhaRackETV() {
    glPushMatrix();
        // Posicionado na parede esquerda, de frente para a cama
        glTranslatef(5.5f, 0.0f, 7.0f);
        glRotatef(90.0f,.0f,1.0f,.0f);
        glScalef(1.5f,1.5f,1.5f);
        //  PÉS INCLINADOS 
        matMadeira(0.7f, 0.5f, 0.3f);
        // Traseiro Esquerdo
        glPushMatrix();
            glTranslatef(-0.2f, 0.15f, -1.3f);
            glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
            glScalef(0.08f, 0.3f, 0.08f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Traseiro Direito
        glPushMatrix();
            glTranslatef(-0.2f, 0.15f, 1.3f);
            glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
            glScalef(0.08f, 0.3f, 0.08f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Dianteiro Esquerdo 
        glPushMatrix();
            glTranslatef(0.2f, 0.15f, -1.3f);
            glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
            glScalef(0.08f, 0.3f, 0.08f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Dianteiro Direito
        glPushMatrix();
            glTranslatef(0.2f, 0.15f, 1.3f);
            glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
            glScalef(0.08f, 0.3f, 0.08f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //  BASE DO RACK 
        glPushMatrix();
            glTranslatef(0.0f, 0.35f, 0.0f);
            glScalef(0.7f, 0.1f, 2.9f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //  CORPO DO RACK 
        glPushMatrix();
            matMadeira(0.9f, 0.88f, 0.85f);
            glTranslatef(0.0f, 0.8f, 0.0f); // Eleva acima dos pés e base
            caixaTex(texMoldura,0.7f, 0.8f, 2.8f); // Proporções do rack
            //glutSolidCube(1.0f);
        glPopMatrix();

        //  DETALHE SUPERIOR (Friso amadeirado sob o tampo)
        glPushMatrix();
            matMadeira(0.6f, 0.45f, 0.3f);
            glTranslatef(0.01f, 1.15f, 0.0f);
            glScalef(0.7f, 0.06f, 2.82f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //  FRONTAL DO RACK 
        matMadeira(0.88f, 0.86f, 0.83f);
        // Porta Esquerda
        glPushMatrix();
            glTranslatef(0.36f, 0.77f, -0.92f);
            caixaTex(texMoldura,0.02f, 0.7f, 0.9f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        // Porta Central
        glPushMatrix();
            glTranslatef(0.36f, 0.77f, 0.0f);
            caixaTex(texMoldura,0.02f, 0.7f, 0.9f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        // Porta Direita
        glPushMatrix();
            glTranslatef(0.36f, 0.77f, 0.92f);
            caixaTex(texMoldura,0.02f, 0.7f, 0.9f);
            //glutSolidCube(1.0f);
        glPopMatrix();


        //  TELEVISÃO 
        glPushMatrix();
            glTranslatef(0.0f, 1.2f, 0.0f);

            // Base de apoio central 
            matPlastico(0.12f, 0.12f, 0.12f);
            glPushMatrix();
                glTranslatef(0.0f, 0.02f, 0.0f);
                glScalef(0.35f, 0.03f, 0.7f); // Apoio plano
                glutSolidCube(1.0f);
            glPopMatrix();

            // Haste de conexão
            glPushMatrix();
                glTranslatef(0.0f, 0.15f, 0.0f);
                glScalef(0.08f, 0.25f, 0.12f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // Painel da Tela 
            glPushMatrix();
                glTranslatef(0.0f, 0.75f, 0.0f);

                // Moldura
                matPlastico(0.05f, 0.05f, 0.05f);
                glPushMatrix();
                    glScalef(0.08f, 1.1f, 2.1f);
                    glutSolidCube(1.0f);
                glPopMatrix();

                // Display/Vidro
                if (!tvLigada) {
                    // TV Desligada: Tela preta 
                    matVidro(0.15f, 0.15f, 0.18f);
                    glPushMatrix();
                        glTranslatef(0.045f, 0.0f, 0.0f); 
                        glScalef(0.01f, 1.02f, 2.02f);
                        glutSolidCube(1.0f);
                    glPopMatrix();
                } else {
                    // TV Ligada: Tela Azul 
                    matVidro(1, 1, 1);
                    float emissaoTV[] = {0.6f, 0.6f, 0.6f, 1.0f};
                    glMaterialfv(GL_FRONT, GL_EMISSION, emissaoTV);
                    telaTV();
                    float semEmissao[] = {0, 0, 0, 1};
                    glMaterialfv(GL_FRONT, GL_EMISSION, semEmissao);
                }
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}

// Tela da TV (coordenadas locais do painel). Barras rolam com o tempo.
void telaTV() {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(glutGet(GLUT_ELAPSED_TIME) * 0.0002f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    planoTex(texTV, 0.056f,-0.51f,1.01f,  0,0,-2.02f,  0,1.02f,0,  1,0,0,  1,1, 4,4);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}
