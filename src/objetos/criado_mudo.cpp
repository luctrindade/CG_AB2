// criado_mudo.cpp - criado-mudo com gaveta, livro e abajur (luz spot LIGHT1)
#include "comum.h"

void desenhaCriadoMudo(){
    //movel 
    glPushMatrix(); // matriz separada da cama
        glTranslatef(3.0f, 0.0f, -6.0f); // lado esquerdo da mesa
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        
        // corpo
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            matMadeira(.4f,.25f,.15f);
            caixaTex(texMadeira, 1.5f, 2.0f, 1.5f);
        glPopMatrix();

        // Gaveta interativa e puxador
        glPushMatrix();
            glTranslatef(-0.1f - gavetaX, 1.5f, 0.0f); 
            matMadeira(.6f,.45f,.35f);
            
            //  Fundo (Base da gaveta)
            glPushMatrix();
                glTranslatef(0.0f, -0.25f, 0.0f);
                caixaTex(texMesa, 1.5f, 0.1f, 1.4f);
            glPopMatrix();

            //  Parede Frontal 
            glPushMatrix();
                glTranslatef(-0.7f, 0.0f, 0.0f);
                caixaTex(texMesa, 0.1f, 0.6f, 1.4f);
            glPopMatrix();

            //  Parede Traseira
            glPushMatrix();
                glTranslatef(0.7f, 0.0f, 0.0f);
                glScalef(0.1f, 0.6f, 1.4f);
                glutSolidCube(1.0f);
            glPopMatrix();

            //  Parede Lateral Esquerda
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, -0.65f);
                caixaTex(texMesa, 1.5f, 0.6f, 0.1f);
            glPopMatrix();

            //  Parede Lateral Direita
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, 0.65f);
                caixaTex(texMesa,1.5f, 0.6f, 0.1f);
            glPopMatrix();
            
            // Puxador 
            glPushMatrix();
                glTranslatef(-0.8f, 0.0f, 0.0f); 
                matMetal(0.7f, 0.7f, 0.7f);
                glutSolidSphere(0.1, 10, 10);
            glPopMatrix();

            glPushMatrix();
                // Posiciona o livro no centro da gaveta. 
                glTranslatef(-0.25f, 0.15f, 0.0f); 
                glRotatef(-90.0f,0.0f,1.0f,.0f);
                // Capa Preta
                matTecido(1.0f, 1.0f, 1.0f);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texDN);
                glPushMatrix();
                    glScalef(0.4f, 0.03f, 0.5f); // Livro deitado
                    cuboTex();
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
                // Folhas Brancas
                glColor3f(0.9f, 0.9f, 0.9f);
                glPushMatrix();
                    // Desloca milimetricamente no eixo para as páginas saltarem da capa
                    glTranslatef(0.011f, 0.0f, 0.0f); 
                    glScalef(0.38f, 0.01f, 0.48f); // Levemente menor que a capa
                    glutSolidCube(1.0f);
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix(); // Fim do livro
        glPopMatrix(); // Fecha gaveta

        //abajur encima do movel
        glPushMatrix();
            glTranslatef(0.0f, 2.0f, 0.0f); 
            
            //base
            glPushMatrix();
                matMetal(0.7f, 0.6f, 0.2f);
                glScalef(0.4f, 0.05f, 0.4f);
                glutSolidCube(1.0f);
            glPopMatrix();

            //haste
            glPushMatrix();
                glTranslatef(0.0f, 0.4f, 0.0f);
                matMetal(0.7f, 0.6f, 0.2f); 
                glScalef(0.05f, 0.8f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
            //cupula
            glPushMatrix();
                glTranslatef(0.0f, 0.65f, 0.0f); // dentro da cupula
                
                // Define que a luz GL_LIGHT0 nasce exatamente desta coordenada
                float posLuzAbajur[] = {0.0f, 0.5f, 0.0f, 1.0f};
                glLightfv(GL_LIGHT1, GL_POSITION, posLuzAbajur);

                // EFEITO DA LUZ NO ABAJUR
                float direcaoLuz[] = {0.0f, -1.0f, 0.0f}; // Aponta reto para o chão
                glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcaoLuz);
                glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80.0f); // Ângulo de abertura do cone
                glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0f); // Deixa a borda da luz suave
                // brilho lampada
                if (luzAbajur) { 
                    float emissao[] = {0.8f, 0.8f, 0.5f, 1.0f}; 
                    glMaterialfv(GL_FRONT, GL_EMISSION, emissao);
                }
                
                matTecido(0.9f, 0.9f, 0.4f);
                glutSolidSphere(0.08, 10, 10); // Desenha a bolinha
                
                // desliga emissao nao afeta cor objetos
                float semEmissao[] = {0.0f, 0.0f, 0.0f, 1.0f};
                glMaterialfv(GL_FRONT, GL_EMISSION, semEmissao);
            glPopMatrix();

            //cupula 
            glPushMatrix();
                glTranslatef(0.0f, 0.5f, 0.0f);
                glColor3f(0.9f, 0.9f, 0.4f); // Amarelo base
                
                //cupula brilha ao ligar
                if (luzAbajur) {
                    float emissaoCupula[] = {0.6f, 0.6f, 0.2f, 1.0f}; // Brilho amarelado
                    glMaterialfv(GL_FRONT, GL_EMISSION, emissaoCupula);
                }

                glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
                static GLUquadric* qAbajur = NULL;
                if (!qAbajur) qAbajur = gluNewQuadric();
                gluCylinder(qAbajur, 0.4, 0.15, 0.5, 16, 16);
                
                // Desliga a emissão para não afetar os próximos objetos
                float semEmissaoCupula[] = {0.0f, 0.0f, 0.0f, 1.0f};
                glMaterialfv(GL_FRONT, GL_EMISSION, semEmissaoCupula);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix(); 
}
