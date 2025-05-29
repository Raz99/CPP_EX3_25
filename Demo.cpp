// Demonstration program for the Coup card game mechanics
// This file showcases gameplay features and role-specific abilities
// filepath: /home/razco/Uni/CPP/CPP_EX3_25/Demo.cpp
#include "include/roles/Governor.hpp"
#include "include/roles/Spy.hpp"
#include "include/roles/Baron.hpp"
#include "include/roles/General.hpp"
#include "include/roles/Judge.hpp"
#include "include/roles/Merchant.hpp"
#include "include/Player.hpp"
#include "include/Game.hpp"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace coup;

int main() {
    // Initialize game instance for demonstration
    Game game_1{};

    // Create players with different roles to showcase unique abilities
    Governor governor(game_1, "Moshe");   // Can undo other players' actions
    Spy spy(game_1, "Yossi");             // Can spy on players and block arrest
    Baron baron(game_1, "Meirav");        // Can invest for economic advantage
    General general(game_1, "Reut");      // Can block coup attempts
    Judge judge(game_1, "Gilad");         // Can block bribe actions

    // Display all registered players in the game
    vector<string> players = game_1.players();
    
    // Expected output: List of all player names
    // Moshe, Yossi, Meirav, Reut, Gilad
    for(string name : players){
        cout << name << endl;
    }

    // Start the game and begin turn-based gameplay
    // First player's turn begins automatically
    game_1.startGame();
    game_1.turn(); // Display current player's turn

    // Demonstrate basic gather action for all players
    // Each player collects 1 coin per gather action
    governor.gather(); // Moshe gathers 1 coin
    spy.gather();      // Yossi gathers 1 coin
    baron.gather();    // Meirav gathers 1 coin
    general.gather();  // Reut gathers 1 coin
    judge.gather();    // Gilad gathers 1 coin

    // Demonstrate turn order enforcement - spy cannot act twice
    try{
        spy.gather(); // This should fail since it's not spy's turn
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n'; // Display error message
    }

    // Continue with next actions in proper turn order
    governor.gather(); // Governor's second action
    spy.tax();         // Spy performs tax action for more coins

    // Display current coin counts to verify action effects
    cout << governor.coins() << endl; // Expected: 2 coins from two gather actions
    cout << spy.coins() << endl;      // Expected: 3 coins (1 gather + 2 tax)

    // Demonstrate Governor's unique undo ability
    governor.undo(spy); // Governor reverses spy's tax action
    cout << spy.coins() << endl; // Expected: 1 coin (tax undone)

    // Continue gameplay demonstration with various actions
    baron.tax();    // Baron performs tax action
    general.gather(); // General gathers coins
    judge.gather(); // Judge gathers coins

    // More actions to build up Baron's economy
    governor.tax();   // Governor performs tax
    spy.gather();     // Spy gathers
    baron.invest();   // Baron's special ability: invest 3 coins for 6 return
    general.gather(); // General gathers
    judge.gather();   // Judge gathers
    
    cout << baron.coins() << endl; // Expected: 6 coins after investment

    // Additional rounds of basic actions
    governor.tax();  // Governor taxes
    spy.gather();    // Spy gathers
    baron.gather();  // Baron gathers
    general.gather(); // General gathers
    judge.gather();  // Judge gathers

    // Build up for coup demonstration
    governor.tax();  // Governor taxes again
    spy.gather();    // Spy gathers again
    cout << baron.coins() << endl; // Expected: 7 coins total
    baron.coup(governor); // Baron performs coup against Governor (costs 7 coins)
    general.gather(); // General continues after coup
    judge.gather();  // Judge continues after coup
    
    // Check remaining players after coup action
    players = game_1.players();
    // Expected output: Governor should be eliminated if coup wasn't blocked
    // Remaining players: Yossi, Meirav, Reut, Gilad
    for (string name : players) {
        cout << name << endl; // Display remaining active players
    }
}
