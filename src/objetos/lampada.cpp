// lampada.cpp - lampada pendente do teto
#include "comum.h"

void desenhaLampada(){
    glPushMatrix();
        // Posiciona no centro do quarto, encostada no teto
        glTranslatef(0.0f, 6.0f, 0.0f); 

        // Fio/Cabo descendo do teto
        matMetal(0.1f, 0.1f, 0.1f); 
        glPushMatrix();
            glTranslatef(0.0f, -0.4f, 0.0f); // Desce a metade do tamanho do fio
            glScalef(0.02f, 0.45f, 0.02f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Lâmpada/Bulbo 
        glPushMatrix();
            glTranslatef(0.0f, -0.5f, 0.0f); 
            
            // Se a variável luzLampada for true, faz o material emitir luz branca
            if (luzLampada) {
                float emissaoBranca[] = {1.0f, 1.0f, 1.0f, 1.0f};
                glMaterialfv(GL_FRONT, GL_EMISSION, emissaoBranca);
                glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para a lâmpada ligada
            } else {
                glColor3f(0.3f, 0.3f, 0.3f); // Cor cinza/opaca para a lâmpada desligada
            }
            
            glutSolidSphere(0.15, 16, 16); 
            
            float semEmissao[] = {0.0f, 0.0f, 0.0f, 1.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, semEmissao);
        glPopMatrix();

    glPopMatrix();
}
