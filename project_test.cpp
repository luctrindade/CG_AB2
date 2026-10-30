#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
float camX = .0f, camY = 3.0f, camZ= 6.0f;
//vetor de direção do personagem
float dirX = 0.0f, dirZ = -1.0f;
//angulo de rotação
float angulo = -1.57f;
float anguloCadeira = 0.0f;

int ultimoMouseX = 400, ultimoMouseY = 300; // Centro da tela
float pitch = 0.0f; // Ângulo vertical
float dirY = 0.0f;

float gavetaX = .0f;
bool gavetaAberta = false;
bool mousePressionado = false;
bool primeiroMouse = true;
bool luzLampada = true;
bool luzAbajur = false;
bool portaAberta = false;
float anguloPorta = .0f;
bool tvLigada = false;
bool monitorLigado = false;


bool cortinaAberta = false;
float cortinaDeslocamento = 0.0f;

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


#include "extras.h"
#include "texturas.h"
#include "bmp_loader.h"

void gerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressionado = true;
            ultimoMouseX = x; // Grava onde o clique começou
            ultimoMouseY = y;
        } else if (state == GLUT_UP) {
            mousePressionado = false; // Soltou o clique, trava a câmera
        }
    }
}

void movimentoMouse(int x, int y) {
    if (!mousePressionado) return; // Só move a câmera se estiver segurando o clique

    float deltaX = x - ultimoMouseX;
    float deltaY = y - ultimoMouseY;

    ultimoMouseX = x;
    ultimoMouseY = y;

    float sensibilidade = 0.005f; 
    angulo += deltaX * sensibilidade; 
    pitch -= deltaY * sensibilidade;  

    if (pitch > 1.5f) pitch = 1.5f;
    if (pitch < -1.5f) pitch = -1.5f;

    dirX = cos(pitch) * cos(angulo);
    dirY = sin(pitch);
    dirZ = cos(pitch) * sin(angulo);

    glutPostRedisplay();
}
void teclado(unsigned char key, int x_mouse, int y_mouse){
    float len = sqrt(dirX*dirX + dirZ*dirZ);
    float fx = dirX / len, fz = dirZ / len;   // sem perder velocidade ao olhar para cima/baixo
    float velocidade = 0.5f;
    switch(tolower(key)){
        case 'w': mover( fx*velocidade,  fz*velocidade); break;
        case 's': mover(-fx*velocidade, -fz*velocidade); break;
        case 'd': mover(-fz*velocidade,  fx*velocidade); break;
        case 'a': mover( fz*velocidade, -fx*velocidade); break;
        case 'h': mostraHUD = !mostraHUD; break;
        case 'r': {
            float dist = sqrt(pow(camX, 2) + pow(camZ + 4.0f, 2));
            if (dist < 2.5f) anguloCadeiraAlvo += 45.0f;
            break;
        }
        case 'e': {
            float distGaveta  = sqrt(pow(camX - 3.0f, 2) + pow(camZ + 6.0f, 2));
            float distPorta   = sqrt(pow(camX + 5.5f, 2) + pow(camZ - 8.0f, 2));
            float distTV      = sqrt(pow(camX - 5.5f, 2) + pow(camZ - 7.0f, 2));
            float distMonitor = sqrt(pow(camX, 2) + pow(camZ + 6.0f, 2));

            float minDist = 999.0f;
            int objID = -1;

            if (distGaveta < 3.0f && distGaveta < minDist)   { minDist = distGaveta;  objID = 0; }
            if (distPorta < 4.0f && distPorta < minDist)     { minDist = distPorta;   objID = 1; }
            if (distTV < 4.0f && distTV < minDist)           { minDist = distTV;      objID = 2; }
            if (distMonitor < 3.5f && distMonitor < minDist) { minDist = distMonitor; objID = 3; }

            if (objID == 0) {
                gavetaAberta = !gavetaAberta;
                gavetaAlvo = gavetaAberta ? 1.0f : 0.0f;
            } else if (objID == 1) {
                portaAberta = !portaAberta;
                anguloPortaAlvo = portaAberta ? 90.0f : 0.0f;
            } else if (objID == 2) {
                tvLigada = !tvLigada;
                if (tvLigada) glEnable(GL_LIGHT2); else glDisable(GL_LIGHT2);
            } else if (objID == 3) {
                monitorLigado = !monitorLigado;
            }
            break;
        }
        case 'f':{
            float distLampada = sqrt(pow(camX - (3.0f), 2) + pow(camZ - (-6.0f), 2));
            
            if (distLampada < 3.0f) {
                // Se estiver perto, controla o abajur
                luzAbajur = !luzAbajur;
                if(luzAbajur) glEnable(GL_LIGHT1);
                else glDisable(GL_LIGHT1);
            } else {
                // Se estiver longe, controla a luz do teto
                luzLampada = !luzLampada;
                if(luzLampada) glEnable(GL_LIGHT0);
                else glDisable(GL_LIGHT0);
            }
            break;
        }
        case 'c':{
            float distJanela = sqrt(pow(camX - 4.0f, 2) + pow(camZ - (-7.9f), 2));
            if (distJanela < 4.0f) {
                cortinaAberta = !cortinaAberta;
                cortinaAlvo = cortinaAberta ? 1.0f : 0.0f;
            }
            break;
        }
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}


void reshape(int w, int h){
    if (h == 0) h = 1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(float)w/(float)h,.1,100.0);
    glMatrixMode(GL_MODELVIEW);
}



void desenhaLampada(){
    glPushMatrix();
        // Posiciona no centro do quarto, encostada no teto
        glTranslatef(0.0f, 6.0f, 0.0f); 

        // 1. Fio/Cabo descendo do teto
        matMetal(0.1f, 0.1f, 0.1f); 
        glPushMatrix();
            glTranslatef(0.0f, -0.4f, 0.0f); // Desce a metade do tamanho do fio
            glScalef(0.02f, 0.45f, 0.02f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 2. Lâmpada/Bulbo 
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

void desenhaArmario() {
    glPushMatrix();
        // Posicionado na parede da esquerda, mais próximo da porta 
        glTranslatef(-7.2f, 0.0f, 3.0f);

        // Corpo do Armário 
        matMadeira(.4f,.25f,.15f);
        //glColor3f(0.4f, 0.25f, 0.15f);
        glPushMatrix();
            glTranslatef(0.0f, 2.5f, 0.0f);
            caixaTex(texMadeira,1.5f, 5.0f, 3.0f); // 1.5m prof, 5m alt, 3m larg
            //glutSolidCube(1.0f);
        glPopMatrix();

        // Portas do Armário 
        matMadeira(.45f,.3f,.2f);
        //glColor3f(0.45f, 0.3f, 0.2f);
        
        // Porta Esquerda
        glPushMatrix();
            glTranslatef(0.76f, 2.5f, -0.75f);
            caixaTex(texMadeira,0.05f, 4.8f, 1.4f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        
        // Porta Direita
        glPushMatrix();
            glTranslatef(0.76f, 2.5f, 0.75f);
            caixaTex(texMadeira,0.05f, 4.8f, 1.4f);
            //glutSolidCube(1.0f);
        glPopMatrix();

        // Puxadores 
        matMetal(0.7f, 0.7f, 0.7f);
        //glColor3f(0.7f, 0.7f, 0.7f);
        
        // Puxador Esquerdo
        glPushMatrix();
            glTranslatef(0.8f, 2.5f, -0.1f);
            glScalef(0.05f, 0.6f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
        
        // Puxador Direito
        glPushMatrix();
            glTranslatef(0.8f, 2.5f, 0.1f);
            glScalef(0.05f, 0.6f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

void desenhaQuadrosEsquerda() {
    glPushMatrix();
        // Encostados na parede da esquerda 
        glTranslatef(-7.9f, 3.5f, -2.0f); 
        // Gira 90 graus para colar na parede lateral
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 

        // Função lambda/interna rápida para desenhar um quadro genérico
        auto desenhaUmQuadro = [](GLuint tex) {
            // Tela Branca
            glPushMatrix();
                matTecido(0.9f, 0.9f, 0.9f);
                glScalef(1.5f, 2.0f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
            matTecido(1, 1, 1);
            planoTex(tex, -0.75f,-1.0f,0.03f,  1.5f,0,0,  0,2.0f,0,  0,0,1,  1,1, 4,4);
            // Moldura Escura
            matMadeira(0.2f, 0.1f, 0.1f);
            glPushMatrix(); glTranslatef(0.0f, 1.05f, 0.0f); glScalef(1.6f, 0.1f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
            glPushMatrix(); glTranslatef(0.0f, -1.05f, 0.0f); glScalef(1.6f, 0.1f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
            glPushMatrix(); glTranslatef(-0.75f, 0.0f, 0.0f); glScalef(0.1f, 2.0f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
            glPushMatrix(); glTranslatef(0.75f, 0.0f, 0.0f); glScalef(0.1f, 2.0f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
        };
        
        // Quadro 1
        glPushMatrix();
            glTranslatef(-1.5f, 0.0f, 0.0f); 
            desenhaUmQuadro(texQuadroAbstrato);
        glPopMatrix();

        // Quadro 2 
        glPushMatrix();
            glTranslatef(1.5f, 0.5f, 0.0f); 
            desenhaUmQuadro(texQuadroPaisagem);
        glPopMatrix();

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
            glScalef(0.7f, 0.8f, 2.8f); // Proporções do rack
            glutSolidCube(1.0f);
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
            glScalef(0.02f, 0.7f, 0.9f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Porta Central
        glPushMatrix();
            glTranslatef(0.36f, 0.77f, 0.0f);
            glScalef(0.02f, 0.7f, 0.9f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Porta Direita
        glPushMatrix();
            glTranslatef(0.36f, 0.77f, 0.92f);
            glScalef(0.02f, 0.7f, 0.9f);
            glutSolidCube(1.0f);
        glPopMatrix();


        //  TELEVISÃO 
        glPushMatrix();
            glTranslatef(0.0f, 1.2f, 0.0f);

            // Base de apoio central (Preto)
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
                    // TV Desligada: Tela preta convencional
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

void desenhaPorta() {
    glPushMatrix();
        // Centraliza exatamente no buraco feito na parede
        glTranslatef(-5.5f, 2.0f, 8.0f); 
        
        // Moldura/Batentes fixos
        matMadeira(.3f,.15f,.05f);
        glPushMatrix(); // Batente Esquerdo
            glTranslatef(-1.05f, 0.0f, 0.0f);
            glScalef(0.1f, 4.0f, 0.15f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix(); // Batente Direito
            glTranslatef(1.05f, 0.0f, 0.0f);
            glScalef(0.1f, 4.0f, 0.15f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix(); // Batente Superior
            glTranslatef(0.0f, 2.05f, 0.0f);
            glScalef(2.2f, 0.1f, 0.15f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Dobradiça e Folha da Porta 
        glPushMatrix();
            // Joga o eixo de rotação na dobradiça esquerda 
            glTranslatef(-1.0f, 0.0f, 0.0f);
            glRotatef(anguloPorta, 0.0f, 1.0f, 0.0f);
            glTranslatef(1.0f, 0.0f, 0.0f); // Volta o desenho para o centro

            // Folha da porta
            glPushMatrix();
                matMadeira(.4f,.2f,.1f);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texMadeira); 
            
                glScalef(2.0f, 4.0f, 0.05f);
                cuboTex();
            
                glDisable(GL_TEXTURE_2D);
                //caixaTex(texMadeira,2.0f, 4.0f, 0.05f);
                //glutSolidCube(1.0f);
            glPopMatrix();

            // Maçanetas
            matMetal(0.8f, 0.8f, 0.2f);
            glPushMatrix();
                glTranslatef(0.8f, 0.0f, 0.05f); // Lado de fora
                glutSolidSphere(0.08, 10, 10);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.8f, 0.0f, -0.05f); // Lado de dentro
                glutSolidSphere(0.08, 10, 10);
            glPopMatrix();
            
        glPopMatrix();
    glPopMatrix();
}

void desenhaQuadro() {
    glPushMatrix();
        // lateral esquerda
        glTranslatef(-3.5f, 3.5f, -7.9f);
        
        glRotatef(90.0,.0f,1.0f,.0f);

        // tela do Quadro (Branca, pronta para receber a textura)
        glPushMatrix();
            glColor3f(0.9f, 0.9f, 0.9f);
            glScalef(0.05f, 2.0f, 3.0f); 
            glutSolidCube(1.0f);
        glPopMatrix();
        matTecido(1, 1, 1);
        planoTex(texQuadroPaisagem, -0.03f,-1.0f,-1.5f,  0,0,3.0f,  0,2.0f,0,  -1,0,0,  1,1, 6,4);
        // moldura escura
        glColor3f(0.2f, 0.1f, 0.1f);
        
        // topo e Base da Moldura
        glPushMatrix();
            glTranslatef(0.0f, 1.05f, 0.0f);
            glScalef(0.1f, 0.1f, 3.2f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, -1.05f, 0.0f);
            glScalef(0.1f, 0.1f, 3.2f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // lateral moldura 
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -1.55f);
            glScalef(0.1f, 2.0f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.55f);
            glScalef(0.1f, 2.0f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}


void desenhaEstante() {
    glPushMatrix();
        //parede esquerda
        glTranslatef(-6.45f, 0.0f, -7.45f);
        //glRotatef(90.0,.0f,1.0f,.0f); 
        matMadeira(0.5f, 0.3f, 0.15f);

        // Laterais
        glPushMatrix();
            glTranslatef(-1.0f, 2.0f, 0.0f);
            caixaTex(texMadeira,0.1f, 4.0f, 1.0f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0f, 2.0f, 0.0f);
            caixaTex(texMadeira,0.1f, 4.0f, 1.0f);
            //glutSolidCube(1.0f);
        glPopMatrix();

        // Topo e Base
        glPushMatrix();
            glTranslatef(0.0f, 4.0f, 0.0f);
            caixaTex(texMadeira,2.1f, 0.1f, 1.0f);
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 0.05f, 0.0f);
            caixaTex(texMadeira,2.1f, 0.1f, 1.0f);
            //glutSolidCube(1.0f);
        glPopMatrix();

        // Prateleiras
        for(float y = 1.0f; y <= 3.0f; y += 1.0f) {
            glPushMatrix();
                glTranslatef(0.0f, y, 0.0f);
                caixaTex(texMadeira,2.0f, 0.1f, 0.9f);
                //glutSolidCube(1.0f);
            glPopMatrix();
        }

        // Prateleira de baixo y = 1
        // Livros deitados 
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texLivro);

        glPushMatrix();
            matTecido(0.8f,0.5f,0.1f);
            //glColor3f(0.8f, 0.5f, 0.1f);  //Laranja 
            glTranslatef(0.5f, 1.1f, 0.0f);
            glScalef(0.6f, 0.15f, 0.8f);
            //glutSolidCube(1.0f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(0.2f,0.6f,0.6f);
            //glColor3f(0.2f, 0.6f, 0.6f); // Ciano escuro
            glTranslatef(0.5f, 1.25f, 0.0f);
            glScalef(0.55f, 0.15f, 0.75f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        // Livros em pe
        glPushMatrix();
            matTecido(.5f,.1f,.5f);
            //glColor3f(0.5f, 0.1f, 0.5f); // Roxo
            glTranslatef(-0.6f, 1.3f, 0.0f);
            glScalef(0.15f, 0.6f, 0.7f);
            //glutSolidCube(1.0f);
            cuboTex();
        glPopMatrix();
        glPushMatrix();
            matTecido(.9f,.8f,.1f);
            //glColor3f(0.9f, 0.8f, 0.1f); // Amarelo
            glTranslatef(-0.4f, 1.3f, 0.0f);
            glScalef(0.2f, 0.55f, 0.7f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();

        // prateleira do meio y =2 
        glPushMatrix();
            matTecido(.8f,.1f,.1f);
            //glColor3f(0.8f, 0.1f, 0.1f); // Vermelho
            glTranslatef(-0.5f, 2.3f, 0.0f);
            glScalef(0.2f, 0.5f, 0.7f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            matTecido(.1f,.2f,.8f);
            //glColor3f(0.1f, 0.2f, 0.8f); // Azul
            glTranslatef(-0.2f, 2.3f, 0.0f);
            glScalef(0.15f, 0.5f, 0.7f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            //glColor3f(0.1f, 0.7f, 0.2f); // Verde 
            matTecido(.1f,.7f,.2f);
            glTranslatef(.01f, 2.3f, 0.0f);
            glRotatef(15.0f, 0.0f, 0.0f, 1.0f); // Inclinacao
            glScalef(0.15f, 0.5f, 0.7f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            //glColor3f(0.4f, 0.4f, 0.4f); // Cinza
            matTecido(.4f,.4f,.4f);
            glTranslatef(0.5f, 2.3f, 0.0f);
            glScalef(0.18f, 0.55f, 0.7f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            //glColor3f(0.7f, 0.3f, 0.5f); // Rosa escuro
            matTecido(.7f,.3f,.5f);
            glTranslatef(0.7f, 2.3f, 0.0f);
            glScalef(0.15f, 0.5f, 0.7f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();

        // prateleira de cima y = 3
        glPushMatrix();
            //glColor3f(0.1f, 0.1f, 0.5f); // Azul escuro
            matTecido(.1f,.1f,.5f);
            glTranslatef(-0.7f, 3.25f, 0.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            //lColor3f(0.1f, 0.1f, 0.5f); // Azul escuro
            matTecido(.1f,.1f,.5f);
            glTranslatef(-0.55f, 3.25f, 0.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            //glColor3f(0.6f, 0.1f, 0.2f); // Vinho
            matTecido(.6f,.1f,.2f);
            glTranslatef(-0.4f, 3.25f, 0.0f);
            glScalef(0.15f, 0.45f, 0.65f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            //glColor3f(0.3f, 0.5f, 0.2f); // Verde musgo 
            matTecido(.3f,.5f,.2f);
            glTranslatef(-0.17f, 3.2f, 0.0f);
            glRotatef(25.0f, 0.0f, 0.0f, 1.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            cuboTex();
            //glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

void desenhaJanela() {
    glPushMatrix();
        //proximo a parede
        glTranslatef(4.0f, 3.5f, -7.9f);

        //vidro janela
        glPushMatrix();
            matVidro(1.0f, 1.0f, 1.0f); 
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texCortina); 
            
            glScalef(3.0f, 2.0f, 0.05f);
            cuboTex();
            
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //moldura topo
        matPlastico(0.9f, 0.9f, 0.9f);
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            glScalef(3.2f, 0.1f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, -1.0f, 0.0f);
            glScalef(3.2f, 0.1f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //moldura laterais 
        glPushMatrix();
            glTranslatef(-1.55f, 0.0f, 0.0f);
            glScalef(0.1f, 2.0f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.55f, 0.0f, 0.0f);
            glScalef(0.1f, 2.0f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}
void desenhaCamaEMovel(){
    // CAMA
    glPushMatrix();
        glTranslatef(5.5f, 0.0f, -4.0f);
        
        // base da cama
        glPushMatrix();
            glTranslatef(0.0f, 0.4f, 0.0f);
            matMadeira(0.5f, 0.3f, 0.2f);
            caixaTex(texMadeira, 3.0f, 0.8f, 6.0f);
        glPopMatrix();
        
        // colchao
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            matTecido(0.3f, 0.4f, 0.7f);
            glScalef(2.8f, 0.4f, 5.8f);
            glutSolidCube(1.0f);
        glPopMatrix();
        desenhaColcha();
        // travesseiro
        glPushMatrix(); // 
            glTranslatef(0.0f, 1.3f, -2.0f); 
            glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
            matTecido(1, 1, 1);
            caixaTex(texMouse, 1.5f, 0.2f, 1.0f);
            //glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix(); 

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
            matMadeira(.4f,.25f,.15f);
            
            //  Fundo (Base da gaveta)
            glPushMatrix();
                glTranslatef(0.0f, -0.25f, 0.0f);
                caixaTex(texMesa, 1.5f, 0.1f, 1.4f);
            glPopMatrix();

            //  Parede Frontal (onde fica o puxador)
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
            
            // Puxador  ajustado para a nova parede frontal
            glPushMatrix();
                glTranslatef(-0.8f, 0.0f, 0.0f); 
                matMetal(0.1f, 0.1f, 0.1f);
                glutSolidSphere(0.1, 10, 10);
            glPopMatrix();

            glPushMatrix();
                // Posiciona o livro no centro da gaveta. 
                glTranslatef(-0.25f, 0.15f, 0.0f); 
                glRotatef(-90.0f,0.0f,1.0f,.0f);
                // Capa Preta
                matTecido(1.0f, 1.0f, 1.0f); // Branco reflete a imagem fielmente
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
                float posLuzAbajur[] = {0.0f, 0.0f, 0.0f, 1.0f};
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
                GLUquadric* quadAbajur = gluNewQuadric();
                gluCylinder(quadAbajur, 0.4, 0.15, 0.5, 16, 16);
                gluDeleteQuadric(quadAbajur);
                
                // Desliga a emissão para não afetar os próximos objetos
                float semEmissaoCupula[] = {0.0f, 0.0f, 0.0f, 1.0f};
                glMaterialfv(GL_FRONT, GL_EMISSION, semEmissaoCupula);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix(); 
}

void desenhaCadeira(){
    glPushMatrix();
        //interacao
        glTranslatef(0.0f, 0.0f, -4.0f);
        glRotatef(anguloCadeira, 0.0f, 1.0f, 0.0f);

        //assento
        glPushMatrix();
            glTranslatef(0.0f, 1.2f, 0.0f); 
            matCouro(0.2f, 0.3f, 0.4f);
            glScalef(1.2f, 0.2f, 1.0f);     
            glutSolidCube(1.0f);
        glPopMatrix();

        //encosto
        glPushMatrix();
            glTranslatef(0.0f, 1.7f, 0.45f); //atras do assento
            matCouro(0.2f, 0.3f, 0.4f);
            glScalef(1.2f, 1.0f, 0.15f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //apoio braço esq
        glPushMatrix();
            //haste vertical
            glPushMatrix();
                glTranslatef(-0.55f, 1.35f, 0.1f);
                matMetal(0.1f, 0.1f, 0.1f);
                glScalef(0.05f, 0.3f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
            //apoio horizontal
            glPushMatrix();
                glTranslatef(-0.55f, 1.5f, 0.0f);
                glScalef(0.1f, 0.05f, 0.6f);
                glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();

        //apoio braço dir
        glPushMatrix();
            //haste vertical
            glPushMatrix();
                glTranslatef(0.55f, 1.35f, 0.1f);
                matMetal(0.1f, 0.1f, 0.1f);
                glScalef(0.05f, 0.3f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
            //apoio horizontal
            glPushMatrix();
                glTranslatef(0.55f, 1.5f, 0.0f);
                glScalef(0.1f, 0.05f, 0.6f);
                glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();
        
        //haste central 
        glPushMatrix();
            glTranslatef(0.0f, 0.6f, 0.0f);
            matMetal(0.1f, 0.1f, 0.1f); 
            glScalef(0.1f, 1.2f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //base
        glPushMatrix();
            glTranslatef(0.0f, 0.1f, 0.0f);
            matMetal(0.1f, 0.1f, 0.1f);
            glScalef(1.5f, 0.1f, 0.2f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.1f, 0.0f);
            matMetal(0.1f, 0.1f, 0.1f);
            glScalef(0.2f, 0.1f, 1.5f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

void desenhaPeriferico(GLuint texTopo, float w, float h, float d) {
    float x = w/2.0f, y = h/2.0f, z = d/2.0f;
    
    // Topo 
    matPlastico(1.0f, 1.0f, 1.0f);
    planoTex(texTopo,  x, y,-z, -w,0,0,  0,0,d,  0,1,0,  1,1, 1,1);
    
    matPlastico(0.1f, 0.1f, 0.1f);
    planoTex(0, -x,-y,-z,  w,0,0,  0,0,d,  0,-1,0, 1,1, 1,1); // Fundo
    planoTex(0, -x,-y, z,  w,0,0,  0,h,0,  0,0,1,  1,1, 1,1); // Frente
    planoTex(0, -x,-y,-z,  w,0,0,  0,h,0,  0,0,-1, 1,1, 1,1); // Trás
    planoTex(0,  x,-y,-z,  0,0,d,  0,h,0,  1,0,0,  1,1, 1,1); // Direita
    planoTex(0, -x,-y,-z,  0,0,d,  0,h,0, -1,0,0,  1,1, 1,1); // Esquerda
}
void desenhaAcessorio(){
    // Teclado
    glPushMatrix();
        //encostar na mesa
        glTranslatef(0.0f, 2.11f, -5.2f); 
        
        // Inclinação ergonómica de no eixo X
        glRotatef(7.0f, 1.0f, 0.0f, 0.0f);

        // Corpo principal 
        glPushMatrix();
            glTranslatef(0.0f, 0.05f, -0.05f); 
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            desenhaPeriferico(texTeclado, 1.2f, 0.06f, 0.35f);
        glPopMatrix();

        // Descanso de pulso frontal 
        glPushMatrix();
            glTranslatef(0.0f, 0.02f, 0.15f); 
            matPlastico(0.12f, 0.12f, 0.12f);
            glScalef(1.2f, 0.04f, 0.12f);
            glutSolidCube(1.0f);
        glPopMatrix();
        
        // Pezinhos de elevação traseiros
        matPlastico(0.05f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(-0.5f, 0.02f, -0.2f);
            glScalef(0.08f, 0.06f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.5f, 0.02f, -0.2f);
            glScalef(0.08f, 0.06f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix();

    // (Mouse)
    glPushMatrix();
        glTranslatef(0.8f, 2.1f, -5.2f); 

        // Corpo curvo do mouse
        matPlastico(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            // Transforma a esfera
            glScalef(0.12f, 0.06f, 0.18f); 
            esferaTex(texMouse, 1.0f);     
        glPopMatrix();

        // Scroll
        glPushMatrix();
            // Acompanha a descida e avança para a curva frontal do rato
            glTranslatef(0.0f, 0.06f, -0.12f); 
            matMetal(0.1f, 0.1f, 0.1f);        
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glutSolidTorus(0.012, 0.020, 15, 15); 
        glPopMatrix();
    glPopMatrix();

    // Caneca 
    glPushMatrix();
        glTranslatef(-1.0f, 2.13f, -5.5f);
        glRotatef(-90.0f, 1, 0, 0);
        matCeramica(1, 1, 1);
        cilindroTex(texMouse, 0.15f, 0.15f, 0.3f);
    glPopMatrix();
}

void desenhaMonitor(){
    glPushMatrix();
        glTranslatef(.0f,2.1f,-6.0f);
        matPlastico(0.2f, 0.2f, 0.2f);
        //apoio
        glPushMatrix();
            glScalef(1.0f, 0.1f, 0.8f);
            glutSolidCube(1.0);
        glPopMatrix();
        //haste vertical
        glPushMatrix();
            glTranslatef(0.0f, 0.4f, -0.2f);
            glScalef(0.2f, 0.8f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
        //tela principal
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            
            // Moldura Traseira 
            matPlastico(0.15f, 0.15f, 0.15f);
            glPushMatrix();
                glScalef(2.0f, 1.2f, 0.2f);
                glutSolidCube(1.0);
            glPopMatrix();

            // Display / Vidro da Tela
            if (!monitorLigado) {
                // Monitor Desligado
                matVidro(0.1f, 0.1f, 0.12f);
                glPushMatrix();
                    glTranslatef(0.0f, 0.0f, 0.11f); // Desloca para frente 
                    glScalef(1.9f, 1.1f, 0.02f);
                    glutSolidCube(1.0);
                glPopMatrix();
            } else {
            matVidro(1, 1, 1);
            float emissaoMonitor[] = {0.6f, 0.6f, 0.6f, 1.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, emissaoMonitor);
            telaMonitor();
            float semEmissao[] = {0, 0, 0, 1};
            glMaterialfv(GL_FRONT, GL_EMISSION, semEmissao);
        }
        glPopMatrix();
    glPopMatrix();
}

void desenhaMesa(){
    glPushMatrix();
        glTranslatef(.0f,.0f,-6.0f);
        
        matMadeira(1.0f, 1.0f, 1.0f); 

        // Tampo 
        glPushMatrix();
            glTranslatef(.0f,2.0f,.0f);
            caixaTex(texMesa, 4.0f, 0.2f, 2.0f);  // aplica textura
        glPopMatrix();
        
        // Perna Fesq (Larg: 0.2, Alt: 2.0, Prof: 0.2)
        glPushMatrix();
            glTranslatef(-1.8f, 1.0f, 0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
        
        // Perna Fdir
        glPushMatrix();
            glTranslatef(1.8f, 1.0f, 0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
        
        // Perna Tesq
        glPushMatrix();
            glTranslatef(-1.8f, 1.0f, -0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
        
        // Perna Tdir
        glPushMatrix();
            glTranslatef(1.8f, 1.0f, -0.8f);
            caixaTex(texMesa, 0.2f, 2.0f, 0.2f);
        glPopMatrix();
    glPopMatrix();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX,camY,camZ, camX + dirX,camY + dirY,camZ + dirZ,.0f,1.0f,.0f);
    float posicaoLuzTeto[] = {0.0f, 5.4f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuzTeto);
    float posLuzTV[] = {5.5f, 2.9f, 6.2f, 1.0f};
    glLightfv(GL_LIGHT2, GL_POSITION, posLuzTV);
    
    desenhaSala();
    desenhaTapete();
    desenhaMesa();
    desenhaMonitor();
    desenhaAcessorio();
    desenhaCadeira();
    desenhaCamaEMovel();
    desenhaJanela();
    desenhaEstante();
    desenhaQuadro();
    desenhaPorta();
    desenhaRackETV();
    desenhaArmario();
    desenhaQuadrosEsquerda();
    desenhaLampada();
    
    desenhaCortinas();
    desenhaHUD();
    
    glutSwapBuffers();
}

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_NORMALIZE);
    //ativando iluminacao e cores
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //luz principal (0)
    float luzAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    float luzDifusa[]   = {0.6f, 0.6f, 0.6f, 1.0f}; 
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glEnable(GL_LIGHT0); 

    // Luz 1: Abajur (Amarelada)
    float luzAmbiente1[] = {0.2f, 0.2f, 0.0f, 1.0f}; 
    float luzDifusa1[]   = {1.0f, 0.9f, 0.2f, 1.0f}; 
    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa1);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);
    //glEnable(GL_LIGHT1);
    // Luz 0 (teto)
    float espec0[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, espec0);

    // Luz 1 (abajur)
    float espec1[] = {1.0f, 0.9f, 0.4f, 1.0f};
    glLightfv(GL_LIGHT1, GL_SPECULAR, espec1);

    // Brilho calculado com a posição real da câmera 
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // Luz ambiente global baixinha, para o quarto não ficar preto com a luz apagada
    float ambGlobal[] = {0.08f, 0.08f, 0.08f, 1.0f};
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGlobal);

    glShadeModel(GL_SMOOTH);
    initExtras();
    initTexAvancadas();
    carregaImagens();
}

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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