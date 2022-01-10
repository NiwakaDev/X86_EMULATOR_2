#include "../Gui.h"

inline bool Gui::IsQuit(){
    return this->quit;
}

inline void Gui::Finish(){
    this->quit = true;
}
