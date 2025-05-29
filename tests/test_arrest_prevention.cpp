// Test suite for arrest prevention mechanics in Coup card game
// This file validates the spy's ability to block arrest actions and timing constraints
// filepath: /home/razco/Uni/CPP/CPP_EX3_25/tests/test_arrest_prevention.cpp
#include "include/Game.hpp"
#include "include/Player.hpp"
#include <iostream>
#include <cassert>

using namespace coup;

void test_arrest_prevention() {
    std::cout << "Testing arrest prevention feature...\n";
    
    // Initialize game environment with three test players
    Game game;
    Player player1(game, "Alice");   // Player index 0 - primary test subject
    Player player2(game, "Bob");     // Player index 1 - target for arrest
    Player player3(game, "Charlie"); // Player index 2 - additional participant
    
    // Initialize game state for testing arrest mechanics
    game.startGame();
    
    // Provide sufficient funds for arrest actions (cost varies by implementation)
    player1.addCoins(5); // Ensure Alice can afford arrest action
    player2.addCoins(5); // Ensure Bob has coins for counter-actions
    player3.addCoins(5); // Ensure Charlie can participate if needed
    
    std::cout << "Initial setup complete. Alice: " << player1.coins() 
              << " coins, Bob: " << player2.coins() 
              << " coins, Charlie: " << player3.coins() << " coins\n";
    
    // Verify turn order system is functioning correctly
    std::cout << "Current turn check:\n";
    std::cout << "Alice's turn: " << (game.isPlayerTurn(&player1) ? "YES" : "NO") << "\n";
    std::cout << "Bob's turn: " << (game.isPlayerTurn(&player2) ? "YES" : "NO") << "\n";
    std::cout << "Charlie's turn: " << (game.isPlayerTurn(&player3) ? "YES" : "NO") << "\n";
    
    // Test 1: Basic arrest functionality - Alice arrests Bob
    // This should succeed as Alice has initiative and sufficient resources
    try {
        player1.arrest(player2); // Execute arrest action
        std::cout << "✓ Test 1 passed: Alice successfully arrested Bob\n";
        std::cout << "After arrest - Alice: " << player1.coins() 
                  << " coins, Bob: " << player2.coins() << " coins\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 1 failed: " << e.what() << "\n";
        return; // Exit test if basic functionality fails
    }
    
    // Verify turn progression after Alice's action
    std::cout << "\nAfter Alice's turn:\n";
    std::cout << "Alice's turn: " << (game.isPlayerTurn(&player1) ? "YES" : "NO") << "\n";
    std::cout << "Bob's turn: " << (game.isPlayerTurn(&player2) ? "YES" : "NO") << "\n";
    std::cout << "Charlie's turn: " << (game.isPlayerTurn(&player3) ? "YES" : "NO") << "\n";
    
    // Test 2: Self-arrest prevention - Bob cannot arrest himself
    // This validates the prevention mechanism for self-targeting
    try {
        player2.arrest(player2); // Attempt self-arrest (should fail)
        std::cout << "✗ Test 2 failed: Bob should not be able to arrest himself\n";
        return;
    } catch (const std::exception& e) {
        std::cout << "✓ Test 2 passed (self-arrest blocked): " << e.what() << "\n";
    }
    
    // Test 3: Counter-arrest capability - Bob arrests Alice
    // This demonstrates reciprocal arrest actions between players
    try {
        player2.arrest(player1); // Bob retaliates against Alice
        std::cout << "✓ Test 3 passed: Bob successfully arrested Alice\n";
        std::cout << "After second arrest - Alice: " << player1.coins() 
                  << " coins, Bob: " << player2.coins() << " coins\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 3 failed: " << e.what() << "\n";
        return;
    }
    
    // Check turn rotation to Charlie after Bob's action
    std::cout << "\nAfter Bob's turn:\n";
    std::cout << "Alice's turn: " << (game.isPlayerTurn(&player1) ? "YES" : "NO") << "\n";
    std::cout << "Bob's turn: " << (game.isPlayerTurn(&player2) ? "YES" : "NO") << "\n";
    std::cout << "Charlie's turn: " << (game.isPlayerTurn(&player3) ? "YES" : "NO") << "\n";
    
    // Test 4: Consecutive arrest prevention - Charlie cannot re-arrest Alice
    // This tests the prevention of repeated targeting of the same player
    try {
        player3.arrest(player1);
        std::cout << "✗ Test 4 failed: Charlie should not be able to arrest Alice consecutively\n";
        return;
    } catch (const std::exception& e) {
        std::cout << "✓ Test 4 passed: " << e.what() << "\n";
    }
    
    // Test 5: Charlie arrests Bob instead - should work
    try {
        player3.arrest(player2);
        std::cout << "✓ Test 5 passed: Charlie successfully arrested Bob\n";
        std::cout << "After third arrest - Bob: " << player2.coins() 
                  << " coins, Charlie: " << player3.coins() << " coins\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 5 failed: " << e.what() << "\n";
        return;
    }
    
    // Now it should be Alice's turn again
    std::cout << "\nAfter Charlie's turn:\n";
    std::cout << "Alice's turn: " << (game.isPlayerTurn(&player1) ? "YES" : "NO") << "\n";
    std::cout << "Bob's turn: " << (game.isPlayerTurn(&player2) ? "YES" : "NO") << "\n";
    std::cout << "Charlie's turn: " << (game.isPlayerTurn(&player3) ? "YES" : "NO") << "\n";
    
    // Test 6: Alice tries to arrest Bob again (Bob was last arrested) - should fail
    try {
        player1.arrest(player2);
        std::cout << "✗ Test 6 failed: Alice should not be able to arrest Bob consecutively\n";
        return;
    } catch (const std::exception& e) {
        std::cout << "✓ Test 6 passed: " << e.what() << "\n";
    }
    
    // Test 7: Alice arrests Charlie instead - should work and clear the last arrested player
    try {
        player1.arrest(player3);
        std::cout << "✓ Test 7 passed: Alice successfully arrested Charlie\n";
        std::cout << "After fourth arrest - Alice: " << player1.coins() 
                  << " coins, Charlie: " << player3.coins() << " coins\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 7 failed: " << e.what() << "\n";
        return;
    }
    
    std::cout << "\n🎉 All arrest prevention tests passed!\n";
}

int main() {
    try {
        test_arrest_prevention();
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
