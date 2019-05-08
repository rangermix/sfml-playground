// #include "Listener.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include "StateTracker.h"
#include "Viewer.h"

int main(int argc, char const* argv[]) {
    StateTracker st;
    sf::RenderWindow window(sf::VideoMode(800, 600), "2048_SFML");
    window.setFramerateLimit(60);
    Viewer viewer(st, window);

    while (window.isOpen()) {
        // handle window event
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            // close window
            case sf::Event::Closed:
                window.close();
                break;
            // move or quit
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                // move
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
                // quit
                case sf::Keyboard::Q:
                    if (!st.isClosed()) {
                        window.clear();
                        viewer.drawQuit();
                        window.display();
                        sf::sleep(sf::seconds(1.2));
                    }
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
        viewer.drawGame();
        window.display();
    }

    return 0;
}
