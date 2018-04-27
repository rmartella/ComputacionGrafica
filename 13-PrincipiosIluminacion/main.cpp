#include <GL\glut.h>
#include <math.h>
#include <stdio.h>

#include "CamaraFPS.h"

#define PI 3.14159

bool* keyStates = new bool[256];

CamaraFPS camara;

float lastX = 0.0, lastY = 0.0;

GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

GLfloat light_ambient[] = { 1.0, 0.0, 0.2, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 0.0, 0.0, 1.5, 1.0 };

double spin = 0;

float ** CreaMatriz(int x, int y)
{
	int i;
	float **matriz;
	matriz = (float **)malloc(y * sizeof(float *));
	for (i = 0; i<y; i++)
		matriz[i] = (float *)malloc(x * sizeof(float));
	return matriz;
}

void renderGrid(float sizecell, int numcells) {
	glBegin(GL_LINES);
	for (int i = -numcells; i <= numcells; i++) {
		glVertex3f((float) i * sizecell , (float) numcells * sizecell, 0.0f);
		glVertex3f((float)i * sizecell, -(float) numcells * sizecell, 0.0f);
		glVertex3f((float)numcells * sizecell, (float)i * sizecell, 0.0f);
		glVertex3f(-(float)numcells * sizecell, (float)i * sizecell, 0.0f);
	}
	glEnd();
}

void renderGrid2(float sizecell, int numcells) {
	for (int i = -numcells; i < numcells; i++)
		for (int j = -numcells; j < numcells; j++){
			glBegin(GL_QUADS);
			float rrand = rand() % 255;
			rrand = rrand / 255;
			glColor3f(rrand, rrand, rrand);
			glVertex3f((float) i, (float)j, 0.0f);
			glVertex3f((float)(i + 1), (float)j, 0.0f);
			glVertex3f((float)(i + 1), (float)(j + 1), 0.0f);
			glVertex3f((float) i, (float)(j + 1), 0.0f);
			glEnd();
		}
}

float grad2rad(float grados)
{
	return (float)(grados*PI) / 180;
}

void renderPrimitiveQuad()
{
	glBegin(GL_QUADS);
	glVertex3f(-sqrt(2.0) / 2.0f, -sqrt(2.0) / 2.0f, -4.0f);
	glVertex3f(sqrt(2.0) / 2.0f, -sqrt(2.0) / 2.0f, -4.0f);
	glVertex3f(sqrt(2.0) / 2.0f, sqrt(2.0) / 2.0f, -4.0f);
	glVertex3f(-sqrt(2.0) / 2.0f, sqrt(2.0) / 2.0f, -4.0f);
}

void renderPrimitivePoints()
{
	glPointSize(20.0f);
	glBegin(GL_POINTS);
	glVertex3f(-0.5f, -0.5f, -3.0f);
	glVertex3f(-0.5f, 0.5f, -3.0f);
	glVertex3f(0.5f, 0.5f, -3.0f);
	glVertex3f(0.5f, -0.5f, -3.0f);
	glEnd();
}

void renderPrimitiveLineLoop() {
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5f, -0.5f, -3.0f);
	glVertex3f(-0.5f, 0.5f, -3.0f);
	glVertex3f(0.5f, 0.5f, -3.0f);
	glVertex3f(0.5f, -0.5f, -3.0f);
	glEnd();
}

void renderPrimitiveTriangle()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f, -0.5f, -3.0f);
	glVertex3f(0.5f, -0.5f, -3.0f);
	glVertex3f(0.0f, 0.5f, -3.0f);
	glEnd();
}

void renderCono(int slice, float radio, float altot, int wired)
{
	int i;
	float incr = 360.0 / slice;
	float *x, *z;
	float angulo = 0.0f;
	float alto = altot / 2;
	x = (float *)malloc((slice + 1) * sizeof(float));
	z = (float *)malloc((slice + 1) * sizeof(float));
	if (x != NULL && z != NULL)
	{
		for (i = 0; i<slice; i++)
		{
			x[i] = radio * cos(grad2rad(angulo));
			z[i] = radio * sin(grad2rad(angulo));
			angulo += incr;
		}
		x[slice] = x[0];
		z[slice] = z[0];
		if (wired == 0)
			glBegin(GL_TRIANGLES);
		else
			glBegin(GL_LINE_STRIP);
		for (i = 0; i<slice; i++)
		{
			glVertex3f(x[i], -alto, z[i]);
			glVertex3f(x[i + 1], -alto, z[i + 1]);
			glVertex3f(0.0f, alto, 0.0);
			
			glVertex3f(x[i], -alto, z[i]);
			glVertex3f(x[i + 1], -alto, z[i + 1]);
			glVertex3f(0.0f, -alto, 0.0);
		}
		glEnd();
		free(x);
		free(z);
	}
}

void renderConoDoble(int slice, float radio, float altot, int wired)
{
	int i;
	float incr = 360.0 / slice;
	float *x, *z;
	float angulo = 0.0f;
	float alto = altot / 2;
	x = (float *)malloc((slice + 1) * sizeof(float));
	z = (float *)malloc((slice + 1) * sizeof(float));
	if (x != NULL && z != NULL)
	{
		for (i = 0; i<slice; i++)
		{
			x[i] = radio * cos(grad2rad(angulo));
			z[i] = radio * sin(grad2rad(angulo));
			angulo += incr;
		}
		x[slice] = x[0];
		z[slice] = z[0];
		if (wired == 0)
			glBegin(GL_TRIANGLES);
		else
			glBegin(GL_LINE_LOOP);
		for (i = 0; i<slice; i++)
		{
			glVertex3f(x[i], 0.0f, z[i]);
			glVertex3f(x[i + 1], 0.0f, z[i + 1]);
			glVertex3f(0.0f, alto, 0.0);

			glVertex3f(x[i], 0.0, z[i]);
			glVertex3f(x[i + 1], 0.0, z[i + 1]);
			glVertex3f(0.0f, -alto, 0.0f);
		}
		glEnd();
		free(x);
		free(z);
	}
}

void renderCilindro(int slice, float radio, float altot, int wired)
{
	int i;
	float incr = 360.0 / slice;
	float *x, *z;
	float angulo = 0.0f;
	float alto = altot / 2;
	x = (float *)malloc((slice + 1) * sizeof(float));
	z = (float *)malloc((slice + 1) * sizeof(float));
	if (x != NULL && z != NULL)
	{
		for (i = 0; i<slice; i++)
		{
			x[i] = radio * cos(grad2rad(angulo));
			z[i] = radio * sin(grad2rad(angulo));
			angulo += incr;
		}
		x[slice] = x[0];
		z[slice] = z[0];
		if (wired == 0)
			glBegin(GL_QUADS);
		else
			glBegin(GL_LINE_STRIP);
		for (i = 0; i<slice; i++)
		{
			glVertex3f(x[i], -alto, z[i]);
			glVertex3f(x[i + 1], -alto, z[i + 1]);
			glVertex3f(x[i + 1], alto, z[i + 1]);
			glVertex3f(x[i], alto, z[i]);

			glVertex3f(0.0f, alto, 0.0f);
			glVertex3f(x[i], alto, z[i]);
			glVertex3f(x[i + 1], alto, z[i + 1]);
			glVertex3f(0.0f, alto, 0.0f);

			glVertex3f(0.0f, -alto, 0.0f);
			glVertex3f(x[i], -alto, z[i]);
			glVertex3f(x[i + 1], -alto, z[i + 1]);
			glVertex3f(0.0f, -alto, 0.0f);
		}
		glEnd();
		free(x);
		free(z);
	}
}


void renderEsfera(int sliceh, int slicev, float radio, int wired)
{
	int i, j;
	float **x, **y, **z;
	float incrtheta = 360.0 / slicev;
	float incrphi = 180.0 / (sliceh - 1);
	float theta, phi;
	x = CreaMatriz(slicev + 1, sliceh);
	y = CreaMatriz(slicev + 1, sliceh);
	z = CreaMatriz(slicev + 1, sliceh);
	if (x != NULL && y != NULL && z != NULL)
	{
		phi = 0;
		for (i = 0; i<sliceh; i++)
		{
			theta = 0;
			for (j = 0; j <= slicev; j++)
			{
				x[i][j] = radio * cos(grad2rad(theta))*sin(grad2rad(phi));
				y[i][j] = radio * cos(grad2rad(phi));
				z[i][j] = radio * sin(grad2rad(theta))*sin(grad2rad(phi));
				theta += incrtheta;
			}
			phi += incrphi;
		}
		if (wired == 0)
			glBegin(GL_QUADS);
		else
			glBegin(GL_LINE_STRIP);
		for (i = 0; i<sliceh - 1; i++)
			for (j = 0; j<slicev; j++)
			{
				glVertex3f(x[i][j], y[i][j], z[i][j]);
				glVertex3f(x[i + 1][j], y[i + 1][j], z[i + 1][j]);
				glVertex3f(x[i + 1][j + 1], y[i + 1][j + 1], z[i + 1][j + 1]);
				glVertex3f(x[i][j + 1], y[i][j + 1], z[i][j + 1]);
			}
		glEnd();
		free(x);
		free(y);
		free(z);
	}
}

void renderToroide(int sliceht, int slicev, float radio1, float radio2, int wired)
{
	int sliceh = sliceht + 1;
	int i, j;
	float **x, **y, **z;
	float incrtheta = 360.0 / slicev;
	float incrphi = 360.0 / (sliceh - 1);
	float theta, phi;
	x = CreaMatriz(slicev + 1, sliceh);
	y = CreaMatriz(slicev + 1, sliceh);
	z = CreaMatriz(slicev + 1, sliceh);
	if (x != NULL && y != NULL && z != NULL)
	{
		phi = 0;
		for (i = 0; i<sliceh; i++)
		{
			theta = 0;
			for (j = 0; j <= slicev; j++)
			{
				x[i][j] = (radio1 + (radio2*cos(grad2rad(phi))))*cos(grad2rad(theta));
				y[i][j] = radio2 * sin(grad2rad(phi));
				z[i][j] = (radio1 + (radio2*cos(grad2rad(phi))))*sin(grad2rad(theta));
				theta += incrtheta;
			}
			phi += incrphi;
		}
		if (wired == 0)
			glBegin(GL_QUADS);
		else
			glBegin(GL_LINE_STRIP);
		for (i = 0; i<sliceh - 1; i++)
			for (j = 0; j<slicev; j++)
			{
				glVertex3f(x[i][j], y[i][j], z[i][j]);
				glVertex3f(x[i + 1][j], y[i + 1][j], z[i + 1][j]);
				glVertex3f(x[i + 1][j + 1], y[i + 1][j + 1], z[i + 1][j + 1]);
				glVertex3f(x[i][j + 1], y[i][j + 1], z[i][j + 1]);
			}
		glEnd();
		free(x);
		free(y);
		free(z);
	}

}

void init(void) 
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
   
   if (spin > 360.0)
	   spin -= 360;

   spin += 0.1;

   glPushMatrix();

	   camara.setViewMatrix();

	   glPushMatrix();
		   glRotatef((double) spin, 1.0, 0.0, 0.0);
		   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		   glTranslated(0.0, 0.0, 1.5);
		   glDisable(GL_LIGHTING);
		   glColor3f(0.0, 1.0, 1.0);
		   glutWireCube(0.1);
		   glEnable(GL_LIGHTING);
	   glPopMatrix();

	   glPushMatrix();
		   glTranslatef(0.0, -0.5, 0.0);
		   glRotatef(-90.0f, 1.0, 0.0, 0.0);
		   renderGrid(0.3, 20);
	   glPopMatrix();

	   glPushMatrix();
		   glTranslatef(0.0, 0.0, -0.5f);
		   glutSolidSphere(0.5, 20, 20);
	   glPopMatrix();

	   glPushMatrix();
			glTranslatef(3.0, 0.0, -0.5f);
			glutSolidSphere(0.5, 20, 20);
	   glPopMatrix();
   glPopMatrix();
   
   // renderCono(20, 1.0, 2.0, true);
   // renderGrid2(1.0, 20);
   // renderConoDoble(20, 1.0, 1.0, true);
   // renderEsfera(20, 20, 1.0, true);
   // renderCilindro(20.0, 1.0, 1.0, true);
   // renderToroide(20.0, 20.0, 1.0, 1.0, true);
   // renderPrimitivePoints();
   // renderPrimitiveLineLoop();
   // renderPrimitiveTriangle();
   glFlush();
}

void idleFunc(void)
{
	glutPostRedisplay();
}

void reshape(int width, int height) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)width / (GLfloat)height, 0.01, 100.0);
	// glFrustum(-0.004, 0.004, -0.004, 0.004, 0.01, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void keyboard(unsigned char key, int x, int y)
{
	keyStates[key] = true;
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'a':
	case 'A':
		printf("Se presiono la tecla A\n");
		break;
	case 'd':
	case 'D':
		printf("Se presiono la tecla D\n");
		break;
	case 'w':
	case 'W':
		printf("Se presiono la tecla W\n");
		break;
	case 's':
	case 'S':
		printf("Se presiono la tecla S\n");
		break;
	}
}

void specialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		camara.ProcessKeyboard(Camera_Movement::C_FORWARD, 0.01);
	break;
	case GLUT_KEY_DOWN:
		camara.ProcessKeyboard(Camera_Movement::C_BACKWARD, 0.01);
		break;
	case GLUT_KEY_LEFT:
		camara.ProcessKeyboard(Camera_Movement::C_LEFT, 0.01);
		break;
	case GLUT_KEY_RIGHT:
		camara.ProcessKeyboard(Camera_Movement::C_RIGHT, 0.01);
		break;
	}
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

void mouseFunc(int button, int state, int x, int y)
{
	printf("Mouse botton %d estado %d x %d y %d\n", button, state, x, y);
}

void motionFunc(int x, int y)
{
	float xoffset = lastX - x;
	float yoffset = lastY - y;
	printf("Mouse Motion func %d, %d\n", x, y);
	camara.ProcessMouseMovement(-xoffset, yoffset, false);
	lastX = x;
	lastY = y;
}

void passiveMotionFunc(int x, int y)
{
	printf("Passive Mouse motion func %d, %d\n", x, y);
	lastX = x;
	lastY = y;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Teclado");
   init();
   glutDisplayFunc(display);
   glutIdleFunc(idleFunc);
   glutKeyboardFunc(keyboard);
   glutKeyboardUpFunc(keyUp);
   glutSpecialFunc(specialFunc);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouseFunc);
   glutMotionFunc(motionFunc);
   glutPassiveMotionFunc(passiveMotionFunc);
   glutMainLoop();
   return 0;
}
