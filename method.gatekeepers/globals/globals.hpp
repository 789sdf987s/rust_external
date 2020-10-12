#ifndef _GLOBALS_HPP
#define _GLOBALS_HPP

#include <list>
#include "../classes/Player.hpp"

class Globals
{
public:
    Vector2 ScreenSize{};

    /* Used to store to_process entities */
    /* Format: "base networkable address", "game object address" */
    ThreadVec<Player> players = {};
    ThreadVec<Player> discovered = {};

    /* The camera address */
    uint64_t camera;
    Matrix matrix;

    /* The LocalPlayer */
    Player local_player;

    /* THe Selected Player In The Menu */
    Player* selected_menu_player;

    std::list<uint64_t> team_table{};

    std::list<uint64_t> shit_list{};
};

extern Globals* globals;

#endif // _GLOBALS_HPP