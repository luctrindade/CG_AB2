//sombra planar
//
// Tecnica: a matriz de projecao planar achata a geometria sobre o piso, projetando a
// partir da luz do teto:  M = (P.L) * I - L * P^T,  com plano P = (0, 1, 0, -y).
// Passada 1: a silhueta projetada so escreve no stencil (sem cor e sem profundidade).
// Passada 2: um quadrilatero preto semitransparente cobre o piso e so passa onde stencil == 1,
//            entao areas onde as sombras se sobrepoem escurecem uma unica vez.
#include <GL/glut.h>
#include <stdio.h>
#include "sombras.h"
#include "estado.h"
#include "cena.h"

bool sombrasAtivas = true;

// Achata sobre o plano y = yPlano, a partir da luz L (x, y, z, w). Ordem coluna-maior (OpenGL).
void calculaMatrizSombra(const float L[4], float yPlano, float m[16]) {
    float P[4] = {0.0f, 1.0f, 0.0f, -yPlano};
    float dot = P[0] * L[0] + P[1] * L[1] + P[2] * L[2] + P[3] * L[3];
    for (int col = 0; col < 4; col++)
        for (int lin = 0; lin < 4; lin++)
            m[col * 4 + lin] = (lin == col ? dot : 0.0f) - L[lin] * P[col];
}

static void caixaSol(float cx, float cy, float cz, float sx, float sy, float sz) {
    glPushMatrix();
        glTranslatef(cx, cy, cz);
        glScalef(sx, sy, sz);
        glutSolidCube(1.0f);
    glPopMatrix();
}

// Silhuetas simplificadas (so geometria; cor, luz e textura nao importam na passada de sombra)
static void sombraMesa() {
    caixaSol( 0.0f, 2.0f, -6.0f, 4.0f, 0.2f, 2.0f);       // tampo
    caixaSol(-1.8f, 1.0f, -5.2f, 0.2f, 2.0f, 0.2f);       // pernas
    caixaSol( 1.8f, 1.0f, -5.2f, 0.2f, 2.0f, 0.2f);
    caixaSol(-1.8f, 1.0f, -6.8f, 0.2f, 2.0f, 0.2f);
    caixaSol( 1.8f, 1.0f, -6.8f, 0.2f, 2.0f, 0.2f);
    caixaSol( 0.0f, 2.1f, -6.0f, 1.0f, 0.1f, 0.8f);       // monitor: apoio, haste e tela
    caixaSol( 0.0f, 2.5f, -6.2f, 0.2f, 0.8f, 0.2f);
    caixaSol( 0.0f, 3.1f, -6.0f, 2.0f, 1.2f, 0.2f);
}

static void sombraCadeira() {
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -4.0f);
        glRotatef(anguloCadeira, 0.0f, 1.0f, 0.0f);        // acompanha o giro da cadeira
        caixaSol( 0.0f, 1.2f, 0.0f, 1.2f, 0.2f, 1.0f);     // assento
        caixaSol( 0.0f, 1.7f, 0.45f, 1.2f, 1.0f, 0.15f);   // encosto
        caixaSol( 0.0f, 0.6f, 0.0f, 0.1f, 1.2f, 0.1f);     // haste
        caixaSol( 0.0f, 0.1f, 0.0f, 1.5f, 0.1f, 0.2f);     // base em cruz
        caixaSol( 0.0f, 0.1f, 0.0f, 0.2f, 0.1f, 1.5f);
        caixaSol(-0.55f, 1.5f, 0.0f, 0.1f, 0.05f, 0.6f);   // apoios de braco
        caixaSol( 0.55f, 1.5f, 0.0f, 0.1f, 0.05f, 0.6f);
    glPopMatrix();
}

static void sombraCama() {
    caixaSol(5.5f, 0.4f, -4.0f, 3.0f, 0.8f, 6.0f);        // base da cama
    caixaSol(5.5f, 1.0f, -4.0f, 2.8f, 0.4f, 5.8f);        // colchao
    caixaSol(3.0f, 1.0f, -6.0f, 1.5f, 2.0f, 1.5f);        // criado-mudo
    caixaSol(3.0f, 2.4f, -6.0f, 0.05f, 0.8f, 0.05f);      // abajur: haste e cupula
    caixaSol(3.0f, 2.75f, -6.0f, 0.6f, 0.5f, 0.6f);
}

void desenhaSombras() {
    if (!sombrasAtivas || !luzLampada) return;             // sem luz do teto, sem sombra
    const float yPlano = 0.03f;                            // acima do tapete (y = 0.02)
    float L[4] = {0.0f, 5.4f, 0.0f, 1.0f};                 // mesma posicao da luz do teto (display)
    float m[16];
    calculaMatrizSombra(L, yPlano, m);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glPushMatrix();
        glMultMatrixf(m);
        sombraMesa();
        sombraCadeira();
        sombraCama();
        desenhaEstante();
        desenhaArmario();
        desenhaRackETV();
        desenhaAcessorio();
        desenhaPorta();
        desenhaRackETV();
    glPopMatrix();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.45f);                    // intensidade da sombra (0 a 1)
    glBegin(GL_QUADS);                                     // cobre o piso; so passa onde stencil == 1
        glVertex3f(-8.0f, yPlano,  8.0f);
        glVertex3f( 8.0f, yPlano,  8.0f);
        glVertex3f( 8.0f, yPlano, -8.0f);
        glVertex3f(-8.0f, yPlano, -8.0f);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
}

// Se a janela nao tiver buffer de stencil, desliga as sombras (em vez de escurecer o piso todo)
void initSombras() {
    GLint bits = 0;
    glGetIntegerv(GL_STENCIL_BITS, &bits);
    if (bits == 0) {
        sombrasAtivas = false;
        printf("Sem buffer de stencil: sombras desativadas (use GLUT_STENCIL no glutInitDisplayMode).\n");
    }
}
