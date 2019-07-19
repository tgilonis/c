#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <conio.h>

using namespace std;

int nScreenWidth = 80;
int nScreenHeight = 30;
wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;


int Rotate(int px, int py, int r) {
	int pi = 0;
	//px, py are the original coordinates of the piece in question
	switch (r % 4) {
	case 0: 
		pi = py * 4 + px; //0
		break;
	case 1: 
		pi = 12 + py - (px * 4); //90
		break;
	case 2: 
		pi = 15 - (py * 4) - px; //180
		break;
	case 3: 
		pi = 3 - py + (px * 4); //270
		break;
	}
	return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY) {
	
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++) {
			//get index into piece
			int pi = Rotate(px, py, nRotation);
			//get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			//collision detection - check that piece is not going out of bounds
			if (nPosX + px >= 0 && nPosX + px < nFieldWidth) {
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight) {
					if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
						return false; //fail on first hit
				}
			}
		}
	return true;
}

int main() {
	

	//instead of using cout, this creates a screen buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	//ERROR FIXED - green line to say buffer overrun caused by incorrectly written for loop -> used '=' instead of '<'
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	

	//Create assets;
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");



	//Creating the playing field
	pField = new unsigned char[nFieldWidth * nFieldHeight]; //create play field
	for (int x = 0; x < nFieldWidth; x++) { //board boundary
		for (int y = 0; y < nFieldHeight; y++) {
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
		}
	}


	//Game Logic
	

	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;
	bool bKey[4];
	bool bRotateHold = false; //acts as a latch -> if the key is being held it is set to true; when it is true, the input cannot be registered. As such
							  // only one input registered per key 
	int nSpeed = 20;
	int nSpeedCounter = 0;
	bool bForceDown = false;
	int nPieceCount = 0;
	int nScore = 0;
	vector<int> vLines;
	bool bGameOver = false;

	while (!bGameOver) {
		// GAME TIMING
		this_thread::sleep_for(50ms); //one game tick
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed);


		// INPUT
		for (int k = 0; k < 4; k++)								   //R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0; //checks whether key pressed is true or false

		// GAME LOGIC

		nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		//lines above are a shorter way of writing these if statements -> possible as we are only updating one values per statement
		/*if (bkey[1]) {
			if (doespiecefit(ncurrentpiece, ncurrentrotation, ncurrentx - 1, ncurrenty)) {
				ncurrentx = ncurrentx - 1;
			}
		}
		if (bkey[0]) {
			if (doespiecefit(ncurrentpiece, ncurrentrotation, ncurrentx + 1, ncurrenty)) {
				ncurrentx = ncurrentx + 1;
			}
		}
		if (bkey[2]) {
			if (doespiecefit(ncurrentpiece, ncurrentrotation, ncurrentx, ncurrenty+1)) {
				ncurrenty = ncurrenty + 1;
			}
		}*/
		if (bKey[3]) {
			nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = true;
		}
		else
			bRotateHold = false;

		if (bForceDown) {
			nSpeedCounter = 0;
			nPieceCount++;
			//difficulty increased here
			if (nPieceCount % 10 == 0)
				if (nSpeed >= 10) nSpeed--; //speed is number of game ticks, so if it decreases -> less time between movement
			//test if piece can be moved
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; //if the piece can go down, it does go down
			else {
				//Lock current piece into the field
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				

				//Check if any full lines created
				for (int py = 0; py < 4; py++) 				
					if (nCurrentY + py < nFieldHeight - 1) 
					{
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++)
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

						if (bLine) 
						{
							//remove line, set to =
							for (int px = 1; px < nFieldWidth - 1; px++)
								pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							vLines.push_back(nCurrentY + py);
						}
					}

					nScore += 25;
					//the more lines completed in one go, the more points
					if (!vLines.empty()) nScore += (1 << vLines.size()) * 100;

					//Choose next piece
					//ERROR SOLVED - originally written as int nCurrentX = nFieldWidth / 2; -> creates new object as opposed to updating old one
					nCurrentX = nFieldWidth / 2;
					nCurrentY = 0;
					nCurrentRotation = 0;
					nCurrentPiece = rand() % 7;


					//If piece does not fit
					bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
				}
				
			}

			// RENDER OUTPUT



			//DISPLAY

			//unless gameover, draw the playing field
			//draw field
			for (int x = 0; x < nFieldWidth; x++)
				for (int y = 0; y < nFieldHeight; y++)
					//indexing with the provided string allows for each tetris piece to be represent by the corresponding letter/character
					//can alter this here to look better using the DOS characters

					screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=\u2588"[pField[y * nFieldWidth + x]];

			//draw piece
			for (int px = 0; px < 4; px++)
				for (int py = 0; py < 4; py++)
					if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
						screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

			//Draw score
			swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

			//animate lines
			if (!vLines.empty()) {
				//display frame
				WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
				this_thread::sleep_for(400ms); //delay a bit

				for (auto& v : vLines)
					for (int px = 1; px < nFieldWidth - 1; px++) {
						for (int py = v; py > 0; py--)
							pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
						pField[px] = 0;
					}
				vLines.clear();
				
			}
			//display frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth* nScreenHeight, { 0, 0 }, & dwBytesWritten);
			
	}
	//Game over
	CloseHandle(hConsole);
	cout << "Game Over! Score: " << nScore << endl;
	system("pause");

	return 0;
}
