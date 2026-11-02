// hud.cpp
#include <GL/glut.h>
#include "hud.h"
#include "estado.h"

// ======================= HUD (ajuda na tela) =======================
// ======================= HUD (ajuda na tela) =======================
static void texto(float x, float y, const char* s) {
    glRasterPos2f(x, y);
    while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s++);
}

void desenhaHUD() {
    if (!mostraHUD) return;
    int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);  glPushMatrix(); glLoadIdentity();

    glColor3f(1.0f, 1.0f, 0.6f);
    texto(10, h - 18, "W/A/S/D: mover | Mouse (segurar botao esquerdo): olhar");
    texto(10, h - 34, "E: porta / gaveta / TV / monitor / armario | 1/2 (armario) /3: rack | F: luzes | C: cortina | R: cadeira | H: ocultar ajuda");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
