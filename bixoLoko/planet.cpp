
#include "planet.h"
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

float escalaTamanhoPlaneta = 0.000005f; // planetSizeScale

Planet::Planet(float distanciaSol, float orbitTime, float tempoRotacao, float raio, GLuint textureHandle)
{
	this->distanciaSol = distanciaSol;
	this->orbitTime = orbitTime;
	this->tempoRotacao = tempoRotacao;
	this->raio = raio;
	this->textureHandle = textureHandle;
}

// Calculate its position in 3d spacein the orbit using the given time value
void Planet::calculatePosition(float time)
{
	// find the angle of orientation of the orbit around the sun
	float angle = time * 3.1419f / orbitTime;
	
	// use trig to find the position in space
	position[0] = sin(angle) * distanciaSol;
	position[1] = cos(angle) * distanciaSol;
	position[2] = 0;

	// find the rotation of the planet around its axis
	rotation = time * 360 / tempoRotacao;

	// calculate positions of moons
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].calculatePosition(time);
	}
}

// Render it to the screen
void Planet::render(void)
{
	glPushMatrix();

	// translate to the right positon
	glTranslatef(position[0] * escalaDistancia, position[1] * escalaDistancia, position[2] * escalaDistancia);

	// Draw the moons
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].render();
	}

	/// rotate for the planet's spin
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	
	// bind the planets texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	// render as a GLU sphere quadric object
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	if (distanciaSol < 0.001f) // if this is the sun, dont render it too big, and disable lighting
	{
		float escalaRaio = raio * escalaTamanhoPlaneta;
		if (escalaRaio > 0.5f) escalaRaio = 0.5f;

		glDisable(GL_LIGHTING);
		gluSphere(quadric, escalaRaio, 30, 30);
		glEnable(GL_LIGHTING);
	}
	else
	{
		gluSphere(quadric, raio * escalaTamanhoPlaneta, 30, 30);
	}


	glPopMatrix();
}

// renderiza a orbita dos planetas
void Planet::renderOrbit(void)
{
	// draw a line strip
	glBegin(GL_LINE_STRIP);

	// loop round from 0 to 2*PI and draw around the raio of the orbit using trigonometry
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f)
	{
		glVertex3f(sin(angle) * distanciaSol * escalaDistancia, cos(angle) * distanciaSol * escalaDistancia, 0.0f);
	}
	glVertex3f(0.0f, distanciaSol * escalaDistancia, 0.0f);
	
	glEnd();

	// render the moons' orbit
	glPushMatrix();
	// translate to the center of this planet to draw the moon orbit around it
	glTranslatef(position[0] * escalaDistancia, position[1] * escalaDistancia, position[2] * escalaDistancia);
	// draw all moon orbits
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].renderOrbit();
	}
	glPopMatrix();

}


// Get its position in 3d world space units (after scaling) and put it into the 3d vector
void Planet::getPosition(float* vec)
{
	vec[0] = position[0] * escalaDistancia;
	vec[1] = position[1] * escalaDistancia;
	vec[2] = position[2] * escalaDistancia;
}

// get the raio of this planet
float Planet::getRadius(void)
{
	return raio;
}

// add a moon to this planet
void Planet::addMoon(float distanciaPlaneta, float orbitTime, float tempoRotacao, float raio, GLuint textureHandle)
{
	moons.push_back(Moon(distanciaPlaneta, orbitTime, tempoRotacao, raio, textureHandle));
}