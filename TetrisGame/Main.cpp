#include <SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>
#include<string>
#define BHEIGHT 16
#define BWIDTH 10
int gameBoard[BHEIGHT][BWIDTH], pieceA[2], pieceB[2], pieceC[2], pieceD[2], numRows, bottomLocations[4][2], bottomRow, matrixMovementRotation[4][4], blockID, pointsOfMatrix[2][2];
bool gameState = true, collidedNextChance = false;
SDL_Surface* backbuffer = NULL;
SDL_Window* Window = NULL;
bool KEYS[322];  // 322 is the number of SDLK_DOWN events




void init();
void keyboard();
void printGameboard();
void spawnBlock();
void moveDirection(bool dir);
void renderBlocks(int val);
void checkLineComplete();
void rotateMatrix();
void getMatrix();
void FillRect(SDL_Surface* surface, int x, int y, int width, int height,
	Uint8 r, Uint8 g, Uint8 b);
void renderPoint(SDL_Surface* surface, int x, int y,
	Uint8 r, Uint8 g, Uint8 b);
void updatePositions(int a, int b, int c, int d, int a2, int b2, int c2, int d2);
void checkGameOver();
void checkGameOver()
{
	for (int j = 0; j != BWIDTH; j++)
	{
		if (gameBoard[2][j] == 1)
		{
			gameState = false;
			break;

		}
	}
}
void getMatrix()
{
	for (int i = pointsOfMatrix[0][1]; i <= pointsOfMatrix[1][1]; i++)
	{
		for (int j = pointsOfMatrix[0][0]; j <= pointsOfMatrix[1][0]; j++)
		{

			if (i >= 0 && j >= 0)
			{
				matrixMovementRotation[i - pointsOfMatrix[0][1]][j - pointsOfMatrix[0][0]] = gameBoard[i][j];

			}
			else
			{
				matrixMovementRotation[i - pointsOfMatrix[0][1]][j - pointsOfMatrix[0][0]] = 7;
			}
		}
	}


}
void rotateMatrix()
{


	if (blockID != 0)
	{
		int extraVar, newMatrix[4][4];
		getMatrix();
		bool state = false;
		int counter = 0;
		extraVar = newMatrix[0][0];
		newMatrix[0][0] = matrixMovementRotation[3][0];
		//newMatrix[0][0]=newMatrix[1][0];
		newMatrix[0][1] = matrixMovementRotation[2][0];
		newMatrix[0][2] = matrixMovementRotation[1][0];
		newMatrix[0][3] = matrixMovementRotation[0][0];
		newMatrix[1][3] = matrixMovementRotation[0][1];
		newMatrix[2][3] = matrixMovementRotation[0][2];
		newMatrix[3][3] = matrixMovementRotation[0][3];

		newMatrix[3][2] = matrixMovementRotation[1][3];
		newMatrix[3][1] = matrixMovementRotation[2][3];
		newMatrix[3][0] = matrixMovementRotation[3][3];
		newMatrix[2][0] = matrixMovementRotation[3][2];
		newMatrix[1][0] = matrixMovementRotation[3][1];
		newMatrix[1][1] = matrixMovementRotation[2][1];
		newMatrix[2][1] = matrixMovementRotation[2][2];
		newMatrix[2][2] = matrixMovementRotation[1][2];
		newMatrix[1][2] = matrixMovementRotation[1][1];
		for (int i = 0; i != 4; i++)
		{
			for (int j = 0; j != 4; j++)
			{
				if (newMatrix[i][j] + matrixMovementRotation[i][j] == 9 || matrixMovementRotation[i][j] == 1)
				{
					state = true;
					break;
				}

			}
			if (state)
			{
				break;
			}
		}
		if (!state)
		{
			bottomLocations[0][0] = -1;
			bottomLocations[0][1] = -1;
			bottomLocations[1][0] = -1;
			bottomLocations[1][1] = -1;
			bottomLocations[2][0] = -1;
			bottomLocations[2][1] = -1;
			bottomLocations[3][0] = -1;
			bottomLocations[3][1] = -1;

			for (int i = pointsOfMatrix[0][1]; i <= pointsOfMatrix[1][1]; i++)
			{
				for (int j = pointsOfMatrix[0][0]; j <= pointsOfMatrix[1][0]; j++)
				{
					if (newMatrix[i - pointsOfMatrix[0][1]][j - pointsOfMatrix[0][0]] == 2)
					{
						bottomLocations[j - pointsOfMatrix[0][0]][0] = j;
						bottomLocations[j - pointsOfMatrix[0][0]][1] = i;
						if (counter == 0)
						{
							pieceA[0] = i;
							pieceA[1] = j;
						}
						if (counter == 1)
						{
							pieceB[0] = i;
							pieceB[1] = j;
						}
						if (counter == 2)
						{
							pieceC[0] = i;
							pieceC[1] = j;
						}
						if (counter == 3)
						{
							pieceD[0] = i;
							pieceD[1] = j;
						}
						counter += 1;
					}
					if (newMatrix[i - pointsOfMatrix[0][1]][j - pointsOfMatrix[0][0]] != 7)
					{
						gameBoard[i][j] = newMatrix[i - pointsOfMatrix[0][1]][j - pointsOfMatrix[0][0]];
					}
					else
					{
						gameBoard[i][j] = 0;
					}
				}
			}
		}

	}
}
void updatePositions(int a, int b, int c, int d, int a2, int b2, int c2, int d2)
{
	pieceA[1] = a, pieceA[0] = a2;
	pieceB[1] = b, pieceB[0] = b2;
	pieceC[1] = c, pieceC[0] = c2;
	pieceD[1] = d, pieceD[0] = d2;

}
void keyboard() {
	// message processing loop
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// check for messages
		switch (event.type) {
			// exit if the window is closed
		case SDL_QUIT:
			gameState = false; // set game state to done,(do what you want here)
			break;
			// check for keypresses
		case SDL_KEYDOWN:
			KEYS[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			KEYS[event.key.keysym.sym] = false;
			break;
		default:
			break;
		}
	}
}
void moveDirection(bool dir)
{
	if (dir)
	{
		if (BWIDTH - 1 - pieceA[1] && BWIDTH - 1 - pieceB[1] && BWIDTH - 1 - pieceC[1] && BWIDTH - 1 - pieceD[1])
		{
			if (gameBoard[pieceA[0]][pieceA[1] + 1] != 1 && gameBoard[pieceB[0]][pieceB[1] + 1] != 1 && gameBoard[pieceC[0]][pieceC[1] + 1] != 1 && gameBoard[pieceD[0]][pieceD[1] + 1] != 1)
			{
				renderBlocks(0);
				pieceA[1] += 1;
				pieceB[1] += 1;
				pieceC[1] += 1;
				pieceD[1] += 1;
				for (int i = 0; i != 4; i++)
				{
					if (bottomLocations[i][0] != -1)
					{
						bottomLocations[i][0] += 1;
					}
				}
				pointsOfMatrix[0][0] += 1;
				pointsOfMatrix[1][0] += 1;
			}
		}

	}
	else
	{
		if (pieceA[1] && pieceB[1] && pieceC[1] && pieceD[1])
		{
			if (gameBoard[pieceA[0]][pieceA[1] - 1] != 1 && gameBoard[pieceB[0]][pieceB[1] - 1] != 1 && gameBoard[pieceC[0]][pieceC[1] - 1] != 1 && gameBoard[pieceD[0]][pieceD[1] - 1] != 1)
			{
				renderBlocks(0);
				pieceA[1] -= 1;
				pieceB[1] -= 1;
				pieceC[1] -= 1;
				pieceD[1] -= 1;
				for (int i = 0; i != 4; i++)
				{
					if (bottomLocations[i][0] != -1)
					{
						bottomLocations[i][0] -= 1;
					}
				}
				pointsOfMatrix[0][0] -= 1;
				pointsOfMatrix[1][0] -= 1;
			}
		}
	}
}
void checkLineComplete()
{
	int top_row = 0, i = BHEIGHT, pq;
	bool lineComplete = true, debug = false;
	while (i >= top_row)
	{
		i--;
		lineComplete = true;
		for (int j = 0; j != BWIDTH; j++)
		{
			pq = i;
			if (gameBoard[i][j] == 0)
			{
				lineComplete = false;
				break;
			}
		}
		if (lineComplete)
		{
			top_row += 1;
			i = BHEIGHT;
			for (int i = pq; i != 0; i--)
			{
				for (int j = 0; j != BWIDTH; j++)
				{
					if (gameBoard[i - 1][j] != 2)
					{
						gameBoard[i][j] = gameBoard[i - 1][j];
					}
				}
			}
		}
	}
}
void init()
{
	for (int i = 0; i != BHEIGHT - 1; i++)
	{
		for (int j = 0; j != BWIDTH; j++)
		{
			gameBoard[i][j] = 0;
		}
	}
	for (int j = 0; j != BWIDTH; j++)
	{
		gameBoard[BHEIGHT - 1][j] = 1;
	}

	for (int i = 0; i < 322; i++) { // init them all to false
		KEYS[i] = false;
	}
}
void renderBlocks(int val)
{
	gameBoard[pieceA[0]][pieceA[1]] = val;
	gameBoard[pieceB[0]][pieceB[1]] = val;
	gameBoard[pieceC[0]][pieceC[1]] = val;
	gameBoard[pieceD[0]][pieceD[1]] = val;
}
void spawnBlock()
{
	srand(time(0));
	blockID = rand() % 5;
	switch (blockID)
	{
	case 0:
		/*This case represents the square block*/
		pointsOfMatrix[0][0] = 3, pointsOfMatrix[0][1] = -1;
		pointsOfMatrix[1][0] = 6, pointsOfMatrix[1][1] = 2;
		pieceA[1] = 4, pieceA[0] = 0;
		pieceB[1] = 5, pieceB[0] = 0;
		pieceC[1] = 4, pieceC[0] = 1;
		pieceD[1] = 5, pieceD[0] = 1;
		bottomRow = 1;
		bottomLocations[0][0] = -1;
		bottomLocations[0][1] = -1;
		bottomLocations[1][0] = 4;
		bottomLocations[1][1] = 1;
		bottomLocations[2][0] = 5;
		bottomLocations[2][1] = 1;
		bottomLocations[3][0] = -1;
		bottomLocations[3][1] = -1;
		renderBlocks(2);
		break;
	case 1:
		pointsOfMatrix[0][0] = 3, pointsOfMatrix[0][1] = -1;
		pointsOfMatrix[1][0] = 6, pointsOfMatrix[1][1] = 2;
		pieceA[1] = 3, pieceA[0] = 0;
		pieceB[1] = 4, pieceB[0] = 0;
		pieceC[1] = 5, pieceC[0] = 0;
		pieceD[1] = 6, pieceD[0] = 0;
		bottomRow = 1;
		bottomLocations[0][0] = 3;
		bottomLocations[0][1] = 0;
		bottomLocations[1][0] = 4;
		bottomLocations[1][1] = 0;
		bottomLocations[2][0] = 5;
		bottomLocations[2][1] = 0;
		bottomLocations[3][0] = 6;
		bottomLocations[3][1] = 0;
		renderBlocks(2);
		break;
	case 2:
		pointsOfMatrix[0][0] = 3, pointsOfMatrix[0][1] = -1;
		pointsOfMatrix[1][0] = 6, pointsOfMatrix[1][1] = 2;
		pieceA[1] = 4, pieceA[0] = 0;
		pieceB[1] = 5, pieceB[0] = 0;
		pieceC[1] = 5, pieceC[0] = 1;
		pieceD[1] = 6, pieceD[0] = 1;
		bottomRow = 1;
		bottomLocations[0][0] = -1;
		bottomLocations[0][1] = -1;
		bottomLocations[1][0] = 4;
		bottomLocations[1][1] = 0;
		bottomLocations[2][0] = 5;
		bottomLocations[2][1] = 1;
		bottomLocations[3][0] = 6;
		bottomLocations[3][1] = 1;
		renderBlocks(2);
		break;
	case 3:
		pointsOfMatrix[0][0] = 3, pointsOfMatrix[0][1] = -1;
		pointsOfMatrix[1][0] = 6, pointsOfMatrix[1][1] = 2;
		pieceA[1] = 6, pieceA[0] = 0;
		pieceB[1] = 5, pieceB[0] = 0;
		pieceC[1] = 5, pieceC[0] = 1;
		pieceD[1] = 4, pieceD[0] = 1;
		bottomRow = 1;
		bottomLocations[0][0] = -1;
		bottomLocations[0][1] = -1;
		bottomLocations[1][0] = 6;
		bottomLocations[1][1] = 0;
		bottomLocations[2][0] = 5;
		bottomLocations[2][1] = 1;
		bottomLocations[3][0] = 4;
		bottomLocations[3][1] = 1;
		renderBlocks(2);
		break;
	case 4:
		pointsOfMatrix[0][0] = 3, pointsOfMatrix[0][1] = -1;
		pointsOfMatrix[1][0] = 6, pointsOfMatrix[1][1] = 2;
		pieceA[1] = 6, pieceA[0] = 0;
		pieceB[1] = 5, pieceB[0] = 0;
		pieceC[1] = 5, pieceC[0] = 1;
		pieceD[1] = 4, pieceD[0] = 0;
		bottomRow = 1;
		bottomLocations[0][0] = -1;
		bottomLocations[0][1] = -1;
		bottomLocations[1][0] = 6;
		bottomLocations[1][1] = 0;
		bottomLocations[2][0] = 5;
		bottomLocations[2][1] = 1;
		bottomLocations[3][0] = 4;
		bottomLocations[3][1] = 0;
		renderBlocks(2);
		break;

	}
}
bool calculateCollision()
{
	bool isCollisionDetected = false;
	for (int i = 0; i != 4; i++)
	{
		if (bottomLocations[i][0] != -1)
		{
			if (gameBoard[bottomLocations[i][1] + 1][bottomLocations[i][0]] == 1 || bottomLocations[i][1] >= BHEIGHT - 1)
			{
				isCollisionDetected = true;
				break;
			}
		}
	}
	if (isCollisionDetected)
	{
		collidedNextChance = true;
	}
	return isCollisionDetected;
}
void moveDown()
{
	renderBlocks(0);
	for (int i = 0; i != 4; i++)
	{
		if (bottomLocations[i][0] != -1)
		{
			bottomLocations[i][1] += 1;
		}
	}
	pieceA[0] += 1;
	pieceB[0] += 1;
	pieceC[0] += 1;
	pieceD[0] += 1;
	pointsOfMatrix[0][1] += 1;
	pointsOfMatrix[1][1] += 1;
}
void printGameboard()
{
	//printf("__________");
	FillRect(backbuffer, 0, 0, 5, 320, 255, 255, 255);
	FillRect(backbuffer, 0, 0, 200, 5, 255, 255, 255);
	FillRect(backbuffer, 202, 0, 5, 320, 255, 255, 255);
	FillRect(backbuffer, 0, 322, 200, 5, 255, 255, 255);
	for (int i = 0; i != BHEIGHT; i++)
	{
		for (int j = 0; j != BWIDTH; j++)
		{
			if (gameBoard[i][j] == 2)
			{
				renderPoint(backbuffer, j, i, 0, 0, 255);
			}
			else if (gameBoard[i][j] == 1) {
				renderPoint(backbuffer, j, i, 0, 0, 255);
			}

		}


	}
	//printf(" ¯¯¯¯¯¯¯¯¯¯");
}

bool ProgramIsRunning();

void FillRect(SDL_Surface* surface, int x, int y, int width, int height,
	Uint8 r, Uint8 g, Uint8 b)
{
	Uint32 color;
	color = SDL_MapRGB(surface->format, r, g, b);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	SDL_FillRect(surface, &rect, color);
}
void HollowRect(SDL_Surface* surface, int x, int y, int width, int height,
	Uint8 r, Uint8 g, Uint8 b) {
	FillRect(surface, x, y, width, 1, r, g, b);
	FillRect(surface, x, y + height - 1, width, 1, r, g, b);
	FillRect(surface, x, y, 1, height, r, g, b);
	FillRect(surface, x + width - 1, y, 1, height, r, g, b);

}
void renderPoint(SDL_Surface* surface, int x, int y,
	Uint8 r, Uint8 g, Uint8 b) {
	int width = 20, height = 20;
	HollowRect(surface, 20 * x + 5, 20 * y + 5, width, height, r, g, b);
	FillRect(surface, (20 * x) + 7, (20 * y) + 7, width - 4, height - 4, r, g, b);
}
void DrawPixel(SDL_Surface* surface,
	int x, int y,
	Uint8 r, Uint8 g, Uint8 b);

int main(int argc, char* args[])
{
	std::string anim1 = "Hey, there!";
	char c = _getch();
	if (c != 'N')
	{
		for (int i = 0; i != anim1.length(); i++)
		{
			printf("%c", anim1[i]);
			Sleep(100);
		}
		Sleep(1000);
		anim1 = "Press any key to continue";
		for (int i = 0; i != anim1.length(); i++)
		{
			printf("%c", anim1[i]);
			Sleep(100);
		}
		_getch();
		system("cls");
		FILE* fptr;
		fopen_s(&fptr, "final.txt", "r");
		c = fgetc(fptr);
		while (c != EOF)
		{
			printf("%c", c);
			c = fgetc(fptr);
		}

		fclose(fptr);
		_getch();
		system("cls");
		anim1 = "@<aP-10 'g; [wgwefq. I think 51^9;  my systasd135em'dass been hackeasd. BEAT THIS BOSS AND HELP MEEE!";
		for (int i = 0; i != anim1.length(); i++)
		{
			printf("%c", anim1[i]);
			Sleep(100);
		}
		_getch();
	}
	init();
	spawnBlock();
	checkLineComplete();
	int x = 0;
	bool keyWasPressed = false;

	// Window Set up ====================================
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
			"SDL", "SDL failed to initialize", NULL);
		SDL_Quit();
		return 0;
	}
	Window = SDL_CreateWindow("SDL",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800, 600, SDL_WINDOW_SHOWN);

	backbuffer = SDL_GetWindowSurface(Window);

	// Main Game loop ====================================
	while (gameState == true)
	{
		SDL_FillRect(backbuffer, NULL, 0x000000);
		x = 0;
		while (x <= 500)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					/* Look for a keypress */
				case SDL_KEYDOWN:
					/* Check the SDLKey values and move change the coords */
					switch (event.key.keysym.sym) {
					case SDLK_LEFT:
						checkGameOver();
						if (!keyWasPressed)
						{
							keyWasPressed = true;
							moveDirection(false);
						}
					case SDLK_RIGHT:
						checkGameOver();
						if (!keyWasPressed)
						{
							keyWasPressed = true;
							moveDirection(true);
						}
					case SDLK_DOWN:
						if (!keyWasPressed)
						{
							renderBlocks(2);
							keyWasPressed = true;
							bool zx = calculateCollision();
							checkGameOver();
							if (zx != true)
							{
								moveDown();
							}
						}
					case SDLK_r:
						if (GetKeyState('R') & 0x8000)
						{
							/*Checks if key was being pressed even in the last frame*/
							checkGameOver();
							if (!keyWasPressed)
							{
								keyWasPressed = true;
								rotateMatrix();
							}

						}

					default:
						keyWasPressed = false;
						break;
					}
				}
			}
			/*while (SDL_PollEvent(&event)) {
				// check for messages
				switch (event.type) {
					// exit if the window is closed
				case SDL_QUIT:
					gameState = false; // set game state to done,(do what you want here)
					break;
					// check for keypresses
				case SDL_KEYDOWN:
					KEYS[event.key.keysym.sym] = true;
					break;
				case SDL_KEYRIGHT:
					KEYS[event.key.keysym.sym] = false;
					break;
				default:
					break;
				}
			}*/
			/*if (KEYS[SDLK_UP])
			{
				/*Checks if key was being pressed even in the last frame*/
				/*checkGameOver();
				if (!keyWasPressed)
				{
					keyWasPressed = true;
					moveDirection(false);
				}

			}
			else if (GetKeyState('D') & 0x8000)
			{
				/*Checks if key was being pressed even in the last frame*/
				/* checkGameOver();
				 if (!keyWasPressed)
				 {
					 keyWasPressed = true;
					 moveDirection(true);
				 }

			 }
			 else if (GetKeyState('S') & 0x8000)
			 {
				 /*Checks if key was being pressed even in the last frame*/
				 /*if (!keyWasPressed)
				 {
					 renderBlocks(2);
					 keyWasPressed = true;
					 bool zx = calculateCollision();
					 checkGameOver();
					 if (zx != true)
					 {
						 moveDown();
					 }
				 }

			 }
			 else if (GetKeyState('R') & 0x8000)
			 {
				 /*Checks if key was being pressed even in the last frame*/
				 /*checkGameOver();
				 if (!keyWasPressed)
				 {
					 keyWasPressed = true;
					 rotateMatrix();
				 }

			 }
			 else
			 {
				 /*Sets the variable false as the key was not pressed this time*/
				 /*    keyWasPressed = false;
				 }*/
			x += 1;
			Sleep(1);
			if (x % 17 == 0)
			{
				if (collidedNextChance)
				{
					renderBlocks(1);
					checkGameOver();
					spawnBlock();
					checkLineComplete();
					collidedNextChance = false;
				}
				SDL_FillRect(backbuffer, NULL, 0x000000);
				system("cls");
				renderBlocks(2);
				printGameboard();
			}
			if (x % 40 == 0)
			{
				bool zx = calculateCollision();
				if (zx != true)
				{
					moveDown();
				}
			}
			SDL_UpdateWindowSurface(Window);
		}
		if (x <= 500)
		{
			Sleep(501 - x);
		}
		if (!gameState)
		{
			break;
		}

		// Draw 100 randomly colored pixels per iteration =====================
//		renderPoint(backbuffer, 400, 300, 0, 0, 255);

		// Update the window every 20 milliseconds second=====================
//		SDL_Delay(20);
		SDL_UpdateWindowSurface(Window);
	}
	SDL_Quit();
	return 0;
}

// Input Handling ====================================
bool ProgramIsRunning()
{
	SDL_Event event;
	bool running = true;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			running = false;
	}

	return running;
}

// Drawing pixels function====================================
void DrawPixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	// Check to see if Surface should be locked
	if (SDL_MUSTLOCK(surface))
	{
		if (SDL_LockSurface(surface) < 0)
			return;
	}

	// Algorithm for pixel drawing location
	if (x >= surface->w || x < 0 || y >= surface->h || y < 0)
		return;
	Uint32* buffer;
	Uint32 color;

	// Algorithm for pixel coloration
	color = SDL_MapRGB(surface->format, r, g, b);

	// Algorithm for apending pixel to surface
	buffer = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;

	// Map the pixel color to the location
	(*buffer) = color;

	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
}