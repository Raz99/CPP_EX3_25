// email: razcohenp@gmail.com
#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"

namespace coup {
    class Merchant : public Player {
    public:
        // Constructor
        Merchant(Game& game, const std::string& name);
        
        // Override to get extra coins if turn starts with 3+ coins
        bool isMerchant() const override { return true; } // Override to return true for Merchant
    };
}

#endif