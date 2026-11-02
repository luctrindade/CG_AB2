// rack_tv.cpp - rack e televisao
#include "comum.h"

// Desenha um modem/router com 3 antenas e LEDs frontais
static void desenhaModem() {
    glPushMatrix();
        // Posiciona no centro do meio apoiado sobre a base interna 
        glTranslatef(0.0f, 0.44f, 0.0f);

        // Corpo principal do modem (plástico escuro fosco)
        matPlastico(0.12f, 0.12f, 0.14f);
        glPushMatrix();
            glTranslatef(0.0f, 0.04f, 0.0f);
            glScalef(0.26f, 0.06f, 0.42f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Detalhe superior (painel/grelha no topo)
        matPlastico(0.06f, 0.06f, 0.07f);
        glPushMatrix();
            glTranslatef(0.0f, 0.072f, 0.0f);
            glScalef(0.18f, 0.005f, 0.34f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 3 Antenas traseiras (esquerda, central e direita)
        matPlastico(0.08f, 0.08f, 0.09f);
        float posZAntenas[3] = {-0.16f, 0.0f, 0.16f};
        float inclinacao[3]  = {-12.0f, 0.0f, 12.0f};

        for (int i = 0; i < 3; i++) {
            glPushMatrix();
                glTranslatef(-0.11f, 0.06f, posZAntenas[i]); // Base da antena atrás do modem
                glRotatef(inclinacao[i], 1.0f, 0.0f, 0.0f);  // Leve inclinação lateral nas pontas
                glTranslatef(0.0f, 0.13f, 0.0f);             // Sobe metade da altura da antena
                glScalef(0.018f, 0.26f, 0.018f);
                glutSolidCube(1.0f);
            glPopMatrix();
        }

        // LEDs luminosos no painel frontal 
        float emissaoVerde[]    = {0.1f, 0.9f, 0.3f, 1.0f};
        float emissaoApagada[]  = {0.0f, 0.0f, 0.0f, 1.0f};
        bool piscaRede = ((glutGet(GLUT_ELAPSED_TIME) / 180) % 2) == 0;

        for (int i = 0; i < 4; i++) {
            // O último LED (índice 3) pisca continuamente para simular tráfego de dados
            bool aceso = (i < 3) || piscaRede;
            if (aceso) {
                glMaterialfv(GL_FRONT, GL_EMISSION, emissaoVerde);
                glColor3f(0.2f, 1.0f, 0.4f);
            } else {
                glMaterialfv(GL_FRONT, GL_EMISSION, emissaoApagada);
                glColor3f(0.05f, 0.2f, 0.08f);
            }

            glPushMatrix();
                glTranslatef(0.132f, 0.04f, -0.12f + i * 0.05f);
                glScalef(0.005f, 0.012f, 0.018f);
                glutSolidCube(1.0f);
            glPopMatrix();
        }

        // Desliga a emissão para não afetar os objetos seguintes
        glMaterialfv(GL_FRONT, GL_EMISSION, emissaoApagada);
    glPopMatrix();
}

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
            caixaTex(texRack,0.08f, 0.3f, 0.08f);
        glPopMatrix();
        // Traseiro Direito
        glPushMatrix();
            glTranslatef(-0.2f, 0.15f, 1.3f);
            glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
            caixaTex(texRack,0.08f, 0.3f, 0.08f);
        glPopMatrix();
        // Dianteiro Esquerdo 
        glPushMatrix();
            glTranslatef(0.2f, 0.15f, -1.3f);
            glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
            caixaTex(texRack,0.08f, 0.3f, 0.08f);
        glPopMatrix();
        // Dianteiro Direito
        glPushMatrix();
            glTranslatef(0.2f, 0.15f, 1.3f);
            glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
            caixaTex(texRack,0.08f, 0.3f, 0.08f);
        glPopMatrix();

        //  BASE DO RACK 
        glPushMatrix();
            glTranslatef(0.0f, 0.35f, 0.0f);
            caixaTex(texRack,0.72f, 0.1f, 2.86f);
        glPopMatrix();

        //  CORPO DO RACK (dividido em 3 compartimentos internos)
        matMadeira(0.9f, 0.88f, 0.85f);

        // Tampo superior do corpo 
        glPushMatrix();
            glTranslatef(0.0f, 1.16f, 0.0f);
            caixaTex(texMoldura, 0.7f, 0.08f, 2.8f);
        glPopMatrix();

        // Base inferior interna do corpo 
        glPushMatrix();
            glTranslatef(0.0f, 0.42f, 0.0f);
            caixaTex(texMoldura, 0.7f, 0.04f, 2.8f);
        glPopMatrix();

        // Parede traseira 
        matMadeira(0.75f, 0.73f, 0.70f); // Levemente mais escuro no interior para dar profundidade
        glPushMatrix();
            glTranslatef(-0.33f, 0.78f, 0.0f);
            caixaTex(texMoldura, 0.04f, 0.68f, 2.8f);
        glPopMatrix();

        // Parede lateral esquerda 
        matMadeira(0.9f, 0.88f, 0.85f);
        glPushMatrix();
            glTranslatef(0.02f, 0.78f, -1.38f);
            caixaTex(texMoldura, 0.66f, 0.68f, 0.04f);
        glPopMatrix();

        // Parede lateral direita 
        glPushMatrix();
            glTranslatef(0.02f, 0.78f, 1.38f);
            caixaTex(texMoldura, 0.66f, 0.68f, 0.04f);
        glPopMatrix();

        // Divisória interna 1
        matMadeira(0.8f, 0.78f, 0.75f);
        glPushMatrix();
            glTranslatef(0.02f, 0.78f, -0.46f);
            caixaTex(texMoldura, 0.66f, 0.68f, 0.04f);
        glPopMatrix();

        // Divisória interna 2 
        glPushMatrix();
            glTranslatef(0.02f, 0.78f, 0.46f);
            caixaTex(texMoldura, 0.66f, 0.68f, 0.04f);
        glPopMatrix();

        //  DETALHE SUPERIOR 
        glPushMatrix();
            matMadeira(0.6f, 0.45f, 0.3f);
            glTranslatef(0.01f, 1.15f, 0.0f);
            caixaTex(texRack,0.72f, 0.06f, 2.82f); // Ligeiramente mais largo em X para destacar sobre o tampo
            //glutSolidCube(1.0f);
        glPopMatrix();

        desenhaModem();
        //  FRONTAL DO RACK 
        matMadeira(0.88f, 0.86f, 0.83f);
        
        // Porta Esquerda (índice 0)
        glPushMatrix();
            glTranslatef(0.36f, 1.12f, -0.92f);                 // Pivô na borda superior
            glRotatef(anguloPortaRack[2], 0.0f, 0.0f, 1.0f);    // Abre para cima
            glTranslatef(0.0f, -0.35f, 0.0f);                   // Desce metade da altura (0.7 / 2)
            caixaTex(texMoldura, 0.02f, 0.7f, 0.9f);
        glPopMatrix();

        // Porta Central (índice 1)
        glPushMatrix();
            glTranslatef(0.36f, 1.12f, 0.0f);                   // Pivô na borda superior
            glRotatef(anguloPortaRack[1], 0.0f, 0.0f, 1.0f);    // Abre para cima
            glTranslatef(0.0f, -0.35f, 0.0f);                   // Desce metade da altura
            caixaTex(texMoldura, 0.02f, 0.7f, 0.9f);
        glPopMatrix();

        // Porta Direita (índice 2)
        glPushMatrix();
            glTranslatef(0.36f, 1.12f, 0.92f);                  // Pivô na borda superior
            glRotatef(anguloPortaRack[0], 0.0f, 0.0f, 1.0f);    // Abre para cima
            glTranslatef(0.0f, -0.35f, 0.0f);                   // Desce metade da altura
            caixaTex(texMoldura, 0.02f, 0.7f, 0.9f);
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
