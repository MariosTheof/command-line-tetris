// commandLineTetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <Windows.h>

using namespace std;

wstring tetromino[7]; // tetris shapes

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 30;			// Console Screen Size Y (rows)
// Size of game board. Based on the cell dimensions of classic gameboy
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr; // stores the elements of the field in an unsigned char array. will be dynamically allocated

// Rotation function. Rotates tetromino shapes
int Rotate(int px, int py, int r) {
	switch (r % 4) {
		case 0: return py * 4 + px;
		case 1: return 12 + py - (px * 4);
		case 3: return 3 - py + (px * 4);
	}
	return 0;
}

void initGameBoard() {
	pField = new unsigned char[nFieldHeight * nFieldWidth];
	for (int x = 0; x < nFieldWidth; x++) {
		for (int y = 0; y < nFieldHeight; y++) {
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldHeight - 1 || y == nFieldHeight - 1) ? 9 : 0;
		}
	}
}

int main()
{
    //Create assets ?? shapes
	//Line
	tetromino[0].append(L"..X."); // 4 by 4 . 4 elements wide and 4 elements deep
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	//squiqly 'S' line
	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");
	//squiqly 'S' line inverted
	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");
	//square
	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");
	// line with a hunchback
	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");
	// crane
	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");
	//inverted crane
	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");

	//initialize gameBoard
	initGameBoard();

	// Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	

	return 0;
}