// email: razcohenp@gmail.com
#include "../include/roles/Governor.hpp"
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
        // Check if game has started
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }

        // Ensure player is active
        if (!isActive()) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure target is not the current player
        if (&target == this) {
            throw std::runtime_error("An action against yourself is not allowed");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Ensure target has used tax action in his last turn
        if (!target.usedTaxLastAction()) {
            throw std::runtime_error("Target player did not use tax as his last action");
        }
        
        target.removeCoins(2); // Remove 2 coins from target
        target.resetUsedTaxLastAction(); // Reset target's used tax last action flag since current player undid it
    }
}