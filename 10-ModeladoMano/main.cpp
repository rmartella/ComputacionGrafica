#include <GL\glut.h>
#include <stdio.h>

bool* keyStates = new bool[256];

float shoulder = 0.0;
float elbow = 0.0;
float wrist = 0.0;

void renderPrimitiveQuad()
{
	glBegin(GL_QUADS);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
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

/*
Funcion para renderizar el brazo
*/
void renderArm()
{
	// Articulación del hombro
	glRotatef(shoulder, 0.0, 0.0, 1.0);

	glPushMatrix();
	// Translacion para que el centro de rotación este a la izquierda del enlace
	glTranslatef(1.0, 0.0, 0.0);
	glScalef(2.0, 0.4, 0.4);
	glutWireCube(1.0);
	glPopMatrix();

	// Articulacion del codo
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef(elbow, 0.0, 0.0, 1.0);

	glPushMatrix();
	// Translacion para que el centro de rotación este a la izquierda del enlace
	glTranslatef(1.0, 0.0, 0.0);
	glScalef(2.0, 0.4, 0.4);
	glutWireCube(1.0);
	glPopMatrix();

}

/*
Funcion para renderizar la mano
*/
void renderHand()
{
	// Translacion necesaria para colocar la mano dos unidades por delante del
	// ultimo enlace del brazo
	glTranslatef(2.0, 0.0, 0.0);

	// Articulacion de la muneca
	glRotatef(wrist, 0.0, 0.0, 1.0);

	// Se dibuja la palma de la mano
	glPushMatrix();
	glTranslatef(0.4, 0.0, 0.0);
	glScalef(0.8, 0.3, 0.4);
	glutWireCube(1.0);
	glPopMatrix();

	// Se hace push para poder regresar y crear otro dedo
	// respecto a la palma de la mano
	glPushMatrix();
	// Translaciones necesarias para crear el pulgar
	glTranslatef(-0.35, 0.0, 0.0);
	glTranslatef(0.8, 0.0, 0.4);
	// Primer hueso del pulgar
	glPushMatrix();
	// Translacion para que el centro de rotación este a la izquierda del enlace
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	// Segundo hueso del pulgar
	glTranslatef(0.3, 0.0, 0.0);
	glPushMatrix();
	// Translacion para que el centro de rotación este a la izquierda del enlace
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	// Segundo hueso del pulgar
	glTranslatef(0.3, 0.0, 0.0);
	// Se rota el segundo hueso para hacer el gesto
	glRotatef(-90, 0.0, 0.0, 1.0);
	glPushMatrix();
	// Translacion para que el centro de rotación este a la izquierda del enlace
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();
	// Este pop saca de la pila y regresa a la matriz respecto a la palma de la mano
	glPopMatrix();

	// Esto es para el dedo indice.
	glPushMatrix();
	glTranslatef(0.8, 0.0, 0.15);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();
	glPopMatrix();

	// Esto es para el dedo medio.
	glPushMatrix();
	glTranslatef(0.8, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();
	glPopMatrix();

	// Esto es para el dedo anular.
	glPushMatrix();
	glTranslatef(0.8, 0.0, -0.15);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();
	glPopMatrix();

	// Esto es para el dedo menique.
	glPushMatrix();
	glTranslatef(0.8, 0.0, -0.3);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   for (int i = 0; i < 256; i++)
	   keyStates[i] = false;
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glColor3f(0.0f, 0.0f, 1.0f);

   glTranslatef(-1.0, -3.0, -10.0);
   glRotatef(90, 0.0, 0.0, 1.0);
   glRotatef(-90, 1.0, 0.0, 0.0);
   // Se dibuja el brazo
   renderArm();
   // Se dibuja la mano
   glColor3f(1.0, 1.0, 0.0);
   renderHand();

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
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	keyStates[key] = true;
	switch (key) {
	case 27:
		exit(0);
		break;	
	case 's':
		shoulder += 3.0;
		break;
	case 'w':
		elbow += 3.0;
		break;
	case 'd':
		wrist += 3.0;
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
