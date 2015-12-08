/*
Desenvolvedores:
Andrey B. Ramos
Raffael C. Rossi
Thiago Mendes
*/

// Compilação windows 
#include <cmath>
#include <cstdlib>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glut.h>
#include <GL\glu.h>

#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "tga.h"
#include "solarsystem.h"
#include "camera.h"
#include "globals.h"
#include <vector>


bool camDeus = false;

//Tamanho da tela
int telaLargura, telaAltura;

// Variáveis das texturas
TGA* test, *stars, * moon;

// Identifica qual planeta foi o ultimo selecionado para ver
int planetSelected = 1;

// Variável do sistema solar
SolarSystem sistemaSolar;

// Variávei da câmera
Camera camera;


// Variáveis para controlar a variação do tempo
double time;
double velocidadeTempo;

//Configurado para guardar o estado dos controles
struct ControlStates
{
	bool forward, backward, left, right, yawLeft, yawRight, pitchUp,
		pitchDown, rollLeft, rollRight;
} controls;

//Função do times do display
void timer(int)
{
	glutPostRedisplay(); 
	glutTimerFunc(10, timer, 0); 
}

//Cria um numero randômico
float random(float max)
{
	return (float)(std::rand() % 1000) * max * 0.001;
}

//Adiciona uma lua
void addMoon()
{
	sistemaSolar.addMoon(planetSelected, 
		(500 + random(1500)) * sistemaSolar.getRadiusOfPlanet(planetSelected),
		10 + random(100), 0.5 + random(20),
		sistemaSolar.getRadiusOfPlanet(planetSelected) * (0.05f + random(0.2f)), moon->getTextureHandle());
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	
	//Configura a iluminação
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matAmbience[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);

	GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// Carrega as texturas
	stars = new TGA("images/stars.tga");
	moon = new TGA("images/moon.tga");

	TGA* sol = new TGA("images/sun.tga");
	TGA* mercurio = new TGA("images/mercury.tga");
	TGA* venus = new TGA("images/venus.tga");
	TGA* terra = new TGA("images/earth.tga");
	TGA* marte = new TGA("images/mars.tga");
	TGA* jupiter = new TGA("images/jupiter.tga");
	TGA* saturno = new TGA("images/saturn.tga");
	TGA* urano = new TGA("images/uranus.tga");
	TGA* netuno = new TGA("images/neptune.tga");
	TGA* plutao = new TGA("images/pluto.tga");
    TGA* nave = new TGA("images/nave.tga");

	//Adiciona os planetas no sistema, a distância está em KM e o tempo em dias da terra.
	sistemaSolar.addPlanet(0, 1, 500, 695500, sol->getTextureHandle()); 
	sistemaSolar.addPlanet(57910000, 88, 58.6, 6440, mercurio->getTextureHandle()); 
    sistemaSolar.addPlanet(108200000, 224.65, 243, 12052, venus->getTextureHandle()); 
	sistemaSolar.addPlanet(149600000, 365, 1, 18371, terra->getTextureHandle()); 
	sistemaSolar.addPlanet(227939100, 686, 1.03f, 9389, marte->getTextureHandle());
	sistemaSolar.addPlanet(778500000, 4332, 0.4139, 69911, jupiter->getTextureHandle()); 
	sistemaSolar.addPlanet(1433000000, 10759, 0.44375, 58232, saturno->getTextureHandle()); 
	sistemaSolar.addPlanet(2877000000, 30685, 0.718056, 25362, urano->getTextureHandle());
	sistemaSolar.addPlanet(4503000000, 60188, 0.6713, 24622, netuno->getTextureHandle()); 
	sistemaSolar.addPlanet(5906380000, 90616, 6.39, 1137, plutao->getTextureHandle());

	sistemaSolar.addMoon(3, 13000000, 27.3, 27.3, 3538, moon->getTextureHandle());//Adiciona a lua na terra

    sistemaSolar.addNave(778500000, 4332, 0.4139, 3911, nave->getTextureHandle());//Adiciona a nave no sistema

	//Seta o tempo
	time = 2.552f;
	velocidadeTempo = 1.0f;

	//Inicia os controles
	controls.forward = false;
	controls.backward = false;
	controls.left = false;
	controls.right = false;
	controls.rollRight = false;
	controls.rollLeft = false;
	controls.pitchDown = false;
	controls.pitchUp = false;
	controls.yawLeft = false;
	controls.yawRight = false;

	timer(0);
}

void drawCube(void);

void display(void)
{
    float vecCamera[3];
	// Atualiza a simulação
	time += velocidadeTempo;
	sistemaSolar.calculatePositions(time);

	if (controls.forward) camera.forward();		if (controls.backward) camera.backward();
	if (controls.left) camera.left();			if (controls.right) camera.right();
	if (controls.yawLeft) camera.yawLeft();		if (controls.yawRight) camera.yawRight();
	if (controls.rollLeft) camera.rollLeft();	if (controls.rollRight) camera.rollRight();
	if (controls.pitchUp) camera.pitchUp();		if (controls.pitchDown) camera.pitchDown();

    

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	
	//Seta a matriz em perspectiva
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (float)telaLargura / (float)telaAltura, 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	// Efetua a transformação da orientação da câmera
	camera.transformOrientation();
    

	// Desenha o ceu
	glBindTexture(GL_TEXTURE_2D, stars->getTextureHandle());
	drawCube();

	// Efetua a translação da câmera
	camera.transformTranslation();
    
    camera.getPosicao(vecCamera);
    
          
	GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	
	//Renderiza o sistema solar
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	sistemaSolar.render();
    if(vecCamera[0] == 0.0 && vecCamera[1] == 0.0 && vecCamera[2] == 2.5){ 
        vecCamera[0] = 0.764331460f;
        vecCamera[1] = -1.66760659f;
        vecCamera[2] = 0.642456770;    
        sistemaSolar.renderNave(vecCamera);
    }else{
        sistemaSolar.renderNave(vecCamera);
    }   
	glDisable(GL_LIGHTING);

	
	glDisable(GL_DEPTH_TEST);

		
	
	glFlush();
	glutSwapBuffers();
}
void keyDown(unsigned char key, int x, int y)
{
	
	
	if (key > '0' && key <= '8'){
        // Volta à posição normal (inicial)    
        camera.visaoNormal();
        camDeus = false;
            
		//Aponta para o planeta específico
		float vec[3];
		sistemaSolar.getPlanetPosition(key - '0', vec);
		camera.pointAt(vec);

		//Seleciona o planeta
		planetSelected = key - '0';
	}
    if (key == '9'){
        // Eleva a camera e aponta para o centro do sistema solar
		float vec[3];
        vec[0] = 0.1;
        vec[1] = 0.1;
        vec[2] = 0.1;
        camera.visaoDeus();	
        camDeus = true;    	
        camera.pointAt(vec);
    }
	switch (key)
	{
	case '-':
		velocidadeTempo /= 1.0f; 
		break;
	case '=':
		velocidadeTempo *= 1.0f; 
		break;
	case 'w':
		if (!camDeus) controls.forward = true;
		break;
	case 's':
		if (!camDeus) controls.backward = true;
		break;
	case 'a':
		if (!camDeus) controls.left = true;
		break;
	case 'd':
		if (!camDeus) controls.right = true;
		break;
	case 'l':
		if (!camDeus) controls.rollRight = true;
		break;
	case 'j':
		if (!camDeus) controls.rollLeft = true;
		break;
	case 'i':
		if (!camDeus) controls.pitchDown = true;
		break;
	case 'k':
		if (!camDeus) controls.pitchUp = true;
		break;
	case 'q':
		if (!camDeus) controls.yawLeft = true;
		break;
	case 'e':
		if (!camDeus) controls.yawRight = true;
		break;
	}

}

void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		controls.forward = false;
		break;
	case 's':
		controls.backward = false;
		break;
	case 'a':
		controls.left = false;
		break;
	case 'd':
		controls.right = false;
		break;
	case 'l':
		controls.rollRight = false;
		break;
	case 'j':
		controls.rollLeft = false;
		break;
	case 'i':
		controls.pitchDown = false;
		break;
	case 'k':
		controls.pitchUp = false;
		break;
	case 'q':
		controls.yawLeft = false;
		break;
	case 'e':
		controls.yawRight = false;
		break;
	}
}

void reshape(int w, int h)
{
	telaLargura = w;
	telaAltura = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(900, 650);
	glutInitWindowPosition(10, 10);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();
	return 0;
}

void drawCube(void)
{
	glBegin(GL_QUADS);
	//Cubo do ambiente
	//Face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, 1.0f);

	//Face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(1.0f, -1.0f, 1.0f);

	//Face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(1.0f, -1.0f, -1.0f);

	//Face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, -1.0f);

	//Face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, 1.0f);

	//Face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(-1.0f, -1.0f, 1.0f);


	glEnd();
}