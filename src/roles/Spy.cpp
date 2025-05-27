// email: razcohenp@gmail.com
#include "../include/roles/Spy.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    Spy::Spy(Game& game, const std::string& name) : Player(game, name) {}
    
    // Watch another player's amount of coins and prevent him from using arrest action
    void Spy::spy_on(Player& target) {
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

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }
        
        // Display target's coins - This will be handled in GUI

        target.setArrestAvailability(false); // Block the target's arrest ability for their next turn
        // No need to call nextTurn() because this action doesn't count as a turn
    }
}