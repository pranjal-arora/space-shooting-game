// Keyboard control for PLAYER 1 :
// W - UP
// S - DOWN
// A - LEFT
// D - RIGHT
// C - to shoot, Use 'W' and 'S' to change direction.
// Keybord control for PLAYER 2 :
// I - UP
// K - DOWN
// J - LEFT
// L - RIGHT
// M - to shoot, Use 'I' and 'K' to change direction.


// CG concepts:-
// 1.transformation- translation, scaling, rotation, 
// 2.mathematical calculation- finding intersection of laser lines and spaceship circle using discriminant
// 3. Rasterization techniques to display text
// 4.concept of event listeners/handlers
// 5.passivemotionfunc- window to viewport transformation
// 6.matrix transformation in stack-psuh,pop

// other cg concepts:- flush, buffers,  line loop, line strip, convex polygon drawing



#ifdef _WIN32
#include<windows.h> //access Windows API, to manipulate windows gui components
#endif
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#define GL_SILENCE_DEPRECATION //ignore warnings in MacOS

#define XMAX 1200
#define YMAX 700
#define SPACESHIP_SPEED 20
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3


//event handler variables
GLint m_viewport[4]; //GLint- graphic card related int variable 
bool mButtonPressed = false;
float mouseX, mouseY; //cursor coordinates
enum view { INTRO, MENU, INSTRUCTIONS, GAME, GAMEOVER }; //enum is userdefined datatype to contains integral constants
view viewPage = INTRO; // initial value
bool keyStates[256] = { false };
bool direction[4] = { false }; //bool array for 4 directions, RLUD
bool laser1Dir[2] = { false }; //bool array for 2 direction of 1st laser
bool laser2Dir[2] = { false};

int alienLife1 = 100;
int alienLife2 = 100;
bool gameOver = false;
float xOne = 500, yOne = 0; 
float xTwo = 500, yTwo = 0;
bool laser1 = false, laser2 = false;
GLint CI = 0;

// Gfloat valued 3D graphics coordinates
GLfloat a[][2] = { 0,-50, 70,-50, 70,70, -70,70 }; //  4 rows and 2 columns, where each row represents the x and y coordinates of a point, Makes a rectangle?????????

GLfloat LightColor[][3] = { 1,1,0,   0,1,1,   0,1,0 }; // 3 rows and 3 columns, where each row represents the RGB (Red, Green, Blue) color values of a light source //Row 1: Yellow, Row 2: Cyan, Row 3: Green

GLfloat AlienBody[][2] = { {-4,9}, {-6,0}, {0,0}, {0.5,9}, {0.15,12}, {-14,18}, {-19,10}, {-20,0},{-6,0} }; // 9 rows and 2 columns, defines the outline of an alien body.
GLfloat AlienCollar[][2] = { {-9,10.5}, {-6,11}, {-5,12}, {6,18}, {10,20}, {13,23}, {16,30}, {19,39}, {16,38},
						  {10,37}, {-13,39}, {-18,41}, {-20,43}, {-20.5,42}, {-21,30}, {-19.5,23}, {-19,20},
						  {-14,16}, {-15,17},{-13,13},  {-9,10.5} }; //21 rows and 2 columns, defines the outline of an alien collar.
GLfloat ALienFace[][2] = { {-6,11}, {-4.5,18}, {0.5,20}, {0.,20.5}, {0.1,19.5}, {1.8,19}, {5,20}, {7,23}, {9,29},
						{6,29.5}, {5,28}, {7,30}, {10,38},{11,38}, {11,40}, {11.5,48}, {10,50.5},{8.5,51}, {6,52},
						{1,51}, {-3,50},{-1,51}, {-3,52}, {-5,52.5}, {-6,52}, {-9,51}, {-10.5,50}, {-12,49}, {-12.5,47},
						{-12,43}, {-13,40}, {-12,38.5}, {-13.5,33},{-15,38},{-14.5,32},  {-14,28}, {-13.5,33}, {-14,28},
						{-13.8,24}, {-13,20}, {-11,19}, {-10.5,12}, {-6,11} };
// 42 rows and 2 columns, defines the outline of an alien face
GLfloat ALienBeak[][2] = { {-6,21.5}, {-6.5,22}, {-9,21}, {-11,20.5}, {-20,20}, {-14,23}, {-9.5,28}, {-7,27}, {-6,26.5},
						{-4.5,23}, {-4,21}, {-6,19.5}, {-8.5,19}, {-10,19.5}, {-11,20.5} };
//define the vertices of an alien beak shape


//displays a string of text in a 3D graphics environment using the rasterization method.
void displayRasterText(float x, float y, float z, char* stringToDisplay) {
	glRasterPos3f(x, y, z);// sets the raster position for the text to be displayed, with (x, y, z) being the 3D coordinates where the text will start rendering.
	for (char* c = stringToDisplay; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); //renders each character in the string using the specified font and size (GLUT_BITMAP_TIMES_ROMAN_24) at the current raster position
	}
}


//initialization function in OpenGL that sets up the initial state of the graphics rendering context
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0); //sets the color that OpenGL uses to clear the screen to black.
	glColor3f(1.0, 0.0, 0.0); //sets the current drawing color to red
	glMatrixMode(GL_PROJECTION); //selects the projection matrix stack for modification. matrices are used to specify how the 3D geometry is projected onto a 2D screen. so 'projection matrix' is used to transform the 3D geometry into a 2D view frustum. 
	glLoadIdentity();//sets the current matrix to the identity matrix.

	gluOrtho2D(-1200, 1200, -700, 700); //specifies an orthographic projection matrix for 2D drawing.                
	glMatrixMode(GL_MODELVIEW); // selects the modelview matrix stack for modification.
}


//to display the introductory screen of a game. 
void introScreen()
{
	char college[80]="Thapar Institute of Engineering and Technology";
	char dept[80] = "CSED department";
	char proj[40] = "A Computer Graphics OPENGL project on";
	char proj2[40] = "Space Shooting Game";
	char create[40] = "Created by:- ";
	char name1[40] = "Pranjal Arora, 102003402";
	char name2[40] = "Naga Madhurya Peram, 102003407";
	char name3[40] = "Aryan Sood, 102003042";
	char guide[40] = "Under Guidance of  Ms. Kanupriya";
	char year[40] = "EVENSEM 2022-23";
	char start[40] = "Press ENTER to start the game";
	


	glClear(GL_COLOR_BUFFER_BIT); //clears the color buffer	to preset values. it takes a single argument that is the bitwise OR of several values indicating which buffer is to be cleared. GL_COLOR_BUFFER_BIT:Indicates the buffers currently enabled for color writing.
	glColor3f(1.0, 0.0, 0.0);//sets the color to red
	displayRasterText(-425, 490, 0.0, college ); // displays the name of the college
	glColor3f(1.0, 1.0, 1.0);
	displayRasterText(-700, 385, 0.0, dept); //dept
	glColor3f(0.0, 0.0, 1.0);
	displayRasterText(-225, 300, 0.0, proj); //proj
	glColor3f(1.0, 0.0, 1.0);
	displayRasterText(-125, 225, 0.0, proj2); //proj2
	glColor3f(1.0, 0.7, 0.8);
	displayRasterText(-100, 150, 0.0, create); //created by
	//glColor3f(1.0, 1.0, 1.0);
	//displayRasterText(-130, 80, 0.0, temp); 
	glColor3f(1.0, 0.0, 0.0);
	displayRasterText(-800, -100, 0.0, name1); //name 1
	//glColor3f(1.0, 1.0, 1.0);
	displayRasterText(-800, -200, 0.0, name2); //name 2
	displayRasterText(-800, -285, 0.0, name3); //name 3
	glColor3f(1.0, 0.0, 0.0);
	displayRasterText(500, -100, 0.0, guide); //guide
	//glColor3f(1.0, 1.0, 1.0);
	//displayRasterText(500, -200, 0.0, temp);
	glColor3f(1.0, 0.0, 0.0);
	displayRasterText(-250, -400, 0.0, year); //year
	glColor3f(1.0, 1.0, 1.0);//sets the color to white
	displayRasterText(-300, -550, 0.0, start); //start
	glFlush();//force execution of GL commands in finite time. empties all of these buffers, causing all issued commands to be executed as quickly as they are accepted by the actual rendering engine. Though this execution may not be completed in any particular time period, it does complete in finite time.
	glutSwapBuffers();//swaps the buffers (using glutSwapBuffers) to display the screen to the user.
}

void startScreenDisplay()
{
	
	glLineWidth(10); //Set the width of lines to be drawn to 10 pixels
	
	//red rectangular border
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_LOOP);              
	glVertex2f(-750, -500);
	glVertex2f(-750, 550);
	glVertex2f(750, 550);
	glVertex2f(750, -500);
	glEnd();


	glLineWidth(1);// line width set to 1

	//start game yellow polygom
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-200, 300);
	glVertex2f(-200, 400);
	glVertex2f(200, 400);
	glVertex2f(200, 300);
	glEnd();

	//instructions yellow polygon
	glBegin(GL_POLYGON);
	glVertex2f(-200, 50);
	glVertex2f(-200, 150);
	glVertex2f(200, 150);
	glVertex2f(200, 50);
	glEnd();

	//quit yellow polygon
	glBegin(GL_POLYGON);
	glVertex2f(-200, -200);
	glVertex2f(-200, -100);
	glVertex2f(200, -100);
	glVertex2f(200, -200);
	glEnd();

	
	//Check if the mouse pointer is hovering over any of the polygons coordinates


	// If the mouse pointer is over the START GAME polygon 
	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 150 && mouseY <= 200) {

		glColor3f(0, 0, 1); //hover color

		if (mButtonPressed) { //if the mouse button is clicked
			alienLife1 = alienLife2 = 100; //set the life of two aliens to 100 
			viewPage = GAME; //switch to the GAME viewPage.
			mButtonPressed = false;  //set button click back to false
		}
	}
	else
		glColor3f(0, 0, 0); //no hover color

	//Display text at specified coordinates
	char start2[40] = "Start the Game";
	displayRasterText(-100, 340, 0.4, start2);

	//If the mouse pointer is over the INSTRUCTIONS polygon
	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 30 && mouseY <= 80) {

		glColor3f(0, 0, 1);

		if (mButtonPressed) { //the mouse button is pressed
			viewPage = INSTRUCTIONS; //then switch to the INSTRUCTIONS viewPage
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);

	char inst[40] = "Instructions";
	displayRasterText(-120, 80, 0.4, inst);


	// If the mouse pointer is over the QUIT polygon 
	if (mouseX >= -100 && mouseX <= 100 && mouseY >= -90 && mouseY <= -40) {

		glColor3f(0, 0, 1);

		if (mButtonPressed) { // the mouse button is pressed
			mButtonPressed = false;
			exit(0); // exit the program.
		}
	}
	else
		glColor3f(0, 0, 0);

	char quit[40] = "	Quit";
	displayRasterText(-100, -170, 0.4, quit);

	glutPostRedisplay();//glutPostRedisplay marks the current window as needing to be redisplayed. So update the screen.
}


//to go back to the menu page
void backButton() {

	char back[40] = "Back";
	displayRasterText(-1000, -550, 0, back);

	// if the current position of the mouse pointer is on Back
	if (mouseX <= -450 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) { //if mouse button is pressed
			viewPage = MENU;//switch to MENU viewpage
			mButtonPressed = false;
			glutPostRedisplay();
		}
	}
	else glColor3f(1, 0, 0); //if not in the region then color set to red
	
}



//for displaying the instructions screen
void instructionsScreenDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //clear color and depth buffers

	char inst1[40] = "INSTRUCTIONS";
	char inst2[40] = "PLAYER 1";
	char inst3[40] = "PLAYER 2";
	char inst4[40] = "Key 'w' to move up.";
	char inst5[40] = "Key 's' to move down.";
	char inst6[40] = "Key 'd' to move right.";
	char inst7[40] = "Key 'a' to move left.";
	char inst8[40] = "Key 'i' to move up.";
	char inst9[40] = "Key 'k' to move down.";
	char inst10[40] = "Key 'j' to move right.";
	char inst11[40] = "Key 'l' to move left.";
	char inst12[100] = "Key 'c' to shoot, Use 'w' and 's' to change direction.";
	char inst13[100] = "Key 'm' to shoot, Use 'i' and 'k' to change direction.";
	char inst14[100] = "The Objective is to kill your opponent.";
	char inst15[100] = "Each time a player gets shot, LIFE decreases by 5 points.";
	
	glColor3f(1, 0, 0); //red
	displayRasterText(-900, 550, 0.4, inst1);
	glColor3f(1, 0, 0); //red
	displayRasterText(-1000, 400, 0.4, inst2);
	displayRasterText(200, 400, 0.4, inst3);
	glColor3f(1, 1, 1); //white
	displayRasterText(-1100, 300, 0.4, inst4);
	displayRasterText(-1100, 200, 0.4, inst5);
	displayRasterText(-1100, 100, 0.4, inst6);
	displayRasterText(-1100, 0, 0.4, inst7);
	displayRasterText(100, 300, 0.4, inst8);
	displayRasterText(100, 200, 0.4, inst9);
	displayRasterText(100, 100, 0.4, inst10);
	displayRasterText(100, 0, 0.4, inst11);
	displayRasterText(-1100, -100, 0.4, inst12);
	displayRasterText(100, -100, 0.4, inst13);
	displayRasterText(-1100, -300, 0.4, inst14);
	displayRasterText(-1100, -370, 0.4, inst15);

	backButton();//go to back menu
	
}


//to draw the alien body
void DrawAlienBody(bool isPlayer1)
{

	//Body color of player 1 and player 2
	if (isPlayer1)
		glColor3f(0, 1, 0); //green-->player1 (but in game we mentioned it as 2)
	else
		glColor3f(1, 1, 0);		//yellow-->player 2 (but in game we mentioned it as 1)
	

	//Draw polygon body
	glBegin(GL_POLYGON); //interpret/delimit the vertices as a convex POLYGON
	for (int i = 0; i <= 8; i++)
		glVertex2fv(AlienBody[i]); //take vertices from AlienBody array.
	glEnd();

	//Draw outline of body
	glColor3f(0, 0, 0);			
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 8; i++)
		glVertex2fv(AlienBody[i]);
	glEnd();

	//Draw Body line effects
	glBegin(GL_LINES);                
	glVertex2f(-13, 11);
	glVertex2f(-15, 9);
	glEnd();
}


//to draw alien collar
void DrawAlienCollar()
{
	glColor3f(1, 0, 0);	//red

	//draw the collar
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 20; i++)
		glVertex2fv(AlienCollar[i]);//take vertices from AlienCollar array
	glEnd();

	//draw the collar outline
	glColor3f(0, 0, 0);				
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 20; i++)
		glVertex2fv(AlienCollar[i]);
	glEnd();
}


//to draw alien face
void DrawAlienFace(bool isPlayer1)
{	
	glColor3f(0, 0, 1); //blue

	//draw face using polygon
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 42; i++)
		glVertex2fv(ALienFace[i]); //take vertices from AlienFace array
	glEnd();

	//draw outline
	glColor3f(0, 0, 0);			
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 42; i++)
		glVertex2fv(ALienFace[i]);
	glEnd();

	//draw ears
	glBegin(GL_LINE_STRIP);
	glVertex2f(3.3, 22);
	glVertex2f(4.4, 23.5);
	glVertex2f(6.3, 26);
	glEnd();
}

//to draw beak
void DrawAlienBeak()
{
	glColor3f(1, 1, 0);	//yellow

	//draw beak
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 14; i++)
		glVertex2fv(ALienBeak[i]);
	glEnd();

	//draw outline of beak
	glColor3f(0, 0, 0);				
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 14; i++)
		glVertex2fv(ALienBeak[i]);
	glEnd();
}


// to draw alien eyes
void DrawAlienEyes(bool isPlayer1)
{

	glColor3f(0, 1, 1);//cyan
	
	//draw Left eye
	glPushMatrix(); //pushes current matrix onto the stack, so that any tranformation performed on the matrix after this poitn can be undine using pop
	glRotated(-10, 0, 0, 1); // rotate the matrix by -10 degrees around z-axis
	glTranslated(-6, 32.5, 0);// translate the matrix which moves the origin of the matrix to the left eye location
	glScalef(2.5, 4, 0); //scale the matrix in x and y axis
	glutSolidSphere(1, 20, 30); // solid sphere of radius 1----> creates left eye
	glPopMatrix(); //pop the matrix from stack, hence undo all prev transformation

	//draw right eye
	glPushMatrix();
	glRotated(-1, 0, 0, 1);
	glTranslated(-8, 36, 0);							
	glScalef(2.5, 4, 0);
	glutSolidSphere(1, 100, 100);
	glPopMatrix();
}


//to draw alien
void DrawAlien(bool isPlayer1)
{
	DrawAlienBody(isPlayer1);
	DrawAlienCollar();
	DrawAlienFace(isPlayer1);
	DrawAlienBeak();
	DrawAlienEyes(isPlayer1);
}


void DrawSpaceshipBody(bool isPlayer1)
{
	if (isPlayer1)
		glColor3f(1, 0, 0);			//BASE
	else
		glColor3f(0.5, 0, 0.5);

	glPushMatrix();
	glScalef(70, 20, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	glPushMatrix();							//LIGHTS
	glScalef(3, 3, 1);
	glTranslated(-20, 0, 0);			//1
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//2
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//3
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//4
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//5
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//6
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//7
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//8
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//9
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glPopMatrix();
}
void DrawSteeringWheel()
{
	glPushMatrix();
	glLineWidth(3);
	glColor3f(0.20, 0., 0.20);
	glScalef(7, 4, 1);
	glTranslated(-1.9, 5.5, 0);
	glutWireSphere(1, 8, 8);
	glPopMatrix();

}
void DrawSpaceshipDoom()
{
	glColor4f(0.7, 1, 1, 0.0011);
	glPushMatrix();
	glTranslated(0, 30, 0);
	glScalef(35, 50, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
}

void DrawLaser(int x, int y, bool dir[]) {
	
	int xend = -XMAX, yend = y;
	if (dir[0])
		yend = YMAX;
	else if (dir[1])
		yend = -YMAX;
	glLineWidth(5);
	glColor3f(0.043, 0.898, 0.988);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(xend, yend);
	glEnd();

}

void SpaceshipCreate(int x, int y, bool isPlayer1) {
	glPushMatrix();
	glTranslated(x, y, 0);
	DrawSpaceshipDoom();
	glPushMatrix();
	glTranslated(4, 19, 0);
	DrawAlien(isPlayer1);
	glPopMatrix();
	DrawSteeringWheel();
	DrawSpaceshipBody(isPlayer1);

	glEnd();
	glPopMatrix();
}

void DisplayHealthBar1() {
	char temp1[40];
	glColor3f(1, 1, 1);
	sprintf_s(temp1, "  LIFE = %d", alienLife1);
	displayRasterText(-1100, 600, 0.4, temp1);
	glColor3f(1, 0, 0);
}

void DisplayHealthBar2() {
	char temp2[40];
	glColor3f(1, 1, 1);
	sprintf_s(temp2, "  LIFE = %d", alienLife2);
	displayRasterText(800, 600, 0.4, temp2);
	glColor3f(1, 0, 0);
}




void checkLaserContact(int x, int y, bool dir[], int xp, int yp, bool player1) {
//x,y: laser line starting points
//xp, yp: spaceship center points


// #define TOP 0
// #define RIGHT 1
// #define BOTTOM 2
// #define LEFT 3

// bool direction[4] = { false }; top right bottom left
// bool laser1Dir[2] = { false }; top bottom
// bool laser2Dir[2] = { false };


	int xend = -XMAX, yend = y;
	xp += 8; yp += 8; // moving circle slightly up to fix laser issue
	if (dir[0])
		yend = YMAX;
	else if (dir[1])
		yend = -YMAX;

	// Here we find out if the laser(line) intersects with spaceship(circle), by solving the equations for the same and finding the discriminant of the quadratic equation obtained
	float m = (float)(yend - y) / (float)(xend - x); // slope 
	float k = y - m * x; //laser line
	int r = 50; // approx radius of the spaceship

	//calculating value of b, a, and c needed to find discriminant
	float b = 2 * xp - 2 * m * (k - yp); //we get this eqn after solving ax^2 + bx + c = 0 (intersection of circle and line) and x = -b / 2a
	float a = 1 + m * m; //to get this, solev for "y" in this y - k = m * (x - xp) and the eqn of circle
	float c = xp * xp + (k - yp) * (k - yp) - r * r; //spaceship circle

	float d = (b * b - 4 * a * c); // discriminant for the equation
	printf("\nDisc: %f x: %d, y: %d, xp: %d, yp: %d", d, x, y, xp, yp);
	if (d >= 0) {
		if (player1)
			alienLife1 -= 5;
		else
			alienLife2 -= 5;

		printf("%d %d\n", alienLife1, alienLife2);
	}
}

void gameScreenDisplay()
{
	DisplayHealthBar1();
	DisplayHealthBar2();
	glScalef(2, 2, 0);

	if (alienLife1 > 0) {
		SpaceshipCreate(xOne, yOne, true); //true = player1
		if (laser1) {
			DrawLaser(xOne, yOne, laser1Dir);
			checkLaserContact(xOne, yOne, laser1Dir, -xTwo, yTwo, true);
		}
	}
	else {
		viewPage = GAMEOVER;
	}

	if (alienLife2 > 0) {
		glPushMatrix();
		glScalef(-1, 1, 1);
		SpaceshipCreate(xTwo, yTwo, false); //false=player2
		if (laser2) {
			DrawLaser(xTwo, yTwo, laser2Dir);
			checkLaserContact(xTwo, yTwo, laser2Dir, -xOne, yOne, false);
		}
		glPopMatrix();
	}
	else {
		viewPage = GAMEOVER;
	}

	if (viewPage == GAMEOVER) {
		xOne = xTwo = 500;
		yOne = yTwo = 0;
	}
}

void displayGameOverMessage() {
	glColor3f(1, 1, 0);
	char message1[50]= "Game Over! Player 1 won the game";
	char message2[50] = "Game Over! Player 2 won the game";
	
	if (alienLife1 > 0)
		displayRasterText(-350, 600, 0.4, message1);
	else
		displayRasterText(-350, 600, 0.4, message2);

}

void keyOperations() {
	if (keyStates[13] == true && viewPage == INTRO) {
		viewPage = MENU;
		printf("view value changed to %d", viewPage);
		printf("enter key pressed\n");
	}
	if (viewPage == GAME) {
		laser1Dir[0] = laser1Dir[1] = false;
		laser2Dir[0] = laser2Dir[1] = false;
		if (keyStates['c'] == true) {
			laser2 = true;
			if (keyStates['w'] == true) 	laser2Dir[0] = true;
			if (keyStates['s'] == true) 	laser2Dir[1] = true;
		}
		else {
			laser2 = false;
			if (keyStates['d'] == true) xTwo -= SPACESHIP_SPEED;
			if (keyStates['a'] == true) xTwo += SPACESHIP_SPEED;
			if (keyStates['w'] == true) yTwo += SPACESHIP_SPEED;
			if (keyStates['s'] == true) yTwo -= SPACESHIP_SPEED;
		}

		if (keyStates['m'] == true) {
			laser1 = true;
			if (keyStates['i'] == true) laser1Dir[0] = true;
			if (keyStates['k'] == true) laser1Dir[1] = true;
		}
		else {
			laser1 = false;
			if (keyStates['l'] == true) xOne += SPACESHIP_SPEED;
			if (keyStates['j'] == true) xOne -= SPACESHIP_SPEED;
			if (keyStates['i'] == true) yOne += SPACESHIP_SPEED;
			if (keyStates['k'] == true) yOne -= SPACESHIP_SPEED;
		}
	}
}

void display()
{
	//glClearColor(, 0 , 0, 1);
	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT);

	switch (viewPage)
	{
	case INTRO:
		introScreen();
		break;
	case MENU:
		startScreenDisplay();
		break;
	case INSTRUCTIONS:
		instructionsScreenDisplay();
		break;
	case GAME:
		gameScreenDisplay();
		//reset scaling values
		glScalef(1 / 2, 1 / 2, 0);
		break;
	case GAMEOVER:
		displayGameOverMessage();
		startScreenDisplay();
		break;
	}

	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}


void passiveMotionFunc(int x, int y) {

	//when mouse not clicked
	mouseX = float(x) / (m_viewport[2] / 1200.0) - 600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y) / (m_viewport[3] / 700.0) - 350.0);

	//Do calculations to find value of LaserAngle
	glutPostRedisplay();
}

void mouseClick(int buttonPressed, int state, int x, int y) {

	if (buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else
		mButtonPressed = false;
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = true;
	glutPostRedisplay();
}

void refresh() {
	glutPostRedisplay();
}

void keyReleased(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("OpenGL Space Shooting Game");
	init();
	glutIdleFunc(refresh); //glutIdleFunc sets the global idle callback to be func so a GLUT program can perform background processing tasks or continuous animation when window system events are not being received. If enabled, the idle callback is continuously called when events are not being received. The callback routine has no parameters.

	glutKeyboardFunc(keyPressed); // glutKeyboardFunc sets the keyboard callback for the current window.

	glutKeyboardUpFunc(keyReleased); // glutKeyboardFunc sets the keyboard up (key release) callback for the current window. 

	glutMouseFunc(mouseClick); //glutMouseFunc sets the mouse callback for the current window. When a user presses and releases mouse buttons in the window, each press and each release generates a mouse callback. The button parameter is one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
	glutPassiveMotionFunc(passiveMotionFunc); //glutPassiveMotionFunc set the motion and passive motion callback respectively for the current window.
	glGetIntegerv(GL_VIEWPORT, m_viewport); //glGetIntegerv function returns the value or values of a selected parameter.
	glutDisplayFunc(display);
	glutMainLoop();
}

