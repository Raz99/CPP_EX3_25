// email: razcohenp@gmail.com
#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "../Player.hpp"

namespace coup {
    class Merchant : public Player {
    public:
        // Constructor
        Merchant(Game& game, const std::string& name);
        
        // Override role type
        std::string getRoleType() const override { return "Merchant"; }
        
        // Override to get extra coins if turn starts with 3+ coins
        bool isMerchant() const override { return true; } // Override to return true for Merchant
    };
}

#endif