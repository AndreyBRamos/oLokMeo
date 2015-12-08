#ifdef _WIN32
#include <Windows.h>
#include <gl\GL.h>

#else
#include <GL/gl.h>
#endif


class Moon
{
private:
	// distancia para o planeta
	float distanciaPlaneta;

	// tempo para a orbita
	float tempoOrbita;

	// tempo de rotacao
	float tempoRotacao;

	// raio da lua
	float raio;

	// textura para renderizar
	GLuint textureHandle;

	// its position in 3d space relative to the planet
	float position[3];

	// rotacao ao redor do eixo
	float rotacao;

public:
	// distancia em km, e tempo em dias da Terra
	Moon(float distanciaPlaneta, float tempoOrbita, float tempoRotacao, float raio, GLuint textureHandle);

	// Calcula a posicao relativa ao tempo
	void calculatePosition(float tempo);

	void render(void);

	// renderiza a orbita
	void renderOrbit(void);
};
