// animacao.cpp
#include <GL/glut.h>
#include <math.h>
#include "animacao.h"
#include "estado.h"
#include "entrada.h"

// ======================= ANIMACAO SUAVE =======================
float aproxima(float a, float alvo, float vel) {
    if (fabsf(alvo - a) <= vel) return alvo;
    return a + (alvo > a ? vel : -vel);
}

void atualiza(int) {
    atualizaMovimento();

    anguloPorta   = aproxima(anguloPorta,   anguloPortaAlvo,   3.0f);
    gavetaX       = aproxima(gavetaX,       gavetaAlvo,        0.04f);
    cortinaPos    = aproxima(cortinaPos,    cortinaAlvo,       0.03f);
    anguloCadeira = aproxima(anguloCadeira, anguloCadeiraAlvo, 3.0f);

    for (int i = 0; i < 3; i++) {
        anguloPortaRack[i] = aproxima(anguloPortaRack[i], anguloPortaRackAlvo[i], 3.0f);
    }

    for (int i = 0; i < 2; i++) {
        anguloPortaArmario[i] = aproxima(anguloPortaArmario[i], anguloPortaArmarioAlvo[i], 3.0f);
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualiza, 0);
}

void iniciaAnimacao() {
    glutTimerFunc(16, atualiza, 0);
}
