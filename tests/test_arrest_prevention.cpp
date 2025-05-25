#include "include/Game.hpp"
#include "include/Player.hpp"
#include <iostream>
#include <cassert>

using namespace coup;

void test_arrest_prevention() {
    std::cout << "Testing arrest prevention feature...\n";
    
    // Create game and players
    Game game;
    Player player1(game, "Alice");   // Player index 0
    Player player2(game, "Bob");     // Player index 1  
    Player player3(game, "Charlie"); // Player index 2
    
    // Start the game
    game.startGame();
    
    // Give players some coins to perform arrests
    player1.addCoins(5);
    player2.addCoins(5);
    player3.addCoins(5);
    
    std::cout << "Initial setup complete. Alice: " << player1.coins() 
              << " coins, Bob: " << player2.coins() 
              << " coins, Charlie: " << player3.coins() << " coins\n";
    
    // Check who's turn it is initially
    std::cout << "Current turn check:\n";
    std::cout << "Alice's turn: " << (game.isPlayerTurn(&player1) ? "YES" : "NO") << "\n";
    std::cout << "Bob's turn: " << (game.isPlayerTurn(&player2) ? "YES" : "NO") << "\n";
    std::cout << "Charlie's turn: " << (game.isPlayerTurn(&player3) ? "YES" : "NO") << "\n";
    
    // Test 1: Alice arrests Bob - should work (it's Alice's turn initially)
    try {
        player1.arrest(player2);
        std::cout << "✓ Test 1 passed: Alice successfully arrested Bob\n";
        std::cout << "After arrest - Alice: " << player1.coins() 
                  << " coins, Bob: " << player2.coins() << " coins\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 1 failed: " << e.what() << "\n";
        return;
    }
    
    // After Alice's arrest, it should now be Bob's turn
    std::cout << "\nAfter Alice's turn:\n";
    std::cout << "Alice's turn: " << (game.isPlayerTurn(&player1) ? "YES" : "NO") << "\n";
    std::cout << "Bob's turn: " << (game.isPlayerTurn(&player2) ? "YES" : "NO") << "\n";
    std::cout << "Charlie's turn: " << (game.isPlayerTurn(&player3) ? "YES" : "NO") << "\n";
    
    // Test 2: Bob tries to arrest himself (Bob was last arrested) - should fail
    try {
        player2.arrest(player2);
        std::cout << "✗ Test 2 failed: Bob should not be able to arrest himself\n";
        return;
    } catch (const std::exception& e) {
        std::cout << "✓ Test 2 passed (self-arrest blocked): " << e.what() << "\n";
    }
    
    // Test 3: Bob arrests Alice instead - should work
    try {
        player2.arrest(player1);
        std::cout << "✓ Test 3 passed: Bob successfully arrested Alice\n";
        std::cout << "After second arrest - Alice: " << player1.coins() 
                  << " coins, Bob: " << player2.coins() << " coins\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 3 failed: " << e.what() << "\n";
        return;
    }
    
    // Now it should be Charlie's turn
    std::cout << "\nAfter Bob's turn:\n";
    std::cout << "Alice's turn: " << (game.isPlayerTurn(&player1) ? "YES" : "NO") << "\n";
    std::cout << "Bob's turn: " << (game.isPlayerTurn(&player2) ? "YES" : "NO") << "\n";
    std::cout << "Charlie's turn: " << (game.isPlayerTurn(&player3) ? "YES" : "NO") << "\n";
    
    // Test 4: Charlie tries to arrest Alice again (Alice was last arrested) - should fail
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
