/*
 * GameBoard.h
 *
 *  Created on: May 31, 2018
 *      Author: ariel
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

#include <iostream>
#include <vector>
#include <memory>

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

template<int ROWS, int COLS, class GAME_PIECE, int PLAYER_NUM = 2>
class GameBoard {
public:
	GameBoard(): m_rows(ROWS), m_cols(COLS), m_player_num(PLAYER_NUM) {}
	~GameBoard() = default;
	GameBoard operator=(const GameBoard&) = delete;
	GameBoard(const GameBoard&) = delete;
	void print() { std::cout << m_rows << " " << m_cols << " " << m_player_num << std::endl; }
	PieceInfo<GAME_PIECE> getPiece(int row, int col) {
		if (!m_board[row][col]) {
			return nullptr;
		}
		return m_board[row][col];
	}
	PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player);
private:
	int m_rows;
	int m_cols;
	int m_player_num;
	std::vector<std::vector<PieceInfo<GAME_PIECE>> > m_board;
};



#endif /* GAMEBOARD_H_ */
