//#include<GL/glut.h>
//#include<fstream>
//#include<iostream>
//
//#define BUFSIZE 50
//
//struct Point
//{
//	int x, y;
//}mouse;
//
//float topWidth, topThick, legThick, legLen;
//
//void makeFloor(float thickness)
//{
//	float size = 1.4f;
//	float sizeScaled = 1.f / 1.4f;
//	glPushMatrix();
//		glScaled(1, thickness, 1);
//		glutSolidCube(size);
//	glPopMatrix();
//
//	glBegin(GL_QUADS);
//		glColor3f(255, 0, 255);
//		glVertex3f(sizeScaled, thickness, -sizeScaled);
//		glVertex3f(0, thickness, -sizeScaled);
//		glVertex3f(0, thickness, 0);
//		glVertex3f(sizeScaled, thickness, 0);
//	//glEnd();
//
//	//glBegin(GL_QUADS);
//		//glColor3f(255, 0, 255);
//		glVertex3f(0, thickness, 0);
//		glVertex3f(-sizeScaled, thickness, 0);
//		glVertex3f(-sizeScaled, thickness, sizeScaled);
//		glVertex3f(0, thickness, sizeScaled);
//	glEnd();
//}
//
//// Draws one table leg
//void tableLeg(float thick, float len)
//{
//	//glPushMatrix();
//		glScaled(thick, len, thick);
//		glutSolidCube(1.0);
//	//glPopMatrix();
//}
//
//void tableTop(float topWidth, float topThick, float topLength)
//{
//	//glPushMatrix();
//		glScaled(topWidth, topThick, topLength);	//define size
//		glutSolidCube(1.0);	//apply above to cube
//	//glPopMatrix();
//
//}
//
//// Creates the table top and table legs
//void table(float topWidth, float topThick, float legThick, float legLen)
//{
//		float upperPos = -.25;	//x-pos of upper table top
//		//create upper table top
//		glPushMatrix();
//		glLoadName(1);
//			glColor3ub(0, 255, 0);	//green
//			glTranslated(-.25, .26, 0);	//define position upperPos, legLen, 0
//			//glLoadName(1);
//			tableTop(topWidth / 1.5f, topThick, topWidth * 1.5f);	//width, thickness, length
//		glPopMatrix();
//
//		float lowerPos = .25;	//x-pos of lower table top
//		//create lower table top
//		glPushMatrix();
//		glLoadName(2);
//			glColor3ub(255, 0, 0);	//red
//			glTranslated(topWidth - .25f, .26, 0);	// lowerPos, legLen, 0
//			//glLoadName(2);
//			tableTop(topWidth * 1.5f, topThick, topWidth / 1.5f);	//width, thickness, length
//		glPopMatrix();
//
//		//create lower table legs
//		glPushMatrix();
//			//front-most lower table leg on lower table top
//			glLoadName(3);
//			glColor3ub(0, 0, 255);	 //blue
//			glTranslated(topWidth*1.8f - legThick / 2.f - .25f, topThick - legLen + .26, legThick);
//			//glLoadName(3);
//			tableLeg(legThick, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//			//right-most lower table leg on lower table top
//			glLoadName(4);
//			glColor3ub(0, 128, 128);	 //teal
//			glTranslated(topWidth*1.8f - legThick / 2.f - .25f, topThick - legLen + .26, -legThick);
//			//glLoadName(4);
//			tableLeg(legThick, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//			//left-most lower table leg on upper table top
//			glLoadName(5);
//			glColor3ub(160, 80, 40);	 //brown
//			glTranslated(-topWidth / 2.f + legThick*1.1f - .25f, topThick - legLen + .26, topWidth / 1.5f);
//			//glLoadName(5);
//			tableLeg(legThick, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//			//back-most lower table leg on upper table top
//			glLoadName(6);
//			glColor3ub(128, 128, 128);	 //grey
//			glTranslated(-topWidth / 2.f + legThick*1.1f - .25f, topThick - legLen + .26, -topWidth / 1.5f);
//			//glLoadName(6);
//			tableLeg(legThick, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//		//create upper table legs
//			//front-most upper table leg on lower table top
//			glLoadName(7);
//			glColor3ub(255, 255, 0);	 //yellow
//			glTranslated(topWidth*1.8f - legThick / 2.f - .25f, -topThick*1.75f + .26, legThick);
//			//glLoadName(7);
//			tableLeg(legThick / 2.f, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//			//right-most upper table leg on lower table top
//			glLoadName(8);
//			glColor3ub(255, 0, 255);	 //pink
//			glTranslated(topWidth*1.8 - legThick / 2 - .25, -topThick*1.75 + .26, -legThick);
//			//glLoadName(8);
//			tableLeg(legThick / 2.f, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//			//left-most upper table leg on upper table top
//			glLoadName(9);
//			glColor3ub(148, 0, 211);	 //purple
//			glTranslated(-topWidth / 2 + legThick*1.1 - .25, -topThick*1.75 + .26, topWidth / 1.5);
//			//glLoadName(9);
//			tableLeg(legThick / 2.f, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//			//back-most upper table leg on upper table top
//			glLoadName(10);
//			glColor3ub(255, 140, 0);	 //orange
//			glTranslated(-topWidth / 2 + legThick*1.1 - .25, -topThick*1.75 + .26, -topWidth / 1.5);
//			//glLoadName(10);
//			tableLeg(legThick / 2.f, legLen / 2.f);
//		glPopMatrix();
//
//		glPushMatrix();
//			glColor3ub(255, 255, 0);	//yellow
//			glTranslated(0, .40f, 0);
//			glutSolidTeapot(.1);
//		glPopMatrix();
//}
//
//void readFile(float &topWidth, float &topThick, float &legThick, float &legLen)
//{
//	std::ifstream inFile("table.txt");
//
//	if (inFile.is_open())
//	{
//		inFile >> topWidth >> topThick >> legThick >> legLen;
//	}
//	else
//	{
//		std::cout << "Could not open file." << std::endl;
//	}
//
//}
//
//// Displays the object created
//void displaySolid(void)
//{
//	glClearColor(0.f, 0.f, 0.f, 1.f);	//set clear color
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// Sets the view
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(-1, 1, -1, 1, .1, 100);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(1, 1, 1, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
//	//gluLookAt(25.0, 4.0, 2.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
//
//	glPushMatrix();
//		// Creates the floor
//		glColor3ub(255, 255, 255);	//white
//		glTranslated(0, -.3, 0);	//move floor down
//		makeFloor(0.05f);
//	glPopMatrix();
//
//	// Create table
//	//glPushMatrix();
//		table(topWidth, topThick, legThick, legLen);	//table dimensions: top width, top thickness, leg thickness, leg length
//	//glPopMatrix();
//
//
//	glFlush();
//}
//
//// Enables specific GL capabilities
//void init()
//{
//	glEnable(GL_LIGHTING);	//apply lighting techniques to see shape
//	glEnable(GL_LIGHT0);	//include light 0 in lighting equation
//	glEnable(GL_NORMALIZE);		//normalize vectors
//	glEnable(GL_DEPTH_TEST);	//enable and update depth buffer
//	glEnable(GL_COLOR_MATERIAL);	//enable coloring material
//}
//
//// Determines xy position of mouse on window screen
//void pickObj(int button, int state, int x, int y)
//{
//	GLuint selectBuf[BUFSIZE];	// holds select buffer
//	GLint hits;	// holds hits
//	GLint viewport[4];	// holds viewport info
//
//						// if left mouse button is clicked, begin rotating
//	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
//		return;
//
//	// populate viewport info
//	glGetIntegerv(GL_VIEWPORT, viewport);
//
//	// populate select buffer info
//	glSelectBuffer(BUFSIZE, selectBuf);
//
//	// change to select mode
//	glRenderMode(GL_SELECT);
//	
//	// initialize name list
//	glInitNames();
//	glPushName(-1);
//
//	//glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		// create 1x1 pixel picking region near cursor location
//		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0, 1.0, viewport);
//		glOrtho(-1, 1, -1, 1, .1, 100.0);
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//		gluLookAt(1, 1, 1, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);
//		table(topWidth, topThick, legThick, legLen);
//	glPopMatrix();
//	glFlush();
//
//	hits = glRenderMode(GL_RENDER);	// get number of hits and return to render mode
//									//processHits(hits, selectBuf);
//
//	mouse.x = x;
//	mouse.y = 480 - y;	// since windows y is opposite from opengl y, sub y from window size
//
//	std::cout << mouse.x << ", " << mouse.y << " hits: " << hits << std::endl;
//
//	//rotating = true;
//	//rotatingX = true;
//
//	glutPostRedisplay();
//}
//
//int main(int argc, char** argv)
//{
//	readFile(topWidth, topThick, legThick, legLen);
//
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);	//set initial display mode
//	glutInitWindowSize(640, 480);	//specify window size
//	glutInitWindowPosition(350, 100);	//specify window location
//	glutCreateWindow("Table and Checkerboard Floor");	//give window title
//
//	glutDisplayFunc(displaySolid);	//set display callback for current window
//	glutMouseFunc(pickObj);
//	init();
//
//	//glViewport(0, 0, 640, 480);	//defines where to render on screen
//	glutMainLoop();
//
//	return 0;
//}
