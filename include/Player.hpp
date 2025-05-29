// Email: razcohenp@gmail.com

/**
 * Player.hpp
 * Base player class.
 * Defines common attributes and actions available to all players.
 * Serves as foundation for role-specific derived classes.
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

namespace coup {
    class Game; // Forward declaration to avoid circular dependency

    /**
     * Base player class representing a participant in the Coup game.
     * Contains core game mechanics and serves as foundation for specialized roles.
     * Each player has coins, status flags, and can perform various actions.
     */
    class Player {
    protected:
        Game& game; // Reference to the game instance this player belongs to
        std::string name; // Player's display name for identification
        int coin_count; // Current number of coins the player possesses
        bool active; // Whether player is still in the game (not eliminated)
        bool sanctioned; // Whether player is blocked from economic actions
        bool arrest_available; // Whether another player can arrest this player
        bool bribe_used; // Whether player used bribe action this turn
        bool used_tax_last_action; // Tracks if tax was the most recent action
        Player* couped_by; // Pointer to player who performed coup on this player

    public:
        /**
         * Constructor creates a player and automatically adds them to the game.
         * Initializes all status flags and validates input parameters.
         */
        Player(Game& game, const std::string& name);

        /**
         * Gets the player's display name.
         * Used for identification in game interface and logs.
         */
        std::string getName() const;
        
        /**
         * Returns the current number of coins the player has.
         * Used for action validation and game state display.
         */
        int coins() const;
        
        /**
         * Checks if player is still active in the game.
         * Inactive players have been eliminated by coup.
         */
        bool isActive() const;
        
        /**
         * Checks if player is currently sanctioned.
         * Sanctioned players cannot use gather or tax actions.
         */
        bool isSanctioned() const;
        
        /**
         * Checks if this player can be arrested.
         * Some roles or effects can prevent arrests.
         */
        bool isArrestAvailable() const;
        
        /**
         * Checks if player used bribe action this turn.
         * Bribe allows an additional action in the same turn.
         */
        bool isBribeUsed() const;
        
        /**
         * Checks if tax was the player's most recent action.
         * Used by Governor to determine if undo action is possible.
         */
        bool usedTaxLastAction() const;
        
        /**
         * Gets reference to player who performed coup on this player.
         * Used for General's coup blocking ability.
         */
        Player* getCoupedBy() const;

        /**
         * Checks if this player is a General role.
         * Virtual method overridden by General class.
         */
        virtual bool isGeneral() const { return false; }
        
        /**
         * Checks if this player is a Judge role.
         * Virtual method overridden by Judge class.
         */
        virtual bool isJudge() const { return false; }
        
        /**
         * Checks if this player is a Merchant role.
         * Virtual method overridden by Merchant class.
         */
        virtual bool isMerchant() const { return false; }
        
        /**
         * Gets the role type as a string for display purposes.
         * Virtual method overridden by derived role classes.
         */
        virtual std::string getRoleType() const { return "Player"; }

        /**
         * Gather action - takes 1 coin from the treasury.
         * Basic economic action available to all players.
         * Cannot be used when sanctioned.
         */
        void gather();
        
        /**
         * Tax action - takes 2 coins from the treasury.
         * Virtual method as some roles modify the coin amount.
         * Cannot be used when sanctioned.
         */
        virtual void tax();
        
        /**
         * Bribe action - pays 4 coins for an additional action.
         * Allows player to perform another action in the same turn.
         */
        void bribe();
        
        /**
         * Arrest action - takes 1 coin from target player.
         * Cannot target the same player consecutively.
         */
        void arrest(Player& target);
        
        /**
         * Sanction action - blocks target's economic actions.
         * Costs 3 coins and prevents gather/tax until target's next turn.
         */
        void sanction(Player& target);
        
        /**
         * Coup action - eliminates target player from the game.
         * Costs 7 coins and removes target permanently.
         * Mandatory if player has 10+ coins at turn start.
         */
        void coup(Player& target);

        /**
         * Adds coins to the player's treasury.
         * Used by various actions and role abilities.
         */
        void addCoins(int amount);
        
        /**
         * Removes coins from the player's treasury.
         * Throws exception if player doesn't have enough coins.
         */
        void removeCoins(int amount);
        
        /**
         * Sets whether the player is active in the game.
         * Used when player is eliminated or restored.
         */
        void setActivityStatus(bool value);
        
        /**
         * Sets the player's sanction status.
         * Virtual method as some roles have special sanction behavior.
         */
        virtual void setSanctionStatus(bool value);
        
        /**
         * Sets whether this player can be arrested.
         * Used by Spy role to block arrests.
         */
        void setArrestAvailability(bool value);
        
        /**
         * Resets the bribe used flag to false.
         * Called at end of turn cleanup.
         */
        void resetBribeUsed();
        
        /**
         * Resets the tax last action flag.
         * Called when Governor undoes tax or at turn end.
         */
        void resetUsedTaxLastAction();
        
        /**
         * Clears the reference to who performed coup on this player.
         * Called when coup blocking window expires.
         */
        void resetCoupedBy();
    };
}

#endif