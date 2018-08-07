#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <time.h>

float randomf() {
	return ((float)rand())/RAND_MAX;
}

void funcaoDisplay() {
	printf("display\n");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2, 2, -2, 2);


	glClearColor(0.0,0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(10);

	//1o viewport
	glViewport(0, 0, 100, 100);
	glColor3f(1, 1, 1);
	glRecti(-2, -2, 2, 2);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
		for(int i = 0; i < 100; i++)
			glVertex2f(randomf(), randomf());
	glEnd();

	//2o viewport
	glViewport(200, 0, 100, 100);
	glColor3f(1, 1, 1);
	glRecti(-2, -2, 2, 2);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
		for(int i = 0; i < 100; i++)
			glVertex2f(randomf(), randomf());
	glEnd();

	glFlush();
}

void funcaoKeyboard(unsigned char key, int x, int y) {

	printf("%c\n", key);
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello world");
	glutKeyboardFunc(funcaoKeyboard);
	glutDisplayFunc(funcaoDisplay);
	glutMainLoop();

	return 0;
}

