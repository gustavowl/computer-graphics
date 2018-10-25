#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define PI 3.1415926535897932

//double camera_eye[3] = {1, 1, 5};
double random_scale_matrix[5][5];
double radius = 5.5;
int angle_xz = 0, angle_y = 0;
//double max[3] = {radius, 8.0, radius};
//double min[3] = {radius * (-1.0), 3.0, radius * (-1.0)};
double camera_eye[3] = {cos(angle_xz) * radius,
	sin(angle_y) * radius,
	sin(angle_xz) * radius};
//double step[3] = {0.1, 0.07, 0.1};
int step_xz = 3, step_y = 2;

float randomf() {
	return ((float)rand())/RAND_MAX;
}

double degress_to_rad(int degrees) {
	return degrees * PI / 180.0;
}

void initialize_random_scale_matrix() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			random_scale_matrix[i][j] = randomf() * 3;
		}
	}
}

void inicializacao() {
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};
	glShadeModel(GL_FLAT);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

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
	/*system("clear");
	printf("display\n");
	printModelView();
	printProjection();*/

	//limpa a tela com a cor de fundo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//altere gluLookAt para movimentar a camera ao redor da cidade
	gluLookAt(camera_eye[0], camera_eye[1], camera_eye[2],
		0, 0, 0, 0, 1, 0);
	
	glColor3f(0, 0, 0);
	//desenhar um grid de cubos esticados verticalmente (predios)
	/*for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			glPushMatrix();
			//glScalef(1.0, i+3.0, 1.0);
			glScalef(1.0, random_scale_matrix[i+2][j+2], 1.0);
			glTranslatef((double)i, 0.25, (double)j);
			//glutWireCube(0.5);
			glutSolidCube(0.5);
			glPopMatrix();
		}
	}*/
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glutSolidCube(1);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void funcaoKeyboard(unsigned char key, int x, int y) {

	if(key == 'q')
		exit(-1);

	//go back and forward
	if(key == 'k')
		camera_eye[0] += 0.1;
	if(key == 'i')
		camera_eye[0] -= 0.1;

	//go up and down
	if(key == 'w')
		camera_eye[1] += 0.1;
	if(key == 's')
		camera_eye[1] -= 0.1;

	//go left and right
	if(key == 'a')
		camera_eye[2] += 0.1;
	if(key == 'd')
		camera_eye[2] -= 0.1;

	glutPostRedisplay();
}

int t = 0;

void temporizador() {
	t++;
	//printf("%d ", t);	
	if (t == 2112 * 73) {
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

