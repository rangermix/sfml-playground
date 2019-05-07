#ifndef OPERATOR_H
#define OPERATOR_H

// #include <curses.h>
#include <conio.h>
#include <iostream>
#include "StateTracker.h"

class Listener {
   private:
    StateTracker& st;
    static constexpr int UP = 72;
    static constexpr int DOWN = 80;
    static constexpr int LEFT = 75;
    static constexpr int RIGHT = 77;
    StateTracker::Direction map[100];

   public:
    Listener(StateTracker& t) : st(t) {
        map[UP] = StateTracker::Direction::UP;
        map[DOWN] = StateTracker::Direction::DOWN;
        map[LEFT] = StateTracker::Direction::LEFT;
        map[RIGHT] = StateTracker::Direction::RIGHT;
    }

    bool listen() {
        int c = 0;
        bool finished = false;
        while (!finished) {
            finished = true;
            c = _getch();
            if (c==UP||c==DOWN||c==LEFT||c==RIGHT) {
                st.move(map[c]);
            } else if (c=='q') {
                return false;
            } else {
                finished = false;
            }
            // switch ((c = _getch())) {
            //     case UP:
            //     case DOWN:
            //     case LEFT:
            //     case RIGHT:
            //         st.move(map[c]);
            //         break;
            //     case 'q':
            //         exit(0);
            //     default:
            //         finished = false;
            //         break;
            // }
        }
        return true;
    }
};

#endif
