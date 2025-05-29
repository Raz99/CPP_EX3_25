// Email: razcohenp@gmail.com

/**
 * General.hpp
 * General role class.
 * Special abilities: Can block coup actions (costs 5 coins) and recovers from arrest attempts.
 * Represents military leadership with defensive capabilities and resilience.
 */

#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "../Player.hpp"

namespace coup {
    /**
     * General class - a defensive military role in the Coup game.
     * Generals can protect themselves and others from coups and resist arrest attempts.
     * This role specializes in protection and defensive maneuvers.
     */
    class General : public Player {
    public:
        /**
         * Constructor creates a General with the specified name.
         * Inherits all basic player functionality with defensive enhancements.
         */
        General(Game& game, const std::string& name);
        
        /**
         * Returns the role type identifier for GUI display.
         * Used to show role-specific information in the interface.
         */
        std::string getRoleType() const override { return "General"; }
        
        /**
         * Block coup action - prevents coup against any player (including self).
         * Costs 5 coins and causes the coup attempt to fail completely.
         * The player attempting coup loses their 7 coins without effect.
         */
        void block_coup(Player& target);

        /**
         * Override to identify this player as a General.
         * Used by game logic for special General-specific interactions.
         */
        bool isGeneral() const override { return true; }
    };
}

#endif