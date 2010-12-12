/**
 * blocks3d.h
 *
 * 3D Blocks game implemented using the Blocks library
 *
 * @author Timothy Cheeseman
 */

#include <stdbool.h>

/**
 * Game difficulty
 */
typedef enum Difficulty {
	
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_VERY_HARD,
	
} Difficulty;

/**
 * Initialize the GL instances
 */
void initGL();

/**
 * Display function for the main window
 */
void mainWindowDisplay();

/**
 * Reshape function for the main window
 */
void mainWindowReshape(int width, int height);

/**
 * Keyboard input handler for the main window
 */
void mainWindowKeyboard(unsigned char key, int x, int y);

/**
 * Display function for the game sub-window
 */
void gameWindowDisplay();

/**
 * Reshape function for the game sub-window
 */
void gameWindowReshape(int width, int height);

/**
 * Display function for the next piece sub-window
 */
void nextPieceWindowDisplay();

/**
 * Reshape function for the next piece sub-window
 */
void nextPieceWindowReshape(int width, int height);

/**
 * Post a GLUT redisplay for all windows and sub-windows
 */
void refresh();

/**
 * Initialize the game state based on a given difficulty
 */
void initGame(Difficulty difficulty);

/**
 * Function to start the game and rotation timers
 */
void startGame();

/**
 * The GLUT time for triggering a piece fall
 */
void gameTimer(int value);

/**
 * The GLUT timer for rotating the camera
 */
void rotationTimer(int value);

/**
 * The title of the game
 */
const char *Title = "Blocks 3D";

/**
 * The main window handle
 */
int MainWindow;

/**
 * The game sub-window handle
 */
int GameWindow;

/**
 * The next piece sub-window handle
 */
int NextPieceWindow;

/**
 * The game data structure
 */
BlocksGame *Game;

/**
 * Whether or not the game is paused
 */
bool Paused;

/**
 * The speed of the game (time in ms for a piece to drop one level)
 */
int Speed;

/**
 * The current camera X and Y rotation values
 */
GLdouble Rotation[2];

/**
 * The camera X and Y rotation deltas
 */
GLdouble RotationDelta[2];

/**
 * The camera rotation speed
 */
int RotationSpeed;
