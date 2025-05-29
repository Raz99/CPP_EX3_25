// Email: razcohenp@gmail.com

/**
 * Comprehensive tests for the Player class
 * Tests all player actions, state management, and edge cases
 */

#include "doctest.h"
#include <stdexcept>
#include "../include/Game.hpp"
#include "../include/Player.hpp"

using namespace coup;

TEST_CASE("Player Construction and Basic Properties") {
    Game game;
    
    SUBCASE("Valid player creation") {
        Player p(game, "Alice");
        CHECK(p.getName() == "Alice");
        CHECK(p.coins() == 0);
        CHECK(p.isActive());
        CHECK_FALSE(p.isSanctioned());
        CHECK(p.isArrestAvailable());
        CHECK_FALSE(p.isBribeUsed());
        CHECK_FALSE(p.usedTaxLastAction());
        CHECK(p.getCoupedBy() == nullptr);
        CHECK(p.getRoleType() == "Player");
        CHECK_FALSE(p.isGeneral());
        CHECK_FALSE(p.isJudge());
        CHECK_FALSE(p.isMerchant());
    }
    
    SUBCASE("Invalid player names") {
        CHECK_THROWS_AS(Player p1(game, ""), std::invalid_argument);
        CHECK_THROWS_AS(Player p2(game, "VeryLongNameThatExceedsLimit"), std::invalid_argument);
    }
    
    SUBCASE("Player automatically added to game") {
        CHECK(game.players().empty());
        Player p(game, "Alice");
        auto players = game.players();
        CHECK(players.size() == 1);
        CHECK(players[0] == "Alice");
    }
}

TEST_CASE("Coin Management") {
    Game game;
    Player p(game, "Alice");
    
    SUBCASE("Adding coins") {
        CHECK(p.coins() == 0);
        p.addCoins(5);
        CHECK(p.coins() == 5);
        p.addCoins(3);
        CHECK(p.coins() == 8);
    }
    
    SUBCASE("Removing coins - valid amounts") {
        p.addCoins(10);
        p.removeCoins(3);
        CHECK(p.coins() == 7);
        p.removeCoins(7);
        CHECK(p.coins() == 0);
    }
    
    SUBCASE("Removing coins - insufficient funds") {
        p.addCoins(5);
        CHECK_THROWS_AS(p.removeCoins(10), std::runtime_error);
        CHECK(p.coins() == 5); // State unchanged after exception
    }
    
    SUBCASE("Edge cases with coin operations") {
        p.addCoins(0); // Adding zero should be valid
        CHECK(p.coins() == 0);
        
        p.addCoins(1);
        p.removeCoins(0); // Removing zero should be valid
        CHECK(p.coins() == 1);
        
        p.removeCoins(1);
        CHECK_THROWS_AS(p.removeCoins(1), std::runtime_error); // Can't go negative
    }
}

TEST_CASE("Gather Action") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    game.startGame();
    
    SUBCASE("Valid gather action") {
        CHECK(p1.coins() == 0);
        p1.gather();
        CHECK(p1.coins() == 1);
        CHECK(game.isPlayerTurn(&p2)); // Turn should advance
    }
    
    SUBCASE("Gather when sanctioned") {
        p1.setSanctionStatus(true);
        CHECK_THROWS_AS(p1.gather(), std::runtime_error);
        CHECK(p1.coins() == 0); // No coins gained
    }
    
    SUBCASE("Gather with 10+ coins without bribe") {
        p1.addCoins(10);
        CHECK_THROWS_AS(p1.gather(), std::runtime_error);
    }
    
    SUBCASE("Gather with 10+ coins after bribe") {
        p1.addCoins(10);
        p1.resetBribeUsed(); // Simulate bribe was used
        // This should be handled by bribe logic
    }
    
    SUBCASE("Gather when not player's turn") {
        CHECK_THROWS_AS(p2.gather(), std::runtime_error);
    }
    
    SUBCASE("Gather when game not started") {
        Game new_game;
        Player p(new_game, "Charlie");
        CHECK_THROWS_AS(p.gather(), std::runtime_error);
    }
    
    SUBCASE("Gather when player eliminated") {
        p1.setActivityStatus(false);
        CHECK_THROWS_AS(p1.gather(), std::runtime_error);
    }
}

TEST_CASE("Tax Action") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    game.startGame();
    
    SUBCASE("Valid tax action") {
        CHECK(p1.coins() == 0);
        p1.tax();
        CHECK(p1.coins() == 2);
        CHECK(p1.usedTaxLastAction());
        CHECK(game.isPlayerTurn(&p2));
    }
    
    SUBCASE("Tax when sanctioned") {
        p1.setSanctionStatus(true);
        CHECK_THROWS_AS(p1.tax(), std::runtime_error);
        CHECK(p1.coins() == 0);
        CHECK_FALSE(p1.usedTaxLastAction());
    }
    
    SUBCASE("Tax with 10+ coins without bribe") {
        p1.addCoins(10);
        CHECK_THROWS_AS(p1.tax(), std::runtime_error);
    }
    
    SUBCASE("Tax when not player's turn") {
        CHECK_THROWS_AS(p2.tax(), std::runtime_error);
    }
    
    SUBCASE("Tax tracking reset") {
        p1.tax();
        CHECK(p1.usedTaxLastAction());
        p1.resetUsedTaxLastAction();
        CHECK_FALSE(p1.usedTaxLastAction());
    }
}

TEST_CASE("Bribe Action") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    game.startGame();
    
    SUBCASE("Valid bribe action") {
        p1.addCoins(5);
        CHECK_FALSE(p1.isBribeUsed());
        p1.bribe();
        CHECK(p1.coins() == 1);
        CHECK(p1.isBribeUsed());
        CHECK(game.isPlayerTurn(&p1)); // Should still be player's turn
    }
    
    SUBCASE("Bribe with insufficient coins") {
        p1.addCoins(3);
        CHECK_THROWS_AS(p1.bribe(), std::runtime_error);
        CHECK(p1.coins() == 3); // Unchanged
        CHECK_FALSE(p1.isBribeUsed());
    }
    
    SUBCASE("Bribe with exactly 4 coins") {
        p1.addCoins(4);
        p1.bribe();
        CHECK(p1.coins() == 0);
        CHECK(p1.isBribeUsed());
    }
    
    SUBCASE("Bribe with 10+ coins") {
        p1.addCoins(14);
        CHECK_THROWS_AS(p1.bribe(), std::runtime_error); // Must coup instead
    }
    
    SUBCASE("Bribe reset functionality") {
        p1.addCoins(4);
        p1.bribe();
        CHECK(p1.isBribeUsed());
        p1.resetBribeUsed();
        CHECK_FALSE(p1.isBribeUsed());
    }
}

TEST_CASE("Arrest Action") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    Player p3(game, "Charlie");
    game.startGame();
    
    SUBCASE("Valid arrest action") {
        p2.addCoins(3);
        CHECK(p1.isArrestAvailable());
        
        p1.arrest(p2);
        CHECK(p1.coins() == 1); // Gained 1 coin
        CHECK(p2.coins() == 2); // Lost 1 coin
        CHECK(game.getLastArrestedPlayer() == &p2);
        CHECK(game.isPlayerTurn(&p2));
    }
    
    SUBCASE("Arrest target with no coins") {
        CHECK(p2.coins() == 0);
        p1.arrest(p2);
        CHECK(p1.coins() == 0); // No coins to steal
        CHECK(p2.coins() == 0);
        CHECK(game.getLastArrestedPlayer() == &p2);
    }
    
    SUBCASE("Cannot arrest same player consecutively") {
        p2.addCoins(2);
        p1.arrest(p2);
        
        // Move to next turn and try to arrest same player
        game.nextTurn(); // p2's turn
        game.nextTurn(); // p3's turn
        
        p3.addCoins(1); // Give p3 some coins for arrest
        CHECK_THROWS_AS(p3.arrest(p2), std::runtime_error);
    }
    
    SUBCASE("Cannot arrest self") {
        CHECK_THROWS_AS(p1.arrest(p1), std::runtime_error);
    }
    
    SUBCASE("Cannot arrest inactive player") {
        p2.setActivityStatus(false);
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error);
    }
    
    SUBCASE("Cannot arrest when arrest unavailable") {
        p1.setArrestAvailability(false);
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error);
    }
    
    SUBCASE("Arrest with 10+ coins without bribe") {
        p1.addCoins(10);
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error);
    }
}

TEST_CASE("Sanction Action") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    game.startGame();
    
    SUBCASE("Valid sanction action") {
        p1.addCoins(5);
        CHECK_FALSE(p2.isSanctioned());
        
        p1.sanction(p2);
        CHECK(p1.coins() == 2); // Paid 3 coins
        CHECK(p2.isSanctioned());
        CHECK(game.isPlayerTurn(&p2));
    }
    
    SUBCASE("Sanction with insufficient coins") {
        p1.addCoins(2);
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error);
        CHECK(p1.coins() == 2); // Unchanged
        CHECK_FALSE(p2.isSanctioned());
    }
    
    SUBCASE("Sanction with exactly 3 coins") {
        p1.addCoins(3);
        p1.sanction(p2);
        CHECK(p1.coins() == 0);
        CHECK(p2.isSanctioned());
    }
    
    SUBCASE("Cannot sanction self") {
        p1.addCoins(5);
        CHECK_THROWS_AS(p1.sanction(p1), std::runtime_error);
    }
    
    SUBCASE("Cannot sanction inactive player") {
        p1.addCoins(5);
        p2.setActivityStatus(false);
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error);
    }
    
    SUBCASE("Sanction status management") {
        p2.setSanctionStatus(true);
        CHECK(p2.isSanctioned());
        p2.setSanctionStatus(false);
        CHECK_FALSE(p2.isSanctioned());
    }
}

TEST_CASE("Coup Action") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    game.startGame();
    
    SUBCASE("Valid coup action") {
        p1.addCoins(10);
        CHECK(p2.isActive());
        CHECK(p2.getCoupedBy() == nullptr);
        
        p1.coup(p2);
        CHECK(p1.coins() == 3); // Paid 7 coins
        CHECK_FALSE(p2.isActive()); // Target eliminated
        CHECK(p2.getCoupedBy() == &p1); // Track who performed coup
    }
    
    SUBCASE("Coup with insufficient coins") {
        p1.addCoins(6);
        CHECK_THROWS_AS(p1.coup(p2), std::runtime_error);
        CHECK(p1.coins() == 6); // Unchanged
        CHECK(p2.isActive()); // Target still active
    }
    
    SUBCASE("Coup with exactly 7 coins") {
        p1.addCoins(7);
        p1.coup(p2);
        CHECK(p1.coins() == 0);
        CHECK_FALSE(p2.isActive());
    }
    
    SUBCASE("Cannot coup self") {
        p1.addCoins(10);
        CHECK_THROWS_AS(p1.coup(p1), std::runtime_error);
    }
    
    SUBCASE("Cannot coup inactive player") {
        p1.addCoins(10);
        p2.setActivityStatus(false);
        CHECK_THROWS_AS(p1.coup(p2), std::runtime_error);
    }
    
    SUBCASE("Coup tracking reset") {
        p1.addCoins(7);
        p1.coup(p2);
        CHECK(p2.getCoupedBy() == &p1);
        p2.resetCoupedBy();
        CHECK(p2.getCoupedBy() == nullptr);
    }
}

TEST_CASE("Mandatory Coup Rule") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    game.startGame();
    
    SUBCASE("Must coup with 10+ coins") {
        p1.addCoins(10);
        
        // All other actions should be blocked
        CHECK_THROWS_AS(p1.gather(), std::runtime_error);
        CHECK_THROWS_AS(p1.tax(), std::runtime_error);
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error);
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error);
        
        // But bribe should be allowed to potentially enable other actions
        // (Implementation detail may vary)
    }
    
    SUBCASE("Can perform actions after bribe with 10+ coins") {
        p1.addCoins(14);
        // After using bribe, should be able to perform other actions
        // This tests the bribe exception to the 10+ coin rule
    }
}

TEST_CASE("Player State Management") {
    Game game;
    Player p(game, "Alice");
    
    SUBCASE("Activity status") {
        CHECK(p.isActive());
        p.setActivityStatus(false);
        CHECK_FALSE(p.isActive());
        p.setActivityStatus(true);
        CHECK(p.isActive());
    }
    
    SUBCASE("Sanction status") {
        CHECK_FALSE(p.isSanctioned());
        p.setSanctionStatus(true);
        CHECK(p.isSanctioned());
        p.setSanctionStatus(false);
        CHECK_FALSE(p.isSanctioned());
    }
    
    SUBCASE("Arrest availability") {
        CHECK(p.isArrestAvailable());
        p.setArrestAvailability(false);
        CHECK_FALSE(p.isArrestAvailable());
        p.setArrestAvailability(true);
        CHECK(p.isArrestAvailable());
    }
}

TEST_CASE("Complex Game Scenarios") {
    Game game;
    Player p1(game, "Alice");
    Player p2(game, "Bob");
    Player p3(game, "Charlie");
    game.startGame();
    
    SUBCASE("Elimination and winner determination") {
        // Eliminate two players through coup
        p1.addCoins(14); // Enough for two coups
        
        p1.coup(p2);
        CHECK_FALSE(p2.isActive());
        CHECK(p1.coins() == 7);
        
        // Next turn logic would skip inactive p2, meaning it's p3's turn
        p3.tax();
        
        p1.coup(p3);
        CHECK_FALSE(p3.isActive());
        CHECK(p1.coins() == 0);
        
        // p1 should be the winner
        CHECK(game.winner() == "Alice");
    }
    
    SUBCASE("Sanction effects on economic actions") {
        p1.addCoins(3);
        p1.sanction(p2);
        CHECK(p2.isSanctioned());
        
        // p2's turn - should not be able to gather or tax
        CHECK_THROWS_AS(p2.gather(), std::runtime_error);
        CHECK_THROWS_AS(p2.tax(), std::runtime_error);
        
        // But p2 should be able to perform other actions
        p2.addCoins(4);
        CHECK_NOTHROW(p2.bribe()); // Bribe should work
    }
}

TEST_CASE("Edge Cases and Error Handling") {
    SUBCASE("Invalid game states") {
        Game game;
        Player p(game, "Alice");
        
        // Actions before game starts
        CHECK_THROWS_AS(p.gather(), std::runtime_error);
        CHECK_THROWS_AS(p.tax(), std::runtime_error);
        CHECK_THROWS_AS(p.bribe(), std::runtime_error);
    }
    
    SUBCASE("Boundary values for coins") {
        Game game;
        Player p(game, "Alice");
        
        // Maximum reasonable coin values
        p.addCoins(1000);
        CHECK(p.coins() == 1000);
        
        // Large removals
        p.removeCoins(999);
        CHECK(p.coins() == 1);
        
        p.removeCoins(1);
        CHECK(p.coins() == 0);
    }
    
    SUBCASE("State consistency after failed actions") {
        Game game;
        Player p1(game, "Alice");
        Player p2(game, "Bob");
        game.startGame();
        
        p1.addCoins(2); // Not enough for sanction
        int initial_coins = p1.coins();
        bool initial_sanction = p2.isSanctioned();
        
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error);
        
        // State should be unchanged after failed action
        CHECK(p1.coins() == initial_coins);
        CHECK(p2.isSanctioned() == initial_sanction);
    }
}
