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

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

template<int ROWS, int COLS, class GAME_PIECE, int PLAYER_NUM = 2>
class GameBoard {
public:

	GameBoard(): m_rows(ROWS), m_cols(COLS), m_player_num(PLAYER_NUM) {
		// init board
		for (int i = 0; i < m_rows; ++i) {
			for (int j = 0; j < m_cols; ++j) {
				m_board[i][j].first = -1;
			}
		}
	}
	~GameBoard() = default;

	GameBoard operator=(const GameBoard&) = delete;

	GameBoard(const GameBoard&) = delete;

	void print() { std::cout << m_rows << " " << m_cols << " " << m_player_num << std::endl; }

	// getPiece
	PieceInfo<GAME_PIECE> getPiece(int row, int col) {
		if (m_board[row][col].first == -1) {
			return nullptr;
		}
		return std::make_unique<const std::pair<int, GAME_PIECE>>
				(std::make_pair(m_board[row][col].first, m_board[row][col].second));
	}

	// setPiece
	PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player) {
		PieceInfo<GAME_PIECE> l_prevPiece = getPiece(row, col);
		m_board[row][col] = std::make_pair(player, piece);
		return l_prevPiece;
	}

	class GeneralIter {
	public:
		GeneralIter(int a_rows, int a_cols, int a_row, int a_col, std::pair<int, GAME_PIECE>* a_iter_pos = nullptr):
			m_rows(a_rows), m_cols(a_cols), m_row(a_row), m_col(a_col), m_iter_pos(a_iter_pos) {}
		const std::tuple<int, int, GAME_PIECE, int> operator*() {
			return std::tuple<int, int, GAME_PIECE, int>(m_row, m_col, m_iter_pos->second, m_iter_pos->first);
		}
		GeneralIter operator++() {
			// get to end of row
			std::pair<int, GAME_PIECE>* l_temp = m_iter_pos + 1;
			for (int i = m_col + 1; i < m_cols; ++i, ++l_temp) {
				if (l_temp->first != -1) {
					m_iter_pos = l_temp;
					return GeneralIter(m_rows, m_cols, m_row, i, ++m_iter_pos);
				}
			}
			for (int i = m_row + 1; i < m_rows; ++i) {
				for (int j = 0; j < m_cols; ++j, ++l_temp) {
					if (l_temp->first != -1) {
						m_iter_pos = l_temp;
						return GeneralIter(m_rows, m_cols, i, j, m_iter_pos);
					}
				}
			}
			m_iter_pos = l_temp;
			return GeneralIter(m_rows, m_cols, m_rows, m_rows, m_iter_pos);
		}
		bool operator!= (GeneralIter a_otherGenIter) {
			return m_iter_pos != a_otherGenIter.m_iter_pos;
		}
	private:
		int m_rows;
		int m_cols;
		int m_row;
		int m_col;
		std::pair<int, GAME_PIECE>* m_iter_pos;
	};

	GeneralIter begin() {
		return GeneralIter(m_rows, m_cols, 0, 0, &m_board[0][0]);
	}
	GeneralIter end() {
		return GeneralIter(m_rows, m_cols, -1, -1, &m_board[m_rows-1][m_cols-1] + 1);
	}

private:
	int m_rows;
	int m_cols;
	int m_player_num;
	//std::vector<std::vector<std::shared_ptr<std::pair<int, GAME_PIECE>>>> m_board;
	std::pair<int, GAME_PIECE> m_board[ROWS][COLS];
};



#endif /* GAMEBOARD_H_ */
