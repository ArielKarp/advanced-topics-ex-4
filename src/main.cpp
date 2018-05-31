/*
 * main.cpp
 *
 *  Created on: May 31, 2018
 *      Author: ariel
 */


#include "GameBoard.h"

int main(int argc, char* argv[]) {
	GameBoard<5, 10, char> board1;
	board1.print();
	return 0;
}


