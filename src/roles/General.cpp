// email: razcohenp@gmail.com
#include "../include/roles/General.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    General::General(Game& game, const std::string& name) : Player(game, name) {}
    
    // Block coup of another player (or self)
    void General::block_coup(Player& target) {
        // Check if game has started
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Ensure player has enough coins
        if (coin_count < 5) {
            throw std::runtime_error("Not enough coins to block coup");
        }

        // Allow blocking coup if:
        // 1. General is active (can block coup on any inactive player)
        // 2. General is inactive but blocking their own coup (self-revival)
        if (!active && &target != this) {
            throw std::runtime_error("Inactive General can only block their own coup");
        }

        // Ensure target is couped
        if (target.isActive()) {
            throw std::runtime_error("Target player is not couped");
        }
        
        removeCoins(5); // Pay 5 coins to block the coup
        target.setActivityStatus(true); // Restore target's activity status
    }
}