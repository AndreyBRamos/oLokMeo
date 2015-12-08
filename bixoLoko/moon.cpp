#include "moon.h"
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

Moon::Moon(float distanciaPlaneta, float tempoOrbita, float tempoRotacao, float raio, GLuint textureHandle)
{
	this->distanciaPlaneta = distanciaPlaneta;
	this->tempoOrbita = tempoOrbita;
	this->tempoRotacao = tempoRotacao;
	this->raio = raio;
	this->textureHandle = textureHandle;
}

// calcula a posição da lua ao redor do planeta
void Moon::calculatePosition(float time)
{
	// procura o angulo de rotação ao redor do planeta
	float angle = time * 3.1419f / tempoOrbita;
	
	// calcula a posição do planeta 
	position[0] = sin(angle) * distanciaPlaneta;
	position[1] = cos(angle) * distanciaPlaneta;
	position[2] = 0;

	// rotação da lua ao redor do eixo
	//rotacao = time * 360 / tempoRotacao;
    rotacao = 0.0;
}

void Moon::render(void)
{
	glPushMatrix();

	// textura
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// translada para a posição certa e rotaciona
	glTranslatef(position[0] * escalaDistancia, position[1] * escalaDistancia, position[2] * escalaDistancia);
	glRotatef(-rotacao, 0.0f, 0.0f, 1.0f);
	
	// renderiza um obejto quadrico esfera
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	gluSphere(quadric, raio * escalaTamanhoPlaneta, 30, 30);

	glPopMatrix();
}

// renderiza a orbita da lua ao redor do planeta
void Moon::renderOrbit(void)
{
	// desenha a linha do planeta
	glBegin(GL_LINE_STRIP);

	// laço de 0 a 2PI e desenha o raio da orbita usando trigonometria 
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.1f)
	{
		glVertex3f(sin(angle) * distanciaPlaneta * escalaDistancia, cos(angle) * distanciaPlaneta * escalaDistancia, 0.0f);
	}
	glVertex3f(0.0f, distanciaPlaneta * escalaDistancia, 0.0f);

	glEnd();
}