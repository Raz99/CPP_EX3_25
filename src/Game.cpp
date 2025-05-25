// email: your_email@example.com
#include "../include/Game.hpp"
#include "../include/Player.hpp"

#include <iostream> // for printing
#include <stdexcept>

namespace coup {
    // Constructor - initialize with first player
    Game::Game() : current_player_index(0), game_started(false), last_arrested_player(nullptr) {}
    
    // Destructor - cleanup
    Game::~Game() {
        players_list.clear(); // Clear the player list
    }

    // Print current player's name
    void Game::turn() const {
        // Check if game has started
        if (!game_started) {
            throw std::runtime_error("Game has not started yet");
        }

        // Check if there are no players in the game
        if (players_list.empty()) {
            throw std::runtime_error("No players in game");
        }
        
        // Print the name of the current player
        std::cout << players_list[current_player_index]->getName() << std::endl;
    }
    
    // Get active players list
    std::vector<std::string> Game::players() const {
        std::vector<std::string> names;

        // For each player in the game
        for (const Player* player : players_list) {
            // Check if player is active
            if (player->isActive()) {
                names.push_back(player->getName()); // Add active player's name to the list
            }
        }
        
        return names;
    }
    
    // Get winner name (throws if game still active)
    std::string Game::winner() const {
        // Check if game has started
        if (!game_started) {
            throw std::runtime_error("Game has not started yet");
        }

        int active_count = 0; // Count of active players
        std::string winner_name; // Name of the winner
        
        // Count active players and find potential winner
        for (const Player* player : players_list) {
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
    
    // Add player to game
    void Game::addPlayer(Player* player) {
        // Check if game has already started
        if (game_started) {
            throw std::runtime_error("Cannot add players after game has started");
        }

        // Check max players
        if (players_list.size() >= 6) {
            throw std::runtime_error("Maximum 6 players allowed");
        }

        players_list.push_back(player);
    }
    
    // Move to next turn
    void Game::nextTurn() {
        // Check if game has started
        if (!game_started) {
            throw std::runtime_error("Game has not started yet");
        }

        if (players().empty()) {
            throw std::runtime_error("No players in the game");
        }

        // Clear the current player's sanction since their turn is over
        if(players_list[current_player_index]->isSanctioned()) {
            players_list[current_player_index]->setSanctionStatus(false);
        }

        // Clear the current player's arrest block since their turn is over
        if(!players_list[current_player_index]->isArrestAvailable()) {
            players_list[current_player_index]->setArrestAvailability(false);
        }

        // Clear the current player's bribe used flag since their turn is over
        if(players_list[current_player_index]->isBribeUsed()) {
            players_list[current_player_index]->resetBribeUsed();
        }

        int old_player_index = current_player_index; // Store the old player index
        current_player_index = (current_player_index + 1) % players_list.size(); // Move to next player
        Player* next_player = players_list[current_player_index]; // Get the next player

        // While the next player is not active, skip to the next one
        while(!next_player->isActive()) {
            current_player_index = (current_player_index + 1) % players_list.size();

            // If we looped back to the original player, then there's only one player left
            if(current_player_index == old_player_index) {
                return; // No more active players
            }

            next_player = players_list[current_player_index];
        }

        // // Check if player has 10+ coins - must perform coup
        // if (next_player->coins() >= 10) {
        //     // std::cout << next_player->getName() << " must perform coup this turn (has 10+ coins)" << std::endl;
        // }

        // If the next player is a Merchant, check for bonus coin
        if(next_player->isMerchant()) {
            // If merchant has 3+ coins at start of turn, gain an extra
            if (next_player->coins() >= 3) {
                next_player->addCoins(1);
            }
        }
    }
    
    // Check if it's player's turn
    bool Game::isPlayerTurn(const Player* player) const {
        // Check if game has started
        if (!game_started) {
            throw std::runtime_error("Game has not started yet");
        }

        // If no players in the game
        if (players_list.empty()) {
            return false;
        }

         // Ensure it's the current player's turn
        return players_list[current_player_index] == player;
    }

    // Start the game - checks player count requirements
    void Game::startGame() {
        // Check minimum players
        if (players_list.size() < 2) {
            throw std::runtime_error("Minimum 2 players required to start the game");
        }
        
        // Check maximum players
        if (players_list.size() > 6) {
            throw std::runtime_error("Maximum 6 players allowed to start the game");
        }
        
        game_started = true;
        // current_player_index = 0; // Start with first player
    }
    
    // Check if game is started
    bool Game::isGameStarted() const {
        return game_started;
    }
    
    // Track last arrested player
    void Game::setLastArrestedPlayer(Player* player) {
        last_arrested_player = player;
    }
    
    Player* Game::getLastArrestedPlayer() const {
        return last_arrested_player;
    }
}