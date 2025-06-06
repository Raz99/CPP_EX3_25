// Email: razcohenp@gmail.com

/**
 * Merchant.hpp
 * Merchant role class.
 * Special abilities: Gains bonus coins at turn start and pays treasury instead of losing coins when arrested.
 * Represents commercial expertise with steady income and financial protection mechanisms.
 */

#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "../Player.hpp"

namespace coup {
    /**
     * Merchant class - a commerce-focused role in the Coup game.
     * Merchants generate passive income and have alternative payment methods for penalties.
     * This role excels at consistent resource generation and financial flexibility.
     */
    class Merchant : public Player {
    public:
        /**
         * Constructor creates a Merchant with the specified name.
         * Inherits all basic player functionality with commercial enhancements.
         */
        Merchant(Game& game, const std::string& name);
        
        /**
         * Virtual destructor for proper cleanup of Merchant objects.
         */
        virtual ~Merchant() = default; // Use default destructor
        
        /**
         * Returns the role type identifier for GUI display.
         * Used to show role-specific information in the interface.
         */
        std::string getRoleType() const override { return "Merchant"; }
    };
}

#endif