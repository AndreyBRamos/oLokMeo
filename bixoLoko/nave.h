/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#ifndef RYAN_NAVE_H
#define RYAN_NAVE_H

#ifdef _WIN32
#include <Windows.h>
#include <gl\GL.h>
#else
#include <GL/gl.h>
#endif

class Nave
{
private:
  	// distance from the sun
	float distanceFromSun;

	// time it takes to complete 1 orbit
	float orbitTime;

	// time it takes to spin 360 degrees
	float rotationTime;

	// radius of the planet itself
	float radius;

	// a vector pointing in the directio nyoure facing
	float frenteVec[3];

	// a vector pointing to the right of where your facing (to describe orientation
	float direitaVec[3];

	// a vector pointing upwards from where youre facing
	float cimaVec[3];

	// a vector describing the position of the camera
	float posicao[3];
    float vec[3];

	// the camera speed
	float naveSpeed;
	float naveTurnSpeed;

    // the texture used for rendering
	GLuint textureHandle;


public:
	Nave(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle); //;

    //renderiza
    void render(float* vec);

};

#endif