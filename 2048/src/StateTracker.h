#ifndef STATE_TRACKER_H
#define STATE_TRACKER_H

#include <functional>
#include <random>
#include <tuple>
#include <unordered_set>
#include <vector>

class StateTracker {
   private:
    std::vector<std::vector<int>> board;
    static std::hash<int> int_hasher;
    std::function<size_t(const std::tuple<int, int>&)> hash =
        [](const auto& t) {
            auto [i, j] = t;
            return int_hasher(10 * i + j);
        };
    std::unordered_set<std::tuple<int, int>, decltype(hash)> freeBlocks;
    std::default_random_engine rand_gen;
    int size;
    bool closed = false;
    int score = 0;
    int max_cell = 0;

    auto findEmpty() {
        if (freeBlocks.empty())
            return std::make_tuple(-1, -1);

        std::uniform_int_distribution distribution(
            0, static_cast<int>(freeBlocks.size()) - 1);
        int i = distribution(rand_gen);
        auto it = freeBlocks.begin();

        while (i--) {
            ++it;
        }
        auto tup = *it;
        freeBlocks.erase(it);
        return tup;
    }

    // TODO: add end of game
    void generateBlock() {
        auto [i, j] = findEmpty();
        if (i == -1 && j == -1) {
            closeGame();
            return;
        }
        std::uniform_int_distribution distribution(
            0, 9);
        int val = distribution(rand_gen);
        update(i, j, val==9?4:2);
    }

    void update(int i, int j, int val) {
        board[i][j] = val;
        if (val == 0) {
            freeBlocks.emplace(i, j);
        } else {
            freeBlocks.erase(std::make_tuple(i, j));
        }
    }

    void closeGame() { closed = true; }

   public:
    enum Direction { UP, RIGHT, DOWN, LEFT };

    StateTracker(int n = 4)
        : board(std::vector<std::vector<int>>(n, std::vector<int>(n))),
          freeBlocks(
              std::unordered_set<std::tuple<int, int>, decltype(hash)>(16,
                                                                       hash)),
          size(n) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                freeBlocks.emplace(i, j);
            }
        }
        generateBlock();
        generateBlock();
    }

    void move(Direction drc) {
        int fix;
        int init_curr =
            (drc == Direction::UP || drc == Direction::LEFT) ? 0 : size - 1;
        int init_next =
            (drc == Direction::UP || drc == Direction::LEFT) ? 1 : size - 2;
        int delta = (drc == Direction::UP || drc == Direction::LEFT) ? 1 : -1;

        int pcurr = init_curr;
        int pnext = init_next;
        int& p1 =
            (drc == Direction::UP || drc == Direction::LEFT) ? pnext : pcurr;
        int& p2 =
            (drc == Direction::UP || drc == Direction::LEFT) ? pcurr : pnext;

        int& row_curr =
            (drc == Direction::UP || drc == Direction::DOWN) ? pcurr : fix;
        int& row_next =
            (drc == Direction::UP || drc == Direction::DOWN) ? pnext : fix;
        int& col_curr =
            (drc == Direction::LEFT || drc == Direction::RIGHT) ? pcurr : fix;
        int& col_next =
            (drc == Direction::LEFT || drc == Direction::RIGHT) ? pnext : fix;

        bool moved = false;

        for (fix = 0; fix < size; ++fix) {
            pcurr = init_curr;
            pnext = init_next;
            while (pnext < size && pnext >= 0) {
                if (board[row_next][col_next] != 0) {
                    if (board[row_curr][col_curr] == 0) {
                        update(row_curr, col_curr, board[row_next][col_next]);
                        update(row_next, col_next, 0);
                        moved = true;
                    } else if (board[row_next][col_next] ==
                               board[row_curr][col_curr]) {
                        update(row_curr, col_curr,
                               board[row_curr][col_curr] * 2);
                        update(row_next, col_next, 0);
                        moved = true;
                        pcurr += delta;
                    } else if (p1 - p2 > 1) {
                        pcurr += delta;
                        continue;
                    } else {
                        pcurr = pnext;
                    }
                }
                pnext += delta;
            }
        }

        if (moved)
            generateBlock();
        else if (freeBlocks.empty())
            closeGame();
    }

    const auto& getBoard() { return board; }

    const bool isClosed() { return closed; }

    const auto getStatistics() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                score += board[i][j];
                max_cell = std::max(max_cell, board[i][j]);
            }
        }
        return std::make_tuple(score, max_cell);
    }
};

#endif
