// Merchant role implementation for Coup card game
// This file implements the Merchant role with enhanced income abilities
// filepath: /home/razco/Uni/CPP/CPP_EX3_25/src/roles/Merchant.cpp
// email: razcohenp@gmail.com
#include "../include/roles/Merchant.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Initialize Merchant player with game reference and name
    // Merchant specializes in economic advantages and income generation
    Merchant::Merchant(Game& game, const std::string& name) : Player(game, name) {}
}