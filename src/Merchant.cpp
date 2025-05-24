// email: razcohenp@gmail.com
#include "../include/roles/Merchant.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    Merchant::Merchant(Game& game, const std::string& name) : Player(game, name) {}
}