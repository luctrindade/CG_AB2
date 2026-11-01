// main.cpp - janela, callbacks e laco principal
#include "comum.h"
#include "bmp_loader.h"
#include "iluminacao.h"
#include "sombras.h"
#include "hud.h"
#include "animacao.h"
#include "entrada.h"

void reshape(int w, int h){
    if (h == 0) h = 1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(float)w/(float)h,.1,100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX,camY,camZ, camX + dirX,camY + dirY,camZ + dirZ,.0f,1.0f,.0f);
    posicionaLuzes();
    
    desenhaSala();
    desenhaTapete();
    desenhaSombras();
    desenhaMesa();
    desenhaMonitor();
    desenhaAcessorio();
    desenhaCadeira();
    desenhaCama();
    desenhaCriadoMudo();
    desenhaJanela();
    desenhaEstante();
    desenhaQuadro();
    desenhaPorta();
    desenhaRackETV();
    desenhaArmario();
    desenhaQuadrosEsquerda();
    desenhaLampada();
    desenhaRelogio();

    desenhaCortinas();
    desenhaHUD();
    
    glutSwapBuffers();
}

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    initIluminacao();

    glShadeModel(GL_SMOOTH);
    initTexturas();
    initTexAvancadas();
    initSombras();
    carregaImagens();
    iniciaAnimacao();
}

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Escritorio");

    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutKeyboardFunc(teclado);
    glutMouseFunc(gerenciaMouse);     
    glutMotionFunc(movimentoMouse);
    glutMainLoop();
    return 0;
}
