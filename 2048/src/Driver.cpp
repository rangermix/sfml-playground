// #include "Listener.h"
#include "StateTracker.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include "Viewer.h"

int main(int argc, char const* argv[]) {
    StateTracker st;
    sf::RenderWindow window(sf::VideoMode(800, 600), "2048_SFML");
    window.setFramerateLimit(60);
    Viewer viewer(st, window);

    while (window.isOpen()) {
        // handle window event
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Left:
                    st.move(StateTracker::Direction::LEFT);
                    break;
                case sf::Keyboard::Key::Right:
                    st.move(StateTracker::Direction::RIGHT);
                    break;
                case sf::Keyboard::Key::Up:
                    st.move(StateTracker::Direction::UP);
                    break;
                case sf::Keyboard::Key::Down:
                    st.move(StateTracker::Direction::DOWN);
                    break;
                case sf::Keyboard::Q:
                    window.close();
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        // draw the game
        window.clear();
        viewer.drawBoard();
        window.display();
    }


    // // Viewer viewer(st);
    // Listener listener(st);
    // while (1) {
    //     // viewer.clearScreen();
    //     // viewer.printBoard();
    //     if (st.isClosed()) {
    //         // viewer.printFinal();
    //         break;
    //     }
    //     if (!listener.listen()) {
    //         // viewer.printQuit();
    //         break;
    //     }
    // }
    return 0;
}
