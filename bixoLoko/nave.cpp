#include "nave.h"
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>

#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "globals.h"
#include "tga.h"

// sets vec to (x,y,z)
void vectorSeta(float* vec, float x, float y, float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}
// adds v2 to v1
void vectorAdiciona(float* v1, float* v2)
{
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
}

// copies v2 into v1
void vectorCopia(float* v1, float* v2)
{
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
}

// multiplies vec by the scalar
void vectorMult(float* vec, float scalar)
{
	vec[0] *= scalar;
	vec[1] *= scalar;
	vec[2] *= scalar;
}

Nave::Nave(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle){
    this->distanceFromSun = distanceFromSun;
	this->orbitTime = 0; //orbitTime;
	this->rotationTime = 0; //rotationTime;
	this->radius = radius;
	this->textureHandle = textureHandle;
    vectorSeta(posicao, 0.764331460f, -1.66760659f, 0.612456770); // 0.642456770
    vectorSeta(frenteVec,-0.398769796f, 0.763009906f, -0.508720219f);
	vectorSeta(direitaVec, 0.886262059f, 0.463184059f, 0.000000000f);  
    vectorSeta(cimaVec, -0.235630989f, 0.450859368f, 0.860931039f);
    naveSpeed = 0.005f;
    naveTurnSpeed = 0.01f;
}

// Calculate its position in 3d spacein the orbit using the given time value
/*void Nave::setaPosicao(float time){
    	vectorSet(position, 0.764331460f, -1.66760659f, 0.642456770);
        vectorSet(frenteVec,-0.398769796f, 0.763009906f, -0.508720219f);
    	vectorSet(direitaVec, 0.886262059f, 0.463184059f, 0.000000000f);  
	    vectorSet(cimaVec, -0.235630989f, 0.450859368f, 0.860931039f);
        naveSpeed = 0.005f;
	    naveTurnSpeed = 0.01f;
}
*/
// Render it to the screen
void Nave::render(float* vec)
{
	glPushMatrix();
    posicao[0] = vec[0];
    posicao[1] = vec[1];
    posicao[2] = vec[2]-0.03;

	// translate to the right positon
	glTranslatef(posicao[0], posicao[1], posicao[2]);

	
	// bind the planets texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	// render as a GLU sphere quadric object
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	
    gluSphere(quadric, radius * escalaTamanhoPlaneta, 30, 30);

	glPopMatrix();
}

void Nave::transformacaoTranslation(){
	// translate to emulate camera position
	glTranslatef(-posicao[0], -posicao[1], -posicao[2]);
}

// move the camera forward
void Nave::forward(void)
{
	// make a movement vector the right speed facing the forward direction
	float vec[3];
	vectorCopia(vec, frenteVec);
	vectorMult(vec, naveSpeed);

	// add the movement vec to the position vec
	vectorAdiciona(posicao, vec);
}

// move the camera backward
void Nave::backward(void)
{
	// make a movement vector the right speed facing the backward direction
	float vec[3];
	vectorCopia(vec, frenteVec);
	vectorMult(vec, -naveSpeed); // -cameraSpeed for backwards

	// add the movement vec to the position vec
	vectorAdiciona(posicao, vec);
}

// strafe left
void Nave::left(void)
{
	// make a movement vector the right speed facing the left direction
	float vec[3];
	vectorCopia(vec, direitaVec);
	vectorMult(vec, -naveSpeed); // -cameraSpeed for left

	// add the movement vec to the position vec
	vectorAdiciona(posicao, vec);
}

// strafe right
void Nave::right(void)
{
	// make a movement vector the right speed facing the right direction
	float vec[3];
	vectorCopia(vec, direitaVec);
	vectorMult(vec, naveSpeed);

	// add the movement vec to the position vec
	vectorAdiciona(posicao, vec);
}
