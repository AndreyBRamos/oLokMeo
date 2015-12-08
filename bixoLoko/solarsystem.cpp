

#include "solarsystem.h"

SolarSystem::SolarSystem()
{
	
}

//calcula a posicao dos planetas
void SolarSystem::calculatePositions(float time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].calculatePosition(time);
	}
}
// adiciona nave (como se fosse um planeta)
void SolarSystem::addNave(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle){ 
    naves.push_back(Nave(distanceFromSun, orbitTime, rotationTime, radius, textureHandle));
}

// adiciona planeta
void SolarSystem::addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets.push_back(Planet(distanceFromSun, orbitTime, rotationTime, radius, textureHandle));
}

// especifica qual planeta recebe a lua e a insere
void SolarSystem::addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets[planetIndex].addMoon(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle);
}

// renderizacao dos planetas
void SolarSystem::render()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].render();
	}
}
// renderizacao da nave
void SolarSystem::renderNave(float * vec){
    for (int i = 0; i < naves.size(); i++){ 
        naves[i].render(vec);
    }
}


// renderizacao das orbidas (linhas)
void SolarSystem::renderOrbits()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].renderOrbit();
	}
}

// retorna posicao de um planeta (vetor)
void SolarSystem::getPlanetPosition(int index, float* vec)
{
	planets[index].getPosition(vec);
}


// retorna o raio de um planeta
float SolarSystem::getRadiusOfPlanet(int index)
{
	return planets[index].getRadius();
}
