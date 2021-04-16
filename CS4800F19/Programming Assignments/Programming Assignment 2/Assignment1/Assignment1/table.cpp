#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <list>	// list
#include <utility> // pair

using namespace std;

#define BUFSIZE 512

// constants to relate numbers to table pieces
const int GREEN = 1;
const int RED = 2;
const int BLUE = 3;
const int TEAL = 4;
const int BROWN = 5;
const int GREY = 6;
const int YELLOW = 7;
const int PINK = 8;
const int PURPLE = 9;
const int ORANGE = 10;

// window dimensions
const int WIDTH = 960;
const int HEIGHT = 480;

struct MousePoint
{
	int x, y;
}mouse;

struct OriginPoint
{
	double x, y, z;
}green, red, blue, teal, brown, 
grey, yellow, pink, purple, orange, objRot, objOrigin;	// table obj structs

double topWidth, topThick, legThick, legLen, angle, minRot, maxRot;
bool rotating = false, rotatingX = false, rotatingY = false, rotatingZ = false;
int name;	// object name
char keyPressed;

// Makes a checkerboard floor that's 20x20
void makeFloor(double thickness)
{
	double currX, currZ, yAdj, adj = .0014, xRght, zBttm;
	double baseFloorSize = 1.4;
	double numSquares = 10;
	double chkSize = 1.0 / (baseFloorSize * numSquares) - adj;	// -adj to make fit nicely
	double baseVal = 1.0 / baseFloorSize;	// base value for starting x and starting z positions

	// main floor
	glPushMatrix();	//push and pop to not mess with checkerboard pattern
		glColor3ub(47, 79, 79);	//dark slate grey
		glScaled(1, thickness, 1);
		glutSolidCube(baseFloorSize);
	glPopMatrix();

	yAdj = (3/2) * thickness;	// (3/2) to make slightly above, thick/2 since half above = thick + thick/2 = 3/2
	adj *= 10;	// small adjustment to make checkerboard fit better

	// checkerboard pattern
	// top row: (-x, -z) -> (x, -z) start from first position; (-z) -> (z)
	// second row: (-x, -z) -> (x, -z) start from second position; (-z) -> (z)
	// outer loop updates z row, inner loop goes across z row for each x position
	for (int i = 0; i < 20; i++)
	{
		// determine if odd or even row for starting x position
		if (i % 2 == 0)
		{
			// draw row that starts from first position
			currX = -baseVal;
		}
		else
		{
			// draw row that starts from second position
			currX = -baseVal + chkSize;
		}

		currZ = -baseVal + chkSize * i;	// how far to adjust z for each row
		zBttm = currZ + chkSize;	// z bottom position

		// from starting position, draw square every other spot
		glBegin(GL_QUADS);	// call here to avoid multiple calls
			glColor3ub(255, 255, 255);	// white
			for (currX; currX < (baseVal - chkSize); currX = currX + (2 * chkSize))
			{
				xRght = currX + chkSize;	// x right position

				glVertex3d(currX + adj, yAdj, zBttm + adj);	// btm left
				glVertex3d(xRght + adj, yAdj, zBttm + adj);	// btm right
				glVertex3d(xRght + adj, yAdj, currZ + adj);	// top right
				glVertex3d(currX + adj, yAdj, currZ + adj);	// top left
			}
		glEnd();
	}
}

// Creates one table leg of (thickness, length)
void tableLeg(double thick, double len)
{
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
}

// Creates a table top (width, thickness, length)
void tableTop(double topWidth, double topThick, double topLength)
{
	glScaled(topWidth, topThick, topLength);	//define size
	glutSolidCube(1.0);	//apply above to cube

}

// Rotates about a point
void rotateAboutPt()
{
	double xO, yO, zO;	// origin points
	bool top = false, leg = false;	// rotating which piece

	// start rotating from x then y then z
	if (rotating)
	{
		// set origin 
		xO = objOrigin.x;
		yO = objOrigin.y;
		zO = objOrigin.z;

		// determine LCS for x y z
		switch (name)
		{
			//IF NOT TOP, THEN LEG
			// tops: x=z, y=y, z=x
			case GREEN:
			case RED:
				// x = 1, y = 0, z = 0 == x = 0, y = 0, z = 1
				// x = 0, y = 1, z = 0 == x = 0, y = 1, z = 0
				// x = 0, y = 0, z = 1 == x = 1, y = 0, z = 0
				top = true;
				break;
				// legs: x=y, y=z, z=x
			case BLUE:
			case TEAL:
			case BROWN:
			case GREY:
			case YELLOW:
			case PINK:
			case PURPLE:
			case ORANGE:
				// x = 1, y = 0, z = 0 == x = 0, y = 0, z = 1
				// x = 0, y = 1, z = 0 == x = 1, y = 0, z = 0
				// x = 0, y = 0, z = 1 == x = 0, y = 1, z = 0
				leg = true;
				break;
			default:
				break;
		}

		glTranslated(xO, yO, zO);	// translate to object origin

		if (rotatingX)
		{
			// z=x for any piece
			glRotated(angle, 0, 0, 1);	// rotate about 'x' in LCS

			if (angle == maxRot)
			{
				rotatingX = false;
				rotatingY = true;
				angle = minRot;
			}
		}

		if (rotatingY)
		{
			if (top)
				glRotated(angle, 0, 1, 0);	// rotate about 'y' in LCS
			else
				glRotated(angle, 1, 0, 0);	// rotate about 'y' in LCS

			if (angle == maxRot)
			{
				rotatingY = false;
				rotatingZ = true;
				angle = minRot;
			}
		}

		if (rotatingZ)
		{
			if (top)
				glRotated(angle, 1, 0, 0);	// rotate about 'z' in LCS
			else
				glRotated(angle, 0, 1, 0);	// rotate about 'z' in LCS

			if (angle == maxRot)
			{
				rotatingZ = false;
				rotating = false;
				angle = minRot;
			}
		}

		angle += 5;
		glTranslated(-xO, -yO, -zO);	// translate back to origin
	}
}

// Creates the table top and table legs
void table(double topWidth, double topThick, double legThick, double legLen)
{
	double xPosAdj = .25, topsXAdj = .022;	//x-pos for upper table top and lower table top
	double yAdj = .52431;	// adjustment for y so table sits on floor
	double yLegAdj = legLen / 4; // change position of y so that origin is at top of leg (pos y direction), divide by 4 because half of half
	double yTopAdj = topThick / 2; // change position of y so that origin is at top of top (pos y direction), divide by 2 because half
	
	// push and pop table as a whole to treat as one object for rotating
	glPushMatrix();
		rotateAboutPt();
		//create upper table top
		glPushMatrix();
			glColor3ub(0, 255, 0);	//green
			green.x = -xPosAdj - topsXAdj;
			green.y = yAdj + yTopAdj;
			green.z = 0;
			glTranslated(green.x, green.y - yTopAdj, green.z);	//define position upperPos, legLen, 0
			glLoadName(GREEN);
			tableTop(topWidth / 1.5, topThick, topWidth * 1.5);	//width, thickness, length
		glPopMatrix();

		//create lower table top
		glPushMatrix();
			glColor3ub(255, 0, 0);	//red
			red.x = topWidth - xPosAdj + topsXAdj;
			red.y = yAdj + yTopAdj;
			red.z = 0;
			glTranslated(red.x, red.y - yTopAdj, red.z);	// lowerPos, legLen, 0
			glLoadName(RED);
			tableTop(topWidth * 1.5, topThick, topWidth / 1.5);	//width, thickness, length
		glPopMatrix();


		//create lower table legs

		//front-most lower table leg on lower table top
		glPushMatrix();
			glColor3ub(0, 0, 255);	 //blue
			blue.x = legLen * 1.8 - legThick / 2.0 - xPosAdj;
			blue.y = legThick - legLen + yAdj + yLegAdj;
			blue.z = legThick;
			glTranslated(blue.x, blue.y - yLegAdj, blue.z);
			glLoadName(BLUE);
			tableLeg(legThick, legLen / 2.0);
		glPopMatrix();

		//back-most lower table leg on lower table top
		glPushMatrix();
			glColor3ub(0, 128, 128);	 //teal
			teal.x = legLen * 1.8 - legThick / 2.0 - xPosAdj;
			teal.y = legThick - legLen + yAdj + yLegAdj;
			teal.z = -legThick;
			glTranslated(teal.x, teal.y - yLegAdj, teal.z);
			glLoadName(TEAL);
			tableLeg(legThick, legLen / 2.0);
		glPopMatrix();

		//front-most lower table leg on upper table top
		glPushMatrix();
			glColor3ub(160, 80, 40);	 //brown
			brown.x = -legLen / 2.0 + legThick * 1.1 - xPosAdj;
			brown.y = legThick - legLen + yAdj + yLegAdj;
			brown.z = legLen / 1.5;
			glTranslated(brown.x, brown.y - yLegAdj, brown.z);
			glLoadName(BROWN);
			tableLeg(legThick, legLen / 2);
		glPopMatrix();

		//back-most lower table leg on upper table top
		glPushMatrix();
			glColor3ub(128, 128, 128);	 //grey
			grey.x = -legLen / 2.0 + legThick * 1.1 - xPosAdj;
			grey.y = legThick - legLen + yAdj + yLegAdj;
			grey.z = -legLen / 1.5;
			glTranslated(grey.x, grey.y - yLegAdj, grey.z);
			glLoadName(GREY);
			tableLeg(legThick, legLen / 2.0);
		glPopMatrix();


		//create upper table legs

		//front-most upper table leg on lower table top
		glPushMatrix();
			glColor3ub(255, 255, 0);	 //yellow
			yellow.x = legLen * 1.8 - legThick / 2.0 - xPosAdj;
			yellow.y = -legThick * 1.75 + yAdj + yLegAdj;
			yellow.z = legThick;
			glTranslated(yellow.x, yellow.y - yLegAdj, yellow.z);
			glLoadName(YELLOW);
			tableLeg(legThick / 2, legLen / 2);
		glPopMatrix();

		//back-most upper table leg on lower table top
		glPushMatrix();
			glColor3ub(255, 0, 255);	 //pink
			pink.x = legLen * 1.8 - legThick / 2.0 - xPosAdj;
			pink.y = -legThick * 1.75 + yAdj + yLegAdj;
			pink.z = -legThick;
			glTranslated(pink.x, pink.y - yLegAdj, pink.z);
			glLoadName(PINK);
			tableLeg(legThick / 2.0, legLen / 2.0);
		glPopMatrix();

		//front-most upper table leg on upper table top
		glPushMatrix();
			glColor3ub(148, 0, 211);	 //purple
			purple.x = -legLen / 2.0 + legThick * 1.1 - xPosAdj;
			purple.y = -legThick * 1.75 + yAdj + yLegAdj;
			purple.z = legLen / 1.5;
			glTranslated(purple.x, purple.y - yLegAdj, purple.z);
			glLoadName(PURPLE);
			tableLeg(legThick / 2.0, legLen / 2.0);
		glPopMatrix();

		//back-most upper table leg on upper table top
		glPushMatrix();
			glColor3ub(255, 140, 0);	 //orange
			orange.x = -legLen / 2.0 + legThick * 1.1 - xPosAdj;
			orange.y = -legThick * 1.75 + yAdj + yLegAdj;
			orange.z = -legLen / 1.5;
			glTranslated(orange.x, orange.y - yLegAdj, orange.z);
			glLoadName(ORANGE);
			tableLeg(legThick / 2.0, legLen / 2.0);
		glPopMatrix();

		// teapot for fun
	glPushMatrix();
		glColor3ub(255, 255, 0);	//yellow
		glTranslated(.1, .12 + yAdj, 0);	// teapot center and on table
		glutSolidTeapot(.1);
	glPopMatrix();

	glPopMatrix();
}

// reads the table shape and joint files
void readFiles(double &topWidth, double &topThick, double &legThick, double &legLen, double &minRot, double &maxRot)
{
	std::ifstream inFile1("table.txt");
	std::ifstream inFile2("joint_file.txt");

	if (inFile1.is_open() && inFile2.is_open())
	{
		inFile1 >> topWidth >> topThick >> legThick >> legLen;
		inFile2 >> minRot >> maxRot;

		inFile1.close();
		inFile2.close();
	}
	else
	{
		std::cout << "Could not open one of the files." << std::endl;
	}

}

// draws the world coordinate axes to help with visualization
void drawAxes(void)
{
	// draw coordinate lines to help visualize
	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(1.0, 0.0, 0.0);

		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 1.0, 0.0);

		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 1.0);
	glEnd();
}

// draws a legend defining the shapes and numbers to pick
void drawLegend(void)
{
	std::cout << "0 = GREEN"
		"\n1 = RED"
		"\n2 = BLUE"
		"\n3 = TEAL"
		"\n4 = BROWN"
		"\n5 = GREY"
		"\n6 = YELLOW"
		"\n7 = PINK"
		"\n8 = PURPLE"
		"\n9 = ORANGE" << std::endl;
}

// draws the scene with a table and checkerboard floor
void drawTable()
{
	//set view 1
	glViewport(0, 0, WIDTH / 2, HEIGHT);	// left half
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 1.0, 1.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
	// Create table
	table(topWidth, topThick, legThick, legLen);

	//drawAxes();

	
	//set view 2
	glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);	// right half
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 1.0, -1.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
	// Create table
	table(topWidth, topThick, legThick, legLen);

	//drawAxes();
}

// draws the floor to both views
void drawFloor()
{
	//set view 1
	glViewport(0, 0, WIDTH / 2, HEIGHT);	// left half
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 1.0, 1.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
	// Creates the floor
	makeFloor(0.001);

	//set view 2
	glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);	// right half
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 1.0, -1.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
	// Creates the floor
	makeFloor(0.001);
}

// Displays the object created
void display(void)
{
	glClearColor(0.f, 0.f, 0.f, 1.f);	//set clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawFloor();
	drawTable();

	glFlush();
}

// Enables specific GL capabilities
void init()
{
	//glEnable(GL_LIGHTING);	//apply lighting techniques to see shape
	//glEnable(GL_LIGHT0);	//include light 0 in lighting equation
	glEnable(GL_NORMALIZE);		//normalize vectors
	glEnable(GL_DEPTH_TEST);	//enable and update depth buffer
	glEnable(GL_COLOR_MATERIAL);	//enable coloring material
}

// timer for rotation
void timer(int extra)
{
	glutPostRedisplay();
	glutTimerFunc(50, timer, 0);
}

// Use the selection buffer to determine which object selected
void processPick(int hits, int name)
{
	std::cout << mouse.x << ", " << mouse.y << " hits: " << hits << ", ";

	// determine pick that relates to which of the 10 table pieces
	switch (name)
	{
		// set origin point
		case GREEN:
			objOrigin = green;
			std::cout << "GREEN" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case RED:
			objOrigin = red;
			std::cout << "RED" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case BLUE:
			objOrigin = blue;
			std::cout << "BLUE" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case TEAL:
			objOrigin = teal;
			std::cout << "TEAL" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case BROWN:
			objOrigin = brown;
			std::cout << "BROWN" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case GREY:
			objOrigin = grey;
			std::cout << "GREY" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case YELLOW:
			objOrigin = yellow;
			std::cout << "YELLOW" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case PINK:
			objOrigin = pink;
			std::cout << "PINK" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case PURPLE:
			objOrigin = purple;
			std::cout << "PURPLE" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		case ORANGE:
			objOrigin = orange;
			std::cout << "ORANGE" << std::endl;
			rotating = true;
			rotatingX = true;
			break;
		default:
			std::cout << "Miss" << std::endl;
			break;
	}
}

// compare function for list to compare first val in pair
bool compFirst(pair<float, int> one, pair<float, int> two)
{
	return (one.first < two.first);
}

// Processes the hits to determine which object was hit first
// Returns the nearest object name
int processHits(int hits, unsigned int buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr;
	GLint name;
	GLfloat z1;
	pair <float, int> currPair;	// pair with z1 and name
	list<pair<float, int>> nearName;
	
	if (hits > 0)
	{
		ptr = (GLuint *)buffer;
		for (i = 0; i < (unsigned int)hits; i++) {  /* for each hit  */
			names = *ptr;
			//printf(" number of names for hit = %d\n", names);
			ptr++;
			z1 = (float)*ptr / 0x7fffffff;
			//printf("  z1 is %g;", z1);
			ptr++;
			//printf(" z2 is %g\n", (float)*ptr / 0x7fffffff);
			ptr++;
			//printf("   the name is ");
			for (j = 0; j < names; j++) {  /* for each name */
				name = *ptr;
				//printf("%d ", name);
				ptr++;
			}

			currPair = make_pair(z1, name);	// make pair
			nearName.push_back(currPair);	// keep list of all paired hits
			//printf("\n\n");
		}

		// sort list by nearest objects and return smallest's name
		nearName.sort(compFirst);
		return nearName.front().second;
	}

	return -1;

	//rotating = true;
	//rotatingX = true;
}

// Determines xy position of mouse on window screen
void pickObj(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];	// holds select buffer
	GLint hits;	// holds hits
	GLint viewport1[] = { 0, 0, WIDTH / 2, HEIGHT }, viewport2[] = { WIDTH / 2, 0, WIDTH / 2, HEIGHT };	// cheat viewport since it is known already

	// if currently rotating, don't rotate again until done
	if (rotating)
	{
		return;
	}

	// if left mouse button is clicked, begin rotating
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
	{
		return;
	}

	mouse.x = x;
	mouse.y = HEIGHT - y;	// since windows y is opposite from opengl y, sub y from window height

	// populate viewport info
	//glGetIntegerv(GL_VIEWPORT, viewport);
	
	// populate select buffer info
	glSelectBuffer(BUFSIZE, selectBuf);

	// change to select mode
	glRenderMode(GL_SELECT);

	// initialize name list
	glInitNames();
	glPushName(-1);

	// recreate view process for displaying table
	glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// create 1x1 pixel picking region near cursor location
		gluPickMatrix(mouse.x, mouse.y, 1.0, 1.0, viewport1);
		glOrtho(-1, 1, -1, 1, .1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(1.0, 1.0, 1.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
		// Create table
		table(topWidth, topThick, legThick, legLen);	// draw table in select mode
	glPopMatrix();

	glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// create 1x1 pixel picking region near cursor location
		gluPickMatrix(mouse.x, mouse.y, 1.0, 1.0, viewport2);
		glOrtho(-1, 1, -1, 1, .1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(1.0, 1.0, -1.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
		// Create table
		table(topWidth, topThick, legThick, legLen);	// draw table in select mode
	glPopMatrix();

	glFlush();

	hits = glRenderMode(GL_RENDER);	// get number of hits and return to render mode
	name = processHits(hits, selectBuf);;	// get only closest object hit name
	processPick(hits, name);

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	readFiles(topWidth, topThick, legThick, legLen, minRot, maxRot);
	//drawLegend();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);	//set initial display mode
	glutInitWindowSize(WIDTH, HEIGHT);	//specify window size
	glutInitWindowPosition(200, 100);	//specify window location
	glutCreateWindow("Table and Checkerboard Floor");	//give window title

	init();

	glutMouseFunc(pickObj);	//sets the mouse function
	//glutKeyboardFunc(keyPick);
	glutTimerFunc(0, timer, 0);	//sets the rotation timer to update the image
	glutDisplayFunc(display);	//set display callback for current window

	glutMainLoop();

	return 0;
}