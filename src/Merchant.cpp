// email: razcohenp@gmail.com
#include "../include/Merchant.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor
    Merchant::Merchant(Game& game, const std::string& name) : Player(game, name) {}
}