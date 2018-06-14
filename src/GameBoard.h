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
#include <tuple>

//using std::make_unqiue;
using std::unique_ptr;
using std::make_pair;
using std::pair;
using std::cout;
using std::endl;

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

template<int ROWS, int COLS, class GAME_PIECE, int PLAYER_NUM = 2>
class GameBoard {
public:

	GameBoard(): m_rows(ROWS), m_cols(COLS), m_player_num(PLAYER_NUM) {
		// init board
		m_board = new Elem**[m_rows];
		for (int i = 0; i < m_rows; ++i) {
			m_board[i] = new Elem*[m_cols];
		}
		for (int i = 0; i < m_rows; ++i) {
			for (int j = 0; j < m_cols; ++j) {
				m_board[i][j] = nullptr;
			}
		}

	}
	~GameBoard() {
		for (int i = 0; i < m_rows; ++i) {
			delete[] m_board[i];
		}
		delete[] m_board;
	}

	GameBoard operator=(const GameBoard&) = delete;

	GameBoard(const GameBoard&) = delete;

	void print() { }
	// getPiece
	PieceInfo<GAME_PIECE> getPiece(int row, int col) {
		if (m_board[row][col] == nullptr) {
			return nullptr;
		}
		return std::make_unique<const pair<int, GAME_PIECE>>(make_pair(m_board[row][col]->getPlayerNum(), m_board[row][col]->getPiece()));
	}

	// setPiece
	PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player) {
		PieceInfo<GAME_PIECE> l_prevPiece = getPiece(row, col);
		if (m_board[row][col] == nullptr) {
			// new piece
			m_board[row][col] = new Elem(player, piece);
		} else {
			m_board[row][col]->setPlayerNum(player);
			m_board[row][col]->setGamePiece(piece);
		}
		return l_prevPiece;
	}

	class Elem{
	public:
		Elem(int a_playerNum, const GAME_PIECE& a_piece) : m_playerNum(a_playerNum), m_piece(a_piece) {}
		int getPlayerNum() const { return m_playerNum; }
		GAME_PIECE getPiece() const { return m_piece; }
		void setPlayerNum(int a_playerNum) { m_playerNum = a_playerNum; }
		void setGamePiece(GAME_PIECE a_gamePiece) {m_piece = a_gamePiece; }
	private:
		int m_playerNum;
		GAME_PIECE m_piece;
	};

	class GeneralIter {
	public:
		GeneralIter(int a_rows, int a_cols, int a_row, int a_col, Elem*** a_boardHandle = nullptr):
		m_rows(a_rows), m_cols(a_cols), m_row(a_row), m_col(a_col), m_boardHandle(a_boardHandle){
		}

		const std::tuple<int, int, GAME_PIECE, int> operator*() {
			return std::tuple<int, int, GAME_PIECE, int>(m_row, m_col, m_boardHandle[m_row][m_col]->getPiece(), m_boardHandle[m_row][m_col]->getPlayerNum());
		}
		GeneralIter& operator++() {

			// get to end of row
			for (int j = (m_col + 1); j < m_cols; ++j) {
				if (m_boardHandle[m_row][j] != nullptr) {
					cout << "Inside location : i " << m_row << " j :" << j << endl;
					m_col = j;
					return *this;
				}
			}

			// go over the board
			for (int i = (m_row + 1); i < m_rows; ++i) {
				for (int j = 0; j < m_cols; ++j) {
					if (m_boardHandle[i][j] != nullptr) {
						m_row = i;
						m_col = i;
						return *this;
					}
				}
			}
			// in case not on board, set to end
			m_row++;
			m_col++;
			return *this;
		}
		bool operator!= (GeneralIter a_otherGenIter) {
			if (m_row != a_otherGenIter.m_row || m_col != a_otherGenIter.m_col) {
				return true;
			}
			return false;
			//return m_pos != a_otherGenIter.m_pos;
		}
	private:
		int m_rows;
		int m_cols;
		int m_row;
		int m_col;
		Elem*** m_boardHandle;
	};

	GeneralIter begin() {
		// search for starting pos
		int l_row = 0;
		int l_col = 0;
		int flag = 0;
		for (int i = 0; i < m_rows && !flag; ++i) {
			for (int j = 0; j < m_cols; ++j) {
				if (m_board[i][j] != nullptr) {
					l_row = i;
					l_col = j;
					flag = 1;
					break;
				}
			}
		}
		return GeneralIter(m_rows, m_cols, l_row, l_col, m_board);
	}
	GeneralIter end() {
		return GeneralIter(m_rows, m_cols, m_rows, m_cols);
	}

private:
	Elem*** m_board;
	int m_rows;
	int m_cols;
	int m_player_num;
};



#endif /* GAMEBOARD_H_ */
