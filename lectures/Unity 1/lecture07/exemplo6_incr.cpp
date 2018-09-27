#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

float tAtual = 0.0; //um ponto é renderizado na reta
int delta = 1; //= 1 ou -1... variação positiva ou negativa de tAtual, quando animacao = 1
int animacao = 1;

//vetor que contem as abscissas dos pontos
float vx[] = {2, 1.5};
//vetor que contem as ordenadas dos pontos
float vy[] = {3, 4};

void inicializacao() {
	glClearColor(0.5, 0.5, 0.5, 0.0);
}

//retorna o valor v(t)
//se v é vx, então quando t = 0 deve retornar vx[0]
//			e quando t = 1 deve retornar vx[1]
float interpolacaoLinear(float v[], float t) {
	//since only LINEAR interpolation is considered
	//length(v[]) is fixed = 2
	return v[0] + t*(v[1] - v[0]);
}

void funcaoDisplay() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-4, 6, -4, 6);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2f(vx[0], vy[0]);
	glVertex2f(vx[1], vy[1]);
	glEnd();

	//renderizacao de tAtual
	float px = interpolacaoLinear(vx, tAtual);
	float py = interpolacaoLinear(vy, tAtual);

	glPointSize(6.0);
	glBegin(GL_POINTS);
	glVertex2f(px, py);
	glEnd();

	//renderiza a reta
	glBegin(GL_LINES);
		glVertex2f(interpolacaoLinear(vx, 0), interpolacaoLinear(vy, 0));
		glVertex2f(interpolacaoLinear(vx, 1), interpolacaoLinear(vy, 1));
	glEnd();

	//escreve um texto na tela para acompanhar a coordenada para t = tAtual
	glRasterPos2f(px+0.1, py+0.1);
	char texto[30];
	sprintf(texto, "t = %.2f, (x, y) = (%.2f, %.2f)", tAtual, px, py);
	for(int i = 0; i < strlen(texto); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);

	glutSwapBuffers();
	glFlush();
}

void funcaoKeyboard(unsigned char key, int x, int y) {

	if(key == 'q') {
		exit(-1);
	}
	if(!animacao) {
		if(key == 'a') {
			tAtual -= 0.01;
		}
		if(key == 'd') {
			tAtual += 0.01;
		}
	}
	if(key == 's')
		animacao = 1 - animacao;
	glutPostRedisplay();
}

void temporizador() {
	glutPostRedisplay();
	if(animacao) {
		tAtual += 0.003*delta;
		if(tAtual > 1) {
			tAtual = 1;
			delta = -1;
		}
		if(tAtual < 0) {
			tAtual = 0;
			delta = 1;
		}
	}
}

void funcaoMouse(int button, int state, int x, int y) {

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble worldX, worldY, worldZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	winZ = 0;

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
	if(button == GLUT_LEFT_BUTTON) {
		vx[0] = worldX;
		vy[0] = worldY;
	}
	if(button == GLUT_RIGHT_BUTTON) {
		vx[1] = worldX;
		vy[1] = worldY;
	}
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello world");
	glutKeyboardFunc(funcaoKeyboard);
	glutMouseFunc(funcaoMouse);
	glutDisplayFunc(funcaoDisplay);
	glutIdleFunc(temporizador);
	inicializacao();

	glutMainLoop();

	return 0;
}

