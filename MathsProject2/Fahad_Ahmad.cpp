/*********
CTIS164 - Template Source Program
----------
STUDENT : Fahad Ahmad
SECTION : 164-2
HOMEWORK: 1
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/

#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  10   // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, go = false, fire = false, change = false, hit = false, more=false, less=false, begin = true;
int  winWidth, winHeight; // current Window width and height
int Cx = -500, Cy = 0; // The refernce x, y values
int points, total=0; // for calculating points at collision
int countt = 0, countb = 0;
typedef struct{
	int X, Y;
}coordinates;

coordinates plane[50]; //50 planes have simply been created
coordinates ball;
int times=19, timems=99 ; //for the time counter 
int z=0;
bool ctimer = false; //indicator of active timer
int val = 0;
int counter = 0, minutes = 0;

void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void displayBackground() //the background is displayed
{
	glColor3f(0.2, 0.2, 0.2);
	glRectf(500, -400, 380, 400);
	glRectf(500, -400, -500, -270);
	glColor3f(244, 66, 89);
	if (minutes<=20)
	{
		times = 19 - minutes;
		timems = 99 - counter;
		
		glColor3ub(244, 66, 89);
		vprint(400, 250, GLUT_BITMAP_8_BY_13, "REMAINING");
		vprint(400, 225, GLUT_BITMAP_8_BY_13, "  TIME");	
		

		vprint(400, 150, GLUT_BITMAP_8_BY_13, "  TOTAL");
		vprint(400, 125, GLUT_BITMAP_8_BY_13, " ROCKETS");
	

		vprint(400, 50, GLUT_BITMAP_8_BY_13, "  BURST");
		vprint(400, 25, GLUT_BITMAP_8_BY_13, " ROCKETS");
		
		vprint(400, -70, GLUT_BITMAP_8_BY_13, "  LAST ");
		vprint(400, -95, GLUT_BITMAP_8_BY_13, " POINTS");
		

		vprint(400, -200, GLUT_BITMAP_8_BY_13, "  TOTAL ");
		vprint(400, -225, GLUT_BITMAP_8_BY_13, " POINTS");

		vprint(-240, -285, GLUT_BITMAP_8_BY_13, "      <Spacebar>                  <F1>");
		glColor3f(1, 1, 1);
		vprint(400, -250, GLUT_BITMAP_8_BY_13, "    %d", total);
		vprint(400, -120, GLUT_BITMAP_8_BY_13, "    %d", points);
		vprint(400, 0, GLUT_BITMAP_8_BY_13, "    %d", countb);
		vprint(400, 100, GLUT_BITMAP_8_BY_13, "    %d", countt);
		if(begin==false)
			vprint(400, 200, GLUT_BITMAP_8_BY_13, "  00.00");
		else
		vprint(400, 200, GLUT_BITMAP_8_BY_13, "  %02d.%02d", times, timems);

		vprint(-240, -285, GLUT_BITMAP_8_BY_13, "Fire               Pause/Restart   ");



	}
}

void displayplane(int x, int y) //function for displaying the planes that act as targets
{
	glColor3ub(242, 212, 106);
	
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 13, y + 100);
	glVertex2f(x , y + 80);
	glVertex2f(x + 26, y + 80);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(x + 13, y + 20);
	glVertex2f(x - 10, y - 20);
	glVertex2f(x + 26 + 10, y - 20);
	glEnd();

	glColor3ub(50, 132, 168);
	glRectf(x, y, x + 26, y + 80);
	glBegin(GL_QUADS);
	glVertex2f(x, y + 65);
	glVertex2f(x , y + 40);
	glVertex2f(x - 30, y + 15);
	glVertex2f(x - 30, y + 30);
	glEnd();


	glBegin(GL_QUADS);
	glVertex2f(x+26, y + 65);
	glVertex2f(x+26, y + 40);
	glVertex2f(x+26 + 30, y + 15);
	glVertex2f(x+26 + 30, y + 30);
	glEnd();

	
}

void displayball() //the shooting object
{
	glColor3ub(86, 85, 84);
	circle(ball.X+60 , ball.Y, 20);
}
void displaycannon(int X, int Y) //the weapon that shoots
{
	
	glColor3ub(206, 147, 76);
	circle(X + 25, Y , 25); //cannon body
	glRectf(X + 25, Y - 25, X + 80, Y + 25); //cannon body
	
	glColor3ub(63, 63, 63);
	glRectf(X + 80, Y - 30, X + 85, Y + 30); //cannon body
	glRectf(X + 30, Y - 25, X + 60, Y - 40); //cannon stand
	glRectf(X + 20, Y - 35, X + 70, Y - 40); //cannon stand
}

void displayscreen() // for displaying the screen colour
{
	glBegin(GL_QUADS);
	glColor3ub(244, 66, 113);
	glVertex2f( -500, -270);
	glVertex2f(380, -270);

	glColor3ub(255, 119, 155);
	glVertex2f(380, 300);

	glVertex2f(-500, 300);
	
	glEnd();
}
void display() 
{
	glClearColor(0, 0.4, 0.6, 0.7);
	glClear(GL_COLOR_BUFFER_BIT);
	displayscreen();
	displayball();
	displaycannon(Cx, Cy);
	if (up == true && Cy != 270 && ctimer)
	{
		Cy += 5;

		if (change == false)
			ball.Y += 5;
	}
	else if (down == true  && Cy != -230 && ctimer)
	{
		Cy -= 5;
		if (change == false)
			ball.Y -= 5;
	}
	
	for(z=0;z<49;z++)
		displayplane(plane[z].X, plane[z].Y);
	displayBackground();
	glColor3f(1, 1, 1);

	

	
	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void initializeall() //initializes important variables
{
	plane[0].Y = -400;
	for (int i = 1; i < 49; i++)
	{
		plane[i].Y = plane[i - 1].Y - 150;
		plane[i].X = (rand() % 600) - 300;
	}
	ball.X = -500;
	ball.Y = 0;
	minutes = 0;
	Cx = -500, Cy = 0;
	displaycannon(Cx, Cy);
	points = 0;
	total = 0;
	countt = 0;
	countb = 0;
}
void onKeyDown(unsigned char key, int x, int y)
{

	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == 32)
		if (go == true)
		{
			fire = true;
			change = true;
		}
		
		
	// to refresh the window it calls display() function
	glutPostRedisplay();

	
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//


void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	case GLUT_KEY_F1: 
		go = !go;
		ctimer = !ctimer;
		if (begin == false)
		{
			begin = true;
			initializeall();
		}
		break;


		// to refresh the window it calls display() function
		glutPostRedisplay();
	}
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (ctimer)
	{
		counter++;
		if (counter % 100 == 0)
		{
			minutes++;
			counter = 0;

			if (minutes == 20) //once 20 seconds finish
			{
				begin = false;
				ctimer = false;
				go = false;
				fire = false;
				change = false;
			}
		}
		
	}
	
	if (begin==true)
	{
		if (fire == true && go == true && ball.X < 300)
		{
			ball.X += 8;

		}
		if (go == true)
		{
			for (int k = 0; k < 49; k++)
			{
				if (plane[k].Y == -250)
					countt++;
				plane[k].Y += 3;
				if ((ball.X+20 >= plane[k].X && ball.X+20 <=plane[k].X+26 && ball.Y >= plane[k].Y+20 && ball.Y <= plane[k].Y+60 && fire==true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y -18  >= plane[k].Y + 20 || ball.Y + 20 >= plane[k].Y+20) && (ball.Y-18 <= plane[k].Y + 60 || ball.Y + 20 <= plane[k].Y + 60) && fire == true))

				{
					points = 5;
					total += points;
					plane[k].X += 2000; //for disappearance, we are simply increasing the x value so that the plane disappears from the screen
					countb++;
				}
				if ((ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y >= plane[k].Y - 20 && ball.Y < plane[k].Y -10 && fire == true) || (ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y > plane[k].Y + 90 && ball.Y <= plane[k].Y + 100 && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y - 18  > plane[k].Y + 90 || ball.Y + 20  > plane[k].Y + 90) && (ball.Y - 18 <= plane[k].Y + 100 || ball.Y + 20 <= plane[k].Y + 100) && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y - 18  >= plane[k].Y -20 || ball.Y + 20  >= plane[k].Y -20) && (ball.Y - 18 < plane[k].Y - 10 || ball.Y + 20 < plane[k].Y -10) && fire == true))
				{
					points = 1;
					total += points;
					plane[k].X += 2000;
					countb++;
				}
				if ((ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y >= plane[k].Y - 10 && ball.Y < plane[k].Y && fire == true) || (ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y > plane[k].Y + 80 && ball.Y <= plane[k].Y + 90 && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y -18 > plane[k].Y + 80 || ball.Y - 18 > plane[k].Y + 80) && (ball.Y -18 <= plane[k].Y + 90 || ball.Y + 20 <= plane[k].Y + 90) && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y - 18 >= plane[k].Y -10 || ball.Y + 20 >= plane[k].Y -10) && (ball.Y - 18 < plane[k].Y || ball.Y + 20 <= plane[k].Y) && fire == true))
				{
					points = 2;
					total += points;
					plane[k].X += 2000;
					countb++;
				}
				if ((ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y >= plane[k].Y && ball.Y < plane[k].Y + 10 && fire == true) || (ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y > plane[k].Y + 70 && ball.Y <= plane[k].Y + 80 && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y - 18 > plane[k].Y + 70 || ball.Y + 20 > plane[k].Y + 70) && (ball.Y - 18 <= plane[k].Y + 80 || ball.Y + 20 <= plane[k].Y + 80) && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y - 18 >= plane[k].Y || ball.Y + 20 >= plane[k].Y) && (ball.Y - 18 < plane[k].Y + 10 || ball.Y + 20 < plane[k].Y + 10) && fire == true))
				{
					points = 3;
					total += points;
					plane[k].X += 2000;
					countb++;
				}
				if ((ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y >= plane[k].Y + 10 && ball.Y < plane[k].Y + 20 && fire == true) || (ball.X + 20 >= plane[k].X && ball.X + 20 <= plane[k].X + 26 && ball.Y > plane[k].Y + 60 && ball.Y <= plane[k].Y + 70 && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y-18 > plane[k].Y + 60 || ball.Y + 20) > plane[k].Y + 60 && (ball.Y -18  <= plane[k].Y + 70 || ball.Y + 20 <= plane[k].Y + 70) && fire == true) || (ball.X >= plane[k].X && ball.X <= plane[k].X + 26 && (ball.Y - 18 >= plane[k].Y + 10 || ball.Y + 20 >= plane[k].Y + 10) && (ball.Y - 18 < plane[k].Y + 20 || ball.Y + 20 < plane[k].Y + 20) && fire == true))
				{
					points = 4;
					total += points;
					plane[k].X += 2000;
					countb++;
				}
					
			}
		}
		
		if (ball.X == 300)
		{
			ball.X = -500;
			fire = false;
			change = false;
			ball.Y = Cy;
		}
	}
	
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
	

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("\"Cannon shoot!\" by Fahad Ahmad");
	
	initializeall();
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);


	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);
	srand(time(NULL));

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}

