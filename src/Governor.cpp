// email: razcohenp@gmail.com
#include "../include/Governor.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    Governor::Governor(Game& game, const std::string& name) : Player(game, name) {}
    
    // Override tax to take 3 coins instead of 2
    void Governor::tax() {
        Player::tax(); // Handle all validation and take 2 coins
        addCoins(1); // Add 1 more coin for a total of 3
    }
    
    // Block another player's tax action
    void Governor::undo(Player& target) {
        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }
        
        target.disableTax(); // Disable target's tax action
    }
}