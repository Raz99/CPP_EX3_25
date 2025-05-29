// Email: razcohenp@gmail.com

/**
 * Baron.hpp
 * Baron role class.
 * Special abilities: Can invest coins for profit and receives compensation when sanctioned.
 * Represents wealthy nobility with investment acumen and financial resilience.
 */

#ifndef BARON_HPP
#define BARON_HPP

#include "../Player.hpp"

namespace coup {
    /**
     * Baron class - a wealthy investment-focused role in the Coup game.
     * Barons can multiply their wealth through investments and have financial protection.
     * This role excels at resource multiplication and economic resilience.
     */
    class Baron : public Player {
    public:
        /**
         * Constructor creates a Baron with the specified name.
         * Inherits all basic player functionality with investment capabilities.
         */
        Baron(Game& game, const std::string& name);
        
        /**
         * Returns the role type identifier for GUI display.
         * Used to show role-specific information in the interface.
         */
        std::string getRoleType() const override { return "Baron"; }
        
        /**
         * Investment action - converts 3 coins into 6 coins.
         * Represents the Baron's ability to generate wealth through investments.
         * Doubles the invested amount for substantial economic advantage.
         */
        void invest();
        
        /**
         * Override sanction handling to provide compensation.
         * When sanctioned, Baron receives 1 coin as financial protection.
         * Demonstrates wealth-based resilience against economic attacks.
         */
        void setSanctionStatus(bool value) override;
    };
}

#endif