#include <GL/glut.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

//COMPILE: -lGL -lGLU -lglut

int b = 0;

void keyboard_func(unsigned char key, int x, int y) {
	//glut.h
	printf("%c\n", key);
	glutPostRedisplay();
}

void display_func() {
	//gl.h
	glClear(GL_COLOR_BUFFER_BIT);
	if (b == 0) {
		glClearColor(0, 0.7, 0.3 ,0); //rgba range:[0,1]
		b = 1;
	}
	else {
		glClearColor(0.7, 0.3, 0, 0); //rgba range:[0,1]
		b = 0;
	}

	glPointSize(10);
	glColor3f(0,0,0);
	glBegin(GL_POINTS);
		glVertex2f(-0.9,-0.95);
		glVertex2f(2.0, 1.0);
		glColor3f(1, 1, 1);
		glVertex2i(100,12);
	glEnd();
	glRecti(50, 50, 75, 75);


	glFlush();
}

int main(int argc, char** argv) {
	//glut.h
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("taitou"); //title
	glutKeyboardFunc(keyboard_func);
	glutDisplayFunc(display_func);

	//alter size of window shown
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(25, 100, 25, 100);
	//end
	glViewport(-2, -2, 210, 210);

	glutMainLoop();
	return 0;
}
