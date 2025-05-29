// Email: razcohenp@gmail.com

/**
 * Comprehensive tests for the Player class
 * Tests all player actions, state management, and edge cases
 * Covers the core player mechanics of the Coup card game including:
 * - Basic actions: gather (1 coin), tax (2 coins), bribe (4 coins cost)
 * - Interaction actions: arrest (steal 1 coin), sanction (3 coins to block actions)
 * - Coup action: pay 7 coins to eliminate a player
 * - Mandatory coup rule: must coup when having 10+ coins
 * - Player state management and turn validation
 */

#include "doctest.h"
#include <stdexcept>
#include "../include/Game.hpp"
#include "../include/Player.hpp"

using namespace coup;

TEST_CASE("Player Construction and Basic Properties") {
    Game game; // Create game instance for player testing
    
    SUBCASE("Valid player creation") {
        Player p(game, "Alice"); // Create player with valid name
        CHECK(p.getName() == "Alice"); // Name should be set correctly
        CHECK(p.coins() == 0); // Players start with 0 coins
        CHECK(p.isActive()); // Players start active (not eliminated)
        CHECK_FALSE(p.isSanctioned()); // Players start unsanctioned
        CHECK(p.isArrestAvailable()); // Players can be arrested initially
        CHECK_FALSE(p.isBribeUsed()); // Players haven't used bribe initially
        CHECK_FALSE(p.usedTaxLastAction()); // Players haven't used tax initially
        CHECK(p.getCoupedBy() == nullptr); // Players haven't been couped initially
        CHECK(p.getRoleType() == "Player"); // Base player type
        CHECK_FALSE(p.isGeneral()); // Not a General role
        CHECK_FALSE(p.isJudge()); // Not a Judge role
        CHECK_FALSE(p.isMerchant()); // Not a Merchant role
    }
    
    SUBCASE("Invalid player names") {
        CHECK_THROWS_AS(Player p1(game, ""), std::invalid_argument); // Empty name not allowed
        CHECK_THROWS_AS(Player p2(game, "VeryLongNameThatExceedsLimit"), std::invalid_argument); // Name too long
    }
    
    SUBCASE("Player automatically added to game") {
        CHECK(game.players().empty()); // Game starts with no players
        Player p(game, "Alice"); // Create player
        auto players = game.players(); // Get player list
        CHECK(players.size() == 1); // Should have 1 player now
        CHECK(players[0] == "Alice"); // Player name should be in list
    }
}

TEST_CASE("Coin Management") {
    Game game; // Create game for coin testing
    Player p(game, "Alice"); // Create player for testing
    
    SUBCASE("Adding coins") {
        CHECK(p.coins() == 0); // Start with 0 coins
        p.addCoins(5); // Add 5 coins
        CHECK(p.coins() == 5); // Should have 5 coins
        p.addCoins(3); // Add 3 more coins
        CHECK(p.coins() == 8); // Should have 8 coins total
    }
    
    SUBCASE("Removing coins - valid amounts") {
        p.addCoins(10); // Give player 10 coins
        p.removeCoins(3); // Remove 3 coins
        CHECK(p.coins() == 7); // Should have 7 coins left
        p.removeCoins(7); // Remove all remaining coins
        CHECK(p.coins() == 0); // Should have 0 coins
    }
    
    SUBCASE("Removing coins - insufficient funds") {
        p.addCoins(5); // Give player 5 coins
        CHECK_THROWS_AS(p.removeCoins(10), std::runtime_error); // Cannot remove more than owned
        CHECK(p.coins() == 5); // State unchanged after exception
    }
    
    SUBCASE("Edge cases with coin operations") {
        p.addCoins(0); // Adding zero should be valid
        CHECK(p.coins() == 0); // Should still have 0 coins
        
        p.addCoins(1); // Add 1 coin
        p.removeCoins(0); // Removing zero should be valid
        CHECK(p.coins() == 1); // Should still have 1 coin
        
        p.removeCoins(1); // Remove the coin
        CHECK_THROWS_AS(p.removeCoins(1), std::runtime_error); // Can't go negative
    }
}

TEST_CASE("Gather Action") {
    Game game; // Create game for gather testing
    Player p1(game, "Alice"); // Current player
    Player p2(game, "Bob"); // Next player
    game.startGame(); // Start game to enable actions
    
    SUBCASE("Valid gather action") {
        CHECK(p1.coins() == 0); // Start with 0 coins
        p1.gather(); // Perform gather action (gains 1 coin)
        CHECK(p1.coins() == 1); // Should have 1 coin now
        CHECK(game.isPlayerTurn(&p2)); // Turn should advance to next player
    }
    
    SUBCASE("Gather when sanctioned") {
        p1.setSanctionStatus(true); // Player is sanctioned (blocked from economic actions)
        CHECK_THROWS_AS(p1.gather(), std::runtime_error); // Cannot gather when sanctioned
        CHECK(p1.coins() == 0); // No coins gained
    }
    
    SUBCASE("Gather with 10+ coins without bribe") {
        p1.addCoins(10); // Give player 10 coins (mandatory coup threshold)
        CHECK_THROWS_AS(p1.gather(), std::runtime_error); // Must coup instead of other actions
    }
    
    SUBCASE("Gather with 10+ coins after bribe") {
        p1.addCoins(10); // Give player 10 coins
        p1.resetBribeUsed(); // Simulate bribe was used
        // This should be handled by bribe logic
    }
    
    SUBCASE("Gather when not player's turn") {
        CHECK_THROWS_AS(p2.gather(), std::runtime_error); // Only current player can act
    }
    
    SUBCASE("Gather when game not started") {
        Game new_game; // Create new unstarted game
        Player p(new_game, "Charlie"); // Add player
        CHECK_THROWS_AS(p.gather(), std::runtime_error); // Cannot act before game starts
    }
    
    SUBCASE("Gather when player eliminated") {
        p1.setActivityStatus(false); // Eliminate player
        CHECK_THROWS_AS(p1.gather(), std::runtime_error); // Eliminated players cannot act
    }
}

TEST_CASE("Tax Action") {
    Game game; // Create game for tax testing
    Player p1(game, "Alice"); // Current player
    Player p2(game, "Bob"); // Next player
    game.startGame(); // Start game to enable actions
    
    SUBCASE("Valid tax action") {
        CHECK(p1.coins() == 0); // Start with 0 coins
        p1.tax(); // Perform tax action (gains 2 coins)
        CHECK(p1.coins() == 2); // Should have 2 coins now
        CHECK(p1.usedTaxLastAction()); // Tax usage should be tracked
        CHECK(game.isPlayerTurn(&p2)); // Turn should advance to next player
    }
    
    SUBCASE("Tax when sanctioned") {
        p1.setSanctionStatus(true); // Player is sanctioned
        CHECK_THROWS_AS(p1.tax(), std::runtime_error); // Cannot tax when sanctioned
        CHECK(p1.coins() == 0); // No coins gained
        CHECK_FALSE(p1.usedTaxLastAction()); // Tax flag not set
    }
    
    SUBCASE("Tax with 10+ coins without bribe") {
        p1.addCoins(10); // Give player 10 coins
        CHECK_THROWS_AS(p1.tax(), std::runtime_error); // Must coup instead
    }
    
    SUBCASE("Tax when not player's turn") {
        CHECK_THROWS_AS(p2.tax(), std::runtime_error); // Only current player can act
    }
    
    SUBCASE("Tax tracking reset") {
        p1.tax(); // Use tax action
        CHECK(p1.usedTaxLastAction()); // Flag should be set
        p1.resetUsedTaxLastAction(); // Reset the flag
        CHECK_FALSE(p1.usedTaxLastAction()); // Flag should be cleared
    }
}

TEST_CASE("Bribe Action") {
    Game game; // Create game for bribe testing
    Player p1(game, "Alice"); // Current player
    Player p2(game, "Bob"); // Next player
    game.startGame(); // Start game to enable actions
    
    SUBCASE("Valid bribe action") {
        p1.addCoins(5); // Give player 5 coins
        CHECK_FALSE(p1.isBribeUsed()); // Bribe not used initially
        p1.bribe(); // Pay 4 coins to bribe (allows actions with 10+ coins)
        CHECK(p1.coins() == 1); // Should have 1 coin left (5-4=1)
        CHECK(p1.isBribeUsed()); // Bribe flag should be set
        CHECK(game.isPlayerTurn(&p1)); // Should still be player's turn
    }
    
    SUBCASE("Bribe with insufficient coins") {
        p1.addCoins(3); // Give player only 3 coins
        CHECK_THROWS_AS(p1.bribe(), std::runtime_error); // Need 4 coins to bribe
        CHECK(p1.coins() == 3); // Unchanged
        CHECK_FALSE(p1.isBribeUsed()); // Bribe flag not set
    }
    
    SUBCASE("Bribe with exactly 4 coins") {
        p1.addCoins(4); // Give player exactly 4 coins
        p1.bribe(); // Use all coins for bribe
        CHECK(p1.coins() == 0); // Should have 0 coins left
        CHECK(p1.isBribeUsed()); // Bribe flag should be set
    }
    
    SUBCASE("Bribe with 10+ coins") {
        p1.addCoins(14); // Give player 14 coins
        CHECK_THROWS_AS(p1.bribe(), std::runtime_error); // Must coup instead of bribe
    }
    
    SUBCASE("Bribe reset functionality") {
        p1.addCoins(4); // Give player 4 coins
        p1.bribe(); // Use bribe
        CHECK(p1.isBribeUsed()); // Flag should be set
        p1.resetBribeUsed(); // Reset the flag
        CHECK_FALSE(p1.isBribeUsed()); // Flag should be cleared
    }
}

TEST_CASE("Arrest Action") {
    Game game; // Create game for arrest testing
    Player p1(game, "Alice"); // Arrester
    Player p2(game, "Bob"); // Target
    Player p3(game, "Charlie"); // Third player
    game.startGame(); // Start game to enable actions
    
    SUBCASE("Valid arrest action") {
        p2.addCoins(3); // Give target 3 coins
        CHECK(p1.isArrestAvailable()); // Player can arrest initially
        
        p1.arrest(p2); // Arrest Bob (steal 1 coin)
        CHECK(p1.coins() == 1); // Alice gained 1 coin
        CHECK(p2.coins() == 2); // Bob lost 1 coin (3-1=2)
        CHECK(game.getLastArrestedPlayer() == &p2); // Track last arrested player
        CHECK(game.isPlayerTurn(&p2)); // Turn advances to arrested player
    }
    
    SUBCASE("Arrest target with no coins") {
        CHECK(p2.coins() == 0); // Bob has no coins
        p1.arrest(p2); // Arrest Bob anyway
        CHECK(p1.coins() == 0); // No coins to steal
        CHECK(p2.coins() == 0); // Bob still has no coins
        CHECK(game.getLastArrestedPlayer() == &p2); // Still tracked as arrested
    }
    
    SUBCASE("Cannot arrest same player consecutively") {
        p2.addCoins(2); // Give Bob coins
        p1.arrest(p2); // Alice arrests Bob
        
        // Move to next turn and try to arrest same player
        p2.tax(); // Bob's turn
        p3.gather(); // Charlie's turn
        
        p3.addCoins(1); // Give Charlie coins for arrest
        CHECK_THROWS_AS(p3.arrest(p2), std::runtime_error); // Cannot arrest same player consecutively
    }
    
    SUBCASE("Cannot arrest self") {
        CHECK_THROWS_AS(p1.arrest(p1), std::runtime_error); // Cannot arrest yourself
    }
    
    SUBCASE("Cannot arrest inactive player") {
        p2.setActivityStatus(false); // Eliminate Bob
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error); // Cannot arrest eliminated player
    }
    
    SUBCASE("Cannot arrest when arrest unavailable") {
        p1.setArrestAvailability(false); // Block arrest ability
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error); // Cannot arrest when blocked
    }
    
    SUBCASE("Arrest with 10+ coins without bribe") {
        p1.addCoins(10); // Give player 10 coins
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error); // Must coup instead
    }
}

TEST_CASE("Sanction Action") {
    Game game; // Create game for sanction testing
    Player p1(game, "Alice"); // Sanctioner
    Player p2(game, "Bob"); // Target
    game.startGame(); // Start game to enable actions
    
    SUBCASE("Valid sanction action") {
        p1.addCoins(5); // Give Alice 5 coins
        CHECK_FALSE(p2.isSanctioned()); // Bob not sanctioned initially
        
        p1.sanction(p2); // Pay 3 coins to sanction Bob
        CHECK(p1.coins() == 2); // Alice has 2 coins left (5-3=2)
        CHECK(p2.isSanctioned()); // Bob is now sanctioned
        CHECK(game.isPlayerTurn(&p2)); // Turn advances to sanctioned player
    }
    
    SUBCASE("Sanction with insufficient coins") {
        p1.addCoins(2); // Give Alice only 2 coins
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error); // Need 3 coins to sanction
        CHECK(p1.coins() == 2); // Coins unchanged
        CHECK_FALSE(p2.isSanctioned()); // Bob not sanctioned
    }
    
    SUBCASE("Sanction with exactly 3 coins") {
        p1.addCoins(3); // Give Alice exactly 3 coins
        p1.sanction(p2); // Use all coins for sanction
        CHECK(p1.coins() == 0); // Alice has 0 coins left
        CHECK(p2.isSanctioned()); // Bob is sanctioned
    }
    
    SUBCASE("Cannot sanction self") {
        p1.addCoins(5); // Give Alice coins
        CHECK_THROWS_AS(p1.sanction(p1), std::runtime_error); // Cannot sanction yourself
    }
    
    SUBCASE("Cannot sanction inactive player") {
        p1.addCoins(5); // Give Alice coins
        p2.setActivityStatus(false); // Eliminate Bob
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error); // Cannot sanction eliminated player
    }
    
    SUBCASE("Sanction status management") {
        p2.setSanctionStatus(true); // Set Bob as sanctioned
        CHECK(p2.isSanctioned()); // Bob should be sanctioned
        p2.setSanctionStatus(false); // Remove sanction
        CHECK_FALSE(p2.isSanctioned()); // Bob should not be sanctioned
    }
}

TEST_CASE("Coup Action") {
    Game game; // Create game for coup testing
    Player p1(game, "Alice"); // Attacker
    Player p2(game, "Bob"); // Target
    game.startGame(); // Start game to enable actions
    
    SUBCASE("Valid coup action") {
        p1.addCoins(10); // Give Alice 10 coins
        CHECK(p2.isActive()); // Bob is active initially
        CHECK(p2.getCoupedBy() == nullptr); // Bob not couped by anyone
        
        p1.coup(p2); // Pay 7 coins to eliminate Bob
        CHECK(p1.coins() == 3); // Alice has 3 coins left (10-7=3)
        CHECK_FALSE(p2.isActive()); // Bob is eliminated
        CHECK(p2.getCoupedBy() == &p1); // Track who performed coup
    }
    
    SUBCASE("Coup with insufficient coins") {
        p1.addCoins(6); // Give Alice only 6 coins
        CHECK_THROWS_AS(p1.coup(p2), std::runtime_error); // Need 7 coins to coup
        CHECK(p1.coins() == 6); // Unchanged
        CHECK(p2.isActive()); // Bob still active
    }
    
    SUBCASE("Coup with exactly 7 coins") {
        p1.addCoins(7); // Give Alice exactly 7 coins
        p1.coup(p2); // Use all coins for coup
        CHECK(p1.coins() == 0); // Alice has 0 coins left
        CHECK_FALSE(p2.isActive()); // Bob is eliminated
    }
    
    SUBCASE("Cannot coup self") {
        p1.addCoins(10); // Give Alice coins
        CHECK_THROWS_AS(p1.coup(p1), std::runtime_error); // Cannot coup yourself
    }
    
    SUBCASE("Cannot coup inactive player") {
        p1.addCoins(10); // Give Alice coins
        p2.setActivityStatus(false); // Eliminate Bob already
        CHECK_THROWS_AS(p1.coup(p2), std::runtime_error); // Cannot coup already eliminated player
    }
    
    SUBCASE("Coup tracking reset") {
        p1.addCoins(7); // Give Alice coins
        p1.coup(p2); // Coup Bob
        CHECK(p2.getCoupedBy() == &p1); // Bob should be tracked as couped by Alice
        p2.resetCoupedBy(); // Clear coup tracking
        CHECK(p2.getCoupedBy() == nullptr); // Should be cleared
    }
}

TEST_CASE("Mandatory Coup Rule") {
    Game game; // Create game for mandatory coup testing
    Player p1(game, "Alice"); // Player with many coins
    Player p2(game, "Bob"); // Potential target
    game.startGame(); // Start game to enable actions
    
    SUBCASE("Must coup with 10+ coins") {
        p1.addCoins(10); // Give Alice 10 coins (mandatory coup threshold)
        
        // All other actions should be blocked
        CHECK_THROWS_AS(p1.gather(), std::runtime_error); // Cannot gather with 10+ coins
        CHECK_THROWS_AS(p1.tax(), std::runtime_error); // Cannot tax with 10+ coins
        CHECK_THROWS_AS(p1.arrest(p2), std::runtime_error); // Cannot arrest with 10+ coins
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error); // Cannot sanction with 10+ coins
    }
}

TEST_CASE("Player State Management") {
    Game game; // Create game for state testing
    Player p(game, "Alice"); // Test player
    
    SUBCASE("Activity status") {
        CHECK(p.isActive()); // Player starts active
        p.setActivityStatus(false); // Eliminate player
        CHECK_FALSE(p.isActive()); // Player should be inactive
        p.setActivityStatus(true); // Restore player
        CHECK(p.isActive()); // Player should be active again
    }
    
    SUBCASE("Sanction status") {
        CHECK_FALSE(p.isSanctioned()); // Player starts unsanctioned
        p.setSanctionStatus(true); // Apply sanction
        CHECK(p.isSanctioned()); // Player should be sanctioned
        p.setSanctionStatus(false); // Remove sanction
        CHECK_FALSE(p.isSanctioned()); // Player should not be sanctioned
    }
    
    SUBCASE("Arrest availability") {
        CHECK(p.isArrestAvailable()); // Player can be arrested initially
        p.setArrestAvailability(false); // Block arrests
        CHECK_FALSE(p.isArrestAvailable()); // Player cannot be arrested
        p.setArrestAvailability(true); // Allow arrests again
        CHECK(p.isArrestAvailable()); // Player can be arrested again
    }
}

TEST_CASE("Complex Game Scenarios") {
    Game game; // Create game for complex scenario testing
    Player p1(game, "Alice"); // First player
    Player p2(game, "Bob"); // Second player
    Player p3(game, "Charlie"); // Third player
    game.startGame(); // Start game
    
    SUBCASE("Elimination and winner determination") {
        // Eliminate two players through coup
        p1.addCoins(14); // Give Alice enough for two coups
        
        p1.coup(p2); // Alice coups Bob
        CHECK_FALSE(p2.isActive()); // Bob should be eliminated
        CHECK(p1.coins() == 7); // Alice should have 7 coins left
        
        // Next turn logic would skip inactive p2, meaning it's p3's turn
        p3.tax(); // Charlie takes a turn
        
        p1.coup(p3); // Alice coups Charlie
        CHECK_FALSE(p3.isActive()); // Charlie should be eliminated
        CHECK(p1.coins() == 0); // Alice should have 0 coins left
        
        // p1 should be the winner
        CHECK(game.winner() == "Alice"); // Alice wins (last player standing)
    }
    
    SUBCASE("Sanction effects on economic actions") {
        p1.addCoins(3); // Give Alice coins
        p1.sanction(p2); // Alice sanctions Bob
        CHECK(p2.isSanctioned()); // Bob should be sanctioned
        
        // p2's turn - should not be able to gather or tax
        CHECK_THROWS_AS(p2.gather(), std::runtime_error); // Sanctioned players cannot gather
        CHECK_THROWS_AS(p2.tax(), std::runtime_error); // Sanctioned players cannot tax
        
        // But p2 should be able to perform other actions
        p2.addCoins(4); // Give Bob coins
        CHECK_NOTHROW(p2.bribe()); // Bribe should work even when sanctioned
    }
}

TEST_CASE("Edge Cases and Error Handling") {
    SUBCASE("Invalid game states") {
        Game game; // Create unstarted game
        Player p(game, "Alice"); // Add player
        
        // Actions before game starts
        CHECK_THROWS_AS(p.gather(), std::runtime_error); // Cannot gather before game starts
        CHECK_THROWS_AS(p.tax(), std::runtime_error); // Cannot tax before game starts
        CHECK_THROWS_AS(p.bribe(), std::runtime_error); // Cannot bribe before game starts
    }
    
    SUBCASE("Boundary values for coins") {
        Game game; // Create game
        Player p(game, "Alice"); // Add player
        
        // Maximum reasonable coin values
        p.addCoins(1000); // Add large number of coins
        CHECK(p.coins() == 1000); // Should handle large values
        
        // Large removals
        p.removeCoins(999); // Remove most coins
        CHECK(p.coins() == 1); // Should have 1 coin left
        
        p.removeCoins(1); // Remove last coin
        CHECK(p.coins() == 0); // Should have 0 coins
    }
    
    SUBCASE("State consistency after failed actions") {
        Game game; // Create game
        Player p1(game, "Alice"); // First player
        Player p2(game, "Bob"); // Second player
        game.startGame(); // Start game
        
        p1.addCoins(2); // Give Alice insufficient coins
        int initial_coins = p1.coins(); // Store initial state
        bool initial_sanction = p2.isSanctioned(); // Store initial sanction status
        
        CHECK_THROWS_AS(p1.sanction(p2), std::runtime_error); // Should fail due to insufficient coins
        
        // State should be unchanged after failed action
        CHECK(p1.coins() == initial_coins); // Coins should be unchanged
        CHECK(p2.isSanctioned() == initial_sanction); // Sanction status should be unchanged
    }
}