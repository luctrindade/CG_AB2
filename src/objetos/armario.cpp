// armario.cpp - armario oco com interior opaco e portas articuladas
#include "comum.h"

// Desenha um cabide e uma peça de roupa pendurada
static void desenhaCabideComRoupa(float posZ, float inclinacaoZ, bool comRoupa,
                                  GLuint tex, float r, float g, float b, float alturaRoupa) {
    glPushMatrix();
        // Posiciona no cabideiro 
        glTranslatef(0.0f, 3.55f, posZ);
        glRotatef(inclinacaoZ, 1.0f, 0.0f, 0.0f);

        // Gancho metálico 
        matMetal(0.75f, 0.75f, 0.75f);
        glPushMatrix();
            glTranslatef(0.0f, 0.02f, 0.0f);
            glScalef(0.015f, 0.08f, 0.015f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Estrutura triangular do cabide (alinhada ao eixo X, profundidade do armário)
        matMadeira(0.65f, 0.45f, 0.25f);
        // Haste inclinada traseira (-X)
        glPushMatrix();
            glTranslatef(-0.18f, -0.08f, 0.0f);
            glRotatef(22.0f, 0.0f, 0.0f, 1.0f);
            glScalef(0.40f, 0.02f, 0.02f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Haste inclinada frontal (+X)
        glPushMatrix();
            glTranslatef(0.18f, -0.08f, 0.0f);
            glRotatef(-22.0f, 0.0f, 0.0f, 1.0f);
            glScalef(0.40f, 0.02f, 0.02f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Barra horizontal inferior do cabide
        glPushMatrix();
            glTranslatef(0.0f, -0.15f, 0.0f);
            glScalef(0.70f, 0.018f, 0.018f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Peça de roupa pendurada (camisa / casaco)
        if (comRoupa) {
            matTecido(r, g, b);
            // Tronco da roupa
            glPushMatrix();
                glTranslatef(0.0f, -0.15f - alturaRoupa / 2.0f, 0.0f);
                caixaTex(tex, 0.52f, alturaRoupa, 0.08f);
            glPopMatrix();

            // Manga traseira (-X)
            glPushMatrix();
                glTranslatef(-0.30f, -0.18f - alturaRoupa * 0.28f, 0.0f);
                glRotatef(12.0f, 0.0f, 0.0f, 1.0f);
                caixaTex(tex, 0.14f, alturaRoupa * 0.55f, 0.07f);
            glPopMatrix();

            // Manga frontal (+X)
            glPushMatrix();
                glTranslatef(0.30f, -0.18f - alturaRoupa * 0.28f, 0.0f);
                glRotatef(-12.0f, 0.0f, 0.0f, 1.0f);
                caixaTex(tex, 0.14f, alturaRoupa * 0.55f, 0.07f);
            glPopMatrix();
        }
    glPopMatrix();
}

// Desenha uma pilha de 3 peças de roupa dobradas sobre uma prateleira
static void desenhaPilhaRoupas(float x, float yBase, float z,
                               GLuint tex1, float r1, float g1, float b1,
                               GLuint tex2, float r2, float g2, float b2,
                               GLuint tex3, float r3, float g3, float b3) {
    glPushMatrix();
        glTranslatef(x, yBase, z);

        // Peça de baixo
        matTecido(r1, g1, b1);
        glPushMatrix();
            glTranslatef(0.0f, 0.04f, 0.0f);
            caixaTex(tex1, 0.75f, 0.08f, 0.45f);
        glPopMatrix();

        // Peça do meio
        matTecido(r2, g2, b2);
        glPushMatrix();
            glTranslatef(0.0f, 0.12f, 0.01f);
            glRotatef(3.0f, 0.0f, 1.0f, 0.0f);
            caixaTex(tex2, 0.72f, 0.08f, 0.43f);
        glPopMatrix();

        // Peça do topo
        matTecido(r3, g3, b3);
        glPushMatrix();
            glTranslatef(0.0f, 0.20f, -0.01f);
            glRotatef(-2.0f, 0.0f, 1.0f, 0.0f);
            caixaTex(tex3, 0.70f, 0.08f, 0.42f);
        glPopMatrix();
    glPopMatrix();
}

void desenhaArmario() {
    glPushMatrix();
        // Posicionado na parede da esquerda, mais próximo da porta
        glTranslatef(-7.2f, 0.0f, 3.0f);

        // ======================= ESTRUTURA OCA E OPACA =======================
        matMadeira(0.4f, 0.25f, 0.15f);

        // Tampo superior 
        glPushMatrix();
            glTranslatef(0.0f, 4.95f, 0.0f);
            caixaTex(texMadeira, 1.5f, 0.1f, 3.0f);
        glPopMatrix();

        // Base inferior 
        glPushMatrix();
            glTranslatef(0.0f, 0.05f, 0.0f);
            caixaTex(texMadeira, 1.5f, 0.1f, 3.0f);
        glPopMatrix();

        // Parede lateral esquerda 
        glPushMatrix();
            glTranslatef(0.05f, 2.5f, -1.45f);
            caixaTex(texMadeira, 1.4f, 4.8f, 0.1f);
        glPopMatrix();

        // Parede lateral direita 
        glPushMatrix();
            glTranslatef(0.05f, 2.5f, 1.45f);
            caixaTex(texMadeira, 1.4f, 4.8f, 0.1f);
        glPopMatrix();

        // Parede traseira opaca 
        matMadeira(0.32f, 0.20f, 0.12f); // Tom interno levemente mais escuro para dar profundidade
        glPushMatrix();
            glTranslatef(-0.70f, 2.5f, 0.0f);
            caixaTex(texMadeira, 0.1f, 4.8f, 3.0f);
        glPopMatrix();

        // Divisória central vertical (fecha a fresta entre as portas e divide os vãos)
        matMadeira(0.36f, 0.22f, 0.14f);
        glPushMatrix();
            glTranslatef(0.05f, 2.5f, 0.0f);
            caixaTex(texMadeira, 1.4f, 4.8f, 0.1f);
        glPopMatrix();

        // Prateleiras internas do lado esquerdo 
        for (float y = 1.3f; y <= 3.8f; y += 1.2f) {
            glPushMatrix();
                glTranslatef(0.02f, y, -0.725f);
                caixaTex(texMadeira, 1.34f, 0.06f, 1.35f);
            glPopMatrix();
        }

        // Prateleira superior e inferior do lado direito 
        glPushMatrix();
            glTranslatef(0.02f, 3.9f, 0.725f);
            caixaTex(texMadeira, 1.34f, 0.06f, 1.35f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.02f, 1.0f, 0.725f);
            caixaTex(texMadeira, 1.34f, 0.06f, 1.35f);
        glPopMatrix();

        // Cabideiro metálico no vão direito
        matMetal(0.65f, 0.65f, 0.65f);
        glPushMatrix();
            glTranslatef(0.0f, 3.55f, 0.725f);
            glScalef(0.04f, 0.04f, 1.35f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // ======================= ROUPAS E CABIDES =======================
        // Pilhas de roupas dobradas nas prateleiras da esquerda
        desenhaPilhaRoupas(0.05f, 1.33f, -0.95f,
                           texAlgodao, 0.85f, 0.85f, 0.90f,
                           texListras, 0.90f, 0.40f, 0.40f,
                           texColcha,  0.35f, 0.55f, 0.85f);

        desenhaPilhaRoupas(0.05f, 2.53f, -0.50f,
                           texXadrez,  0.80f, 0.30f, 0.30f,
                           texAlgodao, 0.30f, 0.65f, 0.45f,
                           texAlgodao, 0.95f, 0.85f, 0.40f);

        // Cobertor/edredom dobrado no compartimento superior direito 
        matTecido(0.75f, 0.80f, 0.95f);
        glPushMatrix();
            glTranslatef(0.05f, 4.10f, 0.725f);
            caixaTex(texColcha, 0.95f, 0.28f, 1.05f);
        glPopMatrix();

        // Cabides no cabideiro direito 
        desenhaCabideComRoupa(0.22f,  0.0f, true,  texXadrez,  0.90f, 0.90f, 0.90f, 1.15f); // Camisa xadrez
        desenhaCabideComRoupa(0.42f,  2.0f, true,  texAlgodao, 0.25f, 0.45f, 0.75f, 1.10f); // Camisa azul
        desenhaCabideComRoupa(0.62f, -1.5f, false, 0,          0.0f,  0.0f,  0.0f,  0.0f);  // Cabide vazio
        desenhaCabideComRoupa(0.80f,  1.0f, true,  texListras, 0.85f, 0.85f, 0.85f, 1.05f); // Camisa listrada
        desenhaCabideComRoupa(1.02f,  0.0f, true,  texCouro,   0.25f, 0.18f, 0.12f, 1.30f); // Casaco de couro
        desenhaCabideComRoupa(1.20f,  3.0f, false, 0,          0.0f,  0.0f,  0.0f,  0.0f);  // Cabide vazio

        // ======================= PORTAS ARTICULADAS =======================
        // Porta Esquerda 
        glPushMatrix();
            glTranslatef(0.76f, 2.5f, -1.45f);                     // Pivô na dobradiça esquerda
            glRotatef(anguloPortaArmario[1], 0.0f, 1.0f, 0.0f);    // Gira para fora (+X)
            glTranslatef(0.0f, 0.0f, 0.70f);                       // Desloca até o centro da folha

            matMadeira(0.45f, 0.3f, 0.2f);
            caixaTex(texMadeira, 0.05f, 4.8f, 1.4f);

            // Puxador Esquerdo (acompanha a porta)
            matMetal(0.7f, 0.7f, 0.7f);
            glPushMatrix();
                glTranslatef(0.04f, 0.0f, 0.65f);
                glScalef(0.05f, 0.6f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();

        // Porta Direita 
        glPushMatrix();
            glTranslatef(0.76f, 2.5f, 1.45f);                      // Pivô na dobradiça direita
            glRotatef(-anguloPortaArmario[0], 0.0f, 1.0f, 0.0f);   // Gira para fora (+X)
            glTranslatef(0.0f, 0.0f, -0.70f);                      // Desloca até o centro da folha

            matMadeira(0.45f, 0.3f, 0.2f);
            caixaTex(texMadeira, 0.05f, 4.8f, 1.4f);

            // Puxador Direito (acompanha a porta)
            matMetal(0.7f, 0.7f, 0.7f);
            glPushMatrix();
                glTranslatef(0.04f, 0.0f, -0.65f);
                glScalef(0.05f, 0.6f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}