// email: razcohenp@gmail.com
#include "include/GameGUI.hpp"
#include <iostream>

using namespace coup;

int main() {
    try {
        std::cout << "🎭 Starting COUP Game with Enhanced GUI..." << std::endl;
        
        // Create and run GUI
        GameGUI gui;
        
        if (!gui.initialize()) {
            std::cerr << "❌ Failed to initialize GUI!" << std::endl;
            return -1;
        }
        
        std::cout << "✅ GUI initialized successfully!" << std::endl;
        std::cout << "🎮 Welcome to COUP!" << std::endl;
        
        gui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "💥 Error: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "👋 Thanks for playing COUP!" << std::endl;
    return 0;
}