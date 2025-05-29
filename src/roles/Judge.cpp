// Judge role implementation for Coup card game
// This file implements the Judge's special ability to block bribe actions
// filepath: /home/razco/Uni/CPP/CPP_EX3_25/src/roles/Judge.cpp
// email: razcohenp@gmail.com
#include "../include/roles/Judge.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Initialize Judge player with game reference and name
    // Judge serves as corruption watchdog with bribe-blocking powers
    Judge::Judge(Game& game, const std::string& name) : Player(game, name) {}
    
    // Judge's special ability: Block another player's bribe attempt
    // This prevents corruption and maintains game integrity
    void Judge::block_bribe(Player& target) {
        // Verify game state before allowing bribe blocking
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Confirm Judge is still active and able to intervene
        if (!isActive()) {
            throw std::runtime_error("Player is eliminated");
        }

        // Prevent self-targeting which would be nonsensical
        // Judge cannot block their own bribe attempts
        if (&target == this) {
            throw std::runtime_error("An action against yourself is not allowed");
        }

        // Verify target is still participating in the game
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Confirm there's actually a bribe action to block
        // Can only block if target recently used bribery
        if (!target.isBribeUsed()) {
            throw std::runtime_error("Target player has not used bribe as a last action");
        }

        // Execute the blocking action by nullifying the bribe
        target.resetBribeUsed(); // Remove bribe effect and deny extra turn
    }
}