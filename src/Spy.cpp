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
        
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }
        
        // == CHANGE THIS ==
        // This will be handled in GUI
        // Display target's coins
        // std::cout << "Spy " << name << " sees that " << target.getName() << " has " << target.coins() << " coins." << std::endl;

        target.setArrestAvailability(false); // Block the target's arrest ability for their next turn
        // No need to call nextTurn() because this action doesn't count as a turn
    }
}