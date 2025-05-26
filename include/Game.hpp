// email: razcohenp@gmail.com
#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>

namespace coup {
    class Player; // Forward declaration to avoid circular dependency
    
    class Game {
    private:
        std::vector<Player*> players_list; // List of players in the game
        int current_player_index; // Index of current player
        bool game_started; // Flag to track if game has started
        Player* last_arrested_player; // Track the last player that was arrested
        
    public:
        // Constructor - initialize with first player
        Game();
        
        // Destructor - cleanup
        ~Game();
        
        // Print current player's name
        void turn() const;
        
        // Get list of active players
        std::vector<std::string> players() const;
        
        // Get winner name (throws if game still active)
        std::string winner() const;
        
        // Add player to the game
        void addPlayer(Player* player);
        
        // Move to next player's turn
        void nextTurn();

        // Check if it's player's turn
        bool isPlayerTurn(const Player* player) const;

        // Start the game - as long as the amount of players is valid (should be 2-6)
        void startGame();
        
        // Check if game is started
        bool isGameStarted() const;
        
        // Get current player
        Player* getCurrentPlayer() const;
        
        // Get all players in the game
        std::vector<Player*> getAllPlayers() const;
        
        // Get active players
        std::vector<Player*> getActivePlayers() const;
        
        // Track last arrested player
        // Set last arrested player
        void setLastArrestedPlayer(Player* player);

        // Get last arrested player
        Player* getLastArrestedPlayer() const;
        
        // Clear all players from the game (only allowed when game not started)
        void clearAllPlayers();
        
        // Remove a specific player from the game (only allowed when game not started)
        void removePlayer(Player* player);
    };
}

#endif