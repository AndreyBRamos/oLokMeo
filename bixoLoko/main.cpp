/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091


// Compilação windows 
#include <cmath>
#include <cstdlib>

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


// the screen size
int telaLargura, telaAltura;

// The TGA texture containing the help dialogue and starfield and moon texture
TGA* help, *stars, * moon;
// toggles if the help dialogue is enabled
bool helpDialogue = true;
// toggles if orbits are drawn
//bool showOrbits = true;
// holds the index of the last planet that was selected with the 1 to 9 number keys
int planetSelected = 1;

// The main instance of the solar system
SolarSystem sistemaSolar;

// The instance of the camera
Camera camera;

// These control the simulation of time
double time;
double velocidadeTempo;

// holds the state of the controls for the camera - when true, the key for that control is being pressed
struct ControlStates
{
	bool forward, backward, left, right, yawLeft, yawRight, pitchUp,
		pitchDown, rollLeft, rollRight;
} controls;


// timer function called every 10ms or more
void timer(int)
{
	glutPostRedisplay(); // post for display func
	glutTimerFunc(10, timer, 0); // limit frame drawing to 100fps
}

// creates a random number up to the max specified
float random(float max)
{
	return (float)(std::rand() % 1000) * max * 0.001;
}

// adds a moon to the selected planet
void addMoon()
{
	// make a moon using random values
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
	
	// set up lighting
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

	// Load all image data
	//help = new TGA("images/help.tga");
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

	// Add all the planets with accurate data. Distance measured in km, time measured in terra days.
	sistemaSolar.addPlanet(0, 1, 500, 695500, sol->getTextureHandle()); // sol
	sistemaSolar.addPlanet(57910000, 88, 58.6, 6440, mercurio->getTextureHandle()); // mercurio
	sistemaSolar.addPlanet(108200000, 224.65, 243, 12052, venus->getTextureHandle()); // venus
	sistemaSolar.addPlanet(149600000, 365, 1, 18371, terra->getTextureHandle()); // terra 6371
	sistemaSolar.addPlanet(227939100, 686, 1.03f, 9389, marte->getTextureHandle()); // marte
	/*sistemaSolar.addPlanet(778500000, 4332, 0.4139, 69911, jupiter->getTextureHandle()); // jupiter
	sistemaSolar.addPlanet(1433000000, 10759, 0.44375, 58232, saturno->getTextureHandle()); // saturno
	sistemaSolar.addPlanet(2877000000, 30685, 0.718056, 25362, urano->getTextureHandle()); // urano
	sistemaSolar.addPlanet(4503000000, 60188, 0.6713, 24622, netuno->getTextureHandle()); // netuno
	sistemaSolar.addPlanet(5906380000, 90616, 6.39, 1137, plutao->getTextureHandle()); // plutao*/

	sistemaSolar.addMoon(3, 13000000, 27.3, 27.3, 3538, moon->getTextureHandle()); // test moon for the terra

	// set up time
	time = 2.552f;
	velocidadeTempo = 1.0f;

	// reset controls
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
	// update the logic and simulation
	time += velocidadeTempo;
	sistemaSolar.calculatePositions(time);

	if (controls.forward) camera.forward();		if (controls.backward) camera.backward();
	if (controls.left) camera.left();			if (controls.right) camera.right();
	if (controls.yawLeft) camera.yawLeft();		if (controls.yawRight) camera.yawRight();
	if (controls.rollLeft) camera.rollLeft();	if (controls.rollRight) camera.rollRight();
	if (controls.pitchUp) camera.pitchUp();		if (controls.pitchDown) camera.pitchDown();

	// clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	
	// set up the perspective matrix for rendering the 3d world
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (float)telaLargura / (float)telaAltura, 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	// perform the camera orientation transform
	camera.transformOrientation();

	// draw the skybox
	glBindTexture(GL_TEXTURE_2D, stars->getTextureHandle());
	drawCube();

	// perform the camera translation transform
	camera.transformTranslation();

	
	
	GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	
	// render the solar system
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	sistemaSolar.render();
	glDisable(GL_LIGHTING);

	// possibly render orbits
	/*if (showOrbits)
		sistemaSolar.renderOrbits();*/
	
	glDisable(GL_DEPTH_TEST);

	// set up ortho matrix for showing the UI (help dialogue)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) telaLargura, (GLdouble) telaAltura, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// draw the help dialogue
	/*if (helpDialogue)
	{
		glBindTexture(GL_TEXTURE_2D, help->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}*/

	glFlush();
	glutSwapBuffers();
}
void keyDown(unsigned char key, int x, int y)
{
	// check for numerical keys
	if (key > '0' && key <= '8'){
        // Volta à posição normal (inicial)    
        camera.visaoNormal();
            
		// point at the specified planet
		float vec[3];
		sistemaSolar.getPlanetPosition(key - '0', vec);
		camera.pointAt(vec);

		// select that planet
		planetSelected = key - '0';
	}
    if (key == '9'){
        // Eleva a camera e aponta para o centro do sistema solar
		float vec[3];
        vec[0] = 0.1;
        vec[1] = 0.1;
        vec[2] = 0.1;
        camera.visaoDeus();	    	
        camera.pointAt(vec);
    }
	switch (key)
	{
	case '-':
		velocidadeTempo /= 2.0f; // half the rate of time passing
		break;
	case '=':
		velocidadeTempo *= 2.0f; // double the rate of time passing
		break;
	/*case 'h':
		helpDialogue = !helpDialogue; // toggle the dialogue
		break;*/
	case '[':
		//escalaTamanhoPlaneta /= 1.2; // make planet scale smaller
		break;
	case ']':
		//escalaTamanhoPlaneta *= 1.2; // make planet scale bigger
		break;
	/*case 'o':
		showOrbits = !showOrbits; // toggle show orbits
		break;*/
	case 'm':
		addMoon(); // add a moon to the selected planet
		break;
	case 'r':
		//escalaTamanhoPlaneta = distanceScale;
		break;
	case ',':
		camera.slowDown(); // slow down camera
		break;
	case '.':
		camera.speedUp(); // speed up camera
		break;
		// these are all camera controls
	case 'w':
		controls.forward = true;
		break;
	case 's':
		controls.backward = true;
		break;
	case 'a':
		controls.left = true;
		break;
	case 'd':
		controls.right = true;
		break;
	case 'l':
		controls.rollRight = true;
		break;
	case 'j':
		controls.rollLeft = true;
		break;
	case 'i':
		controls.pitchDown = true;
		break;
	case 'k':
		controls.pitchUp = true;
		break;
	case 'q':
		controls.yawLeft = true;
		break;
	case 'e':
		controls.yawRight = true;
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
	// nova face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, 1.0f);

	// nova face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(1.0f, -1.0f, 1.0f);

	// nova face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(1.0f, -1.0f, -1.0f);

	// nova face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, -1.0f);

	// nova face
	glTexCoord2f(0.0f, 0.0f);	
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 1.0f);	
	glVertex3f(-1.0f, 1.0f, 1.0f);

	// nova face
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