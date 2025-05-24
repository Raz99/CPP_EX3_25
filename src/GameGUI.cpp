// email: razcohenp@gmail.com
#include "../include/GameGUI.hpp"
#include "../include/roles/Governor.hpp"
#include "../include/roles/Baron.hpp" 
#include "../include/roles/Spy.hpp"
#include "../include/roles/General.hpp"
#include "../include/roles/Judge.hpp"
#include "../include/roles/Merchant.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cmath>

namespace coup {
// EnhancedButton implementation
EnhancedButton::EnhancedButton(sf::Vector2f position, sf::Vector2f size, const std::string& label, 
                               const std::string& actionType, sf::Color normal)
    : action(actionType), enabled(true), hovered(false), normalColor(normal) {
    
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(normalColor);
    
// Create hover and disabled colors
    hoverColor = sf::Color(
        std::min(255, (int)normal.r + 30),
        std::min(255, (int)normal.g + 30),
        std::min(255, (int)normal.b + 30)
    );
    disabledColor = sf::Color(100, 100, 100);
    
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);
    
// Add rounded corners effect with outline
    shape.setOutlineThickness(3);
    
    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    
// Create small icon circle
    icon.setRadius(8);
    icon.setFillColor(sf::Color(255, 215, 0)); // Gold
    icon.setPosition(position.x + 10, position.y + size.y/2 - 8);
}

bool EnhancedButton::contains(sf::Vector2f point) const {
    return enabled && shape.getGlobalBounds().contains(point);
}

void EnhancedButton::setEnabled(bool enable) {
    enabled = enable;
    if (enabled) {
        shape.setFillColor(hovered ? hoverColor : normalColor);
        text.setFillColor(sf::Color::White);
        icon.setFillColor(sf::Color(255, 215, 0));
    } else {
        shape.setFillColor(disabledColor);
        text.setFillColor(sf::Color(150, 150, 150));
        icon.setFillColor(sf::Color(80, 80, 80));
    }
}

void EnhancedButton::setHovered(bool hover) {
    hovered = hover && enabled;
    if (enabled) {
        shape.setFillColor(hovered ? hoverColor : normalColor);
        shape.setOutlineColor(hovered ? sf::Color::Yellow : sf::Color::White);
    }
}

void EnhancedButton::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(icon);
    window.draw(text);
}

void EnhancedButton::setFont(const sf::Font& font) {
    text.setFont(font);
// Center text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        shape.getPosition().x + (shape.getSize().x - textBounds.width) / 2 - textBounds.left + 15,
        shape.getPosition().y + (shape.getSize().y - textBounds.height) / 2 - textBounds.top
    );
}

// PlayerCard implementation
PlayerCard::PlayerCard(sf::Vector2f position, sf::Vector2f size)
    : isCurrentPlayer(false), isActive(true), role(RoleType::PLAYER) {
    
    background.setPosition(position);
    background.setSize(size);
    background.setFillColor(sf::Color(40, 40, 60));
    background.setOutlineThickness(3);
    background.setOutlineColor(sf::Color::White);
    
// Player avatar (circular)
    playerAvatar.setRadius(25);
    playerAvatar.setFillColor(sf::Color(100, 150, 200));
    playerAvatar.setPosition(position.x + 10, position.y + 10);
    
// Role icon
    roleIcon.setSize(sf::Vector2f(30, 30));
    roleIcon.setPosition(position.x + size.x - 40, position.y + 10);
    roleIcon.setFillColor(sf::Color(200, 100, 100));
    
// Text elements
    nameText.setPosition(position.x + 70, position.y + 15);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::White);
    nameText.setStyle(sf::Text::Bold);
    
    roleText.setPosition(position.x + 70, position.y + 40);
    roleText.setCharacterSize(14);
    roleText.setFillColor(sf::Color::Cyan);
    
    coinsText.setPosition(position.x + 15, position.y + 70);
    coinsText.setCharacterSize(16);
    coinsText.setFillColor(sf::Color::Yellow);
    coinsText.setStyle(sf::Text::Bold);
    
    statusText.setPosition(position.x + 15, position.y + 95);
    statusText.setCharacterSize(12);
    statusText.setFillColor(sf::Color::Green);
    
// Coin icon
    coinIcon.setSize(sf::Vector2f(15, 15));
    coinIcon.setFillColor(sf::Color::Yellow);
    coinIcon.setPosition(position.x + 90, position.y + 75);
}

void PlayerCard::updateInfo(const Player* player, bool current, RoleType playerRole) {
    isCurrentPlayer = current;
    isActive = player->isActive();
    role = playerRole;
    
    nameText.setString(player->getName());
    coinsText.setString("Coins: " + std::to_string(player->coins()));
    roleText.setString("Role: " + std::string(role == RoleType::PLAYER ? "Player" : 
                       role == RoleType::GOVERNOR ? "Governor" :
                       role == RoleType::SPY ? "Spy" :
                       role == RoleType::BARON ? "Baron" :
                       role == RoleType::GENERAL ? "General" :
                       role == RoleType::JUDGE ? "Judge" : "Merchant"));
    
// Update status
    std::string status = "";
    if (!isActive) {
        status = "ELIMINATED";
        statusText.setFillColor(sf::Color::Red);
    } else if (player->isSanctioned()) {
        status = "SANCTIONED";
        statusText.setFillColor(sf::Color(255, 165, 0)); // Orange
    } else {
        status = "ACTIVE";
        statusText.setFillColor(sf::Color::Green);
    }
    statusText.setString(status);
    
// Update visual appearance
    if (isCurrentPlayer) {
        background.setFillColor(sf::Color(80, 40, 120)); // Purple
        background.setOutlineColor(sf::Color(255, 215, 0)); // Gold
        background.setOutlineThickness(4);
        playerAvatar.setFillColor(sf::Color(255, 215, 0)); // Gold
    } else if (!isActive) {
        background.setFillColor(sf::Color(30, 30, 30)); // Dark
        background.setOutlineColor(sf::Color::Red);
        playerAvatar.setFillColor(sf::Color(100, 100, 100)); // Gray
    } else {
        background.setFillColor(sf::Color(40, 40, 60)); // Normal
        background.setOutlineColor(sf::Color::White);
        playerAvatar.setFillColor(sf::Color(100, 150, 200)); // Blue
    }
    
// Role-specific colors
    sf::Color roleColor = sf::Color(200, 100, 100);
    switch (role) {
        case RoleType::GOVERNOR: roleColor = sf::Color(200, 150, 50); break;
        case RoleType::SPY: roleColor = sf::Color(100, 100, 200); break;
        case RoleType::BARON: roleColor = sf::Color(150, 100, 200); break;
        case RoleType::GENERAL: roleColor = sf::Color(200, 100, 100); break;
        case RoleType::JUDGE: roleColor = sf::Color(150, 150, 150); break;
        case RoleType::MERCHANT: roleColor = sf::Color(100, 200, 100); break;
        default: roleColor = sf::Color(120, 120, 120); break;
    }
    roleIcon.setFillColor(roleColor);
}

void PlayerCard::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(playerAvatar);
    window.draw(roleIcon);
    window.draw(nameText);
    window.draw(roleText);
    window.draw(coinsText);
    window.draw(statusText);
    window.draw(coinIcon);
}

void PlayerCard::setFont(const sf::Font& font) {
    nameText.setFont(font);
    roleText.setFont(font);
    coinsText.setFont(font);
    statusText.setFont(font);
}

// InputField implementation
InputField::InputField(sf::Vector2f position, sf::Vector2f size, const std::string& placeholderText)
    : active(false), showCursor(false) {
    
    background.setPosition(position);
    background.setSize(size);
    background.setFillColor(sf::Color(50, 50, 50));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::White);
    
    displayText.setPosition(position.x + 10, position.y + 10);
    displayText.setCharacterSize(18);
    displayText.setFillColor(sf::Color::White);
    
    placeholder.setString(placeholderText);
    placeholder.setPosition(position.x + 10, position.y + 10);
    placeholder.setCharacterSize(18);
    placeholder.setFillColor(sf::Color(150, 150, 150));
}

void InputField::handleInput(sf::Uint32 unicode) {
    if (!active) return;
    
    if (unicode == 8) { // Backspace
        if (!content.empty()) {
            content.pop_back();
            displayText.setString(content);
        }
    } else if (unicode >= 32 && unicode < 127) { // Printable characters
        if (content.length() < 20) { // Max length
            content += static_cast<char>(unicode);
            displayText.setString(content);
        }
    }
}

void InputField::setActive(bool isActive) {
    active = isActive;
    background.setOutlineColor(active ? sf::Color::Yellow : sf::Color::White);
    background.setOutlineThickness(active ? 3 : 2);
}

bool InputField::contains(sf::Vector2f point) const {
    return background.getGlobalBounds().contains(point);
}

void InputField::update() {
    if (active && cursorClock.getElapsedTime().asSeconds() > 0.5f) {
        showCursor = !showCursor;
        cursorClock.restart();
        
        std::string displayStr = content;
        if (showCursor) displayStr += "|";
        displayText.setString(displayStr);
    }
}

void InputField::draw(sf::RenderWindow& window) const {
    window.draw(background);
    if (content.empty() && !active) {
        window.draw(placeholder);
    } else {
        window.draw(displayText);
    }
}

void InputField::setFont(const sf::Font& font) {
    displayText.setFont(font);
    placeholder.setFont(font);
}

// GameGUI implementation
GameGUI::GameGUI() 
    : game(nullptr),
      currentState(GameState::MAIN_MENU), 
      waitingForTarget(false), 
      waitingForRole(false),
      selectedTarget(nullptr),
      playerNameInput(sf::Vector2f(500, 400), sf::Vector2f(300, 40), "Enter player name..."),
      addPlayerButton(sf::Vector2f(820, 400), sf::Vector2f(150, 40), "Add Player", "add_player"),
      startGameButton(sf::Vector2f(600, 500), sf::Vector2f(200, 50), "Start Game", "start_game"),
      backButton(sf::Vector2f(50, 50), sf::Vector2f(100, 40), "Back", "back"),
      returnToMenuButton(sf::Vector2f(600, 600), sf::Vector2f(200, 50), "Return to Menu", "menu", sf::Color(70, 130, 180)),
      selectedRole(RoleType::PLAYER) {
    
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "COUP", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    
// Setup color theme
    theme.background = sf::Color(15, 15, 25);
    theme.primary = sf::Color(70, 130, 180);
    theme.secondary = sf::Color(100, 149, 237);
    theme.accent = sf::Color(255, 215, 0);
    theme.text = sf::Color::White;
    theme.textSecondary = sf::Color(200, 200, 200);
    theme.success = sf::Color(50, 205, 50);
    theme.error = sf::Color(220, 20, 60);
    theme.warning = sf::Color(255, 165, 0);
}

GameGUI::~GameGUI() {
    if (game) delete game;
}

bool GameGUI::initialize() {
// Load fonts
    if (!mainFont.loadFromFile("tahoma.ttf")) {
        std::cout << "Warning: Could not load tahoma.ttf. Trying absolute path..." << std::endl;
        // Try with absolute path
        if (!mainFont.loadFromFile("/home/razco/Uni/CPP/CPP_EX3_25/tahoma.ttf")) {
            std::cout << "Error: Could not load tahoma.ttf. The game UI may not display properly." << std::endl;
        }
    }
    
    titleFont = mainFont; // Use same font for now
    
    // Initialize message text
    messageText.setFont(mainFont);
    messageText.setCharacterSize(18);
    messageText.setFillColor(theme.success);
    messageText.setPosition(50, WINDOW_HEIGHT - 40);
    
// Setup all UI elements
    setupMainMenu();
    createDecorativeElements();
    
    return true;
}

void GameGUI::setupMainMenu() {
// Main title
    titleText.setFont(titleFont);
    titleText.setString("COUP");
    titleText.setCharacterSize(72);
    titleText.setFillColor(theme.accent);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(getCenterPosition(sf::Vector2f(200, 80)));
    titleText.move(0, -200);
    
// Subtitle
    subtitleText.setFont(mainFont);
    subtitleText.setString("Created by Raz Cohen");
    subtitleText.setCharacterSize(24);
    subtitleText.setFillColor(theme.textSecondary);
    // Center the subtitle text
    subtitleText.setPosition(getCenterPosition(sf::Vector2f(subtitleText.getLocalBounds().width, subtitleText.getLocalBounds().height)));
    subtitleText.move(0, -120);
    
    createMenuButtons();
}

void GameGUI::createMenuButtons() {
    menuButtons.clear();
    
    std::vector<std::pair<std::string, std::string>> buttons = {
        {"New Game", "new_game"},
        {"How to Play", "how_to_play"},
        {"Exit", "exit"}
    };
    
    sf::Vector2f startPos = getCenterPosition(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    startPos.y += 50;
    
    for (size_t i = 0; i < buttons.size(); ++i) {
        sf::Vector2f pos = sf::Vector2f(startPos.x, startPos.y + i * 80);
        menuButtons.emplace_back(pos, sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), 
                                buttons[i].first, buttons[i].second, theme.primary);
        menuButtons.back().setFont(mainFont);
    }
}

void GameGUI::setupPlayerSetup() {
// Setup input field and buttons fonts
    playerNameInput.setFont(mainFont);
    addPlayerButton.setFont(mainFont);
    startGameButton.setFont(mainFont);
    backButton.setFont(mainFont);
    
// Instruction text
    instructionText.setFont(mainFont);
    instructionText.setString("Add 2-6 players to start the game");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(theme.text);
    instructionText.setPosition(getCenterPosition(sf::Vector2f(400, 30)));
    instructionText.move(0, -150);
    
    createPlayerCards();
}

void GameGUI::createPlayerCards() {
    playerCards.clear();
    
    int playersPerRow = 3;
    sf::Vector2f cardSize(CARD_WIDTH, CARD_HEIGHT);
    sf::Vector2f spacing(20, 20);
    // Position cards within the player panel
    sf::Vector2f startPos(490, 80);
    
    for (size_t i = 0; i < players.size(); ++i) {
        int row = i / playersPerRow;
        int col = i % playersPerRow;
        
        sf::Vector2f pos(
            startPos.x + col * (cardSize.x + spacing.x),
            startPos.y + row * (cardSize.y + spacing.y)
        );
        
        playerCards.emplace_back(pos, cardSize);
        playerCards.back().setFont(mainFont);
        
        // Determine player role for display (this would need to be enhanced)
        RoleType displayRole = RoleType::PLAYER; // Default for now
        playerCards.back().updateInfo(players[i].get(), false, displayRole);
    }
}

void GameGUI::createActionButtons() {
    actionButtons.clear();
    
    // Basic actions available to all players
    std::vector<std::pair<std::string, std::string>> basicActions = {
        {"Gather (1 coin)", "gather"},
        {"Tax (2 coins)", "tax"},
        {"Bribe (4 coins)", "bribe"},
        {"Arrest", "arrest"},
        {"Sanction (3 coins)", "sanction"},
        {"Coup (7 coins)", "coup"}
    };
    
    // Position buttons in action panel
    sf::Vector2f startPos(70, 280);
    sf::Vector2f buttonSize(180, 45);
    int spacing = 55;
    
    // Add basic action buttons
    for (size_t i = 0; i < basicActions.size(); ++i) {
        sf::Vector2f pos(startPos.x, startPos.y + i * spacing);
        actionButtons.emplace_back(pos, buttonSize, 
                                 basicActions[i].first, basicActions[i].second, theme.primary);
        actionButtons.back().setFont(mainFont);
    }
    
    // Add role-specific action buttons
    Player* currentPlayer = getCurrentPlayer();
    if (currentPlayer) {
        RoleType role = getPlayerRole(currentPlayer);
        addRoleSpecificButtons(role, startPos, buttonSize, spacing, basicActions.size());
    }
}

void GameGUI::addRoleSpecificButtons(RoleType role, sf::Vector2f startPos, sf::Vector2f buttonSize, 
                                   int spacing, int startIndex) {
    std::vector<std::pair<std::string, std::string>> roleActions;
    sf::Color roleColor = theme.accent; // Gold color for special abilities
    
    switch (role) {
        case RoleType::GOVERNOR:
            roleActions = {
                {"Undo Tax Block", "undo"}
            };
            break;
            
        case RoleType::BARON:
            roleActions = {
                {"Invest (3→6 coins)", "invest"}
            };
            break;
            
        case RoleType::SPY:
            roleActions = {
                {"Spy On Player", "spy_on"}
            };
            break;
            
        case RoleType::GENERAL:
            roleActions = {
                {"Block Coup (5 coins)", "block_coup"}
            };
            break;
            
        case RoleType::JUDGE:
            roleActions = {
                {"Block Bribe", "block_bribe"}
            };
            break;
            
        case RoleType::MERCHANT:
            // Merchant has passive ability only
            break;
            
        default:
            break;
    }
    
    // Add role-specific buttons
    for (size_t i = 0; i < roleActions.size(); ++i) {
        sf::Vector2f pos(startPos.x + 200, startPos.y + (startIndex + i) * spacing);
        actionButtons.emplace_back(pos, buttonSize, 
                                 roleActions[i].first, roleActions[i].second, roleColor);
        actionButtons.back().setFont(mainFont);
    }
}

void GameGUI::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void GameGUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                handleMouseClick(mousePos);
            }
        }
        
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
            handleMouseMove(mousePos);
        }
        
        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
        
        if (event.type == sf::Event::TextEntered) {
            handleTextInput(event.text.unicode);
        }
    }
}

void GameGUI::handleMouseClick(sf::Vector2f mousePos) {
    switch (currentState) {
        case GameState::MAIN_MENU:
            for (auto& button : menuButtons) {
                if (button.contains(mousePos)) {
                    if (button.action == "new_game") {
                        changeState(GameState::SETUP_PLAYERS);
                    } else if (button.action == "exit") {
                        window.close();
                    }
                }
            }
            break;
            
        case GameState::SETUP_PLAYERS:
            if (playerNameInput.contains(mousePos)) {
                playerNameInput.setActive(true);
            } else {
                playerNameInput.setActive(false);
            }
            
            if (addPlayerButton.contains(mousePos)) {
                addNewPlayer();
            }
            
            if (startGameButton.contains(mousePos) && players.size() >= 2) {
                startNewGame();
            }
            
            if (backButton.contains(mousePos)) {
                changeState(GameState::MAIN_MENU);
            }
            break;
            
        case GameState::PLAYING:
            // Handle game actions
            for (auto& button : actionButtons) {
                if (button.contains(mousePos)) {
                    currentAction = button.action;
                    if (currentAction == "arrest" || currentAction == "sanction" || currentAction == "coup") {
                        waitingForTarget = true;
                        updateMessage("Select a target player");
                    } else {
                        executeAction(currentAction);
                    }
                }
            }
            
            // Handle target selection
            if (waitingForTarget) {
                for (size_t i = 0; i < playerCards.size(); ++i) {
                    if (playerCards[i].background.getGlobalBounds().contains(mousePos)) {
                        if (players[i].get() != getCurrentPlayer() && players[i]->isActive()) {
                            executeAction(currentAction, players[i].get());
                            waitingForTarget = false;
                            currentAction = "";
                        } else {
                            updateMessage("Invalid target - select an active opponent", true);
                        }
                    }
                }
            }
            break;
            
        case GameState::GAME_OVER:
            // Handle game over screen
            if (returnToMenuButton.contains(mousePos)) {
                if (game) {
                    delete game;
                    game = nullptr;
                }
                players.clear();
                changeState(GameState::MAIN_MENU);
            }
            break;
    }
}

void GameGUI::handleMouseMove(sf::Vector2f mousePos) {
    // Update hover states for buttons
    std::vector<EnhancedButton*> allButtons;
    
    switch (currentState) {
        case GameState::MAIN_MENU:
            for (auto& btn : menuButtons) allButtons.push_back(&btn);
            break;
        case GameState::SETUP_PLAYERS:
            allButtons.push_back(&addPlayerButton);
            allButtons.push_back(&startGameButton);
            allButtons.push_back(&backButton);
            break;
        case GameState::PLAYING:
            for (auto& btn : actionButtons) allButtons.push_back(&btn);
            break;
        case GameState::GAME_OVER:
            allButtons.push_back(&returnToMenuButton);
            break;
    }
    
    for (auto* btn : allButtons) {
        btn->setHovered(btn->contains(mousePos));
    }
}

void GameGUI::handleTextInput(sf::Uint32 unicode) {
    if (currentState == GameState::SETUP_PLAYERS) {
        playerNameInput.handleInput(unicode);
    }
}

void GameGUI::handleKeyPress(sf::Keyboard::Key key) {
    // Handle keyboard shortcuts and navigation
    switch (currentState) {
        case GameState::MAIN_MENU:
            if (key == sf::Keyboard::Escape) {
                window.close();
            } else if (key == sf::Keyboard::Enter || key == sf::Keyboard::Space) {
                // Start new game with Enter key
                changeState(GameState::SETUP_PLAYERS);
            }
            break;
            
        case GameState::SETUP_PLAYERS:
            if (key == sf::Keyboard::Escape) {
                changeState(GameState::MAIN_MENU);
            } else if (key == sf::Keyboard::Enter) {
                // Add player or start game
                if (players.size() >= 2) {
                    startNewGame();
                } else {
                    addNewPlayer();
                }
            }
            break;
            
        case GameState::PLAYING:
            if (key == sf::Keyboard::Escape) {
                // Provide option to return to menu or cancel action
                if (waitingForTarget) {
                    waitingForTarget = false;
                    updateMessage("Action canceled", false, true);
                }
            }
            break;
            
        case GameState::GAME_OVER:
            if (key == sf::Keyboard::Escape || key == sf::Keyboard::Enter || key == sf::Keyboard::Space) {
                // Return to main menu
                if (game) {
                    delete game;
                    game = nullptr;
                }
                players.clear();
                changeState(GameState::MAIN_MENU);
            }
            break;
    }
}

void GameGUI::addNewPlayer() {
    std::string name = playerNameInput.getText();
    if (name.empty()) {
        updateMessage("Please enter a player name!", true);
        return;
    }
    
    if (players.size() >= 6) {
        updateMessage("Maximum 6 players allowed!", true);
        return;
    }
    
    // Check for duplicate names
    for (const auto& player : players) {
        if (player->getName() == name) {
            updateMessage("Player name already exists!", true);
            return;
        }
    }
    
    if (!game) {
        game = new Game();
    }
    
    try {
        players.push_back(std::make_unique<Player>(*game, name));
        playerNameInput.clear();
        createPlayerCards();
        updateMessage("Player " + name + " added successfully!", false);
        
        // Enable start button if we have enough players
        startGameButton.setEnabled(players.size() >= 2);
    } catch (const std::exception& e) {
        updateMessage("Error adding player: " + std::string(e.what()), true);
    }
}

void GameGUI::startNewGame() {
    if (!game || players.size() < 2) {
        updateMessage("Need at least 2 players to start!", true);
        return;
    }
    
    try {
        game->startGame();
        changeState(GameState::PLAYING);
        updateMessage("Game started! " + getCurrentPlayer()->getName() + "'s turn");
    } catch (const std::exception& e) {
        updateMessage("Error starting game: " + std::string(e.what()), true);
    }
}

void GameGUI::executeAction(const std::string& action, Player* target) {
    Player* currentPlayer = getCurrentPlayer();
    if (!currentPlayer) {
        updateMessage("No current player!", true);
        return;
    }
    
    try {
        // Basic actions
        if (action == "gather") {
            currentPlayer->gather();
            updateMessage(currentPlayer->getName() + " gathered 1 coin");
        }
        else if (action == "tax") {
            currentPlayer->tax();
            updateMessage(currentPlayer->getName() + " collected tax (2 coins)");
        }
        else if (action == "bribe") {
            currentPlayer->bribe();
            updateMessage(currentPlayer->getName() + " paid bribe for extra action");
        }
        else if (action == "arrest" && target) {
            currentPlayer->arrest(*target);
            updateMessage(currentPlayer->getName() + " arrested " + target->getName());
        }
        else if (action == "sanction" && target) {
            currentPlayer->sanction(*target);
            updateMessage(currentPlayer->getName() + " sanctioned " + target->getName());
        }
        else if (action == "coup" && target) {
            currentPlayer->coup(*target);
            updateMessage(currentPlayer->getName() + " performed coup on " + target->getName());
        }
        
        // Role-specific actions
        else if (action == "undo" && target) {
            Governor* governor = dynamic_cast<Governor*>(currentPlayer);
            if (governor) {
                governor->undo(*target);
                updateMessage(governor->getName() + " blocked " + target->getName() + "'s tax ability");
            } else {
                updateMessage("Only Governors can use undo!", true);
            }
        }
        else if (action == "invest") {
            Baron* baron = dynamic_cast<Baron*>(currentPlayer);
            if (baron) {
                baron->invest();
                updateMessage(baron->getName() + " invested 3 coins to get 6 coins!");
            } else {
                updateMessage("Only Barons can invest!", true);
            }
        }
        else if (action == "spy_on" && target) {
            Spy* spy = dynamic_cast<Spy*>(currentPlayer);
            if (spy) {
                spy->spy_on(*target);
                updateMessage(spy->getName() + " spied on " + target->getName() + 
                            " (Coins: " + std::to_string(target->coins()) + ", blocked arrest)");
            } else {
                updateMessage("Only Spies can spy on players!", true);
            }
        }
        else if (action == "block_coup" && target) {
            General* general = dynamic_cast<General*>(currentPlayer);
            if (general) {
                general->block_coup(*target);
                updateMessage(general->getName() + " blocked coup and revived " + target->getName());
            } else {
                updateMessage("Only Generals can block coups!", true);
            }
        }
        else if (action == "block_bribe" && target) {
            Judge* judge = dynamic_cast<Judge*>(currentPlayer);
            if (judge) {
                judge->block_bribe(*target);
                updateMessage(judge->getName() + " blocked " + target->getName() + "'s bribe");
            } else {
                updateMessage("Only Judges can block bribes!", true);
            }
        }
        
        updatePlayerCards();
        
        // Check for game over
        try {
            std::string winner = game->winner();
            updateMessage("Game Over! Winner: " + winner);
            changeState(GameState::GAME_OVER);
        } catch (const std::exception&) {
            // Game continues
        }
        
    } catch (const std::exception& e) {
        updateMessage("Error: " + std::string(e.what()), true);
    }
}

void GameGUI::changeState(GameState newState) {
    currentState = newState;
    
    switch (newState) {
        case GameState::MAIN_MENU:
            setupMainMenu();
            break;
        case GameState::SETUP_PLAYERS:
            setupPlayerSetup();
            break;
        case GameState::PLAYING:
            setupGameScreen();
            break;
        case GameState::GAME_OVER:
            setupGameOver();
            break;
    }
}

void GameGUI::setupGameScreen() {
    createActionButtons();
    updatePlayerCards();
    
    // Setup main game panels
    setupGamePanels();
    
    // Setup game info text
    gameInfoText.setFont(mainFont);
    gameInfoText.setCharacterSize(18);
    gameInfoText.setFillColor(theme.text);
    gameInfoText.setPosition(70, 130);
    
    // Setup current player display
    currentPlayerText.setFont(mainFont);
    currentPlayerText.setCharacterSize(24);
    currentPlayerText.setFillColor(theme.accent);
    currentPlayerText.setStyle(sf::Text::Bold);
    currentPlayerText.setPosition(70, 80);
    
    // Setup game status text
    gameStatusText.setFont(mainFont);
    gameStatusText.setCharacterSize(16);
    gameStatusText.setFillColor(theme.textSecondary);
    gameStatusText.setPosition(70, 160);
    
    // Setup action feedback
    actionFeedbackText.setFont(mainFont);
    actionFeedbackText.setCharacterSize(18);
    actionFeedbackText.setFillColor(theme.success);
    actionFeedbackText.setPosition(480, 680);
    
    // Setup section headers
    actionSectionHeader.setFont(mainFont);
    actionSectionHeader.setString("ACTIONS");
    actionSectionHeader.setCharacterSize(20);
    actionSectionHeader.setFillColor(theme.accent);
    actionSectionHeader.setStyle(sf::Text::Bold);
    actionSectionHeader.setPosition(70, 255);
    
    playerSectionHeader.setFont(mainFont);
    playerSectionHeader.setString("PLAYERS");
    playerSectionHeader.setCharacterSize(20);
    playerSectionHeader.setFillColor(theme.accent);
    playerSectionHeader.setStyle(sf::Text::Bold);
    playerSectionHeader.setPosition(490, 55);
    
    gameInfoHeader.setFont(mainFont);
    gameInfoHeader.setString("GAME STATUS");
    gameInfoHeader.setCharacterSize(20);
    gameInfoHeader.setFillColor(theme.accent);
    gameInfoHeader.setStyle(sf::Text::Bold);
    gameInfoHeader.setPosition(70, 55);
}

void GameGUI::setupGameOver() {
    // Setup game over screen
    gameOverTitle.setFont(titleFont);
    gameOverTitle.setString("GAME OVER");
    gameOverTitle.setCharacterSize(72);
    gameOverTitle.setFillColor(theme.accent);
    gameOverTitle.setStyle(sf::Text::Bold);
    gameOverTitle.setPosition(getCenterPosition(sf::Vector2f(400, 80)));
    gameOverTitle.move(0, -150);
    
    winnerText.setFont(mainFont);
    winnerText.setCharacterSize(36);
    winnerText.setFillColor(theme.success);
    winnerText.setPosition(getCenterPosition(sf::Vector2f(600, 40)));
    winnerText.move(0, -50);

    // Create return to main menu button
    sf::Vector2f buttonPos = getCenterPosition(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    buttonPos.y += 100;
    returnToMenuButton = EnhancedButton(
        buttonPos,
        sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
        "Return to Menu",
        "menu",
        theme.primary
    );
    returnToMenuButton.setFont(mainFont);
    
    try {
        std::string winner = game->winner();
        winnerText.setString("Winner: " + winner);
    } catch (const std::exception&) {
        winnerText.setString("No winner determined");
    }
}

void GameGUI::setupGamePanels() {
    // Game information panel (left side)
    gameInfoPanel.setSize(sf::Vector2f(400, 180));
    gameInfoPanel.setPosition(50, 50);
    gameInfoPanel.setFillColor(sf::Color(30, 30, 45, 200));
    gameInfoPanel.setOutlineThickness(2);
    gameInfoPanel.setOutlineColor(theme.primary);
    
    // Action panel (left side, below game info)
    actionPanel.setSize(sf::Vector2f(400, 400));
    actionPanel.setPosition(50, 250);
    actionPanel.setFillColor(sf::Color(30, 30, 45, 200));
    actionPanel.setOutlineThickness(2);
    actionPanel.setOutlineColor(theme.secondary);
    
    // Player panel (right side)
    playerPanel.setSize(sf::Vector2f(880, 580));
    playerPanel.setPosition(470, 50);
    playerPanel.setFillColor(sf::Color(25, 25, 40, 180));
    playerPanel.setOutlineThickness(2);
    playerPanel.setOutlineColor(theme.accent);
    
    // Feedback panel (bottom)
    feedbackPanel.setSize(sf::Vector2f(1300, 60));
    feedbackPanel.setPosition(50, 670);
    feedbackPanel.setFillColor(sf::Color(40, 40, 55, 220));
    feedbackPanel.setOutlineThickness(2);
    feedbackPanel.setOutlineColor(theme.text);
}

void GameGUI::updateGameInfo() {
    if (!game || !game->isGameStarted()) {
        currentPlayerText.setString("Game not started");
        gameStatusText.setString("");
        return;
    }
    
    Player* currentPlayer = getCurrentPlayer();
    if (!currentPlayer) {
        currentPlayerText.setString("No current player");
        gameStatusText.setString("");
        return;
    }
    
    // Update current player display
    std::string playerInfo = "Current Turn: " + currentPlayer->getName();
    if (currentPlayer->coins() >= 10) {
        playerInfo += " (MUST COUP!)";
        currentPlayerText.setFillColor(theme.error);
    } else {
        currentPlayerText.setFillColor(theme.accent);
    }
    currentPlayerText.setString(playerInfo);
    
    // Update game status
    std::vector<Player*> activePlayers = getActivePlayers();
    std::string statusInfo = "Active Players: " + std::to_string(activePlayers.size());
    statusInfo += "\nCoins: " + std::to_string(currentPlayer->coins());
    
    // Add status effects
    if (currentPlayer->isSanctioned()) {
        statusInfo += "\nSTATUS: Sanctioned (no gather/tax)";
    }
    if (!currentPlayer->isArrestAvailable()) {
        statusInfo += "\nSTATUS: Arrest blocked";
    }
    if (currentPlayer->isBribeUsed()) {
        statusInfo += "\nSTATUS: Extra action available";
    }
    
    gameStatusText.setString(statusInfo);
}

void GameGUI::updateActionAvailability() {
    if (!game || !game->isGameStarted()) {
        for (auto& button : actionButtons) {
            button.setEnabled(false);
        }
        return;
    }
    
    Player* currentPlayer = getCurrentPlayer();
    if (!currentPlayer) {
        for (auto& button : actionButtons) {
            button.setEnabled(false);
        }
        return;
    }
    
    // Update each action button based on availability
    for (auto& button : actionButtons) {
        bool available = isActionAvailable(button.action);
        button.setEnabled(available);
        
        // Add visual feedback for unavailable actions
        if (!available) {
            button.setHovered(false);
        }
    }
}

bool GameGUI::isActionAvailable(const std::string& action) const {
    if (!game || !game->isGameStarted()) return false;
    
    Player* currentPlayer = getCurrentPlayer();
    if (!currentPlayer || !currentPlayer->isActive()) return false;
    
    // If player has 10+ coins and hasn't used bribe, only coup is available
    bool mustCoup = (currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed());
    if (mustCoup && action != "coup") return false;
    
    // Basic actions
    if (action == "gather") {
        return !currentPlayer->isSanctioned();
    }
    else if (action == "tax") {
        return currentPlayer->isTaxAvailable() && !currentPlayer->isSanctioned();
    }
    else if (action == "bribe") {
        return currentPlayer->coins() >= 4 && !currentPlayer->isBribeUsed();
    }
    else if (action == "arrest") {
        return currentPlayer->isArrestAvailable() && !getTargetablePlayers().empty();
    }
    else if (action == "sanction") {
        int cost = 3;
        // Check if any target is a judge (higher cost)
        for (Player* target : getTargetablePlayers()) {
            if (target->isJudge()) {
                cost = 4;
                break;
            }
        }
        return currentPlayer->coins() >= cost && !getTargetablePlayers().empty();
    }
    else if (action == "coup") {
        return currentPlayer->coins() >= 7 && !getTargetablePlayers().empty();
    }
    
    // Role-specific actions
    RoleType role = getPlayerRole(currentPlayer);
    
    if (action == "undo") {
        return role == RoleType::GOVERNOR && !getTargetablePlayers().empty();
    }
    else if (action == "invest") {
        return role == RoleType::BARON && currentPlayer->coins() >= 3;
    }
    else if (action == "spy_on") {
        return role == RoleType::SPY && !getTargetablePlayers().empty();
    }
    else if (action == "block_coup") {
        // General can block coup if there are eliminated players to revive
        if (role == RoleType::GENERAL && currentPlayer->coins() >= 5) {
            for (const auto& player : players) {
                if (!player->isActive()) {
                    return true; // There's at least one eliminated player to revive
                }
            }
        }
        return false;
    }
    else if (action == "block_bribe") {
        // Judge can block bribe if there are players who used bribe
        if (role == RoleType::JUDGE) {
            for (Player* target : getTargetablePlayers()) {
                if (target->isBribeUsed()) {
                    return true;
                }
            }
        }
        return false;
    }
    
    return false;
}

std::vector<Player*> GameGUI::getTargetablePlayers() const {
    std::vector<Player*> targets;
    Player* currentPlayer = getCurrentPlayer();
    
    if (!currentPlayer) return targets;
    
    for (const auto& player : players) {
        if (player.get() != currentPlayer && player->isActive()) {
            targets.push_back(player.get());
        }
    }
    
    return targets;
}

std::vector<Player*> GameGUI::getActivePlayers() const {
    std::vector<Player*> activePlayers;
    
    for (const auto& player : players) {
        if (player->isActive()) {
            activePlayers.push_back(player.get());
        }
    }
    
    return activePlayers;
}

void GameGUI::update() {
    playerNameInput.update();
    updateAnimations();
    
    if (currentState == GameState::PLAYING && game) {
        updatePlayerCards();
        updateGameInfo();
        updateActionAvailability();
    }
}

void GameGUI::updatePlayerCards() {
    if (playerCards.size() != players.size()) {
        createPlayerCards();
        return;
    }
    
    Player* currentPlayer = getCurrentPlayer();
    
    for (size_t i = 0; i < players.size() && i < playerCards.size(); ++i) {
        bool isCurrent = (currentPlayer && players[i].get() == currentPlayer);
        
        // Determine role for display (would need enhancement for actual role detection)
        RoleType displayRole = RoleType::PLAYER;
        
        // You could add role detection logic here based on the actual player types
        // For now, using default player role
        
        playerCards[i].updateInfo(players[i].get(), isCurrent, displayRole);
    }
}

void GameGUI::updateAnimations() {
    // Animate decorative coins
    static sf::Clock animClock;
    float time = animClock.getElapsedTime().asSeconds();
    
    for (size_t i = 0; i < decorativeCoins.size(); ++i) {
        float offset = sin(time + i) * 2;
        sf::Vector2f pos = decorativeCoins[i].getPosition();
        decorativeCoins[i].setPosition(pos.x, pos.y + offset);
    }
}

void GameGUI::render() {
    window.clear(theme.background);
    
    // Draw decorative elements
    for (const auto& coin : decorativeCoins) {
        window.draw(coin);
    }
    
    // Draw based on current state
    switch (currentState) {
        case GameState::MAIN_MENU:
            window.draw(titleText);
            window.draw(subtitleText);
            for (const auto& button : menuButtons) {
                button.draw(window);
            }
            break;
            
        case GameState::SETUP_PLAYERS:
            window.draw(instructionText);
            playerNameInput.draw(window);
            addPlayerButton.draw(window);
            if (players.size() >= 2) {
                startGameButton.draw(window);
            }
            backButton.draw(window);
            
            for (const auto& card : playerCards) {
                card.draw(window);
            }
            break;
            
        case GameState::PLAYING:
            // Draw game panels
            window.draw(gameInfoPanel);
            window.draw(actionPanel);
            window.draw(playerPanel);
            window.draw(feedbackPanel);
            
            // Draw game information
            window.draw(currentPlayerText);
            window.draw(gameStatusText);
            
            // Draw action buttons
            for (const auto& button : actionButtons) {
                button.draw(window);
            }
            
            // Draw player cards with target highlighting
            for (size_t i = 0; i < playerCards.size(); ++i) {
                // Highlight targetable players when waiting for target
                if (waitingForTarget && players[i].get() != getCurrentPlayer() && players[i]->isActive()) {
                    // Draw a selection highlight
                    sf::RectangleShape highlight = playerCards[i].background;
                    highlight.setFillColor(sf::Color(255, 255, 0, 50)); // Semi-transparent yellow
                    highlight.setOutlineColor(sf::Color::Yellow);
                    highlight.setOutlineThickness(3);
                    window.draw(highlight);
                }
                playerCards[i].draw(window);
            }
            
            // Draw action feedback
            window.draw(actionFeedbackText);
            break;
            
        case GameState::GAME_OVER:
            // Draw game over screen
            window.draw(gameOverTitle);
            window.draw(winnerText);
            returnToMenuButton.draw(window);
            break;
    }
    
    // Always draw message
    window.draw(messageText);
    
    window.display();
}

sf::Vector2f GameGUI::getCenterPosition(sf::Vector2f size) const {
    return sf::Vector2f((WINDOW_WIDTH - size.x) / 2, (WINDOW_HEIGHT - size.y) / 2);
}

sf::Vector2f GameGUI::getButtonGridPosition(int index, int columns, sf::Vector2f startPos, sf::Vector2f spacing) const {
    int row = index / columns;
    int col = index % columns;
    return sf::Vector2f(startPos.x + col * spacing.x, startPos.y + row * spacing.y);
}

void GameGUI::updateMessage(const std::string& message, bool isError, bool isWarning) {
    lastMessage = message;
    
    if (isError) {
        messageText.setFillColor(theme.error);
        actionFeedbackText.setFillColor(theme.error);
    } else if (isWarning) {
        messageText.setFillColor(theme.warning);
        actionFeedbackText.setFillColor(theme.warning);
    } else {
        messageText.setFillColor(theme.success);
        actionFeedbackText.setFillColor(theme.success);
    }
    
    messageText.setString(message);
    actionFeedbackText.setString(message);
}

RoleType GameGUI::getPlayerRole(Player* player) const {
    if (!player) return RoleType::PLAYER;
    
    // Check role types using virtual methods
    if (player->isGeneral()) return RoleType::GENERAL;
    if (player->isJudge()) return RoleType::JUDGE;
    if (player->isMerchant()) return RoleType::MERCHANT;
    
    // For other roles, we need to use dynamic_cast
    if (dynamic_cast<const Governor*>(player)) return RoleType::GOVERNOR;
    if (dynamic_cast<const Baron*>(player)) return RoleType::BARON;
    if (dynamic_cast<const Spy*>(player)) return RoleType::SPY;
    
    return RoleType::PLAYER;
}

Player* GameGUI::getCurrentPlayer() const {
    if (!game || !game->isGameStarted()) return nullptr;
    
    // Since we need access to the current player directly, we need to add a method to Game class
    // For now, let's use a workaround by checking which player's turn it is
    for (const auto& player : players) {
        if (game->isPlayerTurn(player.get())) {
            return player.get();
        }
    }
    return nullptr;
}

void GameGUI::createDecorativeElements() {
    decorativeCoins.clear();
    
    // Create some decorative coins around the window
    for (int i = 0; i < 8; ++i) {
        sf::CircleShape coin(12);
        coin.setFillColor(sf::Color(255, 215, 0, 150)); // Semi-transparent gold
        coin.setOutlineThickness(2);
        coin.setOutlineColor(sf::Color(255, 255, 255, 100));
        
        // Position coins around the edges
        float angle = (i * 45) * 3.14159f / 180.0f; // Convert to radians
        float radius = 300;
        float x = WINDOW_WIDTH/2 + cos(angle) * radius;
        float y = WINDOW_HEIGHT/2 + sin(angle) * radius;
        
        coin.setPosition(x, y);
        decorativeCoins.push_back(coin);
    }
}

} // namespace coup