#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
float camX = .0f, camY = 3.0f, camZ= 12.0f;
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
bool primeiroMouse = true;
bool luzLampada = true;
bool luzAbajur = true;

bool cortinaAberta = false;
float cortinaDeslocamento = 0.0f;

void capturaMouse(int x, int y) {
    if (primeiroMouse) {
        // No primeiro movimento, apenas guarda onde o mouse apareceu
        ultimoMouseX = x;
        ultimoMouseY = y;
        primeiroMouse = false;
        return;
    }

    // Calcula apenas a diferença (delta) de onde o mouse estava para onde ele foi
    float deltaX = x - ultimoMouseX;
    float deltaY = y - ultimoMouseY;

    // Atualiza a posição para o próximo frame
    ultimoMouseX = x;
    ultimoMouseY = y;

    float sensibilidade = 0.005f; 
    
    // Se a esquerda/direita continuar invertida para o seu gosto, 
    // troque o '+=' por '-=' nesta linha do angulo:
    angulo += deltaX * sensibilidade; 
    
    // Se o cima/baixo estiver invertido, troque o '-=' por '+=' aqui:
    pitch -= deltaY * sensibilidade;  

    // Limites de rotação do pescoço
    if (pitch > 1.5f) pitch = 1.5f;
    if (pitch < -1.5f) pitch = -1.5f;

    // Atualização dos vetores 3D
    dirX = cos(pitch) * cos(angulo);
    dirY = sin(pitch);
    dirZ = cos(pitch) * sin(angulo);

    glutPostRedisplay();
}
void teclado(unsigned char key, int x_mouse, int y_mouse){
    float velocidade = .5f;
    float giro = .1f;
    switch(tolower(key)){
        case 'w': 
            camX += dirX * velocidade; 
            camZ += dirZ * velocidade; 
            break;
        case 's': 
            camX -= dirX * velocidade; 
            camZ -= dirZ * velocidade; 
            break;
        case 'd': 
            //angulo -= giro; 
            //dirX = cos(angulo); 
            //dirZ = sin(angulo);
            camX -= dirZ * velocidade; 
            camZ += dirX * velocidade;
            break;
        case 'a': 
            //angulo += giro; 
            //dirX = cos(angulo); 
            // dirZ = sin(angulo); 
            camX += dirZ * velocidade; 
            camZ -= dirX * velocidade;
            break;
        case 'r':{
            float dist = sqrt(pow(camX - 0.0f, 2) + pow(camZ - (-0.5f), 2));
            if(dist<2.5f){
                anguloCadeira += 15.0f;
            }
            break;
        }
        case 'e':{
            float distGaveta = sqrt(pow(camX - (-4.0f), 2) + pow(camZ - (-3.0f), 2));
            if (distGaveta < 3.0f) { 
                gavetaAberta = !gavetaAberta;
                gavetaX = gavetaAberta ? 1.0f : 0.0f; // puxar 1 unidade p fora
            }
            break;
        }
        case 'f':{
            float distLampada = sqrt(pow(camX - (-4.0f), 2) + pow(camZ - (-3.0f), 2));
            
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
                // Empurra a cortina 1.2 unidades para os lados quando aberta
                cortinaDeslocamento = cortinaAberta ? 1.2f : 0.0f; 
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

void desenhaQuadro() {
    glPushMatrix();
        // lateral esquerda
        glTranslatef(-3.5f, 3.5f, -7.0f);
        
        glRotatef(90.0,.0f,1.0f,.0f);

        // tela do Quadro (Branca, pronta para receber a textura)
        glPushMatrix();
            glColor3f(0.9f, 0.9f, 0.9f);
            glScalef(0.05f, 2.0f, 3.0f); 
            glutSolidCube(1.0f);
        glPopMatrix();

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
        glTranslatef(-7.45f, 0.0f, 0.0f);
        glRotatef(90.0,.0f,1.0f,.0f); 
        glColor3f(0.5f, 0.3f, 0.15f); // Cor da madeira

        // Laterais
        glPushMatrix();
            glTranslatef(-1.0f, 2.0f, 0.0f);
            glScalef(0.1f, 4.0f, 1.0f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0f, 2.0f, 0.0f);
            glScalef(0.1f, 4.0f, 1.0f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Topo e Base
        glPushMatrix();
            glTranslatef(0.0f, 4.0f, 0.0f);
            glScalef(2.1f, 0.1f, 1.0f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 0.05f, 0.0f);
            glScalef(2.1f, 0.1f, 1.0f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Prateleiras
        for(float y = 1.0f; y <= 3.0f; y += 1.0f) {
            glPushMatrix();
                glTranslatef(0.0f, y, 0.0f);
                glScalef(2.0f, 0.1f, 0.9f);
                glutSolidCube(1.0f);
            glPopMatrix();
        }

        // Prateleira de baixo y = 1
        // Livros deitados 
        glPushMatrix();
            glColor3f(0.8f, 0.5f, 0.1f);  //Laranja 
            glTranslatef(0.5f, 1.1f, 0.0f);
            glScalef(0.6f, 0.15f, 0.8f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.2f, 0.6f, 0.6f); // Ciano escuro
            glTranslatef(0.5f, 1.25f, 0.0f);
            glScalef(0.55f, 0.15f, 0.75f);
            glutSolidCube(1.0f);
        glPopMatrix();
        // Livros em pe
        glPushMatrix();
            glColor3f(0.5f, 0.1f, 0.5f); // Roxo
            glTranslatef(-0.6f, 1.3f, 0.0f);
            glScalef(0.15f, 0.6f, 0.7f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.9f, 0.8f, 0.1f); // Amarelo
            glTranslatef(-0.4f, 1.3f, 0.0f);
            glScalef(0.2f, 0.55f, 0.7f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // prateleira do meio y =2 
        glPushMatrix();
            glColor3f(0.8f, 0.1f, 0.1f); // Vermelho
            glTranslatef(-0.5f, 2.3f, 0.0f);
            glScalef(0.2f, 0.5f, 0.7f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.1f, 0.2f, 0.8f); // Azul
            glTranslatef(-0.2f, 2.3f, 0.0f);
            glScalef(0.15f, 0.5f, 0.7f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.1f, 0.7f, 0.2f); // Verde 
            glTranslatef(.01f, 2.3f, 0.0f);
            glRotatef(15.0f, 0.0f, 0.0f, 1.0f); // Inclinacao
            glScalef(0.15f, 0.5f, 0.7f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.4f, 0.4f, 0.4f); // Cinza
            glTranslatef(0.5f, 2.3f, 0.0f);
            glScalef(0.18f, 0.55f, 0.7f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.7f, 0.3f, 0.5f); // Rosa escuro
            glTranslatef(0.7f, 2.3f, 0.0f);
            glScalef(0.15f, 0.5f, 0.7f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // prateleira de cima y = 3
        glPushMatrix();
            glColor3f(0.1f, 0.1f, 0.5f); // Azul escuro
            glTranslatef(-0.7f, 3.25f, 0.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.1f, 0.1f, 0.5f); // Azul escuro
            glTranslatef(-0.55f, 3.25f, 0.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.6f, 0.1f, 0.2f); // Vinho
            glTranslatef(-0.4f, 3.25f, 0.0f);
            glScalef(0.15f, 0.45f, 0.65f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.3f, 0.5f, 0.2f); // Verde musgo (Inclinado)
            glTranslatef(-0.17f, 3.2f, 0.0f);
            glRotatef(25.0f, 0.0f, 0.0f, 1.0f);
            glScalef(0.12f, 0.45f, 0.6f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

void desenhaJanela() {
    glPushMatrix();
        //proximo a parede
        glTranslatef(4.0f, 3.5f, -7.9f);

        //vidro janela
        glPushMatrix();
            glColor3f(0.5f, 0.7f, 0.9f); // Azul claro
            glScalef(3.0f, 2.0f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //moldura topo
        glColor3f(0.9f, 0.9f, 0.9f);
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

        //cortinas laterais  
        glColor4f(0.8f, 0.7f, 0.5f, 0.7f); 

        // Cortina Esquerda
        glPushMatrix();
            // Ponto âncora: canto esquerdo.
            // Z = 0.2f joga a cortina bem para a frente do vidro.
            glTranslatef(-1.6f, 0.0f, 0.1f);
            
            // Quando aberta, encolhe para 0.2. Quando fechada, cresce para 1.55 (metade da janela).
            float escalaEsq = cortinaAberta ? 0.3f : 1.6f;
            
            glTranslatef(escalaEsq / 2.0f, 0.0f, 0.0f);
            glScalef(escalaEsq, 2.2f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Cortina Direita
        glPushMatrix();
            // Ponto âncora: canto direito.
            glTranslatef(1.6f, 0.0f, 0.1f);
            
            float escalaDir = cortinaAberta ? 0.3f : 1.6f;
            
            glTranslatef(-escalaDir / 2.0f, 0.0f, 0.0f);
            glScalef(escalaDir, 2.2f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}
void desenhaCamaEMovel(){
    // CAMA
    glPushMatrix();
        glTranslatef(5.5f, 0.0f, -1.0f);
        
        // base da cama
        glPushMatrix();
            glTranslatef(0.0f, 0.4f, 0.0f);
            glColor3f(0.5f, 0.3f, 0.2f);
            glScalef(3.0f, 0.8f, 6.0f);
            glutSolidCube(1.0f);
        glPopMatrix();
        
        // colchao
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            glColor3f(0.3f, 0.4f, 0.7f);
            glScalef(2.8f, 0.4f, 5.8f);
            glutSolidCube(1.0f);
        glPopMatrix();
        
        // travesseiro
        glPushMatrix(); // 
            glTranslatef(0.0f, 1.3f, -2.0f); 
            glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glScalef(1.5f, 0.2f, 1.0f);
            glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix(); 

    //movel 
    glPushMatrix(); // matriz separada da cama
        glTranslatef(-4.0f, 0.0f, -2.0f); // lado esquerdo da mesa
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        
        // corpo
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            glColor3f(0.4f, 0.25f, 0.15f); 
            glScalef(1.5f, 2.0f, 1.5f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Gaveta interativa e puxador
        glPushMatrix();
            glTranslatef(-0.1f - gavetaX, 1.5f, 0.0f); 
            glColor3f(0.45f, 0.3f, 0.2f); // Cor da madeira da gaveta
            
            // 1. Fundo (Base da gaveta)
            glPushMatrix();
                glTranslatef(0.0f, -0.25f, 0.0f);
                glScalef(1.5f, 0.1f, 1.4f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // 2. Parede Frontal (onde fica o puxador)
            glPushMatrix();
                glTranslatef(-0.7f, 0.0f, 0.0f);
                glScalef(0.1f, 0.6f, 1.4f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // 3. Parede Traseira
            glPushMatrix();
                glTranslatef(0.7f, 0.0f, 0.0f);
                glScalef(0.1f, 0.6f, 1.4f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // 4. Parede Lateral Esquerda
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, -0.65f);
                glScalef(1.5f, 0.6f, 0.1f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // 5. Parede Lateral Direita
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, 0.65f);
                glScalef(1.5f, 0.6f, 0.1f);
                glutSolidCube(1.0f);
            glPopMatrix();
            
            // Puxador (Bolinha preta) ajustado para a nova parede frontal
            glPushMatrix();
                glTranslatef(-0.8f, 0.0f, 0.0f); 
                glColor3f(0.1f, 0.1f, 0.1f);
                glutSolidSphere(0.1, 10, 10);
            glPopMatrix();
        glPopMatrix(); // Fecha gaveta

        //abajur encima do movel
        glPushMatrix();
            glTranslatef(0.0f, 2.0f, 0.0f); 
            
            //base
            glPushMatrix();
                glColor3f(0.3f, 0.3f, 0.3f); 
                glScalef(0.4f, 0.05f, 0.4f);
                glutSolidCube(1.0f);
            glPopMatrix();

            //haste
            glPushMatrix();
                glTranslatef(0.0f, 0.4f, 0.0f);
                glColor3f(0.7f, 0.6f, 0.2f); 
                glScalef(0.05f, 0.8f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();

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
                
                glColor3f(1.0f, 1.0f, 0.8f); // Cor base da lâmpada
                glutSolidSphere(0.08, 10, 10); // Desenha a bolinha
                
                // desliga emissao nao afeta cor objetos
                float semEmissao[] = {0.0f, 0.0f, 0.0f, 1.0f};
                glMaterialfv(GL_FRONT, GL_EMISSION, semEmissao);
            glPopMatrix();

            //cupula 
            glPushMatrix();
                glTranslatef(0.0f, 0.5f, 0.0f);
                glColor3f(0.9f, 0.9f, 0.4f); // Amarelo base
                
                // --- NOVO: FAZ A CÚPULA BRILHAR QUANDO LIGADA ---
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
        glTranslatef(0.0f, 0.0f, -0.20f);
        glRotatef(anguloCadeira, 0.0f, 1.0f, 0.0f);

        //assento
        glPushMatrix();
            glTranslatef(0.0f, 1.2f, 0.0f); 
            glColor3f(0.2f, 0.3f, 0.4f);
            glScalef(1.2f, 0.2f, 1.0f);     
            glutSolidCube(1.0f);
        glPopMatrix();

        //encosto
        glPushMatrix();
            glTranslatef(0.0f, 1.7f, 0.45f); //atras do assento
            glColor3f(0.2f, 0.3f, 0.4f);
            glScalef(1.2f, 1.0f, 0.15f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //apoio braço esq
        glPushMatrix();
            //haste vertical
            glPushMatrix();
                glTranslatef(-0.55f, 1.35f, 0.1f);
                glColor3f(0.1f, 0.1f, 0.1f);
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
                glColor3f(0.1f, 0.1f, 0.1f);
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
            glColor3f(0.1f, 0.1f, 0.1f); 
            glScalef(0.1f, 1.2f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();

        //base
        glPushMatrix();
            glTranslatef(0.0f, 0.1f, 0.0f);
            glColor3f(0.1f, 0.1f, 0.1f);
            glScalef(1.5f, 0.1f, 0.2f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.1f, 0.0f);
            glColor3f(0.1f, 0.1f, 0.1f);
            glScalef(0.2f, 0.1f, 1.5f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

void desenhaAcessorio(){
    //teclado
    glPushMatrix();
        glTranslatef(0.0f, 2.15f, -1.2f); 
        glColor3f(0.8f, 0.8f, 0.8f); // Cinza claro
        glScalef(1.2f, 0.1f, 0.4f);
        glutSolidCube(1.0);
    glPopMatrix();

    //mouse
    glPushMatrix();
        glTranslatef(0.8f, 2.15f, -1.2f); 

        glPushMatrix();
            glColor3f(0.1f, 0.1f, 0.1f); // Preto/Cinza escuro
            glScalef(0.2f, 0.1f, 0.3f);
            glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
            // Sobe um pouco o eixo Y (+0.06) e vai para a parte da frente (-0.1 no Z)
            glTranslatef(0.0f, 0.08f, -0.12f); 
            glColor3f(1.0f, 1.0f, 1.0f); // Branco
            
            // O Torus nasce "deitado". Giramos 90 graus no eixo Y para ele ficar em pé
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
            
            // Parâmetros: raio interno, raio externo, fatias (lados), anéis
            glutSolidTorus(0.015, 0.04, 15, 15);
        glPopMatrix();
    glPopMatrix();

    //caneca;
    glPushMatrix();
        glTranslatef(-1.0f, 2.1f, -1.5f); 
        glColor3f(0.8f, 0.1f, 0.1f); 
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        GLUquadric* quad = gluNewQuadric();
        
        // Corpo da caneca
        gluCylinder(quad, 0.15, 0.15, 0.3, 16, 16); 
        // Fundo da caneca 
        gluDisk(quad, 0.0, 0.15, 16, 1); 
        
        gluDeleteQuadric(quad);
    glPopMatrix();
}

void desenhaMonitor(){
    glPushMatrix();
        glTranslatef(.0f,2.1f,-2.0f);
        glColor3f(.2f,.2f,.2f);
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
            glScalef(2.0f, 1.2f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();
}

void desenhaMesa(){
    glPushMatrix();
        //mesa no centro
        glTranslatef(.0f,.0f,-2.0f);
        glColor3f(.7f,.4f,.2f);

        //tampo
        glPushMatrix();
            glTranslatef(.0f,2.0f,.0f);
            glScalef(4.0f,.2f,2.0f);
            glutSolidCube(1.0);
        glPopMatrix();
        //perna Fesq
        glPushMatrix();
            glTranslatef(-1.8f, 1.0f, 0.8f);
            glScalef(0.2f, 2.0f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
        //perna Fdir
        glPushMatrix();
            glTranslatef(1.8f, 1.0f, 0.8f);
            glScalef(0.2f, 2.0f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
        //perna Tesq
        glPushMatrix();
            glTranslatef(-1.8f, 1.0f, -0.8f);
            glScalef(0.2f, 2.0f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
        //perna Tdir
        glPushMatrix();
            glTranslatef(1.8f, 1.0f, -0.8f);
            glScalef(0.2f, 2.0f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();



}

void desenhaSala() {
    glBegin(GL_QUADS);
        // chao (Luz rebate para Cima: Y = 1)
        glColor3f(0.4f, 0.25f, 0.15f);
        glNormal3f(0.0f, 1.0f, 0.0f); 
        glVertex3f(-8.0f, 0.0f,  8.0f);
        glVertex3f( 8.0f, 0.0f,  8.0f);
        glVertex3f( 8.0f, 0.0f, -8.0f);
        glVertex3f(-8.0f, 0.0f, -8.0f);

        // parede fundo (Luz rebate para Frente: Z = 1)
        glColor3f(0.5f, 0.5f, 0.5f);
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glVertex3f(-8.0f, 0.0f, -8.0f);
        glVertex3f( 8.0f, 0.0f, -8.0f);
        glVertex3f( 8.0f, 6.0f, -8.0f);
        glVertex3f(-8.0f, 6.0f, -8.0f);

        // parede Lateral Esquerda (Luz rebate para Direita: X = 1)
        glColor3f(0.45f, 0.45f, 0.45f);
        glNormal3f(1.0f, 0.0f, 0.0f); 
        glVertex3f(-8.0f, 0.0f,  8.0f);
        glVertex3f(-8.0f, 0.0f, -8.0f);
        glVertex3f(-8.0f, 6.0f, -8.0f);
        glVertex3f(-8.0f, 6.0f,  8.0f);
    glEnd();
}
/*
void desenhaSala() {
    //SUBDIVISAO PARA MELHORAR A REFLEXAO DA LUZ
    glBegin(GL_QUADS);
        //CHAO SUBDIVIDIDO (Malha 1x1 metro)
        glColor3f(0.4f, 0.25f, 0.15f);
        glNormal3f(0.0f, 1.0f, 0.0f); 
        for(float x = -8.0f; x < 8.0f; x += 0.25f) {
            for(float z = -8.0f; z < 8.0f; z += 0.25f) {
                glVertex3f(x, 0.0f, z + 0.25f);
                glVertex3f(x + 0.25f, 0.0f, z + 0.25f);
                glVertex3f(x + 0.25f, 0.0f, z);
                glVertex3f(x, 0.0f, z);
            }
        }

        //PAREDE DO FUNDO SUBDIVIDIDA
        glColor3f(0.5f, 0.5f, 0.5f);
        glNormal3f(0.0f, 0.0f, 1.0f); 
        for(float x = -8.0f; x < 8.0f; x += 1.0f) {
            for(float y = 0.0f; y < 6.0f; y += 1.0f) {
                glVertex3f(x, y, -8.0f);
                glVertex3f(x + 1.0f, y, -8.0f);
                glVertex3f(x + 1.0f, y + 1.0f, -8.0f);
                glVertex3f(x, y + 1.0f, -8.0f);
            }
        }

        //PAREDE LATERAL ESQUERDA SUBDIVIDIDA
        glColor3f(0.45f, 0.45f, 0.45f);
        glNormal3f(1.0f, 0.0f, 0.0f); 
        for(float z = -8.0f; z < 8.0f; z += 1.0f) {
            for(float y = 0.0f; y < 6.0f; y += 1.0f) {
                glVertex3f(-8.0f, y, z + 1.0f);
                glVertex3f(-8.0f, y, z);
                glVertex3f(-8.0f, y + 1.0f, z);
                glVertex3f(-8.0f, y + 1.0f, z + 1.0f);
            }
        }
    glEnd();
}
*/
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX,camY,camZ, camX + dirX,camY + dirY,camZ + dirZ,.0f,1.0f,.0f);

    float posicaoLuzTeto[] = {0.0f, 5.0f, 0.0f, 1.0f}; 
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuzTeto);

    desenhaSala();
    desenhaMesa();
    desenhaMonitor();
    desenhaAcessorio();
    desenhaCadeira();
    desenhaCamaEMovel();
    desenhaJanela();
    desenhaEstante();
    desenhaQuadro();
    
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
    glEnable(GL_LIGHT1);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glutPassiveMotionFunc(capturaMouse);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop();
    return 0;
}