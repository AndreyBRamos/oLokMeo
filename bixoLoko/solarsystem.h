/*
Desenvolvedores:
Andrey B. Ramos
Raffael C. Rossi
Thiago Mendes
*/

#ifdef _WIN32
#include <Windows.h>
#include <gl\GL.h>
#else
#include <GL/gl.h>
#endif
#include <vector>

#include "planet.h"
#include "nave.h"

class SolarSystem
{
private:
	std::vector<Planet> planets;
    std::vector<Nave> naves;

public:
	SolarSystem();

	// calcula a posição dos planetas
	void calculatePositions(float time);
	
	// adiciona um planeta
	void addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);

    //adiciona a nave
    void addNave(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	
    // adiciona uma lua
	void addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);

	// renderiza o planeta
	void render();
    
    void renderNave(float* vec);

	// renderiza as orbitas
	void renderOrbits();

	// recebe a posição do planeta
	void getPlanetPosition(int index, float* vec);

	// recebe o raio do planeta
	float getRadiusOfPlanet(int index);
};

