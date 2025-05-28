// email: razcohenp@gmail.com
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

namespace coup {
    class Game; // Forward declaration to avoid circular dependency

    class Player {
    protected:
        Game& game; // Reference to the game this player belongs to
        std::string name; // Player's name
        int coin_count; // Number of coins player has
        bool active; // Whether player is still in the game
        bool sanctioned; // Whether player is sanctioned (can't use gather and tax until his next turn)
        bool arrest_available; // Whether another player prevents another player's arrest action
        bool bribe_used; // Whether player used bribe action in the current turn
        bool used_tax_last_action; // Track if tax was used in the last action

    public:
        // Constructor - creates player and adds to game
        Player(Game& game, const std::string& name);

        // Virtual destructor for inheritance
        // virtual ~Player() {}

        // Basic getters
        std::string getName() const; // Get player name
        int coins() const; // Get number of coins
        bool isActive() const; // Check if player is still active
        bool isSanctioned() const; // Check if player is sanctioned
        bool isArrestAvailable() const; // Check if arrest action is available
        bool isBribeUsed() const; // Check if bribe action was used in the current turn
        bool usedTaxLastAction() const; // Check if tax was used in the last action

        // Check if player is a specific type
        virtual bool isGeneral() const { return false; } // Check if player is a General (default: false)
        virtual bool isJudge() const { return false; } // Check if player is a Judge (default: false)
        virtual bool isMerchant() const { return false; } // Check if player is a Merchant (default: false)
        
        // Get player role type for GUI display
        virtual std::string getRoleType() const { return "Player"; } // Default role type

        // Basic actions that all players can perform
        void gather(); // Take 1 coin from bank
        virtual void tax(); // Take 2 coins from bank
        void bribe(); // Pay 4 coins for extra action
        void arrest(Player& target); // Take 1 coin from another player
        void sanction(Player& target); // Block economic actions (gather, tax) of another player
        void coup(Player& target); // Pay 7 coins to eliminate another player

        // Helper methods
        void addCoins(int amount); // Add coins to player
        void removeCoins(int amount); // Remove coins from player (throws if not enough)
        void setActivityStatus(bool value); // Set player's activity status
        virtual void setSanctionStatus(bool value); // Set sanction's status
        void setArrestAvailability(bool value); // Set arrest action availability
        void resetBribeUsed(); // Reset bribe used flag back to false
        void resetUsedTaxLastAction(); // Reset used tax last action flag
    };
}

#endif