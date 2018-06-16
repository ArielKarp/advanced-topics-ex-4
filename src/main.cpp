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
	A(int _i): i(_i) {
	}
	bool operator==(const A& other) {
		if (i == other.i) {
			return true;
		}
		return false;
	}
private:
	int i;
};

// test iter with string and 4 players
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

// Test allPiecesOfPlayer with string, and 4 players
static bool test2(){
	GameBoard<12, 7, string, 4> board;
	int i = 0;
	for (int row = 0; row < 12; row++){
		for(int col =0; col < 7; col++){
			board.setPiece(row, col, "Piece", i);
			i = (i+1)%4;
		}
	}

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

// Test by piece with a class without empty c'tor
static bool test3(){
	GameBoard<12, 7, A, 4> board;
	int i = 0;
	for (int row = 0; row < 12; row++){
		for(int col =0; col < 7; col++){
			board.setPiece(row, col, A(i), i);
			i = (i+1)%4;
		}
	}

	A a(0);
	int occurence[4]= {0,0,0,0};
	for(auto pieceInfo : board.allOccureneceOfPiece(a)){
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

// search for players piece, class string
static bool test4(){
	GameBoard<10, 10, int, 4> board;
	// set all board for 0, for all players
	int i = 0;
	for (int row = 0; row < 10; row++){
		for(int col =0; col < 10; col++){
			board.setPiece(row, col, 0, i);
			i = (i+1)%4;
		}
	}

	int occurence[4]= {0,0,0,0};
	// all occurences of player 4 of
	for(auto pieceInfo : board.allOccureneceOfPieceForPlayer(0, 3)){
		occurence[get<3>(pieceInfo)]++;
	}
	for(int i = 0;i<3;i++){
		if (occurence[i] != 0){
			return false;
		}
	}
	if (occurence[3] != 25) {
		return false;
	}
	return true;
}

// test iter by player with class int

// test general iter with default numbers of players

// test for elem that is not in board

// test for empty board


// **************************** Abnormals ***********************************************

// invalid player

// invalid class piece to search



int main(int argc, char* argv[]) {
	RUN_TEST(test1);
	RUN_TEST(test2);
	RUN_TEST(test3);
	RUN_TEST(test4);
	return 0;
}


