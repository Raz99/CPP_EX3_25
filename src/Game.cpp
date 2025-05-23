// email: your_email@example.com
#include "../include/Game.hpp"
#include "../include/Player.hpp"

#include <iostream> // for printing
#include <stdexcept>

namespace coup {
    // Constructor - initialize with first player
    Game::Game() : current_player_index(0) {}
    
    // Destructor - cleanup
    Game::~Game() {
        player_list.clear(); // Clear the player list
    }
    
    // Add player to game
    void Game::addPlayer(Player* player) {
        player_list.push_back(player);
    }
    
    // Move to next turn
    void Game::nextTurn() {
        current_player_index = (current_player_index + 1) % player_list.size();
    }
    
    // Print current player's name
    void Game::turn() const {
        // Check if there are no players in the game
        if (player_list.empty()) {
            throw std::runtime_error("No players in game");
        }
        
        // Print the name of the current player
        std::cout << player_list[current_player_index]->getName() << std::endl;
    }
    
    // Get active players list
    std::vector<std::string> Game::players() const {
        std::vector<std::string> names;

        // For each player in the game
        for (const Player* player : player_list) {
            // Check if player is active
            if (player->isActive()) {
                names.push_back(player->getName()); // Add active player's name to the list
            }
        }
        
        return names;
    }
    
    // Get winner name (throws if game still active)
    std::string Game::winner() const {
        int active_count = 0; // Count of active players
        std::string winner_name; // Name of the winner
        
        // Count active players and find potential winner
        for (const Player* player : player_list) {
            if (player->isActive()) {
                active_count++;
                winner_name = player->getName();
            }
        }
        
        // Game is still active if more than one player remains
        if (active_count > 1) {
            throw std::runtime_error("Game is still active");
        }
        
        // No players left (shouldn't happen, just a safety check)
        if (active_count == 0) {
            throw std::runtime_error("No active players found");
        }
        
        return winner_name; // Exactly one player remains
    }
    
    // Check if it's player's turn
    bool Game::isPlayerTurn(const Player* player) const {
        // If no players in the game
        if (player_list.empty()) {
            return false;
        }

         // Ensure it's the current player's turn
        return player_list[current_player_index] == player;
    }
}