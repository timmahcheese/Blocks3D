/**
 * blocks3d.c
 *
 * 3D Blocks game implemented using the Blocks library
 *
 * @author Timothy Cheeseman
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "blocks.h"
#include "blocks3d.h"

int main (int argc, char *argv[]) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	
	MainWindow = glutCreateWindow(Title);
	glutDisplayFunc(mainWindowDisplay);
	glutReshapeFunc(mainWindowReshape);
	glutKeyboardFunc(mainWindowKeyboard);
	glutReshapeWindow(640, 480);
	
	GameWindow = glutCreateSubWindow(MainWindow, 10, 10, 460, 460);
	glutDisplayFunc(gameWindowDisplay);
	glutReshapeFunc(gameWindowReshape);
	glutKeyboardFunc(mainWindowKeyboard);
	glutReshapeWindow(460, 460);
	
	NextPieceWindow = glutCreateSubWindow(MainWindow, 495, 230, 130, 130);
	glutDisplayFunc(nextPieceWindowDisplay);
	glutReshapeFunc(nextPieceWindowReshape);
	glutKeyboardFunc(mainWindowKeyboard);
	glutReshapeWindow(130, 130);
	
	initGL();
	
	initGame(DIFFICULTY_EASY);
	Game->game_over = true;
	
	glutMainLoop();
	
    return 0;
}

void initGL()
{
	glutSetWindow(MainWindow);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3ub(255, 255, 255);
	
	glutSetWindow(GameWindow);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glutSetWindow(NextPieceWindow);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void mainWindowDisplay()
{
	int i, j;
	int mainWindowHeight;
	int gameWindowWidth;
	int gameWindowHeight;
	
	int num_instructions = 14;
	const char *instructions[] = {
		
		"Controls:",
		"",
		"E - New Easy Game",
		"N - New Normal Game",
		"H - New Hard Game",
		"V - New Very Hard Game",
		"P - Pause/Unpause",
		"Esc - Quit",
		"",
		"W - Rotate Piece",
		"A - Move Piece Left",
		"D - Move Piece Right",
		"S - Move Piece Down",
		"Spacebar - Drop Piece"
	};
	
	const char *next_piece_text = "Next Piece";
	const char *score_text = "Score:";
	char score_number[11] = "0000000000";
	
	glutSetWindow(GameWindow);	
	gameWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
	gameWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	
	glutSetWindow(MainWindow);
	mainWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// draw game border
	
	glBegin(GL_QUADS);
	glVertex2f(5, mainWindowHeight - gameWindowHeight - 15);
	glVertex2f(gameWindowWidth + 15, mainWindowHeight - gameWindowHeight - 15);
	glVertex2f(gameWindowWidth + 15, mainWindowHeight - 5);
	glVertex2f(5, mainWindowHeight - 5);
	glEnd();
	
	// draw game title
	
	glRasterPos2d(gameWindowWidth + 45, mainWindowHeight - 24);
	
	for(i = 0; i < strlen(Title); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Title[i]);
	
	// draw game instructions
	
	for(i = 0; i < num_instructions; i++)
	{
		glRasterPos2d(gameWindowWidth + 50, mainWindowHeight - 34 - 10 * (i + 2));
		
		for(j = 0; j < strlen(instructions[i]); j++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instructions[i][j]);
	}
	
	// draw next piece area
	
	glRasterPos2d(gameWindowWidth + 45, mainWindowHeight - 220);
		
	for(i = 0; i < strlen(next_piece_text); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, next_piece_text[i]);
	
	glBegin(GL_QUADS);
	glVertex2f(gameWindowWidth + 30, mainWindowHeight - 225);
	glVertex2f(gameWindowWidth + 170, mainWindowHeight - 225);
	glVertex2f(gameWindowWidth + 170, mainWindowHeight - 365);
	glVertex2f(gameWindowWidth + 30, mainWindowHeight - 365);
	glEnd();
	
	// draw score
	glRasterPos2d(gameWindowWidth + 70, mainWindowHeight - gameWindowHeight + 10);
	
	for(i = 0; i < strlen(score_text); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_text[i]);
	
	if(Game)
		snprintf(score_number, 11, "%010ld", Game->score);
	
	glRasterPos2d(gameWindowWidth + 45, mainWindowHeight - gameWindowHeight - 14);
	
	for(i = 0; i < strlen(score_number); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_number[i]);
	
	glutSwapBuffers();
}

void mainWindowReshape(int width, int height)
{	
	glutSetWindow(MainWindow);
	
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	
	glMatrixMode(GL_MODELVIEW);
}

void mainWindowKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'e':
		case 'E':
			if (Game && Game->game_over)
			{
				initGame(DIFFICULTY_EASY);
				startGame();
			}
			break;
		case 'n':
		case 'N':
			if (Game && Game->game_over)
			{
				initGame(DIFFICULTY_NORMAL);
				startGame();
			}
			break;
		case 'h':
		case 'H':
			if (Game && Game->game_over)
			{
				initGame(DIFFICULTY_HARD);
				startGame();
			}
			break;
		case 'v':
		case 'V':
			if (Game && Game->game_over)
			{
				initGame(DIFFICULTY_VERY_HARD);
				startGame();
			}
			break;
		case 'p':
		case 'P':
			if(Game && !Game->game_over)
			{
				Paused = !Paused;
				
				if(!Paused)
					startGame();
			}
			break;
		case 27: // escape key
			if(Game && !Game->game_over)
				blocksFreeGame(Game);
			
			exit(EXIT_SUCCESS);
			break;
		case 'w':
		case 'W':
			if(Game && !Game->game_over)
				blocksRotatePiece(Game);
			break;
		case 'a':
		case 'A':
			if(Game && !Game->game_over)
				blocksMovePiece(Game, DIRECTION_LEFT);
			break;
		case 's':
		case 'S':
			if(Game && !Game->game_over)
				blocksMovePiece(Game, DIRECTION_DOWN);
			break;
		case 'd':
		case 'D':
			if(Game && !Game->game_over)
				blocksMovePiece(Game, DIRECTION_RIGHT);
			break;
		case 32: // spacebar
			if(Game && !Game->game_over)
				blocksDropPiece(Game);
			break;
		default:
			return;
	}
	
	refresh();
}

void gameWindowDisplay()
{
	int i, j;
	const char * game_over_text = "Game Over!";
	
	glutSetWindow(GameWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(Game)
	{
	
		glLoadIdentity();
		gluLookAt(-2.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glRotated(Rotation[0], 1.0, 0.0, 0.0);
		glRotated(Rotation[1], 0.0, 1.0, 0.0);
		
		// draw boundaries
		
		glColor3ub(0, 0, 255);
		
		glPushMatrix();
		glTranslatef(0.0, 5.0, 0.0);
		glScalef(Game->width, Game->height - BLOCKS_BUFFER_HEIGHT, 1.0);
		
		glutWireCube(10.0);
		glPopMatrix();
	
		// draw blocks
	
		for (i = BLOCKS_BUFFER_HEIGHT; i < Game->height; i++)
		{
			for (j = 0; j < Game->width; j++)
			{
				glPushMatrix();
				glTranslatef(-45.0 + 10.0 * j, Game->height * 10.0 - 100.0 - 10.0 * i, 0.0);
				
				if(Game->mask[i][j])
				{
					glColor3ub(255, 255, 255);
					glutSolidCube(10.0);
					
					glColor3ub(0, 0, 0);
					glutWireCube(10.0);	
				}
				
				glPopMatrix();
			}
		}
		
		// draw piece
		for (i = 0; i < Game->current_piece->height; i++)
		{
			int y = Game->current_piece->position[1] + i;
			
			for (j = 0; j < Game->current_piece->width; j++)
			{
				int x = Game->current_piece->position[0] + j;
				
				glPushMatrix();
				glTranslatef(-45.0 + 10.0 * x, Game->height * 10.0 - 100.0 - 10.0 * y, 0.0);
				
				if(Game->current_piece->mask[i][j] && y >= BLOCKS_BUFFER_HEIGHT)
				{
					glColor3ub(Game->current_piece->color.r, Game->current_piece->color.g, Game->current_piece->color.b);
					glutSolidCube(10.0);
					
					glColor3ub(0, 0, 0);
					glutWireCube(10.0);	
				}
				
				glPopMatrix();
			}
		}
		
		if(Game->game_over)
		{
			glLoadIdentity();
			glColor3ub(255, 0, 0);
			glRasterPos3d(-30.0, 0.0, 200.0);
			
			for(i = 0; i < strlen(game_over_text); i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game_over_text[i]);
		}
	}
	
	glutSwapBuffers();
}

void gameWindowReshape(int width, int height)
{
	glutSetWindow(GameWindow);
	
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-125.0, 125.0, -125.0, 125.0, -200.0, 200.0);
	
	glMatrixMode(GL_MODELVIEW);
}

void nextPieceWindowDisplay()
{
	int i, j;
	
	glutSetWindow(NextPieceWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(Game && !Game->game_over)
	{
		glLoadIdentity();
		gluLookAt(-2.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		
		for (i = 0; i < Game->next_piece->height; i++)
		{
			for (j = 0; j < Game->next_piece->width; j++)
			{
				glPushMatrix();
				glTranslatef(-(Game->next_piece->width / 2.0) + 0.5 + 1.0 * j,
							 (Game->next_piece->height / 2.0) - 0.5 - 1.0 * i,
							 0.0);
				
				if(Game->next_piece->mask[i][j])
				{
					glColor3ub(Game->next_piece->color.r, Game->next_piece->color.g, Game->next_piece->color.b);
					glutSolidCube(1.0);
					
					glColor3ub(0, 0, 0);
					glutWireCube(1.0);	
				}
				
				glPopMatrix();
			}
		}
	}
	
	glutSwapBuffers();
}

void nextPieceWindowReshape(int width, int height)
{
	glutSetWindow(NextPieceWindow);
	
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.5, 2.5, -2.5, 2.5, -100.0, 100.0);
	
	glMatrixMode(GL_MODELVIEW);
}

void refresh()
{
	glutPostWindowRedisplay(MainWindow);
	glutPostWindowRedisplay(GameWindow);
	glutPostWindowRedisplay(NextPieceWindow);
}

void initGame(Difficulty difficulty)
{
	if(Game)
		blocksFreeGame(Game);
	
	Game = blocksNewGame(10, 20);
	Paused = 0;
	Speed = 1000;
	
	switch (difficulty) {
		case DIFFICULTY_EASY:
			RotationDelta[0] = 0.0;
			RotationDelta[1] = 0.0;
			Game->score_multiplier = 1;
			break;
		case DIFFICULTY_NORMAL:
			RotationDelta[0] = 0.0;
			RotationDelta[1] = 1.0;
			Game->score_multiplier = 2;
			break;
		case DIFFICULTY_HARD:
			RotationDelta[0] = 1.0;
			RotationDelta[1] = 0.0;
			Game->score_multiplier = 3;
			break;
		case DIFFICULTY_VERY_HARD:
			RotationDelta[0] = 1.0;
			RotationDelta[1] = 1.0;
			Game->score_multiplier = 4;
			break;
	};
	
	Rotation[0] = 0.0;
	Rotation[1] = 0.0;
	RotationSpeed = 50;
}

void startGame()
{
	refresh();
	
	glutTimerFunc(Speed, gameTimer, 0);
	glutTimerFunc(RotationSpeed, rotationTimer, 0);
}

void gameTimer(int value)
{
	if(Paused || Game->game_over)
		return;
	
	blocksMovePiece(Game, DIRECTION_DOWN);
	refresh();
	
	glutTimerFunc(Speed, gameTimer, 0);
}

void rotationTimer(int value)
{
	if(Paused)
		return;
	
	Rotation[0] += RotationDelta[0];
	Rotation[1] += RotationDelta[1];
	
	refresh();
	
	glutTimerFunc(RotationSpeed, rotationTimer, 0);
}
