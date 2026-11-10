// entrada.h - callbacks de mouse e teclado
#ifndef ENTRADA_H
#define ENTRADA_H

void gerenciaMouse(int button, int state, int x, int y);
void movimentoMouse(int x, int y);
void teclado(unsigned char key, int x_mouse, int y_mouse);
void tecladoUp(unsigned char key, int x_mouse, int y_mouse);
void atualizaMovimento();

#endif
