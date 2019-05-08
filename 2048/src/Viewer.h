#ifndef VIEWER_H
#define VIEWER_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window.hpp>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include "StateTracker.h"

class Viewer {
   private:
    std::unordered_map<int, sf::RectangleShape> rec;
    std::unordered_map<int, sf::Text> text;
    std::vector<std::vector<sf::Vector2f>> pos;
    sf::Font font;
    sf::Vector2f scorePos;
    StateTracker& st;
    sf::RenderWindow& window;

   public:
    Viewer(StateTracker& _st, sf::RenderWindow& _window)
        : st(_st), window(_window) {
        rec = std::unordered_map<int, sf::RectangleShape>();
        text = std::unordered_map<int, sf::Text>();
        pos = std::vector<std::vector<sf::Vector2f>>(
            4, std::vector<sf::Vector2f>(4));
        font = sf::Font();
        scorePos = sf::Vector2f(500, 0);

        if (!font.loadFromFile("../res/DejaVuSans.ttf")) {
            throw "Failed to load font!";
        }
        int n = 1;
        for (int i = 1; i <= 13; ++i) {
            n *= 2;
            // set rectangular
            rec.emplace(n, sf::Vector2f(100, 100));
            rec[n].setFillColor(sf::Color(255, 255 * (1 - i / 13.), 0));
            rec[n].setOutlineThickness(5);
            rec[n].setOutlineColor(sf::Color::Black);
            // set text
            text.emplace(n, sf::Text(std::to_string(n), font, 30));
            text[n].setFillColor(sf::Color::Black);
            text[n].setStyle(sf::Text::Bold | sf::Text::Underlined);
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                pos[i][j].x = 100 * (j + 1);
                pos[i][j].y = 100 * (i + 1);
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
                if (number != 0) {
                    rec[number].setPosition(pos[i][j]);
                    text[number].setPosition(pos[i][j]);
                    window.draw(rec[number]);
                    window.draw(text[number]);
                }
            }
        }
    }

    void printFinal() {
        auto max_cell = st.getMaxCell();
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
        std::cout << "You made a " << st.getMaxCell() << " cell!\n";
        std::cout << "Total score " << st.getScore() << "\n";
    }

    void drawScore() {
        sf::Text scoreText("Score: " + std::to_string(st.getScore()), font);
        scoreText.setPosition(scorePos);
        window.draw(scoreText);
    }

    void drawQuit() {
        // draw background
        drawGame();

        // draw transparent background
        sf::RectangleShape shade(sf::Vector2f(800, 600));
        shade.setFillColor(sf::Color(0, 0, 0, 130));
        window.draw(shade);

        // draw quit text
        sf::Text quitText("You Quit!", font, 80);
        quitText.setPosition(0, 300);
        window.draw(quitText);
    }

    void drawFinal() {
        // draw transparent background
        sf::RectangleShape shade(sf::Vector2f(800, 600));
        shade.setFillColor(sf::Color(0, 0, 0, 130));
        window.draw(shade);

        // draw quit text
        sf::Text finishText("You made a " + std::to_string(st.getMaxCell()) +
                                " cell!\nTotal score " +
                                std::to_string(st.getScore()),
                            font, 60);
        finishText.setPosition(0, 200);
        window.draw(finishText);
    }

    void drawGame() {
        drawBoard();
        drawScore();

        if (st.isClosed()) {
            drawFinal();
        }
    }
};

#endif
