// email: razcohenp@gmail.com
#include "../include/roles/Judge.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    Judge::Judge(Game& game, const std::string& name) : Player(game, name) {}
    
    // Block another player's bribe action
    void Judge::block_bribe(Player& target) {
        // Check if game has started
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure target is not the current player
        if (&target == this) {
            throw std::runtime_error("An action against yourself is not allowed");
        }

        // Check if target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Ensure there is a bribe action to block
        if (!target.isBribeUsed()) {
            throw std::runtime_error("Target player has not used bribe");
        }

        // Block the bribe by resetting the bribe_used flag
        target.resetBribeUsed();

        // std::cout << "Judge " << name << " blocks " << target.getName() 
        //           << "'s bribe, causing them to lose their 4 coins." << std::endl;    }
    }
}