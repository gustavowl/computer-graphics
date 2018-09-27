#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>

float tAtual = 0.0; //um ponto é renderizado na reta
int delta = 1; //= 1 ou -1... variação positiva ou negativa de tAtual, quando animacao = 1
int animacao = 1;
float top = 6, bottom = -4, left = -4, right = 6;
int showGuideLines = true;

using namespace std;

//vetor que contem as abscissas dos pontos
vector<float> vx;
//vetor que contem as ordenadas dos pontos
vector<float> vy;

enum {ADICIONARREMOVER, ALTERAR};
int estado = ADICIONARREMOVER;
int pontoAtual = 0;

void inicializacao() {
	glClearColor(0.5, 0.5, 0.5, 0.0);
}

//implementacao recursiva (nao use memoization)
//retorna o ponto da curva bezier em t considerando os pontos Pa, ..., Pb
float bezier(vector<float> v, int a, int b, float t) {
	//assert(a < b)
	if (v.size() > 1) {
		if (a+1 != b) {
			return (1.0 - t) * bezier(v, a, b-1, t) +
				t * bezier(v, a+1, b, t);
		}
		return (1.0 - t) * v.at(a) + t * v.at(b);
	}
	return 0;
}

//Render for bezier
void renderGraph(vector<float> x, vector<float> y, int seg) {
	float prev_x = bezier(x, 0, vx.size() - 1, 0);
	float prev_y = bezier(y, 0, vy.size() -1, 0);

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
		for (float i = 1; i <= seg; i++) {
			glVertex2f(prev_x, prev_y);
			prev_x = bezier(x, 0, vx.size() - 1, i/seg);
			prev_y = bezier(y, 0, vy.size() -1, i/seg);
			glVertex2f(prev_x, prev_y);
		}
	glEnd();
}

//implementacao nao recursiva
//retorna o ponto da curva bezier em t considerando os pontos P0, ..., Pv.size()-1
//De Casteljau
float bezier2(vector<float> v, float t) {
	printf("size: %lu\n", v.size());
	if (v.size() > 1) {
		for (int i = 1; i < v.size(); i++) {
			for (int j = 0; j < v.size() - i; j++) {
				v.at(j) = (1 - t) * v.at(j) + t * v.at(j+1);
			}	
		}
		return v.at(0);
	}
	return 0;
}

//Render for De Casteljau
void renderGraph2(vector<float> x, vector<float> y, int seg) {
	float prev_x = bezier2(x, 0);
	float prev_y = bezier2(y, 0);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		for (float i = 1; i <= seg; i++) {
			glVertex2f(prev_x, prev_y);
			prev_x = bezier2(x, i/seg);
			prev_y = bezier2(y, i/seg);
			glVertex2f(prev_x, prev_y);
		}
	glEnd();
}

void funcaoDisplay() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	if(showGuideLines) {
		for(int i = 0; i < vx.size(); i++) {
			if(i == pontoAtual && estado == ALTERAR) {
				glColor3f(0, 0, 1);
				glPointSize(6.0);
			} else {
				glColor3f(1, 1, 1);
				glPointSize(3.0);
			}
			glBegin(GL_POINTS);
			glVertex2f(vx[i], vy[i]);
			glEnd();
		}
	}

	//renderização de tAtual
	//float px = bezier(vx, 0, vx.size() - 1, tAtual);
	//float py = bezier(vy, 0, vy.size() - 1, tAtual);
	float px = bezier2(vx, tAtual);
	float py = bezier2(vy, tAtual);

	glColor3f(1, 1, 1);
	glPointSize(6.0);
	glBegin(GL_POINTS);
	glVertex2f(px, py);
	glEnd();

	if(vx.size() > 2 && showGuideLines) {
		for(int i = 0; i < vx.size()-1; i++) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINES);
			glVertex2f(vx[i], vy[i]);
			glVertex2f(vx[i+1], vy[i+1]);
			glEnd();
		}
	}

	//renderizacao da curva de bezier
	//assim como na interpolacao por Lagrange, utilize varios segmentos de reta para compor a curva
	int segNum = 20; //TODO: DEFINE CONSTANT
	//renderGraph(vx, vy,  segNum);
	renderGraph2(vx, vy,  segNum);


	//escreve um texto na tela para acompanhar a coordenada para t = tAtual
	glColor3f(1, 1, 1);
	glRasterPos2f(px+0.1, py+0.1);
	char texto[60];
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
	if(key == 't') {
		pontoAtual = (pontoAtual+1)%vx.size();
		estado = ALTERAR;
	}
	if(key == 'r') {
		pontoAtual = (pontoAtual-1+vx.size())%vx.size();
		estado = ALTERAR;
	}
	if(key == 'g')
		showGuideLines = !showGuideLines;

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

	if(state == GLUT_UP) {
		if(estado == ADICIONARREMOVER) {
			if(button == GLUT_LEFT_BUTTON) {
				vx.push_back(worldX);
				vy.push_back(worldY);
			}
			if(button == GLUT_RIGHT_BUTTON && vx.size() > 0) {
				vx.pop_back();
				vy.pop_back();
			}
		} else {
			if(button == GLUT_LEFT_BUTTON) {
				vx[pontoAtual] = worldX;
				vy[pontoAtual] = worldY;
				estado = ADICIONARREMOVER;
			}
		}
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Curvas de Bezier");
	glutKeyboardFunc(funcaoKeyboard);
	glutMouseFunc(funcaoMouse);
	glutDisplayFunc(funcaoDisplay);
	glutIdleFunc(temporizador);
	inicializacao();

	glutMainLoop();

	return 0;
}

