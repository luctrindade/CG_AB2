#include <GL/glut.h>
#include "materiais.h"

// Base: cor (ambiente+difusa via COLOR_MATERIAL), especular e brilho (0..128)
void material(float r, float g, float b, float espec, float brilho) {
    glColor3f(r, g, b);
    float s[] = {espec, espec, espec, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, s);
    glMaterialf(GL_FRONT, GL_SHININESS, brilho);
}

// Presets
void matMadeira (float r, float g, float b) { material(r, g, b, 0.15f, 12.0f); }  // brilho fraco e largo
void matMetal   (float r, float g, float b) { material(r, g, b, 0.90f, 100.0f); } // brilho forte e fechado
void matPlastico(float r, float g, float b) { material(r, g, b, 0.50f, 40.0f);  }
void matCeramica(float r, float g, float b) { material(r, g, b, 0.70f, 70.0f);  }
void matVidro   (float r, float g, float b) { material(r, g, b, 0.95f, 110.0f); }
void matTecido  (float r, float g, float b) { material(r, g, b, 0.00f, 1.0f);   } // totalmente fosco
void matParede  (float r, float g, float b) { material(r, g, b, 0.02f, 2.0f);   }
void matCouro (float r, float g, float b) { material(r, g, b, 0.8f, 50.0f); }
