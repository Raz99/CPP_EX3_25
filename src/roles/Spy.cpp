// Email: razcohenp@gmail.com

// Spy.cpp - Spy role implementation
// This file implements the Spy's surveillance and sabotage abilities

#include "../include/roles/Spy.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <stdexcept>

namespace coup {
    // Initialize Spy player with game reference and name
    // Spy specializes in information gathering and tactical disruption
    Spy::Spy(Game& game, const std::string& name) : Player(game, name) {}
    
    // Spy's special ability: Conduct surveillance on target player
    // Reveals target's coin count and blocks their arrest capability
    void Spy::spy_on(Player& target) {
        // Verify game has begun before allowing spy operations
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Confirm Spy is still active and operational
        if (!isActive()) {
            throw std::runtime_error("Player is eliminated");
        }

        // Prevent self-surveillance which would be meaningless
        // Spy already knows their own information
        if (&target == this) {
            throw std::runtime_error("An action against yourself is not allowed");
        }

        // Verify target is still participating and can be spied upon
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }
        
        // Target's coin count revealed to Spy
        // This information advantage is handled by the GUI interface and by the console output
        std::cout << getName() << " (Spy) spies on " << target.getName() << ": " << target.coins() << " coins revealed" << std::endl;

        target.setArrestAvailability(false); // Sabotage: disable target's arrest ability temporarily
        // Spy action is instantaneous and doesn't consume a turn slot
    }
}