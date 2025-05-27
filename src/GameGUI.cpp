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
        : isCurrentPlayer(false), isActive(true),
        deleteButton(sf::Vector2f(position.x + size.x - 70, position.y + size.y - 35), 
                    sf::Vector2f(50, 25), "X", "delete_player", sf::Color(180, 60, 60)) {
        
        // Customize delete button appearance
        deleteButton.hoverColor = sf::Color(220, 80, 80);
        deleteButton.normalColor = sf::Color(180, 60, 60);
        deleteButton.shape.setFillColor(deleteButton.normalColor);
        deleteButton.shape.setOutlineThickness(2);
        deleteButton.shape.setOutlineColor(sf::Color(120, 40, 40));
        deleteButton.text.setCharacterSize(16);
        deleteButton.text.setStyle(sf::Text::Bold);
        
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
        roleIcon.setOutlineThickness(2);
        roleIcon.setOutlineColor(sf::Color::Black);
        
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
        coinIcon.setFillColor(sf::Color(255, 215, 0)); // Gold color
        coinIcon.setPosition(position.x + 90, position.y + 75);
        coinIcon.setOutlineThickness(1);
        coinIcon.setOutlineColor(sf::Color(139, 69, 19)); // Brown outline for gold coin
    }

    void PlayerCard::updateInfo(const Player* player, bool current, RoleType playerRole) {
        isCurrentPlayer = current;
        isActive = player->isActive();
        role = playerRole;
        
        nameText.setString(player->getName());
        coinsText.setString("Coins: " + std::to_string(player->coins()));
        roleText.setString("Role: " + std::string(
                        role == RoleType::GOVERNOR ? "Governor" :
                        role == RoleType::SPY ? "Spy" :
                        role == RoleType::BARON ? "Baron" :
                        role == RoleType::GENERAL ? "General" :
                        role == RoleType::JUDGE ? "Judge" :
                        role == RoleType::MERCHANT ? "Merchant" : "Unassigned"));
        
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
        
    // Role-specific colors and shapes
        sf::Color roleColor = sf::Color(200, 100, 100);
        switch (role) {
            case RoleType::GOVERNOR: 
                roleColor = sf::Color(200, 150, 50); // Gold-like color
                break;
            case RoleType::SPY: 
                roleColor = sf::Color(100, 100, 200); // Blue color
                break;
            case RoleType::BARON: 
                roleColor = sf::Color(150, 100, 200); // Purple color
                break;
            case RoleType::GENERAL: 
                roleColor = sf::Color(200, 100, 100); // Red color
                break;
            case RoleType::JUDGE: 
                roleColor = sf::Color(150, 150, 150); // Silver/gray color
                break;
            case RoleType::MERCHANT: 
                roleColor = sf::Color(100, 200, 100); // Green color
                break;
            default: 
                roleColor = sf::Color(120, 120, 120); // Default gray
                break;
        }
        roleIcon.setFillColor(roleColor);
        
        // Customize icon appearance based on role
        switch (role) {
            case RoleType::GOVERNOR:
                roleIcon.setSize(sf::Vector2f(30, 20)); // Crown-like shape
                break;
            case RoleType::SPY:
                roleIcon.setSize(sf::Vector2f(25, 25)); // More square shape
                break;
            case RoleType::BARON:
                roleIcon.setSize(sf::Vector2f(35, 25)); // Wider rectangle
                break;
            case RoleType::GENERAL:
                roleIcon.setSize(sf::Vector2f(30, 30)); // Standard square
                break;
            case RoleType::JUDGE:
                roleIcon.setSize(sf::Vector2f(35, 20)); // Wider, flatter shape
                break;
            case RoleType::MERCHANT:
                roleIcon.setSize(sf::Vector2f(25, 30)); // Taller rectangle
                break;
            default:
                roleIcon.setSize(sf::Vector2f(30, 30)); // Default square
                break;
        }
    }

    void PlayerCard::draw(sf::RenderWindow& window, bool showDeleteButton) const {
        window.draw(background);
        window.draw(playerAvatar);
        window.draw(roleIcon);
        window.draw(nameText);
        window.draw(roleText);
        window.draw(coinsText);
        window.draw(statusText);
        // window.draw(coinIcon);
        
        // Only draw delete button when explicitly requested (during player setup)
        if (showDeleteButton) {
            deleteButton.draw(window);
        }
    }

    void PlayerCard::setFont(const sf::Font& font) {
        nameText.setFont(font);
        roleText.setFont(font);
        coinsText.setFont(font);
        statusText.setFont(font);
        deleteButton.setFont(font);
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
        waitingForReactivePlayer(false),
        selectedTarget(nullptr),
        pendingReactiveTarget(nullptr),
        playerNameInput(sf::Vector2f(500, 400), sf::Vector2f(300, 40), "Enter player name..."),
        addPlayerButton(sf::Vector2f(820, 400), sf::Vector2f(150, 40), "Add Player", "add_player"),
        startGameButton(sf::Vector2f(600, 500), sf::Vector2f(200, 50), "Start Game", "start_game"),
        backButton(sf::Vector2f(50, 50), sf::Vector2f(100, 40), "Back", "back"),
        returnToMenuButton(sf::Vector2f(600, 600), sf::Vector2f(200, 50), "Return to Menu", "menu", sf::Color(70, 130, 180)) {
        
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
        
        // Setup selection overlay
        selectionOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        selectionOverlay.setFillColor(sf::Color(0, 0, 0, 180)); // Semi-transparent black
        selectionOverlay.setPosition(0, 0);
        
        selectionTitle.setCharacterSize(32);
        selectionTitle.setFillColor(theme.accent);
        selectionTitle.setStyle(sf::Text::Bold);
        selectionTitle.setPosition(getCenterPosition(sf::Vector2f(400, 40)));
        selectionTitle.move(0, -150);
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
        
        // Initially disable start game button until we have enough players
        startGameButton.setEnabled(false);
        
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
        
        if (!game) return;
        
        std::vector<Player*> allPlayers = game->getAllPlayers();
        
        int playersPerRow = 3;
        sf::Vector2f cardSize(CARD_WIDTH, CARD_HEIGHT);
        sf::Vector2f spacing(20, 20);
        // Position cards within the player panel
        sf::Vector2f startPos(490, 80);
        
        for (size_t i = 0; i < allPlayers.size(); ++i) {
            int row = i / playersPerRow;
            int col = i % playersPerRow;
            
            sf::Vector2f pos(
                startPos.x + col * (cardSize.x + spacing.x),
                startPos.y + row * (cardSize.y + spacing.y)
            );
            
            playerCards.emplace_back(pos, cardSize);
            playerCards.back().setFont(mainFont);
            
            // Use the Player's getRoleType method for proper role detection
            RoleType displayRole = convertRoleType(allPlayers[i]->getRoleType());
            playerCards.back().updateInfo(allPlayers[i], false, displayRole);
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
        
        // Role-specific buttons will be added dynamically based on current player's role
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
                    {"Invest (3 -> 6 coins)", "invest"}
                };
                break;
                
            case RoleType::SPY:
                // Don't add spy_on here - it's a reactive ability
                break;
                
            case RoleType::GENERAL:
                // Don't add block_coup here - it's a reactive ability
                break;
                
            case RoleType::JUDGE:
                // Don't add block_bribe here - it's a reactive ability
                break;
                
            case RoleType::MERCHANT:
                // Merchant has passive ability only
                break;

            case RoleType::PLAYER:
                // No specific actions for unassigned roles
                break;
                
            default:
                break;
        }
        
        // Add role-specific buttons - place them below basic actions instead of to the right
        for (size_t i = 0; i < roleActions.size(); ++i) {
            sf::Vector2f pos(startPos.x, startPos.y + (6 + startIndex + i) * spacing); // Start after the 6 basic actions
            actionButtons.emplace_back(pos, buttonSize, 
                                    roleActions[i].first, roleActions[i].second, roleColor);
            actionButtons.back().setFont(mainFont);
        }
    }

        void GameGUI::addReactiveAbilityButtons() {
        if (!game) return;
        
        std::vector<Player*> allPlayers = game->getAllPlayers();
        sf::Vector2f buttonSize(180, 45);
        sf::Color reactiveColor = sf::Color(255, 140, 0); // Orange color for reactive abilities
        
        // Position reactive buttons in a separate column
        sf::Vector2f reactiveStartPos(280, 280);
        int buttonIndex = 0;
        int spacing = 55;
        
        // Track which reactive abilities are available (only add one button per ability type)
        bool hasActiveSpy = false;
        bool hasActiveGeneral = false;
        bool hasActiveJudge = false;
        
        // Check what reactive abilities are available
        for (Player* player : allPlayers) {
            RoleType playerRole = convertRoleType(player->getRoleType());
            
            switch (playerRole) {
                case RoleType::SPY:
                    if (player->isActive()) {
                        hasActiveSpy = true;
                    }
                    break;
                case RoleType::GENERAL:
                    if (player->coins() >= 5) {
                        hasActiveGeneral = true;
                    }
                    break;
                case RoleType::JUDGE:
                    if (player->isActive()) {
                        hasActiveJudge = true;
                    }
                    break;
                default:
                    break;
            }
        }
        
        // Add one button per available reactive ability type
        if (hasActiveSpy) {
            sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
            actionButtons.emplace_back(pos, buttonSize, 
                                    "Spy On", "spy_on", reactiveColor);
            actionButtons.back().setFont(mainFont);
            buttonIndex++;
        }
        
        if (hasActiveGeneral) {
            sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
            actionButtons.emplace_back(pos, buttonSize, 
                                    "Block Coup (5 coins)", "block_coup", reactiveColor);
            actionButtons.back().setFont(mainFont);
            buttonIndex++;
        }
        
        if (hasActiveJudge) {
            sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
            actionButtons.emplace_back(pos, buttonSize, 
                                    "Block Bribe", "block_bribe", reactiveColor);
            actionButtons.back().setFont(mainFont);
            buttonIndex++;
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
                
                if (startGameButton.contains(mousePos) && game && game->getAllPlayers().size() >= 2) {
                    startNewGame();
                }
                
                if (backButton.contains(mousePos)) {
                    // Clear all players when going back to main menu
                    if (game) {
                        try {
                            game->clearAllPlayers();
                            updateMessage("All players cleared", false);
                        } catch (const std::exception& e) {
                            updateMessage("Error clearing players: " + std::string(e.what()), true);
                        }
                        delete game;
                        game = nullptr;
                    }
                    changeState(GameState::MAIN_MENU);
                }
                
                // Handle delete player button clicks
                if (game) {
                    std::vector<Player*> allPlayers = game->getAllPlayers();
                    for (size_t i = 0; i < playerCards.size() && i < allPlayers.size(); ++i) {
                        if (playerCards[i].deleteButton.contains(mousePos)) {
                            try {
                                std::string playerName = allPlayers[i]->getName();
                                game->removePlayer(allPlayers[i]);
                                createPlayerCards(); // Refresh the player cards
                                updateMessage("Player " + playerName + " removed", false);
                                
                                // Update start button state
                                std::vector<Player*> updatedPlayers = game->getAllPlayers();
                                startGameButton.setEnabled(updatedPlayers.size() >= 2);
                            } catch (const std::exception& e) {
                                updateMessage("Error removing player: " + std::string(e.what()), true);
                            }
                            break; // Only handle one delete at a time
                        }
                    }
                }
                break;
                
            case GameState::PLAYING:
                // Handle reactive player selection first (overlay takes priority)
                if (waitingForReactivePlayer) {
                    for (size_t i = 0; i < reactivePlayerButtons.size(); ++i) {
                        if (reactivePlayerButtons[i].contains(mousePos)) {
                            // Handle General coup decision
                            if (pendingReactiveAction == "general_coup_decision") {
                                if (reactivePlayerButtons[i].action.substr(0, 15) == "general_revive_") {
                                    // Extract the player index from the action string
                                    int playerIndex = std::stoi(reactivePlayerButtons[i].action.substr(15));
                                    if (playerIndex < eligibleReactivePlayers.size()) {
                                        Player* targetPlayer = eligibleReactivePlayers[playerIndex];
                                        
                                        // General chooses to block coup on selected player
                                        try {
                                            General* general = dynamic_cast<General*>(pendingReactiveTarget);
                                            if (general) {
                                                general->block_coup(*targetPlayer);
                                                updateMessage(pendingReactiveTarget->getName() + " (General) chose to revive " + 
                                                            targetPlayer->getName() + " - game continues!");
                                            }
                                        } catch (const std::exception& e) {
                                            updateMessage("Error: " + std::string(e.what()), true);
                                        }
                                        hideReactivePlayerSelection();
                                        return;
                                    }
                                } else if (reactivePlayerButtons[i].action == "general_block_no") {
                                    // General chooses not to block - end game
                                    try {
                                        std::string winner = game->winner();
                                        updateMessage(pendingReactiveTarget->getName() + " (General) declined to block coup - Game Over! Winner: " + winner);
                                        changeState(GameState::GAME_OVER);
                                    } catch (const std::exception&) {
                                        updateMessage("Game ended", false);
                                        changeState(GameState::GAME_OVER);
                                    }
                                    hideReactivePlayerSelection();
                                    return;
                                }
                            }
                            // Handle other reactive player selections
                            else if (i < eligibleReactivePlayers.size()) {
                                executeReactiveAction(pendingReactiveAction, eligibleReactivePlayers[i], pendingReactiveTarget);
                                hideReactivePlayerSelection();
                                return;
                            }
                        }
                    }
                    return; // Block all other clicks while waiting for reactive player selection
                }
                
                // Handle game actions
                for (auto& button : actionButtons) {
                    if (button.contains(mousePos)) {
                        currentAction = button.action;
                        if (currentAction == "arrest" || currentAction == "sanction" || currentAction == "coup" || 
                            currentAction == "undo" || currentAction == "spy_on" || currentAction == "block_coup" || 
                            currentAction == "block_bribe") {
                            waitingForTarget = true;
                            updateMessage("Select a target player");
                        } else {
                            executeAction(currentAction);
                        }
                    }
                }
                
                // Handle target selection
                if (waitingForTarget) {
                    std::vector<Player*> allPlayers = game->getAllPlayers();
                    for (size_t i = 0; i < playerCards.size() && i < allPlayers.size(); ++i) {
                        if (playerCards[i].background.getGlobalBounds().contains(mousePos)) {
                            // Check if target is valid based on action type
                            bool validTarget = false;
                            
                            if (currentAction == "spy_on" || currentAction == "block_bribe") {
                                // Reactive abilities can target any active player (including current player)
                                validTarget = allPlayers[i]->isActive();
                            } else if (currentAction == "block_coup") {
                                // Block coup can only target inactive players (to revive them)
                                validTarget = !allPlayers[i]->isActive();
                            } else {
                                // Regular actions cannot target the current player
                                validTarget = allPlayers[i] != game->getCurrentPlayer() && allPlayers[i]->isActive();
                            }
                            
                            if (validTarget) {
                                executeAction(currentAction, allPlayers[i]);
                                waitingForTarget = false;
                                currentAction = "";
                            } else {
                                if (currentAction == "spy_on" || currentAction == "block_bribe") {
                                    updateMessage("Invalid target - select an active player", true);
                                } else if (currentAction == "block_coup") {
                                    updateMessage("Invalid target - select an inactive player to revive", true);
                                } else {
                                    updateMessage("Invalid target - select an active opponent", true);
                                }
                            }
                        }
                    }
                }
                
                break;

                // Handle reactive player selection first (overlay takes priority)
                if (waitingForReactivePlayer) {
                    for (size_t i = 0; i < reactivePlayerButtons.size(); ++i) {
                        if (reactivePlayerButtons[i].contains(mousePos)) {
                            // Handle General coup decision
                            if (pendingReactiveAction == "general_coup_decision") {
                                if (reactivePlayerButtons[i].action == "general_block_yes") {
                                    // General chooses to block - continue game
                                    updateMessage(pendingReactiveTarget->getName() + " (General) chose to block coup - game continues!");
                                    hideReactivePlayerSelection();
                                    return;
                                } else if (reactivePlayerButtons[i].action == "general_block_no") {
                                    // General chooses not to block - end game
                                    try {
                                        std::string winner = game->winner();
                                        updateMessage("Game Over! Winner: " + winner);
                                        changeState(GameState::GAME_OVER);
                                    } catch (const std::exception&) {
                                        updateMessage("Game ended", false);
                                        changeState(GameState::GAME_OVER);
                                    }
                                    hideReactivePlayerSelection();
                                    return;
                                }
                            }
                            // Handle other reactive player selections
                            else if (i < eligibleReactivePlayers.size()) {
                                executeReactiveAction(pendingReactiveAction, eligibleReactivePlayers[i], pendingReactiveTarget);
                                hideReactivePlayerSelection();
                                return;
                            }
                        }
                    }
                    return; // Block all other clicks while waiting for reactive player selection
                }
                
                break;
                
            case GameState::GAME_OVER:
                // Handle game over screen
                if (returnToMenuButton.contains(mousePos)) {
                    if (game) {
                        delete game;
                        game = nullptr;
                    }
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
                // Add delete buttons from player cards
                for (auto& card : playerCards) {
                    allButtons.push_back(&card.deleteButton);
                }
                break;
            case GameState::PLAYING:
                for (auto& btn : actionButtons) allButtons.push_back(&btn);
                // Add reactive player selection buttons if active
                if (waitingForReactivePlayer) {
                    for (auto& btn : reactivePlayerButtons) allButtons.push_back(&btn);
                }
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
                    if (game && game->getAllPlayers().size() >= 2) {
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
        
        if (!game) {
            game = new Game();
        }
        
        // Check for duplicate names through Game class
        std::vector<Player*> allPlayers = game->getAllPlayers();
        if (allPlayers.size() >= 6) {
            updateMessage("Maximum 6 players allowed!", true);
            return;
        }
        
        for (const auto& player : allPlayers) {
            if (player->getName() == name) {
                updateMessage("Player name already exists!", true);
                return;
            }
        }
        
        try {
            // Assign a random role to the new player
            std::vector<RoleType> roles = { RoleType::GOVERNOR, RoleType::SPY, RoleType::BARON, 
                                            RoleType::GENERAL, RoleType::JUDGE, RoleType::MERCHANT };
            RoleType assignedRole = roles[game->getRandomGenerator()() % roles.size()];
            
            // Create a new player with the assigned role (this already adds the player to the game)
            game->createPlayerWithRole(name, assignedRole);
            
            playerNameInput.clear();
            createPlayerCards();
            updateMessage("Player " + name + " added as " + game->getRoleName(assignedRole) + " successfully!", false);
            
            // Enable start button if we have enough players (get updated player count)
            std::vector<Player*> updatedPlayers = game->getAllPlayers();
            startGameButton.setEnabled(updatedPlayers.size() >= 2);
        } catch (const std::exception& e) {
            updateMessage("Error adding player: " + std::string(e.what()), true);
        }
    }

    void GameGUI::startNewGame() {
        if (!game || game->getAllPlayers().size() < 2) {
            updateMessage("Need at least 2 players to start!", true);
            return;
        }
        
        try {
            game->startGame(); // Start the game using existing players
            changeState(GameState::PLAYING);
            updateMessage("Game started! " + game->getCurrentPlayer()->getName() + "'s turn");
        } catch (const std::exception& e) {
            updateMessage("Error starting game: " + std::string(e.what()), true);
        }
    }
    
    void GameGUI::executeAction(const std::string& action, Player* target) {
        Player* currentPlayer = game->getCurrentPlayer();
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
                if (target->coins() == 0) {
                    updateMessage(currentPlayer->getName() + " arrested " + target->getName() + ", though " + target->getName() + " had no coins", false, true);
                }
                else {
                    updateMessage(currentPlayer->getName() + " arrested " + target->getName());
                }
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
                // Find eligible Spy players for this action
                std::vector<Player*> eligiblePlayers = getEligibleReactivePlayers(action);
                
                if (eligiblePlayers.empty()) {
                    updateMessage("No active Spy available to spy on players!", true);
                } else if (eligiblePlayers.size() == 1) {
                    // Automatic selection for single player
                    executeReactiveAction(action, eligiblePlayers[0], target);
                } else {
                    // Multiple players available - show selection overlay
                    updateMessage("Multiple Spies available - choose one", false);
                    showReactivePlayerSelection(action, target, eligiblePlayers);
                    return; // Important: return here to prevent further execution
                }
            }
            else if (action == "block_coup" && target) {
                // Find eligible General players for this action
                std::vector<Player*> eligiblePlayers = getEligibleReactivePlayers(action);
                
                if (eligiblePlayers.empty()) {
                    updateMessage("No active General with 5+ coins available to block coup!", true);
                } else if (eligiblePlayers.size() == 1) {
                    // Automatic selection for single player
                    executeReactiveAction(action, eligiblePlayers[0], target);
                } else {
                    // Multiple players available - show selection overlay
                    showReactivePlayerSelection(action, target, eligiblePlayers);
                }
            }
            else if (action == "block_bribe" && target) {
                // Find eligible Judge players for this action
                std::vector<Player*> eligiblePlayers = getEligibleReactivePlayers(action);
                
                if (eligiblePlayers.empty()) {
                    updateMessage("No active Judge available to block bribes!", true);
                } else if (eligiblePlayers.size() == 1) {
                    // Automatic selection for single player
                    executeReactiveAction(action, eligiblePlayers[0], target);
                } else {
                    // Multiple players available - show selection overlay
                    showReactivePlayerSelection(action, target, eligiblePlayers);
                }
            }
            
            updatePlayerCards();
            updateGameInfo();

            // Recreate action buttons to update role-specific ones
            createActionButtons();
            Player* currentPlayer = game->getCurrentPlayer();
            if (currentPlayer) {
                RoleType role = convertRoleType(currentPlayer->getRoleType());
                sf::Vector2f startPos(70, 280);
                sf::Vector2f buttonSize(180, 45);
                int spacing = 55;
                addRoleSpecificButtons(role, startPos, buttonSize, spacing, 0);
            }
            
            // Add reactive ability buttons for all players
            addReactiveAbilityButtons();
            
            // Check for game over
            try {
                std::string winner = game->winner();
                
                // Before declaring game over, check if ANY General can block coup to prevent game end
                std::vector<Player*> allPlayers = game->getAllPlayers();
                bool hasGeneralWithCoins = false;
                
                for (Player* player : allPlayers) {
                    if (player->isGeneral() && player->coins() >= 5) {
                        hasGeneralWithCoins = true;
                        break;
                    }
                }
                
                if (hasGeneralWithCoins) {
                    // Show General block coup decision dialog instead of ending game
                    showGeneralBlockCoupDecision();
                } else {
                    updateMessage("Game Over! Winner: " + winner);
                    changeState(GameState::GAME_OVER);
                }
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

        // Add role-specific buttons for current player (for active abilities)
        if (game && game->getCurrentPlayer()) {
            Player* currentPlayer = game->getCurrentPlayer();
            std::string roleType = currentPlayer->getRoleType();
            RoleType role = convertRoleType(roleType);
            
            // Add role-specific buttons for current player
            sf::Vector2f startPos(70, 280);
            sf::Vector2f buttonSize(180, 45);
            int spacing = 55;
            addRoleSpecificButtons(role, startPos, buttonSize, spacing, 0);
        }
        
        // Add reactive ability buttons for all players
        addReactiveAbilityButtons();
        
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
        
        Player* currentPlayer = game->getCurrentPlayer();
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
        
        Player* currentPlayer = game->getCurrentPlayer();
        if (!currentPlayer) {
            for (auto& button : actionButtons) {
                button.setEnabled(false);
            }
            return;
        }
        
        // Update each action button based on availability using state variables
        for (auto& button : actionButtons) {
            bool available = false;
            
            if (button.action == "gather") {
                // Player can gather if: active, not sanctioned, and not forced to coup
                available = currentPlayer->isActive() && 
                        !currentPlayer->isSanctioned() && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed());
            } else if (button.action == "tax") {
                // Player can tax if: active, not sanctioned, tax available, and not forced to coup
                available = currentPlayer->isActive() && 
                        !currentPlayer->isSanctioned() && 
                        currentPlayer->isTaxAvailable() && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed());
            } else if (button.action == "bribe") {
                // Player can bribe if: active, has 4+ coins, and not forced to coup
                available = currentPlayer->isActive() && 
                        currentPlayer->coins() >= 4 && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed());
            } else if (button.action == "arrest") {
                // Player can arrest if: active, arrest available, not forced to coup, and has targets
                available = currentPlayer->isActive() && 
                        currentPlayer->isArrestAvailable() && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed()) && 
                        !getTargetablePlayers().empty();
            } else if (button.action == "sanction") {
                // Check if player can sanction any target
                available = false;
                if (currentPlayer->isActive() && 
                    !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed())) {
                    for (Player* target : getTargetablePlayers()) {
                        // Check if player has enough coins (3 for normal, 4 for Judge)
                        int requiredCoins = target->isJudge() ? 4 : 3;
                        if (currentPlayer->coins() >= requiredCoins) {
                            available = true;
                            break;
                        }
                    }
                }
            } else if (button.action == "coup") {
                // Player can coup if: active, has 7+ coins, and has targets
                available = currentPlayer->isActive() && 
                        currentPlayer->coins() >= 7 && 
                        !getTargetablePlayers().empty();
            }
            // Role-specific actions
            else if (button.action == "undo") {
                Governor* governor = dynamic_cast<Governor*>(currentPlayer);
                available = governor && currentPlayer->isActive() && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed()) &&
                        !getTargetablePlayers().empty();
            } else if (button.action == "invest") {
                Baron* baron = dynamic_cast<Baron*>(currentPlayer);
                available = baron && currentPlayer->isActive() && 
                        currentPlayer->coins() >= 3 &&
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed());
            } 
            // Reactive abilities - available to any player with the appropriate role
            else if (button.action == "spy_on") {
                // Find any active Spy player
                std::vector<Player*> allPlayers = game->getAllPlayers();
                available = false;
                for (Player* player : allPlayers) {
                    Spy* spy = dynamic_cast<Spy*>(player);
                    if (spy && player->isActive() && !getTargetablePlayers().empty()) {
                        available = true;
                        break;
                    }
                }
            } else if (button.action == "block_coup") {
                // Find any General player with 5+ coins (including inactive ones)
                std::vector<Player*> allPlayers = game->getAllPlayers();
                available = false;
                for (Player* player : allPlayers) {
                    General* general = dynamic_cast<General*>(player);
                    if (general && player->coins() >= 5) {
                        // Check if there are any inactive players that could be revived
                        bool hasInactivePlayers = false;
                        for (Player* otherPlayer : allPlayers) {
                            if (!otherPlayer->isActive()) {
                                hasInactivePlayers = true;
                                break;
                            }
                        }
                        if (hasInactivePlayers) {
                            available = true;
                            break;
                        }
                    }
                }
            } else if (button.action == "block_bribe") {
                // Find any active Judge player when there are players who have used bribe
                std::vector<Player*> allPlayers = game->getAllPlayers();
                available = false;
                for (Player* player : allPlayers) {
                    Judge* judge = dynamic_cast<Judge*>(player);
                    if (judge && player->isActive()) {
                        // Check if any player has used bribe
                        bool hasBribeUser = false;
                        for (Player* otherPlayer : allPlayers) {
                            if (otherPlayer->isBribeUsed()) {
                                hasBribeUser = true;
                                break;
                            }
                        }
                        if (hasBribeUser) {
                            available = true;
                            break;
                        }
                    }
                }
            }
            // Add role-specific actions here if needed
            
            button.setEnabled(available);
            
            // Add visual feedback for unavailable actions
            if (!available) {
                button.setHovered(false);
            }
        }
    }

    std::vector<Player*> GameGUI::getTargetablePlayers() const {
        std::vector<Player*> targets;
        Player* currentPlayer = game->getCurrentPlayer();
        
        if (!currentPlayer) return targets;
        
        std::vector<Player*> allPlayers = game->getActivePlayers();
        for (Player* player : allPlayers) {
            if (player != currentPlayer) {
                targets.push_back(player);
            }
        }
        
        return targets;
    }

    std::vector<Player*> GameGUI::getActivePlayers() const {
        if (!game) {
            return std::vector<Player*>();
        }
        return game->getActivePlayers();
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
        std::vector<Player*> allPlayers = game->getAllPlayers();
        if (playerCards.size() != allPlayers.size()) {
            createPlayerCards();
            return;
        }
        
        Player* currentPlayer = game->getCurrentPlayer();
        
        for (size_t i = 0; i < allPlayers.size() && i < playerCards.size(); ++i) {
            bool isCurrent = (currentPlayer && allPlayers[i] == currentPlayer);
            
            // Use the Player's getRoleType method for proper role detection
            RoleType displayRole = convertRoleType(allPlayers[i]->getRoleType());
            
            playerCards[i].updateInfo(allPlayers[i], isCurrent, displayRole);
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
                startGameButton.draw(window);
                backButton.draw(window);
                
                for (const auto& card : playerCards) {
                    card.draw(window, true); // Show delete buttons during setup
                }
                break;
                
            case GameState::PLAYING: {
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
                std::vector<Player*> allPlayers = game->getAllPlayers();
                for (size_t i = 0; i < playerCards.size() && i < allPlayers.size(); ++i) {
                    // Highlight targetable players when waiting for target
                    if (waitingForTarget && allPlayers[i] != game->getCurrentPlayer() && allPlayers[i]->isActive()) {
                        // Draw a selection highlight
                        sf::RectangleShape highlight = playerCards[i].background;
                        highlight.setFillColor(sf::Color(255, 255, 0, 50)); // Semi-transparent yellow
                        highlight.setOutlineColor(sf::Color::Yellow);
                        highlight.setOutlineThickness(3);
                        window.draw(highlight);
                    }
                    playerCards[i].draw(window, false); // Don't show delete buttons during gameplay
                }
                
                // Draw action feedback
                window.draw(actionFeedbackText);
                break;
            }
                
            case GameState::GAME_OVER:
                // Draw game over screen
                window.draw(gameOverTitle);
                window.draw(winnerText);
                returnToMenuButton.draw(window);
                break;
        }
        
        // Draw reactive player selection overlay if waiting for selection
        if (waitingForReactivePlayer) {
            window.draw(selectionOverlay);
            window.draw(selectionTitle);
            
            // Draw question text if it's a General decision
            if (pendingReactiveAction == "general_coup_decision") {
                window.draw(generalDecisionQuestion);
            }

            for (const auto& button : reactivePlayerButtons) {
                button.draw(window);
            }
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

    RoleType GameGUI::convertRoleType(const std::string& roleStr) const {
        if (roleStr == "Governor") return RoleType::GOVERNOR;
        else if (roleStr == "Baron") return RoleType::BARON;
        else if (roleStr == "Spy") return RoleType::SPY;
        else if (roleStr == "General") return RoleType::GENERAL;
        else if (roleStr == "Judge") return RoleType::JUDGE;
        else if (roleStr == "Merchant") return RoleType::MERCHANT;
        else return RoleType::PLAYER;
    }

    std::vector<Player*> GameGUI::getEligibleReactivePlayers(const std::string& action) const {
        std::vector<Player*> eligiblePlayers;
        if (!game) return eligiblePlayers;
        
        std::vector<Player*> allPlayers = game->getAllPlayers();
        
        for (Player* player : allPlayers) {
            if (action == "spy_on") {
                // Check if player is a Spy and is active
                Spy* spy = dynamic_cast<Spy*>(player);
                if (spy && player->isActive()) {
                    eligiblePlayers.push_back(player);
                }
            }
            else if (action == "block_coup") {
                General* general = dynamic_cast<General*>(player);
                if (general && player->coins() >= 5) {
                    // Allow both active and inactive Generals (inactive can block their own coup)
                    eligiblePlayers.push_back(player);
                }
            }
            else if (action == "block_bribe") {
                Judge* judge = dynamic_cast<Judge*>(player);
                if (judge && player->isActive()) {
                    eligiblePlayers.push_back(player);
                }
            }
        }
        
        return eligiblePlayers;
    }

    void GameGUI::showReactivePlayerSelection(const std::string& action, Player* target, const std::vector<Player*>& eligiblePlayers) {
        waitingForReactivePlayer = true;
        pendingReactiveAction = action;
        pendingReactiveTarget = target;
        eligibleReactivePlayers = eligiblePlayers;
        
        // Setup selection title
        std::string roleType = (action == "spy_on") ? "Spy" : 
                               (action == "block_coup") ? "General" : 
                               (action == "block_bribe") ? "Judge" : "Player";
        selectionTitle.setString("Choose " + roleType);
        
        // Center the title
        sf::FloatRect bounds = selectionTitle.getLocalBounds();
        selectionTitle.setPosition((WINDOW_WIDTH - bounds.width) / 2, 200);

        // Add "Who's gonna use it?" title
        selectionTitle.setFont(mainFont);
        selectionTitle.setString("Who's gonna use it?");
        selectionTitle.setCharacterSize(20);
        selectionTitle.setFillColor(theme.textSecondary);
        sf::FloatRect subtitleBounds = selectionTitle.getLocalBounds();
        selectionTitle.setPosition((WINDOW_WIDTH - subtitleBounds.width) / 2, 240);
        
        // Create buttons for each eligible player
        reactivePlayerButtons.clear();
        sf::Vector2f buttonSize(250, 50);
        sf::Vector2f startPos((WINDOW_WIDTH - buttonSize.x) / 2, 280);
        int spacing = 60;
        
        for (size_t i = 0; i < eligiblePlayers.size(); ++i) {
            sf::Vector2f pos(startPos.x, startPos.y + i * spacing);
            std::string buttonText = eligiblePlayers[i]->getName();
            
            reactivePlayerButtons.emplace_back(pos, buttonSize, buttonText, "select_reactive_player_" + std::to_string(i), theme.primary);
            reactivePlayerButtons.back().setFont(mainFont);
        }
    }

    void GameGUI::hideReactivePlayerSelection() {
        waitingForReactivePlayer = false;
        pendingReactiveAction = "";
        pendingReactiveTarget = nullptr;
        eligibleReactivePlayers.clear();
        reactivePlayerButtons.clear();
    }

    void GameGUI::executeReactiveAction(const std::string& action, Player* reactivePlayer, Player* target) {
        try {
            if (action == "spy_on" && target) {
                Spy* spy = dynamic_cast<Spy*>(reactivePlayer);
                if (spy) {
                    spy->spy_on(*target);
                    updateMessage(spy->getName() + " spied on " + target->getName() + 
                                " (Coins: " + std::to_string(target->coins()) + ") and revoked their arrest ability for their next turn");
                } else {
                    updateMessage("Selected player is not a Spy!", true);
                }
            }
            else if (action == "block_coup" && target) {
                General* general = dynamic_cast<General*>(reactivePlayer);
                if (general && reactivePlayer->coins() >= 5) {
                    general->block_coup(*target);
                    updateMessage(general->getName() + " blocked coup and revived " + target->getName());
                } else {
                    updateMessage("Selected player is not a General with 5+ coins!", true);
                }
            }
            else if (action == "block_bribe" && target) {
                Judge* judge = dynamic_cast<Judge*>(reactivePlayer);
                if (judge) {
                    judge->block_bribe(*target);
                    updateMessage(judge->getName() + " blocked " + target->getName() + "'s bribe");
                } else {
                    updateMessage("Selected player is not a Judge!", true);
                }
            }
            
            // Update game state
            updatePlayerCards();
            updateGameInfo(); // Force update of game info to reflect changes
            createActionButtons();
            Player* currentPlayer = game->getCurrentPlayer();
            if (currentPlayer) {
                RoleType role = convertRoleType(currentPlayer->getRoleType());
                sf::Vector2f startPos(70, 280);
                sf::Vector2f buttonSize(180, 45);
                int spacing = 55;
                addRoleSpecificButtons(role, startPos, buttonSize, spacing, 0);
            }
            addReactiveAbilityButtons();
            
        } catch (const std::exception& e) {
            updateMessage("Error: " + std::string(e.what()), true);
        }
    }

    void GameGUI::showGeneralBlockCoupDecision() {
        if (!game) return;
        
        // Find ANY General who can block coup (active or inactive)
        Player* generalPlayer = nullptr;
        std::vector<Player*> allPlayers = game->getAllPlayers();
        
        for (Player* player : allPlayers) {
            if (player->isGeneral() && player->coins() >= 5) {
                generalPlayer = player;
                break;
            }
        }
        
        if (!generalPlayer) return;
        
        // Set up the decision overlay
        waitingForReactivePlayer = true;
        pendingReactiveAction = "general_coup_decision";
        pendingReactiveTarget = generalPlayer;
        
        // Setup selection title
        selectionTitle.setFont(mainFont);
        selectionTitle.setString(generalPlayer->getName() + " (General)");
        selectionTitle.setCharacterSize(32);
        selectionTitle.setFillColor(theme.accent);
        selectionTitle.setStyle(sf::Text::Bold);
        
        // Center the title
        sf::FloatRect bounds = selectionTitle.getLocalBounds();
        selectionTitle.setPosition((WINDOW_WIDTH - bounds.width) / 2, 200);
        
        // Add subtitle asking the question
        sf::Text questionText;
        questionText.setFont(mainFont);
        questionText.setString("Choose a player to block coup on (or decline to end game):");
        questionText.setCharacterSize(20);
        questionText.setFillColor(theme.textSecondary);
        sf::FloatRect questionBounds = questionText.getLocalBounds();
        questionText.setPosition((WINDOW_WIDTH - questionBounds.width) / 2, 250);
        
        // Store the question text
        generalDecisionQuestion = questionText;
        
        // Create buttons for each inactive player + decline option
        reactivePlayerButtons.clear();
        sf::Vector2f buttonSize(200, 45);
        sf::Vector2f startPos((WINDOW_WIDTH - buttonSize.x) / 2, 300);
        int spacing = 55;
        int buttonIndex = 0;
        
        // Add buttons for inactive players
        for (Player* player : allPlayers) {
            if (!player->isActive()) {
                sf::Vector2f pos(startPos.x, startPos.y + buttonIndex * spacing);
                std::string buttonText = "Revive " + player->getName();
                reactivePlayerButtons.emplace_back(pos, buttonSize, buttonText, 
                    "general_revive_" + std::to_string(buttonIndex), sf::Color(50, 205, 50));
                eligibleReactivePlayers.push_back(player); // Store the player for later reference
                buttonIndex++;
            }
        }
        
        // Add decline button
        sf::Vector2f declinePos(startPos.x, startPos.y + buttonIndex * spacing);
        reactivePlayerButtons.emplace_back(declinePos, buttonSize, "Decline (End Game)", 
            "general_block_no", sf::Color(220, 20, 60));
        
        for (auto& button : reactivePlayerButtons) {
            button.setFont(mainFont);
        }
    }
} // namespace coup