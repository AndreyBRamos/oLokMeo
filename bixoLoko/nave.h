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

class Nave
{
private:
  	// distancia do sol
	float distanceFromSun;

	// tempo de orbita
	float orbitTime;

	// rotacao
	float rotationTime;

	// raio da nave
	float radius;

	// vetor para movimentacao (frente)
	float frenteVec[3];

	// vetor para movimentacao (direita)
	float direitaVec[3];

	// vetor para movimentacao (cima)
	float cimaVec[3];

	// declaracao dos vetores de posicao
	float posicao[3];
    	float vec[3];

	// velocidade 
	float naveSpeed;
	float naveTurnSpeed;

    // variavel especial do opengl para textura
	GLuint textureHandle;


public:
	Nave(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle); //;

    //renderiza
    void render(float* vec);

};

