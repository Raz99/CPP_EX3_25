// email: razcohenp@gmail.com
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor - initialize player and add to game
    Player::Player(Game& game, const std::string& name) : game(game), name(name), coin_count(0), active(true), sanctioned(false) {
        // Check if name is empty
        if (name.empty()) {
            throw std::invalid_argument("Player name cannot be empty");
        }

        game.addPlayer(this); // Add player to game
    }

    // Get player name
    std::string Player::getName() const {
        return name;
    }

    // Get coin count
    int Player::coins() const {
        return coin_count;
    }

    // Check if player is active
    bool Player::isActive() const {
        return active;
    }

    // Gather action - take 1 coin
    void Player::gather() {
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure player is not sanctioned
        if (sanctioned) {
            throw std::runtime_error("Player is sanctioned");
        }

        addCoins(1); // Increase coin count
        game.nextTurn(); // Move to next player's turn
    }

    // Tax action - take 2 coins
    void Player::tax() {
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure player is not sanctioned
        if (sanctioned) {
            throw std::runtime_error("Player is sanctioned");
        }

        addCoins(2); // Increase coin count
        game.nextTurn(); // Move to next player's turn
    }

    // Bribe action - pay 4 coins for extra action
    void Player::bribe() {
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure player has enough coins
        if (coin_count < 4) {
            throw std::runtime_error("Not enough coins for bribe");
        }

        removeCoins(4); // Decrease coin count
        // No need to call nextTurn() because player gets another action
    }

    // Arrest action - take 1 coin from target
    void Player::arrest(Player& target) {
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Ensure target has coins
        if (target.coins() < 1) {
            throw std::runtime_error("Target has no coins to take");
        }

        target.removeCoins(1); // Remove 1 coin from target
        addCoins(1); // Add 1 coin to current player
        game.nextTurn(); // Move to next player's turn
    }

    // Sanction action - block target's economic actions
    void Player::sanction(Player& target) {
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure player has enough coins
        if (coin_count < 3) {
            throw std::runtime_error("Not enough coins for sanction");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        removeCoins(3); // Decrease coin count
        target.setSanctioned(); // Mark target as sanctioned
        game.nextTurn(); // Move to next player's turn
    }

    // Coup action - eliminate target for 7 coins
    void Player::coup(Player& target) {
        // Ensure it's the player's turn
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("Not your turn");
        }

        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Ensure player has enough coins
        if (coin_count < 7) {
            throw std::runtime_error("Not enough coins for coup");
        }

        removeCoins(7); // Decrease coin count
        target.eliminate(); // Eliminate target
        game.nextTurn(); // Move to next player's turn
    }

    // Add coins to player
    void Player::addCoins(int amount) {
        // Ensure the amount is non-negative
        if (amount < 0) {
            throw std::invalid_argument("Cannot add negative coins");
        }

        coin_count += amount; // Increase coin count
    }


    // Remove coins from player
    void Player::removeCoins(int amount) {
        // Ensure the amount is non-negative
        if (amount < 0) {
            throw std::invalid_argument("Cannot remove negative coins");
        }

        // Ensure player has enough coins
        if (coin_count < amount) {
            throw std::runtime_error("Not enough coins");
        }

        coin_count -= amount; // Decrease coin count
    }

    // Eliminate player from game
    void Player::eliminate() {
        active = false; // Mark player as inactive
    }

    // Set player as sanctioned
    void Player::setSanctioned() {
        sanctioned = true; // Mark player as sanctioned
    }
}