// commandLineTetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <Windows.h>
#include <thread>

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

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY) {
	for (int px = 0; px < 4; px++) {
		for (int py = 0; py < 4; py++) {
			//Get index into piece
			int pi = Rotate(px, py, nRotation);
			//Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosX == px < nFieldWidth) {
					if (nPosY + py >= 0 && nPosY + py < nFieldHeight) {
						if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0) { // if the tetrimino's piece index is equal to 'X' and playingField not equal to zero
							return false;
						}
					}
			}
			
			}
		}
	return true;
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
	pField = new unsigned char[nFieldWidth*nFieldHeight]; // Create play field buffer
	for (int x = 0; x < nFieldWidth; x++) // Board Boundary
		for (int y = 0; y < nFieldHeight; y++)
			pField[y*nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	// Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	
	bool isGameOver = false;

	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bKey[4];
	bool bRotateHold = false;

	int difficulty = 20; // or speed
	int nSpeedCounter = 0;
	bool bForceDown = false;
	bool bGameOver = false;

	while (!isGameOver) {
		//Game timing ##################
		this_thread::sleep_for(60ms);

		//Pieces fall for every 1 sec
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == difficulty);

		// User Input ##################
		for (int k = 0; k < 4; k++)								// R   L   D Z //with their appropriate hex
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// Game Logic ##################
		if (bKey[1]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) {
				nCurrentX = nCurrentX - 1;
			}
		}
		if (bKey[0]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) {
				nCurrentX = nCurrentX + 1;
			}
		}
		if (bKey[2]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
				nCurrentY = nCurrentY + 1;
			}
		}
		// Rotate, but latch to stop wild spinning
		if (bKey[3])
		{
			nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
		}
		else
			bRotateHold = true;

		if (bForceDown) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
				nCurrentY = nCurrentY + 1;
			}
			else {

				//if piece doesn't fit
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);


				//choose next piece
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7; 

			}
			nSpeedCounter = 0;
		}

		// Render Output #################
		
		// Draw playing Field
		for (int x = 0; x < nFieldWidth; x++) {
			for (int y = 0; y < nFieldHeight; y++) {
				screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];

			}
		}
	
		// Draw current tetromino
		for (int px = 0; px < 4; px++) {
			for (int py = 0; py < 4; py++) {
				if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.'); {
					screen[(nCurrentY + py + 2)* nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65; // eg. 0 + 65 == A in ASCII

				}
			}
		}
	
	}

	return 0;
}