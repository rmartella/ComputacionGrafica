#include <GL\glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159

bool* keyStates = new bool[256];

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
	glBegin(GL_LINE);
	for (int i = -numcells; i <= numcells; i++) {
		glVertex3f((float) i * sizecell , (float) numcells * sizecell, 0.0f);
		glVertex3f((float)i * sizecell, -(float) numcells * sizecell, 0.0f);
		glVertex3f((float)numcells * sizecell, (float)i * sizecell, 0.0f);
		glVertex3f(-(float)numcells * sizecell, (float)i * sizecell, 0.0f);
	}
	glEnd();
	glFlush();
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
	glFlush();
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
	glEnd();
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
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0, 0, -60);
   glColor3f(0, 0, 1.0);
   
   // renderCono(20, 1.0, 2.0, true);
   renderGrid2(1.0, 20);
   // renderConoDoble(20, 1.0, 1.0, true);
   // renderEsfera(20, 20, 1.0, true);
   // renderCilindro(20.0, 1.0, 1.0, true);
   // renderToroide(20.0, 20.0, 1.0, 1.0, true);
   // renderPrimitivePoints();
   // renderPrimitiveLineLoop();
   // renderPrimitiveTriangle();
   glFlush ();
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

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

void mouseFunc(int button, int state, int x, int y)
{
	printf("Mouse botton %d estado %d x %d y %d\n", button, state, x, y);
}

void motionFunc(int x, int y)
{
	printf("Mouse Motion func %d, %d\n", x, y);
}

void passiveMotionFunc(int x, int y)
{
	printf("Passive Mouse motion func %d, %d\n", x, y);
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
   glutReshapeFunc(reshape);
   glutMouseFunc(mouseFunc);
   glutMotionFunc(motionFunc);
   glutPassiveMotionFunc(passiveMotionFunc);
   glutMainLoop();
   return 0;
}
