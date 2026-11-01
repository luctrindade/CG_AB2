// cadeira.cpp - cadeira giratoria
#include "comum.h"

void desenhaCadeira(){
    glPushMatrix();
        //interacao
        glTranslatef(0.0f, 0.0f, -4.0f);
        glRotatef(anguloCadeira, 0.0f, 1.0f, 0.0f);

        //assento
        glPushMatrix();
            glTranslatef(0.0f, 1.2f, 0.0f); 
            matCouro(1.0f, 1.0f, 1.0f); 
            
            
            caixaTex(texCouro, 1.2f, 0.2f, 1.0f);
            
            
        glPopMatrix();

        //encosto
        glPushMatrix();
            glTranslatef(0.0f, 1.7f, 0.45f); 
            matCouro(1.0f, 1.0f, 1.0f);
            caixaTex(texCouro,1.2f, 1.0f, 0.15f);
            
        glPopMatrix();

        //apoio braço esq
        glPushMatrix();
            //haste vertical
            glPushMatrix();
                glTranslatef(-0.55f, 1.35f, 0.1f);
                matMetal(0.5f, 0.5f, 0.5f);
                caixaTex(texCaneca,0.05f, 0.3f, 0.05f);
            glPopMatrix();
            //apoio horizontal
            glPushMatrix();
                glTranslatef(-0.55f, 1.5f, 0.0f);
                matCouro(1.0f, 1.0f, 1.0f);
                caixaTex(texCouro,0.1f, 0.05f, 0.6f);
                //glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();

        //apoio braço dir
        glPushMatrix();
            //haste vertical
            glPushMatrix();
                glTranslatef(0.55f, 1.35f, 0.1f);
                matMetal(0.5f, 0.5f, 0.5f);
                caixaTex(texCaneca,0.05f, 0.3f, 0.05f);
                //glutSolidCube(1.0f);
            glPopMatrix();
            //apoio horizontal
            glPushMatrix();
                glTranslatef(0.55f, 1.5f, 0.0f);
                matCouro(1.0f, 1.0f, 1.0f);
                caixaTex(texCouro,0.1f, 0.05f, 0.6f);
                //glutSolidCube(1.0f);
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
