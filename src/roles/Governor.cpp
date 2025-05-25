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
        
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Check if player has 10 coins and just started his turn - must coup
        if (coin_count >= 10 && !bribe_used) {
            throw std::runtime_error("You have 10 or more coins, must perform coup");
        }

        // Ensure target is not the current player
        if (&target == this) {
            throw std::runtime_error("An action against yourself is not allowed");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }
        
        target.disableTax(); // Disable target's tax action

        // If player used bribe, then let him play another turn
        if(bribe_used) {
            bribe_used = false; // Reset bribe used flag
        }
        
        // If player did not use bribe, then move to next player's turn
        else {
            game.nextTurn(); // Move to next player's turn
        }
    }
}