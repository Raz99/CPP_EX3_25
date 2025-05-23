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
    };
}

#endif