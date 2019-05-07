#include "Listener.h"
#include "StateTracker.h"
#include "Viewer.h"

int main(int argc, char const* argv[]) {
    StateTracker st;
    Viewer viewer(st);
    Listener listener(st);
    while (1) {
        viewer.clearScreen();
        viewer.printBoard();
        if (st.isClosed()) {
            viewer.printFinal();
            break;
        }
        if (!listener.listen()) {
            viewer.printQuit();
            break;
        }
    }
    return 0;
}
