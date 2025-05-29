// Baron role implementation for Coup card game
// This file implements the Baron's special investment ability and sanction benefits
// filepath: /home/razco/Uni/CPP/CPP_EX3_25/src/roles/Baron.cpp
// email: razcohenp@gmail.com
#include "../include/roles/Baron.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Initialize Baron player with game reference and name
    // Baron has unique investment ability and benefits from being sanctioned
    Baron::Baron(Game& game, const std::string& name) : Player(game, name) {}
    
    // Baron's special ability: Invest 3 coins to receive 6 coins (net gain of 3)
    // This powerful economic ability allows rapid wealth accumulation
    void Baron::invest() {
        // Verify game has begun before allowing investment actions
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Confirm it's Baron's turn to act in the game sequence
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Check that Baron is still actively participating in the game
        if (!isActive()) {
            throw std::runtime_error("Player is eliminated");
        }

        // Enforce coup rule: players with 10+ coins must coup instead of other actions
        // This prevents excessive coin hoarding and maintains game balance
        if (coin_count >= 10 && !bribe_used) {
            throw std::runtime_error("You have 10 or more coins, must perform coup");
        }
        
        // Verify Baron has minimum 3 coins required for investment
        if (coin_count < 3) {
            throw std::runtime_error("Not enough coins for investment");
        }
        
        addCoins(3); // Net gain of 3 coins (pay 3 to receive 6, total +3)
        
        // Handle bribe mechanic: if bribe was used this turn, reset flag
        // Otherwise advance to next player's turn normally
        if(bribe_used) {
            bribe_used = false; // Clear bribe flag to end additional turn
        }
        
        // Standard turn progression when no bribe was used
        else {
            game.nextTurn(); // Pass control to next active player
        }
    }
    
    // Override sanction handling to implement Baron's defensive bonus
    // Baron uniquely benefits from being targeted by gaining coins
    void Baron::setSanctionStatus(bool value) {
        Player::setSanctionStatus(value); // Apply standard sanction effects first

         // Baron's defensive ability: gain coin when targeted by sanctions
         // This makes Baron less vulnerable to economic attacks
        if(value) {
            addCoins(1); // Receive 1 coin as compensation for being sanctioned
        }
    }
}