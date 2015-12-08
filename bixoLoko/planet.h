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
	// distancia para o Sol
	float distanciaSol;

	// tempo para completar uma orbita
	float orbitTime;

	// tempo para cada grau de rotação ( 360)
	float tempoRotacao;

	// raio do planeta
	float raio;

	// textura para renderizar
	GLuint textureHandle;

	// posição em 3d do planeta no espaço
	float position[3];
	// its rotation around its axis
	float rotation;

	// lista de luas do planeta
	std::vector<Moon> moons;

public:
	// Distancia em km e tempo em dias ( Terra) 
	Planet(float distanciaSol, float orbitTime, float tempoRotacao, float raio, GLuint textureHandle);

	// Calcula a posição do espaço da orbita em relação ao tempo
	void calculatePosition(float time);

	void render(void);

	// renderiza a orbita do planeta, por default esta off
	void renderOrbit(void);

	// pega a posição do planeta apos a escala.
	void getPosition(float* vec);

	// pega o raio do planeta
	float getRadius(void);

	// adiciona lua aos planeta
	void addMoon(float distanciaPlaneta, float orbitTime, float tempoRotacao, float raio, GLuint textureHandle);
};
