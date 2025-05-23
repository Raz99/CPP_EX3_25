// email: razcohenp@gmail.com
#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

namespace coup {
    class Governor : public Player {
    public:
        // Constructor - inherits from Player
        Governor(Game& game, const std::string& name);
        
        // Override tax method to take 3 coins instead of 2
        void tax() override;
        
        // Block another player's tax action
        void undo(Player& target);
    };
}

#endif