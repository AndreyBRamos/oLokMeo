#ifdef _WIN32
#include <Windows.h>
#include <gl\GL.h>
#else
#include <GL/gl.h>
#endif

#include <vector>
#include "moon.h"

class Planet
{
private:
	// distance from the sun
	float distanciaSol;

	// time it takes to complete 1 orbit
	float orbitTime;

	// time it takes to spin 360 degrees
	float tempoRotacao;

	// raio of the planet itself
	float raio;

	// the texture used for rendering
	GLuint textureHandle;

	// its position in 3d space
	float position[3];
	// its rotation around its axis
	float rotation;

	// list of moons attached to this planet
	std::vector<Moon> moons;

public:
	// Distance is in units of km (kilometers) and time is in units of earth days (365.25 orbit time for earth)
	Planet(float distanciaSol, float orbitTime, float tempoRotacao, float raio, GLuint textureHandle);

	// Calculate its position in 3d spacein the orbit using the given time value
	void calculatePosition(float time);

	// Render it to the screen
	void render(void);

	// render this planets orbit circle
	void renderOrbit(void);

	// Get its position in 3d world space units (after scaling) and put it into the 3d vector
	void getPosition(float* vec);

	// get the raio of this planet
	float getRadius(void);

	// add a moon to this planet
	void addMoon(float distanciaPlaneta, float orbitTime, float tempoRotacao, float raio, GLuint textureHandle);
};
