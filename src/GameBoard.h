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
using std::make_shared;
using std::make_pair;
using std::shared_ptr;
using std::pair;
using std::cout;
using std::endl;

enum class SearchType { REG, BY_PLAYER, BY_PIECE, BOTH };

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

template<int ROWS, int COLS, class GAME_PIECE, int PLAYER_NUM = 2>
class GameBoard {
public:

	GameBoard() {}
	~GameBoard() {}

	GameBoard operator=(const GameBoard&) = delete;

	GameBoard(const GameBoard&) = delete;

	PieceInfo<GAME_PIECE> getPiece(int row, int col) {
		if (m_board[row][col] == nullptr) {
			return nullptr;
		}
		return std::make_unique<const pair<int, GAME_PIECE>>(make_pair(m_board[row][col]->getPlayerNum(), m_board[row][col]->getPiece()));
	}

	PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player) {
		// TODO: check player limits
		PieceInfo<GAME_PIECE> l_prevPiece = getPiece(row, col);
		if (m_board[row][col] == nullptr) {
			// new piece
			m_board[row][col] = make_shared<Elem>(player, piece);
		} else {
			m_board[row][col]->setPlayerNum(player);
			m_board[row][col]->setGamePiece(piece);
		}
		return l_prevPiece;
	}

	// ********************************* Elem class, represents an entry in m_baord  *****************************
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


	// ************************************************** iter class *********************************************
	class iter {
	public:
		// reg iter
		iter(int a_row, int a_col, shared_ptr<Elem> (&a_boardHandle)[ROWS][COLS]):
		m_row(a_row), m_col(a_col), m_boardHandle(a_boardHandle), m_iterType(SearchType::REG){
			m_pieceToSearch = nullptr;
			m_playerNum = -1;
		}

		// by player iter
		iter(int a_row, int a_col, shared_ptr<Elem> (&a_boardHandle)[ROWS][COLS], int playerNum):
			m_row(a_row), m_col(a_col), m_boardHandle(a_boardHandle), m_iterType(SearchType::BY_PLAYER), m_playerNum(playerNum){
			m_pieceToSearch = nullptr;
		}

		// by piece iter
		iter(int a_row, int a_col, shared_ptr<Elem> (&a_boardHandle)[ROWS][COLS], GAME_PIECE piece, SearchType a_searchType):
			m_row(a_row), m_col(a_col), m_boardHandle(a_boardHandle), m_iterType(a_searchType){
			m_pieceToSearch = std::make_shared<GAME_PIECE>(piece);
			m_playerNum = -1;
		}

		// by players piece
		iter(int a_row, int a_col, shared_ptr<Elem> (&a_boardHandle)[ROWS][COLS], GAME_PIECE piece, int playerNum):
			m_row(a_row), m_col(a_col), m_boardHandle(a_boardHandle), m_iterType(SearchType::BOTH){
			m_pieceToSearch = std::make_shared<GAME_PIECE>(piece);
			m_playerNum = playerNum;
		}

		const std::tuple<int, int, GAME_PIECE, int> operator*() {
			return std::tuple<int, int, GAME_PIECE, int>(m_row, m_col, m_boardHandle[m_row][m_col]->getPiece(), m_boardHandle[m_row][m_col]->getPlayerNum());
		}

		bool testConditionBySearchType(int a_row, int a_col) {
			if (m_iterType == SearchType::REG) {
				if (m_boardHandle[a_row][a_col] != nullptr) {
					return true;
				} else {
					return false;
				}
			} else if (m_iterType == SearchType::BY_PLAYER) {
				if (m_boardHandle[a_row][a_col] != nullptr && m_boardHandle[a_row][a_col]->getPlayerNum() == m_playerNum) {
					return true;
				}
				else {
					return false;
				}
			} else if (m_iterType == SearchType::BY_PIECE) {
				if (m_boardHandle[a_row][a_col] != nullptr && m_boardHandle[a_row][a_col]->getPiece() == *m_pieceToSearch) {
					return true;
				} else {
					return false;
				}
			}
			// search type is both
			if (m_boardHandle[a_row][a_col] != nullptr && m_boardHandle[a_row][a_col]->getPlayerNum() == m_playerNum
					&& m_boardHandle[a_row][a_col]->getPiece() == *m_pieceToSearch) {
				return true;
			} else {
				return false;
			}
		}

		iter& operator++() {
			//get to end of row
			for (int j = (m_col + 1); j < COLS; ++j) {
				if (testConditionBySearchType(m_row, j)) {
					// updating location of this
					m_col = j;
					return *this;
				}
			}

			// go over the board
			for (int i = (m_row + 1); i < ROWS; ++i) {
				for (int j = 0; j < COLS; ++j) {
					if (testConditionBySearchType(i, j)) {
						// updating location of this
						m_row = i;
						m_col = j;
						return *this;
					}
				}
			}
			// in case not on board, set to end
			m_row = ROWS;
			m_col = COLS;
			return *this;
		}
		bool operator!= (iter a_otherGenIter) {
			if (m_row != a_otherGenIter.m_row || m_col != a_otherGenIter.m_col) {
				return true;
			}
			return false;
			//return m_pos != a_otherGenIter.m_pos;
		}
	private:
		int m_row;
		int m_col;
		shared_ptr<GAME_PIECE> m_pieceToSearch;
		shared_ptr<Elem> m_boardHandle[ROWS][COLS];
		SearchType m_iterType;
		int m_playerNum;
	};

	// ********************************* iter begin & end for GameBoard class ************************************
	iter begin() {
		// search for starting pos
		int l_row = 0;
		int l_col = 0;
		int flag = 0;
		for (int i = 0; i < ROWS && !flag; ++i) {
			for (int j = 0; j < COLS; ++j) {
				if (m_board[i][j] != nullptr) {
					l_row = i;
					l_col = j;
					flag = 1;
					break;
				}
			}
		}
		// if not found
		if (!flag) {
			l_row = ROWS;
			l_col = COLS;
		}
		return iter(l_row, l_col, m_board);
	}

	iter end() {
		return iter(ROWS, COLS, m_board);
	}

	// ***********************************************************************************************************

	// ******************************************** Filter classes ***********************************************

	class FilterBoardByPlayer {
	public:
		// Filter by player num
		FilterBoardByPlayer(int a_playerNum, shared_ptr<Elem> (&a_boardHandle)[ROWS][COLS]):
		m_playerNum(a_playerNum), m_boardHandle(a_boardHandle){}

		iter begin() {
			// search for starting pos
			int l_row = 0;
			int l_col = 0;
			int flag = 0;
			for (int i = 0; i < ROWS && !flag; ++i) {
				for (int j = 0; j < COLS; ++j) {
					if (m_boardHandle[i][j] != nullptr && m_boardHandle[i][j]->getPlayerNum() == m_playerNum) {
						l_row = i;
						l_col = j;
						flag = 1;
						break;
					}
				}
			}
			// if not found
			if (!flag) {
				l_row = ROWS;
				l_col = COLS;
			}
			return iter(l_row, l_col, m_boardHandle, m_playerNum);
		}
		iter end() {
			return iter(ROWS, COLS, m_boardHandle, m_playerNum);
		}
	private:
		int m_playerNum;
		shared_ptr<Elem> m_boardHandle[ROWS][COLS];
	};

	class FilterBoardByPiece {
	public:
		// Filter by piece
		FilterBoardByPiece(GAME_PIECE a_piece, shared_ptr<Elem> (&a_boardHandle)[ROWS][COLS]):
			m_piece(a_piece), m_boardHandle(a_boardHandle){}

		iter begin() {
			// search for starting pos
			int l_row = 0;
			int l_col = 0;
			int flag = 0;
			for (int i = 0; i < ROWS && !flag; ++i) {
				for (int j = 0; j < COLS; ++j) {
					if (m_boardHandle[i][j] != nullptr && m_boardHandle[i][j]->getPiece() == m_piece) {
						l_row = i;
						l_col = j;
						flag = 1;
						break;
					}
				}
			}
			// if not found
			if (!flag) {
				l_row = ROWS;
				l_col = COLS;
			}
			return iter(l_row, l_col, m_boardHandle, m_piece, SearchType::BY_PIECE);
		}
		iter end() {
			return iter(ROWS, COLS, m_boardHandle, m_piece, SearchType::BY_PIECE);
		}
	private:
		GAME_PIECE m_piece;
		shared_ptr<Elem> m_boardHandle[ROWS][COLS];
	};

	class FilterBoardByPlayerPiece {
	public:
		// Filter by player's piece
		FilterBoardByPlayerPiece(GAME_PIECE a_piece, int a_playerNum, shared_ptr<Elem> (&a_boardHandle)[ROWS][COLS]):
			m_piece(a_piece), m_playerNum(a_playerNum), m_boardHandle(a_boardHandle){}

		iter begin() {
			// search for starting pos
			int l_row = 0;
			int l_col = 0;
			int flag = 0;
			for (int i = 0; i < ROWS && !flag; ++i) {
				for (int j = 0; j < COLS; ++j) {
					if (m_boardHandle[i][j] != nullptr && m_boardHandle[i][j]->getPlayerNum() == m_playerNum && m_boardHandle[i][j]->getPiece() == m_piece) {
						l_row = i;
						l_col = j;
						flag = 1;
						break;
					}
				}
			}
			// if not found
			if (!flag) {
				l_row = ROWS;
				l_col = COLS;
			}
			return iter(l_row, l_col, m_boardHandle, m_piece, m_playerNum);
		}
		iter end() {
			return iter(ROWS, COLS, m_boardHandle, m_piece, m_playerNum);
		}
	private:
		GAME_PIECE m_piece;
		int m_playerNum;
		shared_ptr<Elem> m_boardHandle[ROWS][COLS];
	};

	// ***********************************************************************************************************

	FilterBoardByPlayer allPiecesOfPlayer(int a_playerNum) {
		return FilterBoardByPlayer(a_playerNum, m_board);
	}

	FilterBoardByPiece allOccureneceOfPiece(GAME_PIECE a_piece) {
		return FilterBoardByPiece(a_piece, m_board);
	}

	FilterBoardByPlayerPiece allOccureneceOfPieceForPlayer(GAME_PIECE a_piece, int a_playerNum) {
		return FilterBoardByPlayerPiece(a_piece, a_playerNum, m_board);
	}


private:
	shared_ptr<Elem> m_board[ROWS][COLS];
};



#endif /* GAMEBOARD_H_ */
