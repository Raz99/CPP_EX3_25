// email: razcohenp@gmail.com
#include "../include/Judge.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    Judge::Judge(Game& game, const std::string& name) : Player(game, name) {}
    
    // Block another player's bribe action
    void Judge::block_bribe(Player& target) {
        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Check if target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Ensure there is a bribe action to block
        if (!target.isBribeUsed()) {
            throw std::runtime_error("Target player has not used bribe");
        }

        // std::cout << "Judge " << name << " blocks " << target.getName() 
        //           << "'s bribe, causing them to lose their 4 coins." << std::endl;

        game.nextTurn(); // Move to next player's turn
    }
}