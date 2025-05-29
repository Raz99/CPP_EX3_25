// Email: razcohenp@gmail.com

/**
 * Comprehensive tests for the Game class
 * Tests game initialization, player management, turn progression, and victory conditions
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <stdexcept>
#include <algorithm>
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/roles/Governor.hpp"
#include "../include/roles/General.hpp"
#include "../include/roles/Judge.hpp"
#include "../include/roles/Baron.hpp"
#include "../include/roles/Spy.hpp"
#include "../include/roles/Merchant.hpp"

using namespace coup;

TEST_CASE("Game Construction and Initialization") {
    SUBCASE("Default constructor creates empty game") {
        Game game;
        CHECK_FALSE(game.isGameStarted());
        CHECK(game.players().empty());
        CHECK_THROWS_AS(game.turn(), std::runtime_error);
        CHECK_THROWS_AS(game.winner(), std::runtime_error);
    }
}

TEST_CASE("Player Addition and Management") {
    Game game{};
    
    SUBCASE("Adding valid players") {
        Governor p1(game, "Alice");
        Spy p2(game, "Bob");
        
        auto player_names = game.players();
        CHECK(player_names.size() == 2);
        CHECK(std::find(player_names.begin(), player_names.end(), "Alice") != player_names.end());
        CHECK(std::find(player_names.begin(), player_names.end(), "Bob") != player_names.end());
    }
    
    SUBCASE("Maximum player limit enforcement") {
        // Add 6 players (maximum allowed)
        Governor p1(game, "Player1");
        Spy p2(game, "Player2");
        Baron p3(game, "Player3");
        General p4(game, "Player4");
        Judge p5(game, "Player5");
        Merchant p6(game, "Player6");
        
        CHECK(game.players().size() == 6);
        
        // Adding 7th player should throw exception
        CHECK_THROWS_AS(Governor p7(game, "Player7"), std::runtime_error);
    }
    
    SUBCASE("Cannot add players after game starts") {
        Merchant p1(game, "Alice");
        Baron p2(game, "Bob");
        
        game.startGame();
        CHECK_THROWS_AS(General p3(game, "Charlie"), std::runtime_error);
    }
}

TEST_CASE("Game Start Validation") {
    Game game;
    
    SUBCASE("Cannot start with insufficient players") {
        CHECK_THROWS_AS(game.startGame(), std::runtime_error);
        
        Player p1(game, "Alice");
        CHECK_THROWS_AS(game.startGame(), std::runtime_error);
    }
    
    SUBCASE("Valid game start with minimum players") {
        Player p1(game, "Alice");
        Player p2(game, "Bob");
        
        CHECK_NOTHROW(game.startGame());
        CHECK(game.isGameStarted());
    }
    
    SUBCASE("Valid game start with maximum players") {
        Player p1(game, "Player1");
        Player p2(game, "Player2");
        Player p3(game, "Player3");
        Player p4(game, "Player4");
        Player p5(game, "Player5");
        Player p6(game, "Player6");
        
        CHECK_NOTHROW(game.startGame());
        CHECK(game.isGameStarted());
    }
}

TEST_CASE("Turn Management") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    Player p3(game, "Charlie");
    
    SUBCASE("Turn progression before game starts") {
        CHECK_THROWS_AS(game.turn(), std::runtime_error);
        CHECK_THROWS_AS(game.nextTurn(), std::runtime_error);
    }
    
    SUBCASE("Turn progression after game starts") {
        game.startGame();
        
        // First player's turn
        CHECK_NOTHROW(game.turn());
        CHECK(game.isPlayerTurn(&p1));
        CHECK_FALSE(game.isPlayerTurn(&p2));
        CHECK_FALSE(game.isPlayerTurn(&p3));
        
        // Advance to second player
        game.nextTurn();
        CHECK(game.isPlayerTurn(&p2));
        CHECK_FALSE(game.isPlayerTurn(&p1));
        CHECK_FALSE(game.isPlayerTurn(&p3));
        
        // Advance to third player
        game.nextTurn();
        CHECK(game.isPlayerTurn(&p3));
        CHECK_FALSE(game.isPlayerTurn(&p1));
        CHECK_FALSE(game.isPlayerTurn(&p2));
        
        // Cycle back to first player
        game.nextTurn();
        CHECK(game.isPlayerTurn(&p1));
    }
    
    SUBCASE("Turn skipping eliminated players") {
        game.startGame();
        
        // Eliminate second player
        p2.setActivityStatus(false);
        
        // Should skip from player 1 to player 3
        CHECK(game.isPlayerTurn(&p1));
        game.nextTurn();
        CHECK(game.isPlayerTurn(&p3));
        
        // Should cycle back to player 1 (skipping inactive player 2)
        game.nextTurn();
        CHECK(game.isPlayerTurn(&p1));
    }
}

TEST_CASE("Winner Determination") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    Player p3(game, "Charlie");
    
    SUBCASE("Winner before game starts") {
        CHECK_THROWS_AS(game.winner(), std::runtime_error);
    }
    
    SUBCASE("No winner with multiple active players") {
        game.startGame();
        CHECK_THROWS_AS(game.winner(), std::runtime_error);
    }
    
    SUBCASE("Winner with single active player") {
        game.startGame();
        
        // Eliminate two players
        p2.setActivityStatus(false);
        p3.setActivityStatus(false);
        
        CHECK(game.winner() == "Alice");
    }
    
    SUBCASE("No winner with all players eliminated") {
        game.startGame();
        
        // Eliminate all players
        p1.setActivityStatus(false);
        p2.setActivityStatus(false);
        p3.setActivityStatus(false);
        
        CHECK_THROWS_AS(game.winner(), std::runtime_error);
    }
}

TEST_CASE("Active Players Management") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    Player p3(game, "Charlie");
    
    game.startGame();
    
    SUBCASE("All players initially active") {
        auto active = game.getActivePlayers();
        CHECK(active.size() == 3);
        
        auto names = game.players();
        CHECK(names.size() == 3);
    }
    
    SUBCASE("Active count after eliminations") {
        p2.setActivityStatus(false);
        
        auto active = game.getActivePlayers();
        CHECK(active.size() == 2);
        
        auto names = game.players();
        CHECK(names.size() == 2);
        CHECK(std::find(names.begin(), names.end(), "Bob") == names.end());
    }
}

TEST_CASE("Last Arrested Player Tracking") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    
    SUBCASE("Initial state") {
        CHECK(game.getLastArrestedPlayer() == nullptr);
    }
    
    SUBCASE("Setting and getting last arrested player") {
        game.setLastArrestedPlayer(&p1);
        CHECK(game.getLastArrestedPlayer() == &p1);
        
        game.setLastArrestedPlayer(&p2);
        CHECK(game.getLastArrestedPlayer() == &p2);
        
        game.setLastArrestedPlayer(nullptr);
        CHECK(game.getLastArrestedPlayer() == nullptr);
    }
}

TEST_CASE("Game State Validation") {
    Game game;
    
    SUBCASE("Operations before game starts") {
        CHECK_FALSE(game.isGameStarted());
        CHECK(game.getCurrentPlayer() == nullptr);
        CHECK_THROWS_AS(game.turn(), std::runtime_error);
        CHECK_THROWS_AS(game.nextTurn(), std::runtime_error);
    }
    
    SUBCASE("Operations after game starts") {
        Player p1(game, "Alice");
        Player p2(game, "Bob");
        game.startGame();
        
        CHECK(game.isGameStarted());
        CHECK(game.getCurrentPlayer() != nullptr);
        CHECK_NOTHROW(game.turn());
        CHECK_NOTHROW(game.nextTurn());
    }
}

TEST_CASE("Edge Cases and Boundary Conditions") {
    SUBCASE("Empty player name") {
        Game game;
        CHECK_THROWS_AS(Player p(game, ""), std::invalid_argument);
    }
    
    SUBCASE("Very long player name") {
        Game game;
        CHECK_THROWS_AS(Player p(game, "VeryLongPlayerNameThatExceedsLimit"), std::invalid_argument);
    }
    
    SUBCASE("Turn progression with no active players") {
        Game game;
        Player p1(game, "Alice");
        Player p2(game, "Bob");
        game.startGame();
        
        // Eliminate all players
        p1.setActivityStatus(false);
        p2.setActivityStatus(false);
        
        // Turn progression should handle empty active player list
        CHECK_THROWS_AS(game.nextTurn(), std::runtime_error);
    }
}

TEST_CASE("Memory Management and Cleanup") {
    SUBCASE("Game destructor cleanup") {
        Game* game = new Game();
        Player* p1 = new Player(*game, "Alice");
        Player* p2 = new Player(*game, "Bob");
        
        // Game destructor should clean up player objects
        delete game; // This should delete associated players
    }
    
    SUBCASE("Clear all players functionality") {
        Game game;
        Player p1(game, "Alice");
        Player p2(game, "Bob");
        
        CHECK(game.players().size() == 2);
        
        // Can clear before game starts
        game.clearAllPlayers();
        CHECK(game.players().empty());
        
        // Cannot clear after game starts
        Player p3(game, "Charlie");
        Player p4(game, "David");
        game.startGame();
        CHECK_THROWS_AS(game.clearAllPlayers(), std::runtime_error);
    }
}

TEST_CASE("Role Assignment and Factory Methods") {
    Game game;
    
    SUBCASE("Role type enumeration") {
        CHECK(game.getRoleName(RoleType::GOVERNOR) == "Governor");
        CHECK(game.getRoleName(RoleType::GENERAL) == "General");
        CHECK(game.getRoleName(RoleType::JUDGE) == "Judge");
        CHECK(game.getRoleName(RoleType::BARON) == "Baron");
        CHECK(game.getRoleName(RoleType::SPY) == "Spy");
        CHECK(game.getRoleName(RoleType::MERCHANT) == "Merchant");
    }
    
    SUBCASE("Player creation with specific roles") {
        Player* governor = game.createPlayerWithRole("Gov", RoleType::GOVERNOR);
        Player* general = game.createPlayerWithRole("Gen", RoleType::GENERAL);
        Player* judge = game.createPlayerWithRole("Judge", RoleType::JUDGE);
        
        CHECK(governor->getRoleType() == "Governor");
        CHECK(general->getRoleType() == "General");
        CHECK(judge->getRoleType() == "Judge");
        
        CHECK(general->isGeneral());
        CHECK(judge->isJudge());
        CHECK_FALSE(governor->isGeneral());
    }
}
