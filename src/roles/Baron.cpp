// email: razcohenp@gmail.com
#include "../include/roles/Baron.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    Baron::Baron(Game& game, const std::string& name) : Player(game, name) {}
    
    // Invest 3 coins to get 6 (net gain of 3)
    void Baron::invest() {
        // Check if game has started
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!isActive()) {
            throw std::runtime_error("Player is eliminated");
        }

        // Check if player has 10 coins and just started his turn - must coup
        if (coin_count >= 10 && !bribe_used) {
            throw std::runtime_error("You have 10 or more coins, must perform coup");
        }
        
        // Ensure player has enough coins
        if (coin_count < 3) {
            throw std::runtime_error("Not enough coins for investment");
        }
        
        addCoins(3); // Add 3 coins (Baron pays 3 coins for getting 6 coins, which means he gets 3 coins overall)
        
        // If player used bribe, then let him play another turn
        if(bribe_used) {
            bribe_used = false; // Reset bribe used flag
        }
        
        // If player did not use bribe, then move to next player's turn
        else {
            game.nextTurn(); // Move to next player's turn
        }
    }
    
    // Override setSanctionStatus() to get a coin when targeted
    void Baron::setSanctionStatus(bool value) {
        Player::setSanctionStatus(value); // Call parent implementation

         // If sanctioned
        if(value) {
            addCoins(1); // Baron gets 1 coin when sanctioned
        }
    }
}