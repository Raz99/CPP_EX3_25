// email: razcohenp@gmail.com
#ifndef SPY_HPP
#define SPY_HPP

#include "../Player.hpp"

namespace coup {
    class Spy : public Player {
    public:
        // Constructor - inherits from Player
        Spy(Game& game, const std::string& name);
        
        // Override role type
        std::string getRoleType() const override { return "Spy"; }
        
        // Watch another player's amount of coins and prevent him from using arrest action
        void spy_on(Player& target);
    };
}

#endif