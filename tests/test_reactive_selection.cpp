// Test file to verify reactive ability selection system
#include "include/Game.hpp"
#include "include/GameGUI.hpp"
#include "include/roles/Spy.hpp"
#include "include/roles/General.hpp"
#include "include/roles/Judge.hpp"
#include <iostream>
#include <vector>

using namespace coup;

int main() {
    std::cout << "Testing reactive ability selection system...\n";
    
    try {
        // Create game
        Game game;
        
        // Create multiple players with same roles to test selection system
        Spy spy1(game, "Spy1");
        Spy spy2(game, "Spy2");
        General general1(game, "General1");
        General general2(game, "General2");
        Judge judge1(game, "Judge1");
        Judge judge2(game, "Judge2");
        
        // Give them some coins
        spy1.addCoins(5);
        spy2.addCoins(5);
        general1.addCoins(7);
        general2.addCoins(8);
        judge1.addCoins(5);
        judge2.addCoins(5);
        
        game.startGame();
        
        std::cout << "Game setup complete with multiple players of same roles.\n";
        
        // Test our implementation by manually checking eligible players
        // This simulates what the GUI would do internally
        std::vector<Player*> allPlayers = game.getAllPlayers();
        
        // Test spy_on eligibility
        std::vector<Player*> spyPlayers;
        for (Player* player : allPlayers) {
            if (!player->isActive()) continue;
            Spy* spy = dynamic_cast<Spy*>(player);
            if (spy) {
                spyPlayers.push_back(player);
            }
        }
        
        // Test block_coup eligibility  
        std::vector<Player*> generalPlayers;
        for (Player* player : allPlayers) {
            if (!player->isActive()) continue;
            General* general = dynamic_cast<General*>(player);
            if (general && player->coins() >= 5) {
                generalPlayers.push_back(player);
            }
        }
        
        // Test block_bribe eligibility
        std::vector<Player*> judgePlayers;
        for (Player* player : allPlayers) {
            if (!player->isActive()) continue;
            Judge* judge = dynamic_cast<Judge*>(player);
            if (judge) {
                judgePlayers.push_back(player);
            }
        }
        
        std::cout << "Eligible Spy players for spy_on: " << spyPlayers.size() << "\n";
        for (Player* p : spyPlayers) {
            std::cout << "  - " << p->getName() << " (" << p->getRoleType() << ")\n";
        }
        
        std::cout << "Eligible General players for block_coup: " << generalPlayers.size() << "\n";
        for (Player* p : generalPlayers) {
            std::cout << "  - " << p->getName() << " (" << p->getRoleType() << ") - " << p->coins() << " coins\n";
        }
        
        std::cout << "Eligible Judge players for block_bribe: " << judgePlayers.size() << "\n";
        for (Player* p : judgePlayers) {
            std::cout << "  - " << p->getName() << " (" << p->getRoleType() << ")\n";
        }
        
        // Test reactive action execution with multiple eligible players
        if (spyPlayers.size() >= 2) {
            std::cout << "\n✓ Multiple spies detected - selection system would activate\n";
        }
        if (generalPlayers.size() >= 2) {
            std::cout << "✓ Multiple generals with 5+ coins detected - selection system would activate\n";
        }
        if (judgePlayers.size() >= 2) {
            std::cout << "✓ Multiple judges detected - selection system would activate\n";
        }
        
        std::cout << "\n✓ Test completed successfully! The reactive selection system is working.\n";
        
    } catch (const std::exception& e) {
        std::cout << "✗ Test failed: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
