/*
 * main.cpp
 *
 *  Created on: May 31, 2018
 *      Author: ariel
 */


#include "GameBoard.h"

int main(int argc, char* argv[]) {
	GameBoard<10, 10, char> board1;
	board1.print();
	board1.setPiece(0, 0, '0', 0);
	board1.setPiece(1, 1, '1', 0);
	board1.setPiece(2, 2, '2', 0);
	for (auto& l_piece : board1) {
		std::cout << std::get<2>(l_piece) << std::endl;
	}
//	GameBoard<10, 10, char>::GeneralIter iter = board1.begin();
//	for (; iter != board1.end(); ++iter) {
//		std::cout << std::get<2>(*iter) << std::endl;
//	}
	return 0;
}


