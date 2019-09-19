

#include <iostream>
#include <Windows.h>
using namespace std;

int nScreenWidth = 80;
int nScreenHeight = 50;
int nFieldWidth = 18;
int nFieldHeight = 18;
unsigned char* pField = nullptr;
bool bGameOver = false;

int main()
{
	/*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, 5);
	cout << "This is a test run" << endl;*/

	wchar_t* screen = new wchar_t[nScreenHeight * nScreenWidth];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	SetConsoleTextAttribute(hConsole, 5);

	while (!bGameOver)
	{
		pField = new unsigned char[nFieldWidth * nFieldHeight]; //create play field
		for (int x = 0; x < nFieldWidth; x++) { //board boundary
			for (int y = 0; y < nFieldHeight; y++) {
				pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
			}
		}

		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=\u2588"[pField[y * nFieldWidth + x]];
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
	}
	return 0;

}

