// email: razcohenp@gmail.com
#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "../Player.hpp"

namespace coup {
    class General : public Player {
    public:
        // Constructor
        General(Game& game, const std::string& name);
        
        // Override role type
        std::string getRoleType() const override { return "General"; }
        
        // Block coup of another player
        void block_coup(Player& target);

        // Override to get a coin when targeted
        bool isGeneral() const override { return true; } // Override to return true for General
    };
}

#endif