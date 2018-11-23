#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "hsv2rgb.h"

#define PI 3.1415926535897932

double camera_eye[3] = {1, 2, 3};
double radius = 5.5;
int angle_xz = 0, angle_y = 0;

hsv hsv_cube = {0.0, 1.0, 1.0};
hsv hsv_sphere = {180.0, 1.0, 1.0};
rgb rgb_cube, rgb_sphere;

GLfloat cubeColor[4] = {1, 0, 0, 1};
GLfloat sphereColor[4] = {0, 1, 0, 1};
GLfloat planeColor[4] = {0.3, 0.3, 0.3, 1};

//https://freestocktextures.com/texture/liquid-orange-marbled-pattern,1012.html
unsigned char* data;
int width, height;
GLuint* tex_v;

float randomf() {
	return ((float)rand())/RAND_MAX;
}

double degress_to_rad(int degrees) {
	return degrees * PI / 180.0;
}

void updateCubeSphereColors() {
	rgb_cube = hsv2rgb(hsv_cube);
	rgb_sphere = hsv2rgb(hsv_sphere);

	cubeColor[0] = rgb_cube.r;
	cubeColor[1] = rgb_cube.g;
	cubeColor[2] = rgb_cube.b;

	sphereColor[0] = rgb_sphere.r;
	sphereColor[1] = rgb_sphere.g;
	sphereColor[2] = rgb_sphere.b;
}

void inicializacao() {
	GLfloat posicaoLuz[4]={50.0, 0.0, 50.0, 1.0};
	glShadeModel(GL_FLAT);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	//GLfloat glVetorCor[4] = {1, 0, 0, 0};
	//glLightfv(GL_LIGHT0, GL_AMBIENT, glVetorCor);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE);

	//cor de fundo eh cinza
	glClearColor(0.8, 0.8, 0.8, 0.0);

	glMatrixMode(GL_PROJECTION);
	//alterne e altere as projecoes para alcancar os resultados desejados
	glFrustum(-1, 1, -1, 1, 1.5, 20.0);
	//glOrtho(-1, 1, -1, 1, 1.5, 20.0);
	//gluPerspective(60, 1, 1.5, 60.0);
	
	updateCubeSphereColors();
}

void printModelView() {
	GLfloat v[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, v);
	printf("=========== MODELVIEW ===========\n");
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++)
			printf("%f ", v[j*4+i]);
		printf("\n");
	}
}

void printProjection() {
	GLfloat v[16];
	glGetFloatv(GL_PROJECTION_MATRIX, v);
	printf("=========== PROJECTION ===========\n");
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++)
			printf("%f ", v[j*4+i]);
		printf("\n");
	}
}

void funcaoDisplay() {
	//limpa a tela com a cor de fundo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//altere gluLookAt para movimentar a camera ao redor da cidade
	gluLookAt(camera_eye[0], camera_eye[1], camera_eye[2],
		0, 0, 0, 0, 1, 0);
	
	glPushMatrix();
	
	//draw plane
	glMaterialfv(GL_FRONT, GL_SPECULAR, planeColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, planeColor);
	glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);

		glTexCoord2f(0, 0);
		glVertex3f(-3, 0, -3);

		glTexCoord2f(0, 1);
		glVertex3f(3, 0, -3);

		glTexCoord2f(1, 1);
		glVertex3f(3, 0, 3);

		glTexCoord2f(1, 0);
		glVertex3f(-3, 0, 3);
	glEnd();

	//draw cube
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cubeColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cubeColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cubeColor);
	glTranslatef(0, 0.5, 0);
	glutSolidCube(1);

	//draw sphere
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphereColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sphereColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphereColor);
	glTranslatef(0, 1.1, 0);
	glutSolidSphere(0.5, 50, 50);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void funcaoKeyboard(unsigned char key, int x, int y) {

	if(key == 'q')
		exit(-1);
/*
	//go left and right
	if(key == 'd')
		camera_eye[0] += 0.1;
	if(key == 'a')
		camera_eye[0] -= 0.1;

	//go up and down
	if(key == 'w')
		camera_eye[1] += 0.1;
	if(key == 's')
		camera_eye[1] -= 0.1;

	//go back and forward
	if(key == 'k')
		camera_eye[2] += 0.1;
	if(key == 'i')
		camera_eye[2] -= 0.1;*/

	glutPostRedisplay();
}

int t = 0;

void temporizador() {
	t++;
	//printf("%d ", t);	
	if (t == 2112 * 73) {
		hsv_cube.h++;
		hsv_cube.h = ((int) hsv_cube.h) % 360;

		hsv_sphere.h++;
		hsv_sphere.h = ((int) hsv_sphere.h) % 360;

		updateCubeSphereColors();

		glutPostRedisplay();
		t = 0;
	}
}

int main(int argc, char **argv) {

	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Predios");
	glutKeyboardFunc(funcaoKeyboard);
	glutDisplayFunc(funcaoDisplay);
	glutIdleFunc(temporizador);
	inicializacao();

	glutMainLoop();

	return 0;
}

