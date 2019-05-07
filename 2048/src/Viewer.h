#ifndef VIEWER_H
#define VIEWER_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iomanip>
#include <iostream>
#include <tuple>
#include "StateTracker.h"
#include <unordered_map>

class Viewer {
   private:
    std::unordered_map<int, sf::RectangleShape> rec;
    std::unordered_map<int, sf::Text> text;
    std::vector<std::vector<sf::Vector2f>> pos;
    sf::Font font;
    StateTracker& st;
    sf::RenderWindow& window;

   public:
       
    Viewer(StateTracker& _st, sf::RenderWindow& _window): st(_st), window(_window) {
        rec = std::unordered_map<int, sf::RectangleShape>();
        text = std::unordered_map<int, sf::Text>();
        pos = std::vector<std::vector<sf::Vector2f>>(4, std::vector<sf::Vector2f>(4));
        font = sf::Font();

        if (!font.loadFromFile("../res/DejaVuSans.ttf")) {
            throw "Failed to load font!";
        }
        int n = 1;
        for (int i=1; i<=13; ++i) {
            n *= 2;
            // set rectangular
            rec.emplace(n, sf::Vector2f(100, 100));
            rec[n].setFillColor(sf::Color(255, 255*(1-i/13.), 0));
            rec[n].setOutlineThickness(5);
            rec[n].setOutlineColor(sf::Color::Black);
            // set text
            text.emplace(n, sf::Text(std::to_string(n), font, 30));
            text[n].setFillColor(sf::Color::Black);
            text[n].setStyle(sf::Text::Bold | sf::Text::Underlined);
        }

        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                pos[i][j].x = 100*(j+1);
                pos[i][j].y = 100*(i+1);
                // pos[i][j] = sf::Vector2f(100*(i+1), 100*(j+1));
            }
        }
    }

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

    void drawBoard() {
        // debug info on consol
        printBoard();

        // draw rectangles at appropriate position
        auto board = st.getBoard();
        int n = board.size(), m = board[0].size(), number;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                number = board[i][j];
                if (number!=0) {
                    rec[number].setPosition(pos[i][j]);
                    text[number].setPosition(pos[i][j]);
                    window.draw(rec[number]);
                    window.draw(text[number]);
                }
            }
        }
            
        // create
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
