/*
 * main.cpp
 *
 *  Created on: May 31, 2018
 *      Author: ariel
 */

#include <string>
#include <utility>


#include "GameBoard.h"
//#include "ex4_header.h"
#include "unit_test_util.h"

using std::string;
using std::get;
using std::cout;
using std::endl;


class A{
public:
	A() = delete;
	A(int i) {
		std::cout << "not in default" << std::endl;
	}
};

static bool test1(){
	GameBoard<12, 7, string, 4> board;
	int i = 0;
	for (int row = 0; row < 12; row++){
		for(int col =0; col < 7; col++){
			board.setPiece(row, col, "Piece", i);
			i = (i+1)%4;
		}
	}

	int occurence[4]= {0,0,0,0};
	for(auto pieceInfo : board){
		occurence[get<3>(pieceInfo)]++;
	}
	for(int i = 0;i<4;i++){
		if (occurence[i] != 21){
			return false;
		}
	}
	return true;
}

static bool test2(){
	GameBoard<12, 7, string, 4> board;
	int i = 0;
	for (int row = 0; row < 12; row++){
		for(int col =0; col < 7; col++){
			board.setPiece(row, col, "Piece", i);
			i = (i+1)%4;
		}
	}

	cout << "print board" << endl;

	for (int row = 0; row < 12; row++){
		for(int col =0; col < 7; col++){
			cout << (board.getPiece(row, col)->first) << " ";
		}
		cout << endl;
	}
	cout << endl;
	int occurence[4]= {0,0,0,0};
	for(auto pieceInfo : board.allPiecesOfPlayer(0)){
		occurence[get<3>(pieceInfo)]++;
	}
	for(int i = 1;i<4;i++){
		if (occurence[i] != 0){
			return false;
		}
	}
	if (occurence[0] != 21) {
		return false;
	}
	return true;
}



int main(int argc, char* argv[]) {
	RUN_TEST(test1);
	RUN_TEST(test2);
	return 0;
}


