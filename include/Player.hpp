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
        bool sanctioned; // Whether player is sanctioned

    public:
        // Constructor - creates player and adds to game
        Player(Game& game, const std::string& name);

        // Virtual destructor for inheritance
        virtual ~Player() {}

        // Basic getters
        std::string getName() const; // Get player name
        int coins() const; // Get number of coins
        bool isActive() const; // Check if player is still active
        bool isSanctioned() const; // Check if player is sanctioned

        // Basic actions that all players can perform
        void gather(); // Take 1 coin from bank
        void tax(); // Take 2 coins from bank
        void bribe(); // Pay 4 coins for extra action
        void arrest(Player& target); // Take 1 coin from another player
        void sanction(Player& target); // Block economic actions (gather, tax) of another player
        void coup(Player& target); // Pay 7 coins to eliminate another player

        // Helper methods
        void addCoins(int amount); // Add coins to player
        void removeCoins(int amount); // Remove coins from player (throws if not enough)
        void eliminate(); // Remove player from game
        void setSanctioned(); // Mark player as sanctioned
    };
}

#endif