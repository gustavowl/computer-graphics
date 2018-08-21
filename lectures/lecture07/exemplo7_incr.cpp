#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

float tAtual = 0.0; //um ponto é renderizado na reta
int delta = 1; //= 1 ou -1... variação positiva ou negativa de tAtual, quando animacao = 1
int animacao = 1;

//vetor que contem as abscissas dos pontos
float vx[] = {2, 3, 4};
//vetor que contem as ordenadas dos pontos
float vy[] = {3, 4, 2};

void inicializacao() {
	glClearColor(0.5, 0.5, 0.5, 0.0);
}

/*void printVector(float *v, int n) {
	printf("v[] = {");
	n--;
	for (int i = 0; i < n; i++) {
		printf("%f, ", v[i]);
	}
	printf("%f", v[n]);
	printf("}\n");

}*/

//retorna o valor v(t), onde n é a quantidade de pontos
//se v é vx, então quando t = 0 deve retornar vx[0]
//			e quando t = 1 deve retornar vx[n-1]
float lagrange(float *v, int n, float t) {
	//supposes points are ordered by x-axis
	//printVector(v, n);
	
	float res = 0.0;
	float lt;
	for (int i = 0; i < n; i++) {
		lt = 1.0;
		for (int j = 0; j < n; j++) {
			if (j != i) {
				lt *= (t - (float)j/(n - 1 )) / 
					((float)(i - j) / (n - 1)  );
				/*printf("HEEEYlt %f\t", lt);
				printf("%d ", (float)(i-j)/(n-1));
				printf("%d ", i-j);
				printf("%d ", n-1);*/
			}
		}
		//printf("\nbackres %f", res);
		res += lt * v[i];
		//printf("\nltloop %f | resloop %f\n", lt, res);
	}
	/*printf("t = %f\n", t);
	printf("res = %f\n", res);
	printf("-------------\n");*/
	return res;
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
	glVertex2f(vx[2], vy[2]);
	glEnd();

	//renderização de tAtual
	float px = lagrange(vx, 3, tAtual);
	float py = lagrange(vy, 3, tAtual);

	glPointSize(6.0);
	glBegin(GL_POINTS);
	glVertex2f(px, py);
	glEnd();

	//renderize a curva utilizando n segmentos de reta
	int n = 20;

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
		vx[2] = worldX;
		vy[2] = worldY;
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

