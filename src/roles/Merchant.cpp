// Email: razcohenp@gmail.com

// Merchant.cpp - Merchant role implementation
// This file implements the Merchant role with enhanced income abilities

#include "../include/roles/Merchant.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Initialize Merchant player with game reference and name
    // Merchant specializes in economic advantages and income generation
    Merchant::Merchant(Game& game, const std::string& name) : Player(game, name) {}
}