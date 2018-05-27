#include <GL\glut.h>
#include <math.h>
#include <stdio.h>

#include "CamaraFPS.h"
#include "Texture.h"
#include "KinectManager.h"

#define PI 3.14159

bool* keyStates = new bool[256];

CamaraFPS camara;
Texture texture(GL_TEXTURE_2D, "../Textures/texturaLadrillos.jpg");

KinectManager mKinectManager;
INuiCoordinateMapper * mCordinateMapper;

float lastX = 0.0, lastY = 0.0;

GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

/*This is for the properties of the point light*/
GLfloat light_ambient[] = { 1.0, 0.0, 0.2, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 0.0, 0.0, 1.5, 1.0 };

/*This is for the properties of the spot light*/
GLfloat light1_ambient[] = { 0.0, 1.0, 0.2, 1.0 };
GLfloat light1_diffuse[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat light1_specular[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat light1_position[] = { -2.0, 0.0, -0.5, 1.0 };
GLfloat spot_direction[] = { 1.0, 0.0, 0.0 };

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
	glTexCoord2f(0.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-0.5f, -0.5f, -3.0f);
	glTexCoord2f(1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.5f, -0.5f, -3.0f);
	glTexCoord2f(0.5, 1.0);
	glNormal3f(0.0, 0.0, 1.0);
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
				glTexCoord2f(j / (float)sliceh, i / (float)slicev);
				glVertex3f(x[i][j], y[i][j], z[i][j]);
				glTexCoord2f(j / (float)sliceh, (i + 1) / (float)slicev);
				glVertex3f(x[i + 1][j], y[i + 1][j], z[i + 1][j]);
				glTexCoord2f((j + 1) / (float)sliceh, (i + 1) / (float)slicev);
				glVertex3f(x[i + 1][j + 1], y[i + 1][j + 1], z[i + 1][j + 1]);
				glTexCoord2f((j + 1) / (float)sliceh, i / (float)slicev);
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

   /*This is for point light*/
   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
   glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);

   texture.load();

   // Inicialización del dispositivo
   mKinectManager.Initialize();

   // Inicialización del stream RGB
   mKinectManager.InitializeImageRGBStream();

   // Inicialización del stream de profunidad
   mKinectManager.InitializeDepthStream();

   // Inicialización del Sekeleton
   mKinectManager.InitializaSkeletonTraking();

   // Se crea el mapeador de coordenadas
   mCordinateMapper = mKinectManager.createCoordinateMapper();

}

void matrixFromAxisAngle(Vertex3 v, float angle, float * mat) {

	double c = cos(angle);
	double s = sin(angle);
	double t = 1.0 - c;
	double magnitude = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	if (magnitude == 0) return;
	v.x /= magnitude;
	v.y /= magnitude;
	v.z /= magnitude;

	mat[0] = c + v.x*v.x*t;
	mat[5] = c + v.y*v.y*t;
	mat[10] = c + v.z*v.z*t;


	double tmp1 = v.x*v.y*t;
	double tmp2 = v.z*s;
	mat[1] = tmp1 + tmp2;
	mat[4] = tmp1 - tmp2;
	tmp1 = v.x*v.z*t;
	tmp2 = v.y*s;
	mat[2] = tmp1 - tmp2;
	mat[8] = tmp1 + tmp2;    tmp1 = v.y*v.z*t;
	tmp2 = v.x*s;
	mat[6] = tmp1 + tmp2;
	mat[9] = tmp1 - tmp2;
}

void drawBone(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1) {
	NUI_SKELETON_POSITION_TRACKING_STATE joint0State = skel.eSkeletonPositionTrackingState[joint0];
	NUI_SKELETON_POSITION_TRACKING_STATE joint1State = skel.eSkeletonPositionTrackingState[joint1];

	if (joint0State == NUI_SKELETON_POSITION_NOT_TRACKED || joint1State == NUI_SKELETON_POSITION_NOT_TRACKED){
		return;
	}

	if (joint0State == NUI_SKELETON_POSITION_INFERRED && joint1State == NUI_SKELETON_POSITION_INFERRED){
		return;
	}

	if (joint0State == NUI_SKELETON_POSITION_TRACKED && joint1State == NUI_SKELETON_POSITION_TRACKED){
		Vertex3 v0(skel.SkeletonPositions[joint0].x, skel.SkeletonPositions[joint0].y, skel.SkeletonPositions[joint0].z);
		Vertex3 v1(skel.SkeletonPositions[joint1].x, skel.SkeletonPositions[joint1].y, skel.SkeletonPositions[joint1].z);
		Vertex3 y(0.0, 1.0, 0.0);
		Vertex3 p = v0.sub(v1);
		Vertex3 t = y.cross(p);
		float angle = acos(y.dot(p) / p.norm());

		Vertex3 pm((v0.x + v1.x) / 2.0f, (v0.y + v1.y) / 2.0f, (v0.z + v1.z) / 2.0f);
		float length = p.norm();

		glPushMatrix();

		glTranslatef(pm.x, pm.y, -pm.z);
		float mat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		matrixFromAxisAngle(t, angle, mat);
		glLoadMatrixf(mat);
		glScalef(0.01, length, 0.01);
		glutSolidCube(1.0);

		glPopMatrix();
		
	}
	else{
		Vertex3 v0(skel.SkeletonPositions[joint0].x, skel.SkeletonPositions[joint0].y, skel.SkeletonPositions[joint0].z);
		Vertex3 v1(skel.SkeletonPositions[joint1].x, skel.SkeletonPositions[joint1].y, skel.SkeletonPositions[joint1].z);
		Vertex3 y(0.0, 1.0, 0.0);
		Vertex3 p = v0.sub(v1);
		Vertex3 t = y.cross(p);
		float angle = acos(y.dot(p) / p.norm());

		Vertex3 pm((v0.x + v1.x) / 2.0f, (v0.y + v1.y) / 2.0f, (v0.z + v1.z) / 2.0f);
		float length = p.norm();

		glPushMatrix();

		glTranslatef(pm.x, pm.y, -pm.z);
		float mat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		matrixFromAxisAngle(t, angle, mat);
		glLoadMatrixf(mat);
		glScalef(0.01, length, 0.01);
		glutSolidCube(1.0);

		glPopMatrix();
	}
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

	   NUI_IMAGE_FRAME rgbFrame;
	   NUI_IMAGE_FRAME depthFrame;
	   NUI_SKELETON_FRAME skeletonFrame;
	   NUI_INTERACTION_FRAME interactionFrame;

	   //DWORD ret = ::WaitForSingleObject(mKinectManager.streamEvent, INFINITE);
	   //ResetEvent(mKinectManager.streamEvent);

	   HRESULT resul;
	   HANDLE * hEvents = mKinectManager.getEventHandles();
	   WaitForMultipleObjects(sizeof(hEvents) / sizeof(hEvents[0]), hEvents, FALSE, INFINITE);

	   WaitForSingleObject(hEvents[0], 0);
	   rgbFrame = mKinectManager.getRGBFrame();
	   NUI_LOCKED_RECT colorData;
	   rgbFrame.pFrameTexture->LockRect(0, &colorData, 0, 0);

	   mKinectManager.releseRGBFrame(&rgbFrame);

	   WaitForSingleObject(hEvents[1], 0);
	   depthFrame = mKinectManager.getDepthFrame(&resul);
	   if (resul == S_OK) {

		   NUI_LOCKED_RECT depthData;
		   INuiFrameTexture* pTexture = mKinectManager.getDepthImagePixelFrameTexture(&depthFrame);
		   pTexture->LockRect(0, &depthData, NULL, 0);
		   mKinectManager.releseDepthFrame(&depthFrame);

		   WaitForSingleObject(hEvents[2], 0);
		   skeletonFrame = mKinectManager.getSkeletonFrame(&resul);

		   if (resul == S_OK) {
			   for (int i = 0; i < NUI_SKELETON_COUNT; i++) {
				   NUI_SKELETON_DATA& skeletonData = skeletonFrame.SkeletonData[i];

				   if (skeletonData.eTrackingState == NUI_SKELETON_TRACKED) {

					   for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j) {
						   if (skeletonData.eSkeletonPositionTrackingState[j] != NUI_SKELETON_POSITION_NOT_TRACKED) {
							   //setJoint(depthImage, j, skeletonData.SkeletonPositions[j]);
							   glPushMatrix();

							   glTranslatef(skeletonData.SkeletonPositions[j].x, skeletonData.SkeletonPositions[j].y, -skeletonData.SkeletonPositions[j].z);
							   glScalef(0.01, 0.01, 0.01);
							   glutSolidSphere(1.0, 30, 30);
								
							   glPopMatrix();

							   glPushMatrix();
							   glTranslatef(0.0, -0.5, 0.0);
							   glRotatef(-90.0f, 1.0, 0.0, 0.0);
							   glDisable(GL_LIGHTING);
							   glColor3f(0.0, 1.0, 1.0);
							   renderGrid(0.3, 20);
							   glEnable(GL_LIGHTING);
							   glPopMatrix();
						   }
					   }
					   drawBone(skeletonData, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

					   drawBone(skeletonData, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

					   drawBone(skeletonData, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

					   drawBone(skeletonData, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

					   drawBone(skeletonData, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
					   drawBone(skeletonData, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);
				   }
			   }
		   }
	   }

   glPopMatrix();
      
   glutSwapBuffers();
   glDisable(GL_TEXTURE_2D);
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
	camara.ProcessMouseMovement(-xoffset, yoffset, true);
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
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
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
