// email: razcohenp@gmail.com
#include "../include/roles/General.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    General::General(Game& game, const std::string& name) : Player(game, name) {}
    
    // Block coup against another player (or self)
    void General::block_coup(Player& target) {
        // Check if game has started
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }
        
        // Ensure player has enough coins
        if (coin_count < 5) {
            throw std::runtime_error("Not enough coins to block coup");
        }

        // Ensure target is couped
        if (target.isActive()) {
            throw std::runtime_error("Target player is not couped");
        }
        
        removeCoins(5); // Pay 5 coins to block the coup
        target.setActivityStatus(true); // Restore target's activity status
    }
}