// Email: razcohenp@gmail.com

/**
 * Governor.hpp
 * Governor role class.
 * Special abilities: Enhanced tax (3 coins instead of 2) and can undo other players' tax actions.
 * Represents a political figure with economic influence and regulatory power.
 */

#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "../Player.hpp"

namespace coup {
    /**
     * Governor class - a powerful economic role in the Coup game.
     * Governors have enhanced taxation abilities and can reverse other players' tax actions.
     * This role excels at resource generation and economic control.
     */
    class Governor : public Player {
    public:
        /**
         * Constructor creates a Governor with the specified name.
         * Inherits all basic player functionality with enhanced abilities.
         */
        Governor(Game& game, const std::string& name);
        
        /**
         * Returns the role type identifier for GUI display.
         * Used to show role-specific information in the interface.
         */
        std::string getRoleType() const override { return "Governor"; }
        
        /**
         * Enhanced tax action that yields 3 coins instead of 2.
         * Demonstrates the Governor's superior economic influence.
         */
        void tax() override;
        
        /**
         * Undo action - reverses another player's tax action.
         * Removes 2 coins from target who used tax as their last action.
         * Represents regulatory power to reverse economic decisions.
         */
        void undo(Player& target);
    };
}

#endif