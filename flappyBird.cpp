/*
	consoleBird

	Just a small flappBird kinda game, but with ASCII characters in console
*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>




// Global Variables

/*
	globalGap: gap between the upper and lower pipes
*/
int globalGap = 12;

/*
	score: no.of pipes crossed by user without any collisions with walls or pipes
*/
int score = 0;

/*
	GetStdHandle(): Used by applications that need to perform read or write to console
	more info: https://docs.microsoft.com/en-us/windows/console/getstdhandle
*/
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

/*
	COORD: Defines coordinates in a console
	more info: https://docs.microsoft.com/en-us/windows/console/coord-str
*/
COORD CursorPosition;




// Function Prototypes
int getRows();
int getColumns();
void goto_x_y(int x, int y);
void setcursor(bool visible, DWORD size);




// Classes

/*
	Window Class
*/
class Window {
	public:
		int rows;
		int columns;

		Window() {
			rows = getRows() - 2;
			columns = getColumns() - 20;
		}

		/*
			draw(): draws the outline of window
		*/
		void draw() {
			for (int i = 1; i <= rows; i++) {
				goto_x_y(1, i);
				std::cout << "*";
				goto_x_y(columns, i);
				std::cout << "*";
			}
			for (int i = 1; i <= columns; i++) {
				goto_x_y(i, 1);
				std::cout << "*";
				goto_x_y(i, rows);
				std::cout << "*";
			}
		}

		/*
			showLogo(): echos "consoleBird" just beside the window outline
		*/
		void showLogo() {
			goto_x_y(columns + 3, rows / 4);
			std::cout << "consoleBird";
			goto_x_y(columns + 3, (rows / 4) + 1);
			std::cout << "-----------";
		}

		/*
			showScore(): echos current score just beside the window outline
		*/
		void showScore() {
			goto_x_y(columns + 3, rows / 3);
			std::cout << "           ";
			goto_x_y(columns + 3, rows / 3);
			std::cout << "Score: " << score;
		}

		/*
			showGameOverMessage(): echos gameover message when there is a collision
		*/
		void showGameOverMessage() {
			int rowStart = columns / 3;
			int rowEnd = 2 * rowStart;
			int colStart = rows / 3;
			int colEnd = 2 * colStart;
			for (int i = rowStart; i <= rowEnd; i++) {
				for (int j = colStart; j <= colEnd; j++) {
					goto_x_y(i, j);
					std::cout << " ";
				}
			}
			for (int i = rowStart; i <= rowEnd; i++) {
				for (int j = 0; j < 3; j++) {
					goto_x_y(i, colStart + j);
					std::cout << ".";
					goto_x_y(i, colEnd - j);
					std::cout << ".";
				}
			}
			goto_x_y((columns / 2) - 5, rows / 2);
			std::cout << "GAME OVER";
			for (int i = 0; i < 3; i++) {
				goto_x_y((columns / 2) - 5, (rows / 2) + 2);
				std::cout << "          ";
				Sleep(250);
				goto_x_y((columns / 2) - 5, (rows / 2) + 2);
				std::cout << "Score: " << score;
				Sleep(250);
			}
		}
};


/*
	BirdCoordinates Class
*/
class BirdCoordinates {
	public:
		float x;
		float y;

		BirdCoordinates(float xCoordinate = 10, float yCoordinate = 10) {
			x = xCoordinate;
			y = yCoordinate;
		}
};


/*
	Bird Class
*/
class Bird {
	public:
		BirdCoordinates birdCoordinates;
		char birdCharacters[2][6] = {'/', '-', '-', 'o', '\\', ' ',
				   					 '|', '_', '_', '_', ' ', '>'};
		
		Window window;

		Bird() {
			birdCoordinates.x = (float)window.columns / 6;
			birdCoordinates.y = (float)window.rows / 2;
		}

		/*
			draw(): draws the bird onto console
		*/
		void draw() {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 6; j++) {
					goto_x_y(birdCoordinates.x + j, birdCoordinates.y + i);
					std::cout << birdCharacters[i][j];
				}
			}
		}

		/*
			erase(): erases the bird from the console
		*/
		void erase() {
			goto_x_y(birdCoordinates.x, birdCoordinates.y);
			std::cout << "      ";
			goto_x_y(birdCoordinates.x, birdCoordinates.y + 1);
			std::cout << "      ";
		}

		/*
			jump(): moves the bird up by 1 row
		*/
		void jump() {
			erase();
			birdCoordinates.y -= 1;
			draw();
		}

		/*
			dive(): moves the bird down by 2 rows
		*/
		void dive() {
			erase();
			birdCoordinates.y += 2;
			draw();
		}

		/*
			gravity(): moves the bird down by 0.15 of row
		*/
		void gravity() {
			erase();
			birdCoordinates.y += 0.15;
			draw();
		}
};


/*
	Pipe Class
*/
class Pipe {
	public:
		int x;
		int gap;
		int upperPipeHeight;

		Window window;

		Pipe(int xCoordinate = 80) {
			x = xCoordinate;
			gap = globalGap;
			upperPipeHeight = getRandomNumber(window, globalGap);
		}

		/*
			getRandomNumber(): returns a random number between [0, (window.rows - gap)]
		*/
		int getRandomNumber(Window &window, int gap) {
			int num = rand() % (window.rows - gap);
			while (num < 2) {
				num = rand() % (window.rows - gap);
			}
			return num;
		}

		/*
			draw(): draws the pipe onto console
		*/
		void draw() {
			for (int i = 2; i < upperPipeHeight; i++) {
				goto_x_y(x, i);
				std::cout << "***";
			}
			for (int i = upperPipeHeight + gap; i < window.rows; i++) {
				goto_x_y(x, i);
				std::cout << "***";
			}
		}

		/*
			erase(): erases the pipe from the console
		*/
		void erase() {
			for (int i = 2; i < upperPipeHeight; i++) {
				goto_x_y(x, i);
				std::cout << "   ";
			}
			for (int i = upperPipeHeight + gap; i < window.rows; i++) {
				goto_x_y(x, i);
				std::cout << "   ";
			}
		}

		/*
			move(): moves the pipe left by 1 column
					if pipe reaches the left wall then, re-positions the pipe at the right wall
		*/
		void move() {
			erase();
			if (x <= 2) {
				upperPipeHeight = getRandomNumber(window, globalGap);
				x = window.columns - 3;
				score += 1;
				window.showScore();
			}
			else {
				x -= 1;
			}
			draw();
		}
};


/*
	PipeSystem Class
*/
class PipeSystem {
	public:
		Pipe pipes[3];
		Window window;

		PipeSystem() {
			pipes[0].x = (window.columns - 3) / 3;
			pipes[1].x = 2 * (window.columns - 3) / 3;
			pipes[2].x = window.columns - 6;
		}

		/*
			draw(): draws pipes onto the console
		*/
		void draw() {
			for (int i = 0; i < 3; i++) {
				pipes[i].draw();
			}
		}

		/*
			animate(): animates(moves left by 1 column) the pipes
		*/
		void animate() {
			for (int i = 0; i < 3; i++) {
				pipes[i].move();
			}
		}
};






// Function Declarations

/*
	getRows(): Gets the no.of rows in the current console window
*/
int getRows() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}


/*
	getColumns(): Gets the no.of columns in the current console window
*/
int getColumns() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}


/*
	goto_x_y(): Places cursor at the given coordinates (x, y)
*/
void goto_x_y(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}


/*
	setursor(): Actually used this to make cursor invisible
	more info: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setcursor
*/
void setcursor(bool visible, DWORD size) {
	if (size == 0) {
		size = 20;
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}


/*
	echoLogo(): Echoes logo to the console
*/
void echoLogo() {
	goto_x_y((getColumns() / 2) - 5, getRows() / 3);
	std::cout << "consoleBird";
}


/*
	eraseLogo(): Erases logo in the console
*/
void eraseLogo() {
	goto_x_y((getColumns() / 2) - 5, getRows() / 3);
	std::cout << "           ";
}


/*
	animateLogo(): Animates Logo, blinks it
*/
void animateLogo() {
	echoLogo();
	Sleep(250);
	eraseLogo();
	Sleep(250);
}


/*
	echoMenu(): Echoes menu(startGame, controls, quit) tho the console
*/
void echoMenu() {
	system("cls");
	goto_x_y((getColumns() / 2) - 7, 2 * getRows() / 3);
	std::cout << "Start Game => 1";
	goto_x_y((getColumns() / 2) - 6, (2 * getRows() / 3) + 1);
	std::cout << "Controls => 2";
	goto_x_y((getColumns() / 2) - 4, (2 * getRows() / 3) + 2);
	std::cout << "Quit => 3";
}


/*
	menuLoop(): This animates logo, shows menu, until the user gives some input through keyboard.
				Returns the character, entered by user
*/
char menuLoop() {
	echoMenu();
	char c;
	while (!kbhit()) {
		animateLogo();
		if (kbhit()) {
			c = getch();
			break;
		}
	}
	return c;
}


/*
	echoControls(): Echoes controls to the console
*/
void echoControls() {
	system("cls");
	goto_x_y((getColumns() / 2) - 4, getRows() / 2);
	std::cout << "Controls";
	goto_x_y((getColumns() / 2) - 4, (getRows() / 2) + 1);
	std::cout << "--------";
	goto_x_y((getColumns() / 2) - 4, (getRows() / 2) + 3);
	std::cout << "Jump => J";
	goto_x_y((getColumns() / 2) - 4, (getRows() / 2) + 4);
	std::cout << "Dive => D";
	goto_x_y((getColumns() / 2) - 15, (getRows() / 2) + 6);
	std::cout << "Press any key to return to menu";
}






// Functions for detecting collisions

/*
	getIndexOfPipeNearBird(): returns the index of the pipe nearest to the bird, in the pipeSystem
*/
int getIndexOfPipeNearBird(int birdXCoordinate, Pipe (&pipes)[3]) {
	int index = 0;
	for (int i = 1; i < 3; i++) {
		if (pipes[i].x - birdXCoordinate < pipes[index].x - birdXCoordinate) {
			index = i;
		}
	}
	return index;
}


/*
	birdNotInGap(): returns true if the bird, is in the gap of the pipe nearest to bird
*/
bool birdNotInGap(int birdYCoordinate, int gap, int upperPipeHeight) {
	if (birdYCoordinate <= upperPipeHeight - 1 || birdYCoordinate >= upperPipeHeight + gap - 1) {
		return true;
	}
	return false;
}


/*
	collisionWithPipes(): returns true if there is a collision with pipe nearest to bird
*/
bool collisionWithPipes(BirdCoordinates &birdCoordinates, Pipe (&pipes)[3]) {
	int index = getIndexOfPipeNearBird(birdCoordinates.x, pipes);
	if (pipes[index].x == (int)birdCoordinates.x + 5 && birdNotInGap(birdCoordinates.y, pipes[index].gap, pipes[index].upperPipeHeight)) {
		return true;
	}
	return false;
}


/*
	collisionWithWindow(): returns true if there is a collision with any of the walls(window outline)
*/
bool collisionWithWindow(BirdCoordinates &birdCoordinates, Window &window) {
	if (birdCoordinates.y < 2 || birdCoordinates.y > window.rows - 2) {
		return true;
	}
	return false;
}


/*
	collisionDetected(): returns true if any of the above two functions returns true
*/
bool collisionDetected(Bird &bird, PipeSystem &pipeSystem, Window &window) {
	if (collisionWithPipes(bird.birdCoordinates, pipeSystem.pipes) || collisionWithWindow(bird.birdCoordinates, window)) {
		return true;
	}
	return false;
}






// Game Loop
void gameLoop() {
	system("cls");	// clears console

	Window window;
	window.draw();
	window.showLogo();
	window.showScore();

	Bird bird;
	bird.draw();
	
	PipeSystem pipeSystem;
	pipeSystem.draw();

	while (1) {
		if (kbhit()) {
			char c = getch();
			if (c == 106) {
				bird.jump();
			}
			else if (c == 100) {
				bird.dive();
			}
			else {
				return;
			}
		}
		if (collisionDetected(bird, pipeSystem, window)) {
			window.showGameOverMessage();
			Sleep(1000);
			return;
		}
		bird.gravity();
		pipeSystem.animate();
	}
	return;
}




int main() {
	srand((unsigned int)clock());	// Seeding

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd,SW_SHOWMAXIMIZED);	// Launches the console window in maximized mode
	
	setcursor(0, 0);	// sets cursor invisible

	char c = menuLoop();

	if (c == '1') {
		gameLoop();
	}
	else if (c == '2') {
		echoControls();
		getch();
		main();
	}
	else {
		return 0;
	}
	return 0;
}
