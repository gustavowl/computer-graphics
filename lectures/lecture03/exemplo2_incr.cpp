#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

float theta = 0;
float red, green, blue;
//red = green = blue = 0;

float randomf() {
	return ((float)rand())/RAND_MAX;
}

void inicializacao() {
	//cor de fundo eh cinza
	glClearColor(0.5, 0.5, 0.5, 0.0);
}

void triangulo() {
	//glColor3d(0, 0, 0);

	glBegin(GL_TRIANGLES);
		glColor3f(red, 0, 0);
		glVertex2f(-1, -1);

		glColor3f(0, green, 0);
		glVertex2f(0, 1);

		glColor3f(0, 0, blue);
		glVertex2f(1, -1);
	glEnd();

	glFlush();
}

void funcaoDisplay() {
	//mensagem para verificar quando esta funcao eh chamada
	printf("display\n");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//limites das coordenadas de mundo
	gluOrtho2D(-4, 4, -4, 4);
	
	//limpa a tela com a cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

	//use a funcao triangulo, glTranslatef e glRotatef para desenhar varios
	//triangulo();
	//triangulos transladados e rotacionados
	glTranslatef(1, 1, 0);
	glRotatef(theta, 0, 0, 1);
	triangulo();

	glFlush();

	glutSwapBuffers();
}

void funcaoKeyboard(unsigned char key, int x, int y) {

	if(key == 'q') {
		exit(-1);
	}
	glutPostRedisplay();
}

int t = 0;
int clockwise = 0;
void temporizador() {
	t++;
	if (t == 2112 * 2) {
		if (clockwise) {
			theta -= 1;
			if (theta == 0)
				clockwise = 0;
		}
		else {
			theta += 1;
			if (theta == 360)
				clockwise = 1;
		}
		red = theta / 255;
		blue = theta / 255;
		green = theta / 255;
		//theta = (float)(theta % 360);
		glutPostRedisplay();
		t = 0;
	}
}

int main(int argc, char **argv) {

	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello world");
	glutKeyboardFunc(funcaoKeyboard);
	glutDisplayFunc(funcaoDisplay);
	glutIdleFunc(temporizador);
	inicializacao();

	glutMainLoop();

	return 0;
}

