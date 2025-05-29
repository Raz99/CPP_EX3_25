// Email: razcohenp@gmail.com

// Governor.cpp - Governor role implementation
// This file implements the Governor's special ability to higher tax and undo other players' tax actions

#include "../include/roles/Governor.hpp"
#include "../include/Game.hpp"
#include <stdexcept> // For exception handling

namespace coup {
    /**
     * Constructor creates a Governor player with enhanced economic abilities.
     * Inherits base player functionality with role-specific enhancements.
     */
    Governor::Governor(Game& game, const std::string& name) : Player(game, name) {}
    
    /**
     * Enhanced tax action yields 3 coins instead of the standard 2.
     * Demonstrates the Governor's superior economic influence and power.
     */
    void Governor::tax() {
        Player::tax(); // Execute standard tax validation and award 2 coins
        addCoins(1); // Governor bonus - award additional coin for total of 3
    }
    
    /**
     * Undo action reverses another player's tax action.
     * Removes 2 coins from target who used tax as their last action.
     */
    void Governor::undo(Player& target) {
        if (!game.isGameStarted()) { // Ensure game is in progress
            throw std::runtime_error("Game has not started yet");
        }

        if (!isActive()) { // Ensure Governor is still in the game
            throw std::runtime_error("Player is eliminated");
        }

        if (&target == this) { // Prevent self-targeting
            throw std::runtime_error("An action against yourself is not allowed");
        }

        if (!target.isActive()) { // Ensure target is still in game
            throw std::runtime_error("Target player is eliminated");
        }

        if (!target.usedTaxLastAction()) { // Verify target used tax recently
            throw std::runtime_error("Target player did not use tax as his last action");
        }
        
        target.removeCoins(2); // Reverse the tax benefit by removing 2 coins
        target.resetUsedTaxLastAction(); // Clear tax tracking since action was undone
    }
}