#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>

const int pixelSize = 5;
float top = 800/pixelSize, bottom = 0, left = 0, right = 800/pixelSize;
int antialiasing = true;

using namespace std;

vector<float> vx;
vector<float> vy;

void inicializacao() {
	glClearColor(0, 0, 0, 0.0);
}

float calcDist(int ax, int ay, int bx, int by, int x, int y) {
	int H = by - ay;
	int W = bx - ax;
	return abs(H*x - W*y + bx*ay - by*ax) /
		sqrt(W*W + H*H);
}

void reta(int ax, int ay, int bx, int by) {
	float y = ay;
	int W = bx - ax;
	int H = by - ay;
	float m = ((float)H)/W;


	/*for(int x = ax; x <= bx; x++) {
		glPointSize(pixelSize);
		y += m;
	
		glColor3f(1, 1, 1);
		glBegin(GL_POINTS);
		glVertex2f(x, roundf(y));
		glEnd();
	}*/

	if (antialiasing) {
		float dist, intensity;
		float f = 2*H - W;
		float c = 2 * W / sqrt(W*W + H*H);

		for (int x = ax; x <= bx; x++) {
			for (int i = -1; i <= 1; i++) {
				dist = calcDist(ax, ay, bx, by, x, y + i);
				intensity = abs(i)*c + pow((-1)*i, i) * dist;

				printf("x = %d, f = %f, dist = %f\n", x, f, intensity);

				if (intensity < 0 && intensity > 1)
					intensity = 0;
				intensity = 1 - intensity;
				
				glPointSize(pixelSize);
				glColor3f(intensity, intensity, intensity);
				glBegin(GL_POINTS);
				glVertex2f(x, roundf(y + i));
				glEnd();
			}
			printf("\n");


			if (f < 0)
				f += 2*H;
			else {
				f += 2*(H - W);
				y++;
			}
		}
		printf("--------------------------------------\n");
	}
	else {
		float f = 2*H - W;
		for(int x = ax; x <= bx; x++) {
			//printf("m = %f, x = %d, f = %f\n", m, x, f);
			
			glPointSize(pixelSize);
			glColor3f(1, 1, 1);
			glBegin(GL_POINTS);
			glVertex2f(x, roundf(y));
			glEnd();

			if (f < 0)
				f += 2*H;
			else {
				f += 2*(H-W);
				y++;
			}

		}
		//printf("----------------------------------------------------\n");
	}
}

void funcaoDisplay() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);
	reta(vx[0], vy[0], vx[1], vy[1]);

	glutSwapBuffers();
	glFlush();
}

void funcaoKeyboard(unsigned char key, int x, int y) {

	if(key == 'q') {
		exit(-1);
	}
	if(key == 't') {
		antialiasing = 1 - antialiasing;
	}
	glutPostRedisplay();
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
		if(button == GLUT_LEFT_BUTTON) {
			vx[0] = worldX;
			vy[0] = worldY;
		}
		if(button == GLUT_RIGHT_BUTTON) {
			vx[1] = worldX;
			vy[1] = worldY;
		}
	}	
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Splines");
	glutKeyboardFunc(funcaoKeyboard);
	glutMouseFunc(funcaoMouse);
	glutDisplayFunc(funcaoDisplay);
	inicializacao();

	int x[] = {top*0.5, 0.75*top};
	int y[] = {top*0.5, 0.7*top};
	for(int i = 0; i < 2; i++) {
		vx.push_back(x[i]);
		vy.push_back(y[i]);
	}
	glutMainLoop();

	return 0;
}

