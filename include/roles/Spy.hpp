// email: razcohenp@gmail.com

/**
 * Spy.hpp
 * Spy role class for the Coup card game.
 * Special abilities: Can observe other players' coins and prevent their arrest actions.
 * Represents intelligence gathering with information warfare and action disruption.
 */

#ifndef SPY_HPP
#define SPY_HPP

#include "../Player.hpp"

namespace coup {
    /**
     * Spy class - an intelligence and surveillance role in the Coup game.
     * Spies can gather information about opponents and disrupt their aggressive actions.
     * This role specializes in reconnaissance and preventing hostile activities.
     */
    class Spy : public Player {
    public:
        /**
         * Constructor creates a Spy with the specified name.
         * Inherits all basic player functionality with surveillance capabilities.
         */
        Spy(Game& game, const std::string& name);
        
        /**
         * Returns the role type identifier for GUI display.
         * Used to show role-specific information in the interface.
         */
        std::string getRoleType() const override { return "Spy"; }
        
        /**
         * Spy action - reveals target's coin count and blocks their next arrest.
         * Provides valuable intelligence while preventing aggressive actions.
         * Does not cost coins and doesn't consume the spy's turn.
         */
        void spy_on(Player& target);
    };
}

#endif