#ifndef VIEWER_H
#define VIEWER_H

#include <conio.h>
#include <iomanip>
#include <iostream>
#include <tuple>
#include "StateTracker.h"

class Viewer {
   private:
    StateTracker& st;

   public:
    Viewer(StateTracker& t) : st(t) {}

    void printBoard() {
        auto board = st.getBoard();
        for (const auto& row : board) {
            for (const auto& num : row) {
                std::cout << std::setw(10) << (num == 0 ? 0 : num);
            }
            std::cout << "\n\n";
        }
        std::cout << "\n\n";
    }

    static void clearScreen() {
        // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
        // clrscr();
        // std::cout << "\033c";
    }

    void printFinal() {
        auto max_cell = std::get<1>(st.getStatistics());
        std::cout << ((max_cell < 2048)
                          ? "*** You Failed! Try harder next time :) ***\n"
                          : "*** You Won! ***\n");
        printStatistics();
    }

    void printQuit() {
        std::cout << "*** You Quit! ***\n";
        printStatistics();
    }

    void printStatistics() {
        auto [score, max_cell] = st.getStatistics();
        std::cout << "You made a " << max_cell << " cell!\n";
        std::cout << "Total score " << score << "\n";
    }
};

#endif
