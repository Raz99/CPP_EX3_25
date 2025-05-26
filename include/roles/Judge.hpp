// email: razcohenp@gmail.com
#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "../Player.hpp"

namespace coup {
    class Judge : public Player {
    public:
        // Constructor
        Judge(Game& game, const std::string& name);
        
        // Override role type
        std::string getRoleType() const override { return "Judge"; }
        
        // Block bribe
        void block_bribe(Player& target);
        
        // Override to get a coin when targeted
        bool isJudge() const override { return true; } // Override to return true for Judge
    };
}

#endif