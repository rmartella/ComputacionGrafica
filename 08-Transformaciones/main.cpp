#include <GL\glut.h>
#include <stdio.h>

bool* keyStates = new bool[256];

bool movingUp = false;
float yLocation = 0.0f;

float yRotationAngle = 0.0f;

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
   glColor3f(0.0f, 0.0f, 1.0f);
   
   glTranslatef(0.0f, 0.0f, -5.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive  
   glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis  
   glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis  
   
   renderPrimitiveQuad();
   // renderPrimitivePoints();
   // renderPrimitiveLineLoop();
   // renderPrimitiveTriangle();
   glFlush ();

   if (movingUp)
	   yLocation -= 0.005f;
   else  // Otherwise  
	   yLocation += 0.005f;

   if (yLocation < -3.0f)
	   movingUp = false;
   else if (yLocation > 3.0f)
	   movingUp = true;

   yRotationAngle += 0.005f;

   if (yRotationAngle > 360.0f)
	   yRotationAngle -= 360.0f;
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
   glutKeyboardFunc(keyUp);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouseFunc);
   glutMotionFunc(motionFunc);
   glutPassiveMotionFunc(passiveMotionFunc);
   glutMainLoop();
   return 0;
}
