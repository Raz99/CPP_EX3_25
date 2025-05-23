// email: razcohenp@gmail.com
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace coup {
    // Constructor - initialize player and add to game
    Player::Player(Game& game, const std::string& name) 
    : game(game), name(name), coin_count(0), active(true), sanctioned(false), tax_available(true), arrest_available(true) {
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

    // Check if player is sanctioned
    bool Player::isSanctioned() const {
        return sanctioned;
    }

    bool Player::isTaxAvailable() const {
        return tax_available && !sanctioned; // If sanctioned, tax is not available
    }

    bool Player::isArrestAvailable() const {
        return arrest_available; // Check if arrest action is available
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
        bribe_used = true;
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

        // Ensure player is able to arrest
        if (!arrest_available) {
            throw std::runtime_error("Arrest action is not available");
        }

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Ensure target has coins
        if (target.coins() < 1) {
            throw std::runtime_error("Target has no coins to take");
        }

        // If target is not a General, then transfer 1 coin from target to current player
        // (If General was arrested, he recieves his 1 coin back)
        if(!target.isGeneral()) {
            target.removeCoins(1); // Remove 1 coin from target
            addCoins(1); // Add 1 coin to current player
        }

        // If target is a Merchant, then take 2 coins from target directly to bank (not to current player)
        else if(target.isMerchant()) {
            target.removeCoins(2);
        }
        
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

        // Ensure target is active
        if (!target.isActive()) {
            throw std::runtime_error("Target player is eliminated");
        }

        // Ensure player has enough coins
        if (coin_count < 3) {
            throw std::runtime_error("Not enough coins for sanction");
        }
        
        // If target is a judge, the player must pay 4 coins
        if(target.isJudge()) {
            if (coin_count < 4) {
                throw std::runtime_error("Not enough coins for sanction (higher fee)");
            }

            removeCoins(1); // Pay 1 coin now and 3 coins later (4 coins in total)
        }

        removeCoins(3); // Pay 3 coins

        target.setSanctionStatus(true); // Mark target as sanctioned
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
        target.setActivityStatus(false); // Eliminate target
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

    // Set player's activity status
    void Player::setActivityStatus(bool value) {
        active = value;
    }

    // Set player as sanctioned or not-sanctioned
    void Player::setSanctionStatus(bool value) {
        sanctioned = value; // Mark player as sanctioned
    }

    // Disable tax action
    void Player::disableTax() {
        // Ensure player is active
        if (!active) {
            throw std::runtime_error("Player is eliminated");
        }

        tax_available = false; // Mark tax action as unavailable
    }

    // Set arrest action availability
    void Player::setArrestAvailability(bool value) {
        arrest_available = value;
    }

    // Reset bribe used flag
    void Player::resetBribeUsed() {
        bribe_used = false; // Reset bribe used flag back to false
    }
}