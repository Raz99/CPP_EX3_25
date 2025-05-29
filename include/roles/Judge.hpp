// Email: razcohenp@gmail.com

/**
 * Judge.hpp
 * Judge role class.
 * Special abilities: Can block bribe actions and penalizes sanctioners with additional costs.
 * Represents judicial authority with corruption prevention and legal protection.
 */

#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "../Player.hpp"

namespace coup {
    /**
     * Judge class - a law enforcement role in the Coup game.
     * Judges can prevent corruption (bribes) and impose legal penalties on attackers.
     * This role specializes in blocking actions and imposing costs on opponents.
     */
    class Judge : public Player {
    public:
        /**
         * Constructor creates a Judge with the specified name.
         * Inherits all basic player functionality with legal enforcement powers.
         */
        Judge(Game& game, const std::string& name);
        
        /**
         * Virtual destructor for proper cleanup of Judge objects.
         */
        virtual ~Judge() = default; // Use default destructor
        
        /**
         * Returns the role type identifier for GUI display.
         * Used to show role-specific information in the interface.
         */
        std::string getRoleType() const override { return "Judge"; }
        
        /**
         * Block bribe action - prevents target's bribe and wastes their coins.
         * Causes the bribing player to lose 4 coins without gaining extra action.
         * Represents judicial power to prevent corruption and waste resources.
         */
        void block_bribe(Player& target);
    };
}

#endif