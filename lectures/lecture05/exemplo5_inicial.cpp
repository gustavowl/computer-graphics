#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//double camera_eye[3] = {1, 1, 5};
double random_scale_matrix[5][5];
double radius = 5.0;
double max[3] = {radius, 8.0, radius};
double min[3] = {radius * (-1.0), 3.0, radius * (-1.0)};
double camera_eye[3] = {radius, 5.0, 0};
double step[3] = {0.1, 0.07, 0.1};

float randomf() {
	return ((float)rand())/RAND_MAX;
}


void initialize_random_scale_matrix() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			random_scale_matrix[i][j] = randomf() * 3;
		}
	}
}

void inicializacao() {
	//cor de fundo eh cinza
	glClearColor(0.8, 0.8, 0.8, 0.0);

	glMatrixMode(GL_PROJECTION);
	//alterne e altere as projecoes para alcancar os resultados desejados
	glFrustum(-1, 1, -1, 1, 1.5, 20.0);
	//glOrtho(-1, 1, -1, 1, 1.5, 20.0);
	//gluPerspective(60, 1, 1.5, 60.0);

	initialize_random_scale_matrix();

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
	//mensagem para verificar quando esta funcao eh chamada
	system("clear");
	printf("display\n");
	printModelView();
	printProjection();

	//limpa a tela com a cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//altere gluLookAt para movimentar a camera ao redor da cidade
	gluLookAt(camera_eye[0], camera_eye[1], camera_eye[2],
		0, 0, 0, 0, 1, 0);
	
	glColor3f(0, 0, 0);
	//desenhar um grid de cubos esticados verticalmente (predios)
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			glPushMatrix();
			//glScalef(1.0, i+3.0, 1.0);
			glScalef(1.0, random_scale_matrix[i+2][j+2], 1.0);
			glTranslatef((double)i, 0.25, (double)j);
			glutWireCube(0.5);
			glPopMatrix();
		}
	}

	glFlush();
	glutSwapBuffers();
}

void funcaoKeyboard(unsigned char key, int x, int y) {

	if(key == 'q')
		exit(-1);
	glutPostRedisplay();
}

int t = 0;

void temporizador() {
	t++;
	//printf("%d ", t);	
	if (t == 2112 * 73) {	
		//camera_eye[0] = ((int)camera_eye[0] + 1) % 10;
		//camera_eye[2] = ((int)camera_eye[2] + 1) % 10;
		for (int i = 0; i < 3; i++) {
			if (camera_eye[i] >= max[i] || camera_eye[i] <= min[i])
				step[i] *= -1.0;
			camera_eye[i] += step[i];
		}
		
		glutPostRedisplay();
		t = 0;
		//printf("================================================");
	}
}

int main(int argc, char **argv) {

	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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

