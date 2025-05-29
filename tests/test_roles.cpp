// Email: razcohenp@gmail.com

/**
 * Comprehensive tests for all role classes
 * Tests unique abilities, role identification, and role-specific interactions
 */

#include "doctest.h"
#include <stdexcept>
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/roles/Governor.hpp"
#include "../include/roles/General.hpp"
#include "../include/roles/Judge.hpp"
#include "../include/roles/Baron.hpp"
#include "../include/roles/Spy.hpp"
#include "../include/roles/Merchant.hpp"

using namespace coup;

TEST_CASE("Governor Role Tests") {
    Game game;
    Governor gov1(game, "Governor");
    Baron baron(game, "Baron");
    game.startGame();
    
    SUBCASE("Governor role identification") {
        CHECK(gov1.getRoleType() == "Governor");
        CHECK_FALSE(gov1.isGeneral());
        CHECK_FALSE(gov1.isJudge());
        CHECK_FALSE(gov1.isMerchant());
    }
    
    SUBCASE("Enhanced tax ability - 3 coins instead of 2") {
        CHECK(gov1.coins() == 0);
        gov1.tax();
        CHECK(gov1.coins() == 3); // Governor gets 3 coins from tax
        CHECK(gov1.usedTaxLastAction());
    }
    
    SUBCASE("Undo action - valid target") {
        // Set up scenario where regular player used tax
        CHECK_NOTHROW(gov1.tax()); // Governor taxes themselves to move the turn forward
        CHECK_NOTHROW(baron.tax());
        CHECK(baron.coins() == 2);
        CHECK(baron.usedTaxLastAction());
        
        // Governor undoes the tax
        gov1.undo(baron);
        CHECK(baron.coins() == 0); // Lost the 2 coins from tax
        CHECK_FALSE(baron.usedTaxLastAction()); // Flag reset
    }
    
    SUBCASE("Undo action - invalid targets") {
        // Cannot undo if target didn't use tax as last action
        CHECK_THROWS_AS(gov1.undo(baron), std::runtime_error);
        
        // Cannot undo on self
        gov1.tax();
        CHECK_THROWS_AS(gov1.undo(gov1), std::runtime_error);
        
        // Cannot undo inactive player
        baron.setActivityStatus(false);
        CHECK_THROWS_AS(gov1.undo(baron), std::runtime_error);
    }
    
    SUBCASE("Undo when game not started") {
        Game new_game;
        Governor new_gov(new_game, "NewGov");
        Baron new_baron(new_game, "NewBaron");
        
        CHECK_THROWS(new_gov.undo(new_baron));
    }
}

TEST_CASE("General Role Tests") {
    Game game;
    General gen(game, "General");
    Player victim(game, "Victim");
    Player attacker(game, "Attacker");
    game.startGame();
    
    SUBCASE("General role identification") {
        CHECK(gen.getRoleType() == "General");
        CHECK(gen.isGeneral());
        CHECK_FALSE(gen.isJudge());
        CHECK_FALSE(gen.isMerchant());
    }
    
    SUBCASE("Block coup - valid scenario") {
        gen.addCoins(10);
        attacker.addCoins(7);
        
        // Attacker coups victim
        game.nextTurn(); // Skip to attacker
        game.nextTurn(); // Skip to attacker's actual turn
        attacker.coup(victim);
        CHECK_FALSE(victim.isActive());
        CHECK(victim.getCoupedBy() == &attacker);
        
        // General blocks the coup
        gen.block_coup(victim);
        CHECK(gen.coins() == 5); // Paid 5 coins to block
        CHECK(victim.isActive()); // Victim restored to active
        CHECK(victim.getCoupedBy() == nullptr); // Coup reference cleared
    }
    
    SUBCASE("Block coup - insufficient coins") {
        gen.addCoins(4); // Not enough for blocking
        attacker.addCoins(7);
        
        // Set up coup scenario
        victim.setActivityStatus(false);
        victim.setCoupedBy(&attacker);
        
        CHECK_THROWS_AS(gen.block_coup(victim), std::runtime_error);
        CHECK(gen.coins() == 4); // Coins unchanged
    }
    
    SUBCASE("Block coup - invalid targets") {
        gen.addCoins(10);
        
        // Cannot block coup on active player (not couped)
        CHECK_THROWS_AS(gen.block_coup(victim), std::runtime_error);
        
        // Cannot block coup when no coup reference exists
        victim.setActivityStatus(false);
        CHECK_THROWS_AS(gen.block_coup(victim), std::runtime_error);
    }
    
    SUBCASE("Block coup timing restrictions") {
        gen.addCoins(5);
        
        // Simulate coup that's too late to block
        victim.setActivityStatus(false);
        victim.setCoupedBy(nullptr); // Coup already resolved
        
        CHECK_THROWS_AS(gen.block_coup(victim), std::runtime_error);
    }
}

TEST_CASE("Judge Role Tests") {
    Game game;
    Judge judge(game, "Judge");
    Player briber(game, "Briber");
    game.startGame();
    
    SUBCASE("Judge role identification") {
        CHECK(judge.getRoleType() == "Judge");
        CHECK_FALSE(judge.isGeneral());
        CHECK(judge.isJudge());
        CHECK_FALSE(judge.isMerchant());
    }
    
    SUBCASE("Block bribe - valid scenario") {
        briber.addCoins(4);
        
        game.nextTurn(); // Switch to briber
        briber.bribe();
        CHECK(briber.isBribeUsed());
        CHECK(briber.coins() == 0);
        
        // Judge blocks the bribe
        judge.block_bribe(briber);
        CHECK_FALSE(briber.isBribeUsed()); // Bribe effect removed
        // Note: coins already spent are not refunded in this implementation
    }
    
    SUBCASE("Block bribe - invalid targets") {
        // Cannot block if target hasn't used bribe
        CHECK_THROWS_AS(judge.block_bribe(briber), std::runtime_error);
        
        // Cannot block on self
        CHECK_THROWS_AS(judge.block_bribe(judge), std::runtime_error);
        
        // Cannot block inactive player
        briber.setActivityStatus(false);
        CHECK_THROWS_AS(judge.block_bribe(briber), std::runtime_error);
    }
}

TEST_CASE("Baron Role Tests") {
    Game game;
    Baron baron(game, "Baron");
    Player regular(game, "Regular");
    game.startGame();
    
    SUBCASE("Baron role identification") {
        CHECK(baron.getRoleType() == "Baron");
        CHECK_FALSE(baron.isGeneral());
        CHECK_FALSE(baron.isJudge());
        CHECK_FALSE(baron.isMerchant());
    }
    
    SUBCASE("Investment ability - valid scenario") {
        baron.addCoins(5);
        CHECK(baron.coins() == 5);
        
        baron.invest();
        CHECK(baron.coins() == 8); // Net gain of 3 coins (paid 3, received 6)
        CHECK(game.isPlayerTurn(&regular)); // Turn should advance
    }
    
    SUBCASE("Investment with minimum coins") {
        baron.addCoins(3);
        baron.invest();
        CHECK(baron.coins() == 6); // 3 - 3 + 6 = 6
    }
    
    SUBCASE("Investment with insufficient coins") {
        baron.addCoins(2);
        CHECK_THROWS_AS(baron.invest(), std::runtime_error);
        CHECK(baron.coins() == 2); // Unchanged
    }
    
    SUBCASE("Investment when not player's turn") {
        baron.addCoins(5);
        game.nextTurn(); // Switch to regular player
        CHECK_THROWS_AS(baron.invest(), std::runtime_error);
    }
    
    SUBCASE("Investment with 10+ coins without bribe") {
        baron.addCoins(10);
        CHECK_THROWS_AS(baron.invest(), std::runtime_error); // Must coup instead
    }
    
    SUBCASE("Investment after bribe") {
        baron.addCoins(10);
        // After using bribe, should be able to invest
        // This would need to be tested with proper bribe setup
    }
}

TEST_CASE("Spy Role Tests") {
    Game game;
    Spy spy(game, "Spy");
    Player target(game, "Target");
    game.startGame();
    
    SUBCASE("Spy role identification") {
        CHECK(spy.getRoleType() == "Spy");
        CHECK_FALSE(spy.isGeneral());
        CHECK_FALSE(spy.isJudge());
        CHECK_FALSE(spy.isMerchant());
    }
    
    SUBCASE("Spy operation - valid target") {
        target.addCoins(5);
        CHECK(target.isArrestAvailable());
        
        spy.spy_on(target);
        CHECK_FALSE(target.isArrestAvailable()); // Target can't be arrested
        // Coin information would be displayed via GUI
        
        // Spy operation doesn't consume a turn
        CHECK(game.isPlayerTurn(&spy));
    }
    
    SUBCASE("Spy operation - invalid targets") {
        // Cannot spy on self
        CHECK_THROWS_AS(spy.spy_on(spy), std::runtime_error);
        
        // Cannot spy on inactive player
        target.setActivityStatus(false);
        CHECK_THROWS_AS(spy.spy_on(target), std::runtime_error);
    }
    
    SUBCASE("Spy operation when game not started") {
        Game new_game;
        Spy new_spy(new_game, "NewSpy");
        Player new_target(new_game, "NewTarget");
        
        CHECK_THROWS_AS(new_spy.spy_on(new_target), std::runtime_error);
    }
    
    SUBCASE("Spy operation when spy eliminated") {
        spy.setActivityStatus(false);
        CHECK_THROWS_AS(spy.spy_on(target), std::runtime_error);
    }
}

TEST_CASE("Merchant Role Tests") {
    Game game;
    Merchant merchant(game, "Merchant");
    Player regular(game, "Regular");
    game.startGame();
    
    SUBCASE("Merchant role identification") {
        CHECK(merchant.getRoleType() == "Merchant");
        CHECK_FALSE(merchant.isGeneral());
        CHECK_FALSE(merchant.isJudge());
        CHECK(merchant.isMerchant());
    }
    
    SUBCASE("Turn start bonus with sufficient coins") {
        merchant.addCoins(3);
        
        // When merchant's turn begins with 3+ coins, they get +1 bonus
        // This would be triggered in nextTurn() logic
        game.nextTurn(); // Move to regular
        game.nextTurn(); // Back to merchant
        
        // In implementation, merchant should get bonus coin
        // CHECK(merchant.coins() == 4); // 3 + 1 bonus
    }
    
    SUBCASE("Turn start bonus with insufficient coins") {
        merchant.addCoins(2);
        
        game.nextTurn(); // Move to regular
        game.nextTurn(); // Back to merchant
        
        // No bonus with less than 3 coins
        CHECK(merchant.coins() == 2);
    }
    
    SUBCASE("Merchant arrest with insufficient coins for defense") {
        merchant.addCoins(1);
        regular.addCoins(1);
        
        game.nextTurn(); // Switch to regular
        regular.arrest(merchant);
        
        // If merchant can't pay 2 coins to treasury, normal arrest applies
        CHECK(merchant.coins() == 0);
        CHECK(regular.coins() == 2); // Gained 1 coin normally
    }
}

TEST_CASE("Role Interaction Scenarios") {
    Game game;
    Governor gov(game, "Governor");
    General gen(game, "General");
    Judge judge(game, "Judge");
    Baron baron(game, "Baron");
    game.startGame();
    
    SUBCASE("Judge blocking Baron's bribe") {
        gov.tax(); // Governor taxes to move the turn forward
        gen.tax(); // General taxes to move the turn forward
        judge.tax(); // Judge taxes to move the turn forward
        baron.addCoins(4);
        baron.bribe();
        CHECK(baron.isBribeUsed());
        
        judge.block_bribe(baron);
        CHECK_FALSE(baron.isBribeUsed()); // Bribe blocked
    }
    
    SUBCASE("General blocking coup on Judge") {
        gov.addCoins(7);
        gen.addCoins(5);
        
        // Attacker coups judge
        gov.coup(judge);
        CHECK_FALSE(judge.isActive());
        
        // General saves judge
        gen.block_coup(judge);
        CHECK(judge.isActive());
        CHECK(gen.coins() == 0);
    }
    
    SUBCASE("Multiple role abilities in sequence") {
        // Complex scenario testing multiple interactionsg
        gov.tax(); // Governor taxes to move the turn forward
        gen.tax(); // General taxes to move the turn forward
        judge.tax(); // Judge taxes to move the turn forward
        gen.addCoins(9);
        judge.addCoins(9);
        baron.addCoins(9);
        
        // Baron invests
        baron.invest();
        CHECK(baron.coins() == 12); // 9 - 3 + 6 = 12
        
        // Judge blocks potential bribe (if baron had used one)
        // General blocks potential coup
        // Governor undoes potential tax
    }
}

TEST_CASE("Role Edge Cases and Error Handling") {
    SUBCASE("Role abilities when game not started") {
        Game game;
        Governor gov(game, "Gov");
        General gen(game, "Gen");
        Judge judge(game, "Judge");
        Baron baron(game, "Baron");
        Spy spy(game, "Spy");
        
        Player target(game, "Target");
        
        CHECK_THROWS_AS(gov.undo(target), std::runtime_error);
        CHECK_THROWS_AS(gen.block_coup(target), std::runtime_error);
        CHECK_THROWS_AS(judge.block_bribe(target), std::runtime_error);
        CHECK_THROWS_AS(baron.invest(), std::runtime_error);
        CHECK_THROWS_AS(spy.spy_on(target), std::runtime_error);
    }
    
    SUBCASE("Role abilities when eliminated") {
        Game game;
        Governor gov(game, "Gov");
        Player target(game, "Target");
        game.startGame();
        
        gov.setActivityStatus(false);
        CHECK_THROWS_AS(gov.undo(target), std::runtime_error);
    }
    
    SUBCASE("Role state consistency after failed actions") {
        Game game;
        Baron baron(game, "Baron");
        Player target(game, "Target");
        game.startGame();
        
        baron.addCoins(2); // Not enough for investment
        int initial_coins = baron.coins();
        
        CHECK_THROWS_AS(baron.invest(), std::runtime_error);
        CHECK(baron.coins() == initial_coins); // State unchanged
    }
    
    SUBCASE("Role polymorphism and casting") {
        Game game;
        
        // Create roles as base Player pointers
        Player* gov = new Governor(game, "Gov");
        Player* gen = new General(game, "Gen");
        Player* judge = new Judge(game, "Judge");
        
        // Test role identification through base pointers
        CHECK(gov->getRoleType() == "Governor");
        CHECK(gen->getRoleType() == "General");
        CHECK(judge->getRoleType() == "Judge");
        
        CHECK_FALSE(gov->isGeneral());
        CHECK(gen->isGeneral());
        CHECK(judge->isJudge());
        
        // Cleanup handled by Game destructor
    }
}
