// Email: razcohenp@gmail.com

// General.cpp - General role implmementation
// This file implements the General's special ability to block coup attempts

#include "../include/roles/General.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Initialize General player with game reference and name
    // General inherits all basic player abilities plus coup blocking
    General::General(Game& game, const std::string& name) : Player(game, name) {}
    
    // General's special ability: Block coup attempts on any player for 5 coins
    // This powerful defensive ability can save players from elimination
    void General::block_coup(Player& target) {
        // Verify game state before allowing coup blocking
        if (!game.isGameStarted()) {
            throw std::runtime_error("Game has not started yet");
        }
        
        // Check if General has sufficient funds to block the coup
        // Blocking requires exactly 5 coins as payment
        if (coin_count < 5) {
            throw std::runtime_error("Not enough coins to block coup");
        }

        // Validate that target player is actually being couped
        // Cannot block coup on active players who aren't under attack
        if (target.isActive()) {
            throw std::runtime_error("Target player is not couped");
        }

        // Ensure coup is still blockable - timing is critical in Coup
        // Once coup resolution begins, it cannot be reversed
        if (target.getCoupedBy() == nullptr) {
            throw std::runtime_error("Too late, you cannot block this coup anymore");
        }
        
        removeCoins(5); // Deduct the blocking fee from General's treasury
        target.resetCoupedBy(); // Remove coup attacker reference from target
        target.setActivityStatus(true); // Restore target to active gameplay status
    }
}