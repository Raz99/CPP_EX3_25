// email: your_email@example.com
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/roles/General.hpp"
#include "../include/roles/Judge.hpp"
#include "../include/roles/Merchant.hpp"
#include "../include/roles/Governor.hpp"
#include "../include/roles/Baron.hpp"
#include "../include/roles/Spy.hpp"

#include <iostream> // for printing
#include <stdexcept>
#include <algorithm> // for std::find
#include <chrono> // for seeding random generator

namespace coup {
    // Constructor - initialize with first player
    Game::Game() : current_player_index(0), game_started(false), last_arrested_player(nullptr) {
        // Seed random generator with current time for role assignment
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        random_generator.seed(seed);
    }
    
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
        if(!players_list[current_player_index]->isTaxAvailable()) {
            players_list[current_player_index]->setTaxAvailability(true);
        }

        // Clear the current player's arrest block since their turn is over
        if(!players_list[current_player_index]->isArrestAvailable()) {
            players_list[current_player_index]->setArrestAvailability(true);
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

            // If we looped back to the original player, check if game should continue
            if(current_player_index == old_player_index) {
                // Count active players and check for General
                int active_count = 0;
                bool has_active_general_with_coins = false;
                for (Player* player : players_list) {
                    if (player->isActive()) {
                        active_count++;
                        if (player->isGeneral() && player->coins() >= 5) {
                            has_active_general_with_coins = true;
                        }
                    }
                }
                
                // If there are exactly 2 active players and one is a General with 5+ coins,
                // don't end the game yet - let the GUI handle the block_coup decision
                if (active_count == 2 && has_active_general_with_coins) {
                    // Find the first active player and set as current
                    for (size_t i = 0; i < players_list.size(); i++) {
                        if (players_list[i]->isActive()) {
                            current_player_index = i;
                            break;
                        }
                    }
                    return;
                }
                
                return; // No more active players or game should end
            }

            next_player = players_list[current_player_index];
        }

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
    
    // Get current player
    Player* Game::getCurrentPlayer() const {
        if (!game_started || players_list.empty()) {
            return nullptr;
        }
        return players_list[current_player_index];
    }
    
    // Get all players in the game
    std::vector<Player*> Game::getAllPlayers() const {
        return players_list;
    }
    
    // Get active players
    std::vector<Player*> Game::getActivePlayers() const {
        std::vector<Player*> activePlayers;
        for (Player* player : players_list) {
            if (player->isActive()) {
                activePlayers.push_back(player);
            }
        }
        return activePlayers;
    }
    
    // Track last arrested player
    void Game::setLastArrestedPlayer(Player* player) {
        last_arrested_player = player;
    }
    
    Player* Game::getLastArrestedPlayer() const {
        return last_arrested_player;
    }
    
    // Clear all players from the game (only allowed when game not started)
    void Game::clearAllPlayers() {
        // Only allow clearing players if game hasn't started
        if (game_started) {
            throw std::runtime_error("Cannot clear players after game has started");
        }
        
        // Delete all player objects
        for (Player* player : players_list) {
            delete player;
        }
        
        // Clear the list
        players_list.clear();
        current_player_index = 0;
        last_arrested_player = nullptr;
    }
    
    // Remove a specific player from the game (only allowed when game not started)
    void Game::removePlayer(Player* player) {
        // Only allow removing players if game hasn't started
        if (game_started) {
            throw std::runtime_error("Cannot remove players after game has started");
        }
        
        // Find and remove the player
        auto it = std::find(players_list.begin(), players_list.end(), player);
        if (it != players_list.end()) {
            delete *it; // Delete the player object
            players_list.erase(it);
            
            // Reset indices if needed
            if (current_player_index >= static_cast<int>(players_list.size())) {
                current_player_index = 0;
            }
            
            // Clear last arrested player if it was this player
            if (last_arrested_player == player) {
                last_arrested_player = nullptr;
            }
        }
    }

    // Check if a General can block coup to prevent game from ending
    bool Game::canGeneralBlockCoupToPreventGameEnd() const {
        if (!game_started) return false;
        
        int active_count = 0;
        bool has_active_general_with_coins = false;
        
        for (Player* player : players_list) {
            if (player->isActive()) {
                active_count++;
                if (player->isGeneral() && player->coins() >= 5) {
                    has_active_general_with_coins = true;
                }
            }
        }
        
        return (active_count == 2 && has_active_general_with_coins);
    }
    
    // Methods for Role Assignment
    // Assign roles to existing players without recreating them
    void Game::assignRolesToExistingPlayers() {
        // Check if there are players to assign roles to
        if (players_list.empty()) {
            throw std::runtime_error("No players in game to assign roles");
        }
        
        // Create list of available roles (can repeat for multiple players)
        std::vector<RoleType> available_roles = {
            RoleType::GOVERNOR,
            RoleType::SPY,
            RoleType::BARON,
            RoleType::GENERAL,
            RoleType::JUDGE, 
            RoleType::MERCHANT
        };
        
        // Store existing players and their names
        std::vector<std::pair<std::string, Player*>> player_data;
        for (Player* player : players_list) {
            player_data.push_back({player->getName(), player});
        }
        
        // Clear the list temporarily
        players_list.clear();
        
        // Create new players with roles and delete the old ones
        for (const auto& data : player_data) {
            // Pick random role from available roles
            RoleType assigned_role = available_roles[random_generator() % available_roles.size()];
            
            // Delete the old player and create new one with role
            delete data.second;
            Player* player = createPlayerWithRole(data.first, assigned_role);
            players_list.push_back(player);
        }
        
        // Reset game state
        current_player_index = 0;
        last_arrested_player = nullptr;
    }
    
    // Get role name as string for display
    std::string Game::getRoleName(RoleType role) const {
        switch (role) {
            case RoleType::GOVERNOR: return "Governor";
            case RoleType::SPY: return "Spy";
            case RoleType::BARON: return "Baron";
            case RoleType::GENERAL: return "General";
            case RoleType::JUDGE: return "Judge";
            case RoleType::MERCHANT: return "Merchant";
            default: return "Unassigned";
        }
    }
    
    // Create player with specific role (for internal use)
    Player* Game::createPlayerWithRole(const std::string& name, RoleType role) {
        switch (role) {
            case RoleType::GOVERNOR:
                return new Governor(*this, name);
            case RoleType::SPY:
                return new Spy(*this, name);
            case RoleType::BARON:
                return new Baron(*this, name);
            case RoleType::GENERAL:
                return new General(*this, name);
            case RoleType::JUDGE:
                return new Judge(*this, name);
            case RoleType::MERCHANT:
                return new Merchant(*this, name);
            default:
                throw std::runtime_error("Invalid role type");
        }
    }
}