/*
 *Matsutaka Masuda
 *CPSC-444
 *Dr. Polack
 *6/16/2014
 *Final Project
 *Mini Golf Scene
*/

/*
PROGRAM CONTROLS AND INFORMATION
-L/R: ROTATE THE CAMERA VIEW AROUND THE SCENE
-Z: PAUSE THE CAMERA VIEW
-RIGHT CLICK FOR USER MENU WITH 6 OPTIONS
-SPIN THE WINDMILL OR ROTATE THE TEAPOT
-CREATE THE GOLF BALL AND HIT A HOLE-IN-ONE!
-THE FLAG WILL DISAPPEAR WHEN SHOOTING 
-ESC: QUIT
*/

#ifndef FILE_STUFF
#define FILE_STUFF
#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <ctime>
using namespace std;
struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;
#endif
#include <gl/glut.h>

void glut_key_action(unsigned char key, int x, int y);//keyboard func
void drawCircle(float x, float y, float radius); //draw hole
void drawGluCylinder( double height, double radius, int slices, int stacks ); //for flag post |
void drawGluSlantCylinder( double height, double radiusBase, double radiusTop, int slices, int stacks );
void drawGluCylinderWithCaps( double height, double radius, int slices, int stacks);
void drawGluSlantCylinderWithCaps( double height, double radiusBase, double radiusTop, int slices, int stacks );
void spinDisplay (void); //spinning windmill
void menuProcess(int num); //evaluate menu
void createMenu(void); //generate menu
void timer(int id); //control animation

//global values
bool solid=true, windmillOn = false, ballOn = false, shootOn = false, teapotOn = false;
bool translating = false, tAnimating = false;
const int milliseconds_per_frame = 10;
float angle=0;
float spin = 0;
float potSpin = 0;
float potR = 0, potG = 0, potB = 1;
float tx = 5.5;
int menu_id;

//for two spheres
GLUquadricObj *q;
GLUquadricObj *ball;

//light stuph
GLfloat light_color0[]={0.3f,0.3f,0.3f,1.0f};
GLfloat light_position0[]={0.0f, 0.0f, 13.0f, 1.0f};



void init(void) 
{
	q =  gluNewQuadric();//set pointers for sphere use
	ball = gluNewQuadric();
	glClearColor (0.0, 0.0, 0.0, 0.0);//clear
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);

	//lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_color0);
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light_color0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glEnable(GL_COLOR_MATERIAL);

}

void display(void)
{
	if(tAnimating)//set speed for animation timer
		glutTimerFunc(milliseconds_per_frame, timer, 1);

	glRotatef(angle,0,1,0);

	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//plant from Lab 10 on textures
	glColor3f(0.133333, 0.545098, 0.133333);
	glPushMatrix();
	glTranslatef(1.25, -1.6, 2);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.05, 3.0, 3, 3);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.25, -1.6, 2);
	glRotatef(-90.0, 5.0, 1.0, 0.0);
	glutSolidCone(0.05, 2.8, 3, 3);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.25, -1.6, 2);
	glRotatef(220.0, 0.0, 1.0, 2.0);
	glutSolidCone(0.05, 2.8, 3, 3);
	glPopMatrix();

	//draw concrete ground
	glPushMatrix();	
		glTranslatef(0, 2,-4);
		glRotatef(-90, 1, 0, 0);
		glBegin(GL_POLYGON);
			glColor3f( 0.439216, 0.501961, 0.564706);
			glVertex3f(-20, -20,-4.0 );
			glVertex3f(20, -20.0, -4.0);
			glVertex3f(20, 20, -4.0);
			glVertex3f(-20, 20, -4.0);
		glEnd();
	glPopMatrix();


	//green course
	glPushMatrix();	
		glTranslatef(0, 2.4,-4);
		glRotatef(-90, 1, 0, 0);
		glBegin(GL_POLYGON);
			glColor3f(0,1,0);
			glVertex3f(-1.5f, -10.0,-4.0 );
			glVertex3f(1.5f, -10.0, -4.0);
			glVertex3f(1.5, 4, -4.0);
			glVertex3f(-1.5, 4, -4.0);
		glEnd();
	glPopMatrix();

	//Brick Siding
	glColor3f(1, 0, 0);
	glPushMatrix();
		glTranslatef(-1.7, -1.5, -1);
		glRotatef(90, 1, 0, 0);
		glScalef(1.5, 55, 1);
		glutSolidCube(.25);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.7, -1.5,-1);
		glRotatef(90, 1, 0, 0);
		glScalef(1.5, 55, 1);
		glutSolidCube(.25);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, -1.5, -8);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		glScalef(1.5, 15.15, 1);
		glutSolidCube(.25);
	glPopMatrix();

	//draw hole
	glPushMatrix();
		glTranslatef(-.75, -1.5, -6.25);
		glRotatef(90, 1, 0, 0);
		drawCircle(.8,.8, .2);
	glPopMatrix();
	
	//draw flag and post while not shooting and remove when shooting
	glPushMatrix();
		if(!(shootOn))
		{
			glTranslatef(0, 1l, -5.4);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			drawGluCylinder( 2.5, .06, 15, 15);
		}
	glPopMatrix();
	glPushMatrix();
		if(!(shootOn))
		{
			glColor3f(1, 0, 0);
			glTranslatef(.3, .88, -5.35);
			glRotatef(90, 0, 1, 0);
			glRotatef(90, 0, 1, 0);
			glRotatef(90, 1, 0, 0);
			glScalef(2.5, .25, 1);
			glutSolidCube(.25);
		}
	glPopMatrix();

	//draw stand and teapot on top of the stand
	glColor3f(0.545098, 0.270588, 0.0745098);
	glPushMatrix();
		glTranslatef(-2.5, -1.5, 1);
		glScalef(1, .25, 1);
		glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2.5, -1.3, 1);
		glScalef(1, .25, 1);
		glutSolidCube(.75);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2.5, -1.1, 1);
		glScalef(1, .25, 1);
		glutSolidCube(.75);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2.5, -.9, 1);
		glScalef(1, .25, 1);
		glutSolidCube(.75);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2.5, -.7, 1);
		glScalef(1, .25, 1);
		glutSolidCube(.75);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2.5, -.5, 1);
		glScalef(1, .25, 1);
		glutSolidCube(.75);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2.5, -.3, 1);
		glScalef(1, .25, 1);
		glutSolidCube(.75);
	glPopMatrix();
	glPushMatrix();
	if(teapotOn)
		{
			glTranslatef(-2.5, -.07, 1);
			glRotatef(potSpin, 0, 1, 0);
			glTranslatef(2.5, .07, -1);
		}
	glPushMatrix();
		glColor3f(potR, potG, potB);
		glTranslatef(-2.5, -.07, 1);
		glutSolidTeapot(.2);
	glPopMatrix();
	glPopMatrix();

	//draw Windmill
	glPushMatrix();
		glColor3f( 0.823529, 0.705882, 0.54902);
		glTranslatef(0, -1.5, 0);
		glRotatef(45, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(.5, 2, 4, 4);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, .5, .1);
		glScalef(.1, .1, .1);
		gluSphere(q, 1, 10, 10);
	glPopMatrix();
	//rotate when selected from the menu
	glPushMatrix();
		if(windmillOn)
		{
			glTranslatef(0, .5, .1);
			glRotatef(spin, 0, 0, 1);
			glTranslatef(0, -.5, -.1);
		}
	glPushMatrix();
		glColor3f(1,1,1);
		glTranslatef(-.875, .5, .2);
		glRotatef(90, 0, 1, 0);
		glScalef(.15, 1, 1);
		glutSolidCone(.25, .9, 4, 4);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1,1,1);
		glTranslatef(.875, .5, .2);
		glRotatef(270, 0, 1, 0);
		glScalef(.15, 1, 1);
		glutSolidCone(.25, .9, 4, 4);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1,1,1);
		glTranslatef(0, -.4, .2);
		glRotatef(90, 0, 0, 1);
		glRotatef(90, 0, 1, 0);
		glScalef(.15, 1, 1);
		glutSolidCone(.25, .9, 4, 4);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1,1,1);
		glTranslatef(0, 1.35, .2);
		glRotatef(270, 0, 0, 1);
		glRotatef(90, 0, 1, 0);
		glScalef(.15, 1, 1);
		glutSolidCone(.25, .9, 4, 4);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, -1.2, 3.75);
		glRotatef(10, 1, 0, 0);
		glBegin(GL_POLYGON);
			glColor3f(0, 0, 0);
			glVertex3f(-.15, -1, -4);
			glVertex3f(.15, -1, -4);
			glVertex3f(.15, -.6, -4);
			glVertex3f(-.15, -.6, -4);
		glEnd();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, .17, 4.13);
		glRotatef(-10, 1, 0, 0);
		glBegin(GL_POLYGON);
			glColor3f(0, 0, 0);
			glVertex3f(-.15, -1, -4);
			glVertex3f(.15, -1, -4);
			glVertex3f(.15, -.6, -4);
			glVertex3f(-.15, -.6, -4);
		glEnd();
	glPopMatrix();

	//draw ball
	glPushMatrix();
		if(shootOn)
		{
			glTranslatef(0, 0, tx);
		}
		glPushMatrix();
			if(ballOn)
			{
				glColor3f(1, 1, 1);
				glTranslatef(0, -1.5, tx);
				gluSphere(ball, .05, 10, 10);
			}	
		glPopMatrix();
	glPopMatrix();

	//animate
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);   
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 5.0,  0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void keyboard (unsigned char key, int x, int y)
{   
	switch (key) 
	{    
	case 27: //escape / quit program       
		exit(0);        
		break;      
	default:         
		break;   
	}
}

int main(int argc, char** argv)
{
	srand (static_cast <unsigned> (time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow ("Underwater View"); 
	glutFullScreen();
	createMenu();
	init ();   
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutIdleFunc(spinDisplay);
	glutKeyboardFunc( glut_key_action );   
	glutMainLoop();
	return 0;
}
void glut_key_action(unsigned char key, int x, int y)

{
	switch (key)
	{
	case 'q':
	case 'Q':
		exit(0);
	case 's':
	case 'S':
		angle=0;
		solid=true;
		break;
	case 'w':
	case 'W':
		angle=0;
		solid=false;
		break;
	case 'r':
		angle+=.025;
		break;
	case 'l':
		angle-=.025;
		break;
	case'z':
		angle=0;
		break;
	case 27: //escape / quit program       
		exit(0);        
		break; 

	default:
		break;
	}
	glutPostRedisplay();
}
//Code for Circle found from github.com; Username: strife25
void drawCircle(float x, float y, float radius)
{
			int i;
			int triangleAmount = 20; //# of triangles used to draw circle
			

		    //radius
			float twicePi = 2.0f * M_PI;
			glColor3f(1,1,1);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(x, y); // center of circle
			for(i = 0; i <= triangleAmount;i++) 
			{ 
				glVertex2f(
					x + (radius * cos(i *  twicePi / triangleAmount)), 
					y + (radius * sin(i * twicePi / triangleAmount)));			
				
			}
			glEnd();

}

//The next four functions for the cylinder found on website http://www.math.ucsd.edu/~sbuss/MathCG/OpenGLsoft/GluCylinders/GluCylinders.c
GLUquadricObj* myReusableQuadric = 0;

void drawGluCylinder( double height, double radius, int slices, int stacks ) {
	drawGluSlantCylinder( height, radius, radius, slices, stacks );
}

void drawGluSlantCylinder( double height, double radiusBase, double radiusTop, int slices, int stacks )
{
	if ( ! myReusableQuadric ) {
		myReusableQuadric = gluNewQuadric();  
		// Should (but don't) check if pointer is still null --- to catch memory allocation errors.
		gluQuadricNormals( myReusableQuadric, GL_TRUE );
	}
	// Draw the cylinder.
	gluCylinder( myReusableQuadric, radiusBase, radiusTop, height, slices, stacks );
}


void drawGluCylinderWithCaps( double height, double radius, int slices, int stacks ) {
	drawGluSlantCylinderWithCaps( height, radius, radius, slices, stacks );
}

void drawGluSlantCylinderWithCaps( double height, double radiusBase, double radiusTop, int slices, int stacks )
{
	// First draw the cylinder
	drawGluSlantCylinder( height, radiusBase, radiusTop, slices, stacks );
}

void spinDisplay (void)
{
	bool spinIt = true, spinTea = true;
	if(spinIt)
	{	
		spin +=.1;
		
	}
	if(spinTea)
	{
		potSpin +=.1;

	}

	glutPostRedisplay();
}

void menuProcess(int num){
	
	if (num == 0){ //windmill start
		windmillOn = true;
	}
	else if(num==1){//windmill stop
		windmillOn = false;
	}
	else if(num==2){//change teapot color
		potR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		potG = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		potB = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glutPostRedisplay();
	}
	else if(num==3){ //teapot spin
		teapotOn = true;
	}
	else if(num==4){ //create ball
		ballOn = true;
	}
	else if(num==5){ //shoot ball
		shootOn = true;
		tAnimating = true;
		translating = true;
	}
	else //exit
	{
		exit(0);
	}

	//call glut redisplay
	glutPostRedisplay();

}

void createMenu(void){     

	// menu_id equal Create a glut Menu and call menuProcess as the callback
	menu_id = glutCreateMenu(menuProcess);
	
	//add menu entry to Start the Rotation and give it the value 0
	glutAddMenuEntry("Start Windmill", 0);
	glutAddMenuEntry("Stop Windmill", 1);
	glutAddMenuEntry("Change Teapot Color", 2);
	glutAddMenuEntry("Make the Teapot Spin", 3);
	glutAddMenuEntry("Create Mini Golf Ball", 4);
	glutAddMenuEntry("Shoot Mini Golf Ball", 5);
	glutAddMenuEntry("Quit", 6);
	//attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

} 
//control speeds and and the ranges of each animation
void timer(int id)
{	
	if(translating)
	{
		if(tx>=-2.75)
		{
			tx-=.0075;
		}
		else if(tx<=-2.75)
		{
			shootOn = false;
			tAnimating = false;
			translating = false;
			ballOn = false;
			tx = 5.5;
		}

	}
	
	glutPostRedisplay();

}