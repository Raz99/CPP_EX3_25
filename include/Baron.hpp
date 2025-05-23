// email: razcohenp@gmail.com
#ifndef BARON_HPP
#define BARON_HPP

#include "Player.hpp"

namespace coup {
    class Baron : public Player {
    public:
        // Constructor
        Baron(Game& game, const std::string& name);
        
        // Invest 3 coins to get 6
        void invest();
        
        // Override sanction to get a coin when targeted
        void setSanctionStatus(bool value) override;
    };
}

#endif