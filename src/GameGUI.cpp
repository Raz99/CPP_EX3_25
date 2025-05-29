// Email: razcohenp@gmail.com

// GameGUI.cpp - Graphical User Interface Implementation
// Implements the complete visual interface using SFML for the Coup card game
// Handles all game states, player interactions, and visual feedback

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
    // EnhancedButton Implementation - Interactive button with hover effects and role-specific styling
    
    EnhancedButton::EnhancedButton(sf::Vector2f position, sf::Vector2f size, const std::string& label, 
                                const std::string& actionType, sf::Color normal)
        : action(actionType), enabled(true), hovered(false), normalColor(normal) {
        
        shape.setPosition(position); // Set button position on screen
        shape.setSize(size); // Define button dimensions
        shape.setFillColor(normalColor); // Apply base color scheme
        
        // Create dynamic color variations for interactive feedback
        hoverColor = sf::Color(
            std::min(255, (int)normal.r + 30), // Brighten red component
            std::min(255, (int)normal.g + 30), // Brighten green component
            std::min(255, (int)normal.b + 30)  // Brighten blue component
        );
        disabledColor = sf::Color(100, 100, 100); // Standard gray for disabled state
        
        shape.setOutlineThickness(2); // Add border for definition
        shape.setOutlineColor(sf::Color::White); // White border for contrast
        
        // Enhanced visual styling with rounded corners effect
        shape.setOutlineThickness(3); // Thicker border for premium look
        
        text.setString(label); // Set button display text
        text.setCharacterSize(18); // Readable font size
        text.setFillColor(sf::Color::White); // High contrast text color
        text.setStyle(sf::Text::Bold); // Bold text for emphasis
        
        // Create decorative icon element for visual interest
        icon.setRadius(8); // Small circular icon
        icon.setFillColor(sf::Color(255, 215, 0)); // Gold accent color
        icon.setPosition(position.x + 10, position.y + size.y/2 - 8); // Left-aligned positioning
    }

    bool EnhancedButton::contains(sf::Vector2f point) const {
        return enabled && shape.getGlobalBounds().contains(point); // Only clickable when enabled
    }

    void EnhancedButton::setEnabled(bool enable) {
        enabled = enable; // Update button state
        if (enabled) {
            // Restore interactive colors for active button
            shape.setFillColor(hovered ? hoverColor : normalColor);
            text.setFillColor(sf::Color::White); // Bright text for active state
            icon.setFillColor(sf::Color(255, 215, 0)); // Gold icon for active state
        } else {
            // Apply muted colors for disabled button
            shape.setFillColor(disabledColor); // Gray background
            text.setFillColor(sf::Color(150, 150, 150)); // Dim text
            icon.setFillColor(sf::Color(80, 80, 80)); // Dim icon
        }
    }

    void EnhancedButton::setHovered(bool hover) {
        hovered = hover && enabled; // Only allow hover effect when enabled
        if (enabled) {
            // Apply hover visual feedback
            shape.setFillColor(hovered ? hoverColor : normalColor);
            shape.setOutlineColor(hovered ? sf::Color::Yellow : sf::Color::White); // Yellow highlight on hover
        }
    }

    void EnhancedButton::draw(sf::RenderWindow& window) const {
        window.draw(shape); // Draw button background
        window.draw(icon); // Draw decorative icon
        window.draw(text); // Draw button label
    }

    void EnhancedButton::setFont(const sf::Font& font) {
        text.setFont(font); // Apply font to button text

        // Calculate precise text centering within button bounds
        sf::FloatRect textBounds = text.getLocalBounds(); // Get text dimensions
        sf::Vector2f buttonCenter = shape.getPosition() + shape.getSize() / 2.0f; // Find button center

        // Position text with offset to account for icon space and perfect centering
        text.setPosition(
            shape.getPosition().x + (shape.getSize().x - textBounds.width) / 2 - textBounds.left + 15, // Horizontal center with icon offset
            shape.getPosition().y + (shape.getSize().y - textBounds.height) / 2 - textBounds.top // Vertical center
        );
    }

    // PlayerCard Implementation - Visual representation of player state and information
    
    PlayerCard::PlayerCard(sf::Vector2f position, sf::Vector2f size)
        : isCurrentPlayer(false), isActive(true),
        deleteButton(sf::Vector2f(position.x + size.x - 70, position.y + size.y - 35), 
                    sf::Vector2f(50, 25), "X", "delete_player", sf::Color(180, 60, 60)) {
        
        // Customize delete button for player removal functionality
        deleteButton.hoverColor = sf::Color(220, 80, 80); // Bright red on hover
        deleteButton.normalColor = sf::Color(180, 60, 60); // Dark red normal state
        deleteButton.shape.setFillColor(deleteButton.normalColor); // Apply base color
        deleteButton.shape.setOutlineThickness(2); // Defined border
        deleteButton.shape.setOutlineColor(sf::Color(120, 40, 40)); // Darker red border
        deleteButton.text.setCharacterSize(16); // Smaller text for compact button
        deleteButton.text.setStyle(sf::Text::Bold); // Bold X for emphasis
        
        // Setup main card background with professional styling
        background.setPosition(position); // Card placement
        background.setSize(size); // Card dimensions
        background.setFillColor(sf::Color(40, 40, 60)); // Dark blue-gray background
        background.setOutlineThickness(3); // Prominent border
        background.setOutlineColor(sf::Color::White); // White border for contrast
        
        // Create circular player avatar for visual identification
        playerAvatar.setRadius(25); // Medium-sized circle
        playerAvatar.setFillColor(sf::Color(100, 150, 200)); // Light blue default
        playerAvatar.setPosition(position.x + 10, position.y + 10); // Top-left positioning
        
        // Role icon for quick role identification
        roleIcon.setSize(sf::Vector2f(30, 30)); // Square icon shape
        roleIcon.setPosition(position.x + size.x - 40, position.y + 10); // Top-right corner
        roleIcon.setFillColor(sf::Color(200, 100, 100)); // Default red color
        roleIcon.setOutlineThickness(2); // Clear definition
        roleIcon.setOutlineColor(sf::Color::Black); // Black border for contrast
        
        // Setup text elements for player information display
        nameText.setPosition(position.x + 70, position.y + 15); // Right of avatar
        nameText.setCharacterSize(20); // Large readable text
        nameText.setFillColor(sf::Color::White); // High contrast
        nameText.setStyle(sf::Text::Bold); // Emphasis on player name
        
        roleText.setPosition(position.x + 70, position.y + 40); // Below name
        roleText.setCharacterSize(14); // Medium size for role info
        roleText.setFillColor(sf::Color::Cyan); // Cyan for role distinction
        
        coinsText.setPosition(position.x + 15, position.y + 70); // Below avatar
        coinsText.setCharacterSize(16); // Clear size for important info
        coinsText.setFillColor(sf::Color::Yellow); // Yellow for coins (gold theme)
        coinsText.setStyle(sf::Text::Bold); // Emphasis on coin count
        
        statusText.setPosition(position.x + 15, position.y + 95); // Bottom of card
        statusText.setCharacterSize(12); // Smaller for status info
        statusText.setFillColor(sf::Color::Green); // Green for positive status
        
        // Decorative coin icon for visual enhancement
        coinIcon.setSize(sf::Vector2f(15, 15)); // Small square icon
        coinIcon.setFillColor(sf::Color(255, 215, 0)); // Gold color matching game theme
        coinIcon.setPosition(position.x + 90, position.y + 75); // Next to coin text
        coinIcon.setOutlineThickness(1); // Subtle border
        coinIcon.setOutlineColor(sf::Color(139, 69, 19)); // Brown outline for realistic gold appearance
    }

    void PlayerCard::updateInfo(const Player* player, bool current, RoleType playerRole) {
        isCurrentPlayer = current; // Mark if this is the active player's turn
        isActive = player->isActive(); // Update elimination status
        role = playerRole; // Store role for visual customization
        
        // Update player information displays
        nameText.setString(player->getName()); // Display player name
        coinsText.setString("Coins: " + std::to_string(player->coins())); // Show current coin count
        
        // Convert role enum to readable string for display
        roleText.setString("Role: " + std::string(
                        role == RoleType::GOVERNOR ? "Governor" :
                        role == RoleType::SPY ? "Spy" :
                        role == RoleType::BARON ? "Baron" :
                        role == RoleType::GENERAL ? "General" :
                        role == RoleType::JUDGE ? "Judge" :
                        role == RoleType::MERCHANT ? "Merchant" : "Unassigned"));
        
        // Determine and display current player status with appropriate coloring
        std::string status = "";
        if (!isActive) {
            status = "ELIMINATED"; // Player has been eliminated from game
            statusText.setFillColor(sf::Color::Red); // Red for elimination
        } else if (player->isSanctioned()) {
            status = "SANCTIONED"; // Player is currently sanctioned
            statusText.setFillColor(sf::Color(255, 165, 0)); // Orange for sanction warning
        } else {
            status = "ACTIVE"; // Player is active and participating
            statusText.setFillColor(sf::Color::Green); // Green for active participation
        }
        statusText.setString(status); // Apply status text
        
        // Apply visual styling based on player state
        if (isCurrentPlayer) {
            // Highlight current player with premium styling
            background.setFillColor(sf::Color(80, 40, 120)); // Rich purple background
            background.setOutlineColor(sf::Color(255, 215, 0)); // Gold border for current player
            background.setOutlineThickness(4); // Thicker border for emphasis
            playerAvatar.setFillColor(sf::Color(255, 215, 0)); // Gold avatar for current player
        } else if (!isActive) {
            // Dim styling for eliminated players
            background.setFillColor(sf::Color(30, 30, 30)); // Very dark background
            background.setOutlineColor(sf::Color::Red); // Red border indicating elimination
            playerAvatar.setFillColor(sf::Color(100, 100, 100)); // Gray avatar for eliminated
        } else {
            // Standard styling for active non-current players
            background.setFillColor(sf::Color(40, 40, 60)); // Normal blue-gray background
            background.setOutlineColor(sf::Color::White); // White border for active players
            playerAvatar.setFillColor(sf::Color(100, 150, 200)); // Blue avatar for active players
        }
        
        // Apply role-specific colors and visual customization
        sf::Color roleColor = sf::Color(200, 100, 100); // Default red color
        switch (role) {
            case RoleType::GOVERNOR: 
                roleColor = sf::Color(200, 150, 50); // Gold-like color representing wealth and authority
                break;
            case RoleType::SPY: 
                roleColor = sf::Color(100, 100, 200); // Blue color representing stealth and intelligence
                break;
            case RoleType::BARON: 
                roleColor = sf::Color(150, 100, 200); // Purple color representing nobility and commerce
                break;
            case RoleType::GENERAL: 
                roleColor = sf::Color(200, 100, 100); // Red color representing military and combat
                break;
            case RoleType::JUDGE: 
                roleColor = sf::Color(150, 150, 150); // Silver/gray color representing justice and law
                break;
            case RoleType::MERCHANT: 
                roleColor = sf::Color(100, 200, 100); // Green color representing trade and economy
                break;
            default: 
                roleColor = sf::Color(120, 120, 120); // Default gray for unassigned roles
                break;
        }
        roleIcon.setFillColor(roleColor); // Apply role-specific coloring
        
        // Customize icon shape based on role for additional visual distinction
        switch (role) {
            case RoleType::GOVERNOR:
                roleIcon.setSize(sf::Vector2f(30, 20)); // Crown-like rectangular shape
                break;
            case RoleType::SPY:
                roleIcon.setSize(sf::Vector2f(25, 25)); // More square shape for espionage
                break;
            case RoleType::BARON:
                roleIcon.setSize(sf::Vector2f(35, 25)); // Wider rectangle for commercial influence
                break;
            case RoleType::GENERAL:
                roleIcon.setSize(sf::Vector2f(30, 30)); // Standard square for military discipline
                break;
            case RoleType::JUDGE:
                roleIcon.setSize(sf::Vector2f(35, 20)); // Wider, flatter shape resembling scales of justice
                break;
            case RoleType::MERCHANT:
                roleIcon.setSize(sf::Vector2f(25, 30)); // Taller rectangle for trade goods
                break;
            default:
                roleIcon.setSize(sf::Vector2f(30, 30)); // Default square for unknown roles
                break;
        }
    }

    void PlayerCard::draw(sf::RenderWindow& window, bool showDeleteButton) const {
        window.draw(background); // Draw card background first
        window.draw(playerAvatar); // Draw player avatar circle
        window.draw(roleIcon); // Draw role identification icon
        window.draw(nameText); // Draw player name
        window.draw(roleText); // Draw role information
        window.draw(coinsText); // Draw coin count
        window.draw(statusText); // Draw current status
        // window.draw(coinIcon); // Optional coin icon (currently disabled)
        
        // Only show delete button during player setup phase
        if (showDeleteButton) {
            deleteButton.draw(window); // Draw delete button for player removal
        }
    }

    void PlayerCard::setFont(const sf::Font& font) {
        // Apply font to all text elements for consistent typography
        nameText.setFont(font); // Player name font
        roleText.setFont(font); // Role description font
        coinsText.setFont(font); // Coin count font
        statusText.setFont(font); // Status indicator font
        deleteButton.setFont(font); // Delete button font
    }

    // InputField Implementation - Text input widget with cursor and placeholder support
    
    InputField::InputField(sf::Vector2f position, sf::Vector2f size, const std::string& placeholderText)
        : active(false), showCursor(false) {
        
        // Setup input field background with professional styling
        background.setPosition(position); // Field position on screen
        background.setSize(size); // Field dimensions
        background.setFillColor(sf::Color(50, 50, 50)); // Dark background for input
        background.setOutlineThickness(2); // Clear border definition
        background.setOutlineColor(sf::Color::White); // White border for visibility
        
        // Setup user input text display
        displayText.setPosition(position.x + 10, position.y + 10); // Padded text position
        displayText.setCharacterSize(18); // Readable text size
        displayText.setFillColor(sf::Color::White); // High contrast text color
        
        // Setup placeholder text for user guidance
        placeholder.setString(placeholderText); // Instructional text
        placeholder.setPosition(position.x + 10, position.y + 10); // Same position as input text
        placeholder.setCharacterSize(18); // Matching text size
        placeholder.setFillColor(sf::Color(150, 150, 150)); // Dimmed color to differentiate from input
    }

    void InputField::handleInput(sf::Uint32 unicode) {
        if (!active) return; // Only process input when field is active
        
        if (unicode == 8) { // Backspace key pressed
            if (!content.empty()) {
                content.pop_back(); // Remove last character
                displayText.setString(content); // Update display
            }
        } else if (unicode >= 32 && unicode < 127) { // Printable ASCII characters only
            if (content.length() < 20) { // Enforce reasonable character limit
                content += static_cast<char>(unicode); // Add new character
                displayText.setString(content); // Update display with new content
            }
        }
    }

    void InputField::setActive(bool isActive) {
        active = isActive; // Update field state
        // Visual feedback for active/inactive state
        background.setOutlineColor(active ? sf::Color::Yellow : sf::Color::White); // Yellow when active
        background.setOutlineThickness(active ? 3 : 2); // Thicker border when active
    }

    bool InputField::contains(sf::Vector2f point) const {
        return background.getGlobalBounds().contains(point); // Check if point is within field bounds
    }

    void InputField::update() {
        // Animate blinking cursor when field is active
        if (active && cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor; // Toggle cursor visibility
            cursorClock.restart(); // Reset cursor timer
            
            std::string displayStr = content; // Start with current content
            if (showCursor) displayStr += "|"; // Add cursor character when visible
            displayText.setString(displayStr); // Update display with cursor
        }
    }

    void InputField::draw(sf::RenderWindow& window) const {
        window.draw(background); // Draw input field background
        if (content.empty() && !active) {
            window.draw(placeholder); // Show placeholder when empty and inactive
        } else {
            window.draw(displayText); // Show actual content or content with cursor
        }
    }

    void InputField::setFont(const sf::Font& font) {
        displayText.setFont(font); // Apply font to input text
        placeholder.setFont(font); // Apply font to placeholder text
    }

    // GameGUI Implementation - Main graphical interface controller for Coup card game
    
    GameGUI::GameGUI() 
        : game(nullptr), // Initialize game pointer to null until game creation
        currentState(GameState::MAIN_MENU), // Start in main menu state
        waitingForTarget(false), // Not waiting for target selection initially
        waitingForRole(false), // Not waiting for role selection initially
        waitingForReactivePlayer(false), // Not waiting for reactive player selection initially
        selectedTarget(nullptr), // No target selected initially
        pendingReactiveTarget(nullptr), // No reactive target pending initially
        // Initialize input field for player name entry
        playerNameInput(sf::Vector2f(500, 400), sf::Vector2f(300, 40), "Enter player name..."),
        // Initialize buttons with positions, sizes, labels, and actions
        addPlayerButton(sf::Vector2f(820, 400), sf::Vector2f(150, 40), "Add Player", "add_player"),
        startGameButton(sf::Vector2f(600, 500), sf::Vector2f(200, 50), "Start Game", "start_game"),
        backButton(sf::Vector2f(50, 50), sf::Vector2f(100, 40), "Back", "back"),
        returnToMenuButton(sf::Vector2f(600, 600), sf::Vector2f(200, 50), "Return to Menu", "menu", sf::Color(70, 130, 180)) {
        
        // Create main game window with specific dimensions and styling
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "COUP", sf::Style::Titlebar | sf::Style::Close);
        
        // Ignore VSync entirely
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(60); // Limit to 60 FPS for smooth performance
        
        // Setup comprehensive color theme for consistent visual design
        theme.background = sf::Color(15, 15, 25); // Dark navy background
        theme.primary = sf::Color(70, 130, 180); // Steel blue for primary elements
        theme.secondary = sf::Color(100, 149, 237); // Cornflower blue for secondary elements
        theme.accent = sf::Color(255, 215, 0); // Gold for accent and highlights
        theme.text = sf::Color::White; // White for primary text
        theme.textSecondary = sf::Color(200, 200, 200); // Light gray for secondary text
        theme.success = sf::Color(50, 205, 50); // Lime green for success messages
        theme.error = sf::Color(220, 20, 60); // Crimson for error messages
        theme.warning = sf::Color(255, 165, 0); // Orange for warning messages
        
        // Setup modal selection overlay for target selection screens
        selectionOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)); // Full screen overlay
        selectionOverlay.setFillColor(sf::Color(0, 0, 0, 180)); // Semi-transparent black backdrop
        selectionOverlay.setPosition(0, 0); // Cover entire window
        
        // Setup title text for selection dialogs
        selectionTitle.setCharacterSize(32); // Large readable text
        selectionTitle.setFillColor(theme.accent); // Gold color for prominence
        selectionTitle.setStyle(sf::Text::Bold); // Bold for emphasis
        selectionTitle.setPosition(getCenterPosition(sf::Vector2f(400, 40))); // Center horizontally
        selectionTitle.move(0, -150); // Move up from center
    }

    GameGUI::~GameGUI() {
        if (game) delete game; // Clean up game instance on destruction
    }

    bool GameGUI::initialize() {
        // Load fonts for consistent typography throughout the application
        if (!mainFont.loadFromFile("resources/tahoma.ttf")) {
            std::cout << "Error: Could not load Tahoma font. The game UI may not display properly." << std::endl;
        }
        
        titleFont = mainFont; // Use same font for title (can be customized later)
        
        // Initialize message display system for user feedback
        messageText.setFont(mainFont); // Apply main font
        messageText.setCharacterSize(18); // Readable size for messages
        messageText.setFillColor(theme.success); // Default to success color
        messageText.setPosition(50, WINDOW_HEIGHT - 40); // Bottom-left positioning
        
        // Setup all UI components for initial display
        setupMainMenu(); // Configure main menu interface
        createDecorativeElements(); // Add visual enhancements
        
        return true; // Initialization successful
    }

    void GameGUI::setupMainMenu() {
        // Configure main title display with prominent styling
        titleText.setFont(titleFont); // Apply title font
        titleText.setString("COUP"); // Game title
        titleText.setCharacterSize(72); // Large, eye-catching size
        titleText.setFillColor(theme.accent); // Gold color for premium feel
        titleText.setStyle(sf::Text::Bold); // Bold for maximum impact
        titleText.setPosition(getCenterPosition(sf::Vector2f(200, 80))); // Center horizontally
        titleText.move(0, -200); // Move up from center for title placement
        
        // Configure subtitle with creator attribution
        subtitleText.setFont(mainFont); // Apply main font
        subtitleText.setString("Created by Raz Cohen"); // Attribution text
        subtitleText.setCharacterSize(24); // Medium size for subtitle
        subtitleText.setFillColor(theme.textSecondary); // Secondary color for less emphasis
        // Center the subtitle text precisely
        subtitleText.setPosition(getCenterPosition(sf::Vector2f(subtitleText.getLocalBounds().width, subtitleText.getLocalBounds().height)));
        subtitleText.move(0, -120); // Position below title
        
        createMenuButtons(); // Setup interactive menu buttons
    }

    void GameGUI::createMenuButtons() {
        menuButtons.clear(); // Clear any existing buttons
        
        // Define main menu button configuration
        std::vector<std::pair<std::string, std::string>> buttons = {
            {"New Game", "new_game"}, // Start new game button
            {"Exit", "exit"} // Exit application button
        };
        
        // Calculate centered starting position for button layout
        sf::Vector2f startPos = getCenterPosition(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        startPos.y += 50; // Offset below title area
        
        // Create and configure each menu button
        for (size_t i = 0; i < buttons.size(); ++i) {
            sf::Vector2f pos = sf::Vector2f(startPos.x, startPos.y + i * 80); // Vertical spacing
            menuButtons.emplace_back(pos, sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), 
                                    buttons[i].first, buttons[i].second, theme.primary); // Create button with theme
            menuButtons.back().setFont(mainFont); // Apply font to new button
        }
    }

    void GameGUI::setupPlayerSetup() {
        // Configure all input elements for player setup phase
        playerNameInput.setFont(mainFont); // Apply font to input field
        addPlayerButton.setFont(mainFont); // Apply font to add button
        startGameButton.setFont(mainFont); // Apply font to start button
        backButton.setFont(mainFont); // Apply font to back button
        
        // Initially disable start game button until minimum players are added
        startGameButton.setEnabled(false);
        
        // Setup instructional text for user guidance
        instructionText.setFont(mainFont); // Apply main font
        instructionText.setString("Add 2-6 players to start the game"); // Clear instructions
        instructionText.setCharacterSize(24); // Large readable size
        instructionText.setFillColor(theme.text); // Primary text color
        instructionText.setPosition(getCenterPosition(sf::Vector2f(400, 150))); // Center horizontally
        instructionText.move(0, 20); // Slight downward offset
        
        createPlayerCards(); // Initialize player card display
    }

    void GameGUI::createPlayerCards() {
        // Clear existing player cards to rebuild from current game state
        playerCards.clear();
        
        // Safety check to ensure game object exists before accessing players
        if (!game) return;
        
        // Retrieve all players from the current game session
        std::vector<Player*> allPlayers = game->getAllPlayers();
        
        // Configure card layout parameters for organized display
        int playersPerRow = 3; // Maximum players per horizontal row
        sf::Vector2f cardSize(CARD_WIDTH, CARD_HEIGHT - 10); // Slightly reduced height for better spacing
        sf::Vector2f spacing(20, 15); // Horizontal and vertical spacing between cards
        // Position cards within the player panel area
        sf::Vector2f startPos(570, 70); // Top-left starting position for card grid
        
        // Create individual player cards in grid layout
        for (size_t i = 0; i < allPlayers.size(); ++i) {
            int row = i / playersPerRow; // Calculate which row this card belongs to
            int col = i % playersPerRow; // Calculate column position within the row
            
            // Calculate exact position for this specific card
            sf::Vector2f pos(
                startPos.x + col * (cardSize.x + spacing.x), // Horizontal position with spacing
                startPos.y + row * (cardSize.y + spacing.y)  // Vertical position with row offset
            );
            
            // Create new player card at calculated position
            playerCards.emplace_back(pos, cardSize);
            playerCards.back().setFont(mainFont); // Apply consistent font styling
            
            // Use the Player's getRoleType method for proper role detection
            RoleType displayRole = convertRoleType(allPlayers[i]->getRoleType()); // Convert role for display
            playerCards.back().updateInfo(allPlayers[i], false, displayRole); // Update card with player information
        }
    }

    void GameGUI::createActionButtons() {
        // Clear existing action buttons to rebuild from current game state
        actionButtons.clear();
        
        // Define all basic actions available to every player in the game
        std::vector<std::pair<std::string, std::string>> basicActions = {
            {"Gather", "gather"}, // Collect 1 coin safely
            {"Tax", "tax"}, // Collect 3 coins (can be undone by Governor)
            {"Bribe", "bribe"}, // Pay 2 coins to bribe player (can be blocked by Judge)
            {"Arrest", "arrest"}, // Pay 7 coins to arrest player (can be blocked by Spy)
            {"Sanction", "sanction"}, // Pay 3 coins to steal 2 coins from player
            {"Coup", "coup"} // Pay 5 coins to eliminate player (can be blocked by General)
        };
        
        // Configure button layout parameters for the action panel
        sf::Vector2f startPos(70, 250); // Top-left starting position for action buttons
        sf::Vector2f buttonSize(200, 40); // Standard button dimensions
        int spacing = 48; // Vertical spacing between buttons
        
        // Create and position each basic action button
        for (size_t i = 0; i < basicActions.size(); ++i) {
            sf::Vector2f pos(startPos.x, startPos.y + i * spacing); // Calculate vertical position
            actionButtons.emplace_back(pos, buttonSize, 
                                    basicActions[i].first, basicActions[i].second, theme.primary); // Create button with display text, action, and theme
            actionButtons.back().setFont(mainFont); // Apply consistent font styling
        }
        
        // Role-specific buttons will be added dynamically based on current player's role
    }

        void GameGUI::addReactiveAbilityButtons() {
        // Safety check to ensure game object exists before accessing players
        if (!game) return;
        
        // Retrieve all players to analyze available reactive abilities
        std::vector<Player*> allPlayers = game->getAllPlayers();
        sf::Vector2f buttonSize(220, 40); // Slightly wider buttons for reactive ability text
        sf::Color reactiveColor = sf::Color(255, 140, 0); // Orange color to distinguish reactive abilities
        
        // Position reactive buttons in separate column from basic actions
        sf::Vector2f reactiveStartPos(290, 250); // Right column positioning to match action buttons
        int buttonIndex = 0; // Track vertical position for button placement
        int spacing = 48; // Consistent spacing to align with action buttons
        
        // Track availability of each reactive ability type to avoid duplicates
        bool hasActiveGovernor = false; // Governor can undo tax actions
        bool hasPlayerUsedTaxLastAction = false; // Tax was used in previous turn
        bool hasActiveSpy = false; // Spy can perform surveillance
        bool hasActiveGeneral = false; // General can block coup attempts
        bool hasActiveJudge = false; // Judge can block bribe attempts

        // Scan all players to determine which reactive abilities are currently available
        for (Player* player : allPlayers) {
            RoleType playerRole = convertRoleType(player->getRoleType()); // Get standardized role type
            
            // Check each role type for specific reactive ability conditions
            switch (playerRole) {
                case RoleType::GOVERNOR:
                    if (player->isActive()) { // Governor must be alive to use abilities
                        hasActiveGovernor = true;
                    }
                    break;
                case RoleType::SPY:
                    if (player->isActive()) { // Spy must be alive to use surveillance
                        hasActiveSpy = true;
                    }
                    break;
                case RoleType::GENERAL:
                    if (player->coins() >= 5) { // General needs 5+ coins to block coup attempts
                        hasActiveGeneral = true;
                    }
                    break;
                case RoleType::JUDGE:
                    if (player->isActive()) { // Judge must be alive to block bribes
                        hasActiveJudge = true;
                    }
                    break;
                break;
                default:
                    break; // Other roles don't have reactive abilities
            }

            // Check if any player used tax as their most recent action (for Governor undo)
            if (player->usedTaxLastAction()) {
                hasPlayerUsedTaxLastAction = true;
            }
        }
        
        // Add reactive ability buttons based on current game state conditions
        if (hasActiveGovernor && hasPlayerUsedTaxLastAction) {
            // Governor can undo tax action if someone used tax recently
            sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
            actionButtons.emplace_back(pos, buttonSize, "Undo Tax", "undo", reactiveColor);
            actionButtons.back().setFont(mainFont); // Apply consistent font styling
            buttonIndex++; // Move to next button position
        }
        if (hasActiveSpy) {
            // Spy can perform surveillance on other players
            sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
            actionButtons.emplace_back(pos, buttonSize, "Spy On", "spy_on", reactiveColor);
            actionButtons.back().setFont(mainFont); // Apply consistent font styling
            buttonIndex++; // Move to next button position
        }
        
        if (hasActiveGeneral) {
            // General can block coup attempts if they have sufficient coins
            sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
            actionButtons.emplace_back(pos, buttonSize, "Block Coup", "block_coup", reactiveColor);
            actionButtons.back().setFont(mainFont); // Apply consistent font styling
            buttonIndex++; // Move to next button position
        }
        
        if (hasActiveJudge) {
            // Judge can block bribe attempts to protect players
            sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
            actionButtons.emplace_back(pos, buttonSize, "Block Bribe", "block_bribe", reactiveColor);
            actionButtons.back().setFont(mainFont); // Apply consistent font styling
            buttonIndex++; // Move to next button position
        }

        // Add role-specific special abilities for the current player
        if (game->getCurrentPlayer()) {
            Player* currentPlayer = game->getCurrentPlayer(); // Get the player whose turn it is
            RoleType role = convertRoleType(currentPlayer->getRoleType()); // Get their role type
            sf::Color roleColor = sf::Color(255, 215, 0); // Gold color for special role abilities
            
            // Add Baron's investment ability if current player is Baron
            if (role == RoleType::BARON) {
                sf::Vector2f pos(reactiveStartPos.x, reactiveStartPos.y + buttonIndex * spacing);
                actionButtons.emplace_back(pos, buttonSize, 
                                        "Invest", "invest", roleColor); // Baron can invest coins for future benefit
                actionButtons.back().setFont(mainFont); // Apply consistent font styling
                buttonIndex++; // Move to next button position
            }
        }
    }

    void GameGUI::run() {
        // Main application loop - continues until window is closed
        while (window.isOpen()) {
            handleEvents(); // Process all user input and system events
            update(); // Update game state and animations
            render(); // Draw current frame to window
        }
    }

    void GameGUI::handleEvents() {
        sf::Event event; // Event object to store incoming events
        
        // Process all events in the event queue
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // User clicked window close button
            }
            
            // Handle mouse button press events
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // Get click position
                    handleMouseClick(mousePos); // Process left mouse button clicks
                }
            }
            
            // Handle mouse movement for hover effects
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y); // Get mouse position
                handleMouseMove(mousePos); // Update hover states
            }
            
            // Handle keyboard key presses
            if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code); // Process key press events
            }
            
            // Handle text input for input fields
            if (event.type == sf::Event::TextEntered) {
                handleTextInput(event.text.unicode); // Process character input
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
                            
                            if (currentAction == "spy_on" || currentAction == "block_bribe" || currentAction == "undo") {
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
        std::string name = playerNameInput.getText(); // Get entered player name
        if (name.empty()) {
            updateMessage("Please enter a player name!", true); // Validation: require non-empty name
            return;
        }

        if (name.length() >= 10) {
            updateMessage("Player name too long! (Max: 9 characters)", true); // Validation: enforce length limit
            return;
        }
        
        if (!game) {
            game = new Game(); // Create new game instance if none exists
        }
        
        // Validation: check for maximum player count and duplicate names
        std::vector<Player*> allPlayers = game->getAllPlayers(); // Get current player list
        if (allPlayers.size() >= 6) {
            updateMessage("Maximum 6 players allowed!", true); // Enforce game limit
            return;
        }
        
        // Check for duplicate player names to ensure uniqueness
        for (const auto& player : allPlayers) {
            if (player->getName() == name) {
                updateMessage("Player name already exists!", true); // Prevent name conflicts
                return;
            }
        }
        
        try {
            // Randomly assign a role to the new player for game variety
            std::vector<RoleType> roles = { RoleType::GOVERNOR, RoleType::SPY, RoleType::BARON, 
                                            RoleType::GENERAL, RoleType::JUDGE, RoleType::MERCHANT };
            RoleType assignedRole = roles[game->getRandomGenerator()() % roles.size()]; // Random role selection
            
            // Create and add player to game with assigned role
            game->createPlayerWithRole(name, assignedRole); // Player creation handles game integration
            
            playerNameInput.clear(); // Clear input field for next player
            createPlayerCards(); // Refresh player card display
            updateMessage("Player " + name + " added as " + game->getRoleName(assignedRole) + " successfully!", false);
            
            // Enable start button if minimum player requirement is met
            std::vector<Player*> updatedPlayers = game->getAllPlayers(); // Get refreshed player list
            startGameButton.setEnabled(updatedPlayers.size() >= 2); // Enable start if 2+ players
        } catch (const std::exception& e) {
            updateMessage("Error adding player: " + std::string(e.what()), true); // Handle creation errors
        }
    }

    void GameGUI::startNewGame() {
        // Validate game setup before starting
        if (!game || game->getAllPlayers().size() < 2) {
            updateMessage("Need at least 2 players to start!", true); // Enforce minimum player requirement
            return;
        }
        
        try {
            game->startGame(); // Initialize game with existing players
            changeState(GameState::PLAYING); // Transition to gameplay state
            updateMessage("Game started! " + game->getCurrentPlayer()->getName() + "'s turn"); // Announce first turn
        } catch (const std::exception& e) {
            updateMessage("Error starting game: " + std::string(e.what()), true); // Handle startup errors
        }
    }
    
    void GameGUI::executeAction(const std::string& action, Player* target) {
        Player* currentPlayer = game->getCurrentPlayer(); // Get currently active player
        if (!currentPlayer) {
            updateMessage("No current player!", true); // Error: no active player found
            return;
        }
        
        try {
            // Execute basic game actions available to all players
            if (action == "gather") {
                currentPlayer->gather(); // Collect 1 coin from treasury
                updateMessage(currentPlayer->getName() + " gathered 1 coin");
            }
            else if (action == "tax") {
                currentPlayer->tax(); // Collect coins based on role
                Governor* governor = dynamic_cast<Governor*>(currentPlayer); // Check if Governor
                if(governor) {
                    updateMessage(currentPlayer->getName() + " collected tax (3 coins for Governor)"); // Governor gets bonus
                } else {
                    updateMessage(currentPlayer->getName() + " collected tax (2 coins)"); // Standard tax amount
                }
            }
            else if (action == "bribe") {
                currentPlayer->bribe(); // Pay 4 coins for extra action
                updateMessage(currentPlayer->getName() + " paid bribe (4 coins) for extra action");
            }
            else if (action == "arrest" && target) {
                currentPlayer->arrest(*target); // Remove 1 coin from target
                if (target->coins() == 0) {
                    updateMessage(currentPlayer->getName() + " arrested " + target->getName() + ", though " + target->getName() + " had no coins", false, true);
                }
                else {
                    updateMessage(currentPlayer->getName() + " arrested " + target->getName());
                }
            }
            else if (action == "sanction" && target) {
                currentPlayer->sanction(*target); // Pay 3 coins to sanction target
                updateMessage(currentPlayer->getName() + " sanctioned (3 coins) " + target->getName());
            }
            else if (action == "coup" && target) {
                currentPlayer->coup(*target); // Pay 7 coins to eliminate target
                updateMessage(currentPlayer->getName() + " performed coup (7 coins) on " + target->getName());
            }
            
            // Execute role-specific special abilities
            else if (action == "invest") {
                Baron* baron = dynamic_cast<Baron*>(currentPlayer); // Verify Baron role
                if (baron) {
                    baron->invest(); // Baron special: pay 3 to get 6 coins
                    updateMessage(baron->getName() + " invested 3 coins to get 6 coins!");
                } else {
                    updateMessage("Only Barons can invest!", true); // Role restriction error
                }
            }
            else if (action == "undo" && target) {
                // Governor reactive ability: undo another player's tax action
                std::vector<Player*> eligiblePlayers = getEligibleReactivePlayers(action);
                
                if (eligiblePlayers.empty()) {
                    updateMessage("No active Governor available to undo tax!", true); // No eligible Governors
                } else if (eligiblePlayers.size() == 1) {
                    executeReactiveAction(action, eligiblePlayers[0], target); // Auto-select single Governor
                } else {
                    showReactivePlayerSelection(action, target, eligiblePlayers); // Multiple Governors: show selection
                }
            }
            else if (action == "spy_on" && target) {
                // Spy reactive ability: surveillance on target player
                std::vector<Player*> eligiblePlayers = getEligibleReactivePlayers(action);
                
                if (eligiblePlayers.empty()) {
                    updateMessage("No active Spy available to spy on players!", true); // No eligible Spies
                } else if (eligiblePlayers.size() == 1) {
                    executeReactiveAction(action, eligiblePlayers[0], target); // Auto-select single Spy
                } else {
                    updateMessage("Multiple Spies available - choose one", false);
                    showReactivePlayerSelection(action, target, eligiblePlayers); // Multiple Spies: show selection
                    return; // Prevent further execution until selection is made
                }
            }
            else if (action == "block_coup" && target) {
                // General reactive ability: block coup attempt by reviving eliminated player
                std::vector<Player*> eligiblePlayers = getEligibleReactivePlayers(action);
                
                if (eligiblePlayers.empty()) {
                    updateMessage("No active General with 5+ coins available to block coup!", true); // No eligible Generals
                } else if (eligiblePlayers.size() == 1) {
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
            
            updatePlayerCards(); // Refresh player card displays with current state
            updateGameInfo(); // Update game information display
            
            // Recreate action buttons to update role-specific ones
            createActionButtons(); // Recreate action buttons to reflect current game state
            
            // Add reactive ability buttons for all players
            addReactiveAbilityButtons(); // Enable role-specific reactive abilities
            
            // Check for game over condition after action completion
            try {
                std::string winner = game->winner(); // Attempt to determine game winner
                
                // Before declaring game over, check if ANY General can block coup to prevent game end
                std::vector<Player*> allPlayers = game->getAllPlayers(); // Get all players for General check
                bool hasGeneralWithCoins = false; // Track if General can afford to block coup
                
                // Search for active General with sufficient coins to block coup
                for (Player* player : allPlayers) {
                    if (player->getRoleType() == "General" && player->coins() >= 5) { // General needs 5+ coins to block
                        hasGeneralWithCoins = true; // Found eligible General
                        break; // Only need to find one eligible General
                    }
                }
                
                if (hasGeneralWithCoins) {
                    // Show General block coup decision dialog instead of ending game
                    showGeneralBlockCoupDecision(); // Let General decide whether to block game end
                } else {
                    updateMessage("Game Over! Winner: " + winner); // Announce game completion
                    changeState(GameState::GAME_OVER); // Transition to game over screen
                }
            } catch (const std::exception&) {
                // Game continues - no winner determined yet
            }
            
        } catch (const std::exception& e) {
            updateMessage("Error: " + std::string(e.what()), true); // Display error message
        }
    }

    void GameGUI::changeState(GameState newState) {
        currentState = newState; // Update current interface state
        
        // Configure interface based on new state
        switch (newState) {
            case GameState::MAIN_MENU:
                setupMainMenu(); // Initialize main menu interface
                break;
            case GameState::SETUP_PLAYERS:
                setupPlayerSetup(); // Initialize player creation interface
                break;
            case GameState::PLAYING:
                setupGameScreen(); // Initialize gameplay interface
                break;
            case GameState::GAME_OVER:
                setupGameOver(); // Initialize game completion interface
                break;
        }
    }

    void GameGUI::setupGameScreen() {
        createActionButtons(); // Initialize action button interface
        updatePlayerCards(); // Refresh player information display
        
        // Setup comprehensive color theme for consistent visual design
        setupGamePanels(); // Configure layout panels for organized interface
        
        addReactiveAbilityButtons(); // Add role-specific reactive ability buttons
        
        // Configure game information text display
        gameInfoText.setFont(mainFont); // Apply main font
        gameInfoText.setCharacterSize(18); // Readable text size
        gameInfoText.setFillColor(theme.text); // Standard text color
        gameInfoText.setPosition(70, 110); // Position in game info panel
        
        // Configure current player indicator
        currentPlayerText.setFont(mainFont); // Apply main font
        currentPlayerText.setCharacterSize(24); // Prominent size for current player
        currentPlayerText.setFillColor(theme.accent); // Gold color for emphasis
        currentPlayerText.setStyle(sf::Text::Bold); // Bold for maximum visibility
        currentPlayerText.setPosition(70, 70); // Top of game info panel
        
        // Configure game status information display
        gameStatusText.setFont(mainFont); // Apply main font
        gameStatusText.setCharacterSize(16); // Smaller size for status info
        gameStatusText.setFillColor(theme.textSecondary); // Secondary color for less emphasis
        gameStatusText.setPosition(70, 100); // Below current player info
        
        // Configure action feedback display for user responses
        actionFeedbackText.setFont(mainFont); // Apply main font
        actionFeedbackText.setCharacterSize(18); // Standard feedback size
        actionFeedbackText.setFillColor(theme.success); // Success color by default
        actionFeedbackText.setPosition(480, 600); // Bottom area for feedback
        
        // Configure section headers for organized interface layout
        actionSectionHeader.setFont(mainFont); // Apply main font
        actionSectionHeader.setString("ACTIONS"); // Section title
        actionSectionHeader.setCharacterSize(20); // Header size
        actionSectionHeader.setFillColor(theme.accent); // Gold accent color
        actionSectionHeader.setStyle(sf::Text::Bold); // Bold for section headers
        actionSectionHeader.setPosition(70, 225); // Top of action panel
        
        playerSectionHeader.setFont(mainFont); // Apply main font
        playerSectionHeader.setString("PLAYERS"); // Section title
        playerSectionHeader.setCharacterSize(20); // Header size
        playerSectionHeader.setFillColor(theme.accent); // Gold accent color
        playerSectionHeader.setStyle(sf::Text::Bold); // Bold for section headers
        playerSectionHeader.setPosition(570, 45); // Top of player panel
        
        gameInfoHeader.setFont(mainFont); // Apply main font
        gameInfoHeader.setString("GAME STATUS"); // Section title
        gameInfoHeader.setCharacterSize(20); // Header size
        gameInfoHeader.setFillColor(theme.accent); // Gold accent color
        gameInfoHeader.setStyle(sf::Text::Bold); // Bold for section headers
        gameInfoHeader.setPosition(70, 45); // Top of game info panel
    }

    void GameGUI::setupGameOver() {
        // Configure game over screen with winner announcement
        gameOverTitle.setFont(titleFont); // Apply title font
        gameOverTitle.setString("GAME OVER"); // Game completion title
        gameOverTitle.setCharacterSize(72); // Large impactful size
        gameOverTitle.setFillColor(theme.accent); // Gold color for prominence
        gameOverTitle.setStyle(sf::Text::Bold); // Bold for maximum impact
        gameOverTitle.setPosition(getCenterPosition(sf::Vector2f(400, 80))); // Center horizontally
        gameOverTitle.move(0, -120); // Move up for title positioning
        
        // Configure winner announcement text
        winnerText.setFont(mainFont); // Apply main font
        winnerText.setCharacterSize(36); // Large size for winner announcement
        winnerText.setFillColor(theme.success); // Success/celebration color
        winnerText.setPosition(getCenterPosition(sf::Vector2f(600, 40))); // Center positioning
        winnerText.move(180, -40); // Adjust for optimal placement

        // Create and configure return to menu button
        sf::Vector2f buttonPos = getCenterPosition(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT)); // Center button
        buttonPos.y += 100; // Move down from center
        returnToMenuButton = EnhancedButton(
            buttonPos, // Button position
            sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), // Button size
            "Return to Menu", // Button label
            "menu", // Button action
            theme.primary // Button color theme
        );
        returnToMenuButton.setFont(mainFont); // Apply font to button
        
        // Determine and display winner information
        try {
            std::string winner = game->winner(); // Get game winner
            winnerText.setString("Winner: " + winner); // Display winner name
        } catch (const std::exception&) {
            winnerText.setString("No winner determined"); // Handle edge cases
        }
    }

    void GameGUI::setupGamePanels() {
        // Game information panel (left side) - displays current game state and player turn info
        gameInfoPanel.setSize(sf::Vector2f(480, 160)); // Adequate size for game info display
        gameInfoPanel.setPosition(50, 40); // Top-left positioning with margin
        gameInfoPanel.setFillColor(sf::Color(30, 30, 45, 200)); // Semi-transparent dark blue background
        gameInfoPanel.setOutlineThickness(2); // Clear panel definition
        gameInfoPanel.setOutlineColor(theme.primary); // Steel blue border matching theme
        
        // Action panel (left side, below game info) - contains all action buttons and commands
        actionPanel.setSize(sf::Vector2f(480, 340)); // Large area for action buttons
        actionPanel.setPosition(50, 220); // Below game info panel with spacing
        actionPanel.setFillColor(sf::Color(30, 30, 45, 200)); // Matching semi-transparent background
        actionPanel.setOutlineThickness(2); // Clear panel definition
        actionPanel.setOutlineColor(theme.secondary); // Cornflower blue border for distinction
        
        // Player panel (right side) - displays all player cards and their current states
        playerPanel.setSize(sf::Vector2f(800, 520)); // Large area for multiple player cards
        playerPanel.setPosition(550, 40); // Right side positioning with margin
        playerPanel.setFillColor(sf::Color(25, 25, 40, 180)); // Slightly different background for variety
        playerPanel.setOutlineThickness(2); // Clear panel definition
        playerPanel.setOutlineColor(theme.accent); // Gold border for emphasis on player area
        
        // Feedback panel (bottom) - displays game messages and action results
        feedbackPanel.setSize(sf::Vector2f(1300, 60)); // Wide panel across bottom of screen
        feedbackPanel.setPosition(50, 580); // Bottom positioning with margin
        feedbackPanel.setFillColor(sf::Color(40, 40, 55, 220)); // Slightly brighter for message visibility
        feedbackPanel.setOutlineThickness(2); // Clear panel definition
        feedbackPanel.setOutlineColor(theme.text); // White border for clear message area
    }

    void GameGUI::updateGameInfo() {
        // Validate game state before updating display information
        if (!game || !game->isGameStarted()) {
            currentPlayerText.setString("Game not started"); // Display initialization message
            gameStatusText.setString(""); // Clear status info when game not active
            return;
        }
        
        Player* currentPlayer = game->getCurrentPlayer(); // Get currently active player
        if (!currentPlayer) {
            currentPlayerText.setString("No current player"); // Error state display
            gameStatusText.setString(""); // Clear status for error state
            return;
        }
        
        // Update current player display with contextual information
        std::string playerInfo = "Current Turn: " + currentPlayer->getName(); // Base player info
        if (currentPlayer->coins() >= 10) {
            playerInfo += " (MUST COUP!)"; // Force coup action when player has 10+ coins
            currentPlayerText.setFillColor(theme.error); // Red color for mandatory action
        } else {
            currentPlayerText.setFillColor(theme.accent); // Gold color for normal state
        }
        currentPlayerText.setString(playerInfo); // Apply updated player information
        
        // Update game status with comprehensive information display
        std::vector<Player*> activePlayers = getActivePlayers(); // Get all active participants
        std::string statusInfo = "Active Players: " + std::to_string(activePlayers.size()); // Show remaining players
        statusInfo += "\nCoins: " + std::to_string(currentPlayer->coins()); // Display current player wealth
        
        // Add status effects affecting current player
        if (currentPlayer->isSanctioned()) {
            statusInfo += "\nSTATUS: Sanctioned (no gather/tax)"; // Sanction restricts income actions
        }
        if (!currentPlayer->isArrestAvailable()) {
            statusInfo += "\nSTATUS: Arrest blocked"; // Spy protection from arrest
        }
        if (currentPlayer->isBribeUsed()) {
            statusInfo += "\nSTATUS: Extra action available"; // Bribe enables additional turn
        }
        
        gameStatusText.setString(statusInfo); // Update status display with all information
    }

    void GameGUI::updateActionAvailability() {
        // Validate game state before updating button availability
        if (!game || !game->isGameStarted()) {
            for (auto& button : actionButtons) {
                button.setEnabled(false); // Disable all buttons when game not started
            }
            return;
        }
        
        Player* currentPlayer = game->getCurrentPlayer(); // Get currently active player
        if (!currentPlayer) {
            for (auto& button : actionButtons) {
                button.setEnabled(false); // Disable all buttons when no current player
            }
            return;
        }
        
        // Update each action button based on availability using game state variables
        for (auto& button : actionButtons) {
            bool available = false; // Default to disabled until validated
            
            if (button.action == "gather") {
                // Player can gather if: active, not sanctioned, and not forced to coup
                available = currentPlayer->isActive() && 
                        !currentPlayer->isSanctioned() && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed()); // Can't gather if must coup
            } else if (button.action == "tax") {
                // Player can tax if: active, not sanctioned, and not forced to coup
                available = currentPlayer->isActive() && 
                        !currentPlayer->isSanctioned() && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed()); // Can't tax if must coup
            } else if (button.action == "bribe") {
                // Player can bribe if: active, has 4+ coins, and not forced to coup
                available = currentPlayer->isActive() && 
                        currentPlayer->coins() >= 4 && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed()); // Can't bribe if must coup
            } else if (button.action == "arrest") {
                // Player can arrest if: active, arrest available, not forced to coup, and has targets
                available = currentPlayer->isActive() && 
                        currentPlayer->isArrestAvailable() && 
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed()) && 
                        !getTargetablePlayers().empty(); // Need valid targets
            } else if (button.action == "sanction") {
                // Check if player can sanction any target based on coin requirements
                available = false;
                if (currentPlayer->isActive() && 
                    !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed())) {
                    for (Player* target : getTargetablePlayers()) {
                        // Check if player has enough coins (3 for normal, 4 for Judge targets)
                        int requiredCoins = target->getRoleType() == "General" ? 4 : 3; // Judges cost extra to sanction
                        if (currentPlayer->coins() >= requiredCoins) {
                            available = true; // Found affordable target
                            break;
                        }
                    }
                }
            } else if (button.action == "coup") {
                // Player can coup if: active, has 7+ coins, and has targets
                available = currentPlayer->isActive() && 
                        currentPlayer->coins() >= 7 && 
                        !getTargetablePlayers().empty(); // Need valid targets for coup
            }
            // Role-specific actions validation
            else if (button.action == "invest") {
                Baron* baron = dynamic_cast<Baron*>(currentPlayer); // Verify Baron role
                available = baron && currentPlayer->isActive() && 
                        currentPlayer->coins() >= 3 &&
                        !(currentPlayer->coins() >= 10 && !currentPlayer->isBribeUsed()); // Baron investment cost
            } 
            // Reactive abilities - available to any player with the appropriate role
            else if (button.action == "undo") {
                // Find any active Governor player for undo tax ability
                std::vector<Player*> allPlayers = game->getAllPlayers();
                available = false;
                for (Player* player : allPlayers) {
                    Governor* governor = dynamic_cast<Governor*>(player); // Check for Governor role
                    if (governor && player->isActive() && !getTargetablePlayers().empty()) {
                        available = true; // Found eligible Governor
                        break;
                    }
                }
            }
            else if (button.action == "spy_on") {
                // Find any active Spy player for surveillance ability
                std::vector<Player*> allPlayers = game->getAllPlayers();
                available = false;
                for (Player* player : allPlayers) {
                    Spy* spy = dynamic_cast<Spy*>(player); // Check for Spy role
                    if (spy && player->isActive() && !getTargetablePlayers().empty()) {
                        available = true; // Found eligible Spy
                        break;
                    }
                }
            } else if (button.action == "block_coup") {
                // Find any General player with 5+ coins for coup blocking ability
                std::vector<Player*> allPlayers = game->getAllPlayers();
                available = false;
                for (Player* player : allPlayers) {
                    General* general = dynamic_cast<General*>(player); // Check for General role
                    if (general && player->coins() >= 5) { // General needs 5+ coins to block
                        // Check if there are any inactive players that could be revived
                        bool hasInactivePlayers = false;
                        for (Player* otherPlayer : allPlayers) {
                            if (!otherPlayer->isActive()) {
                                hasInactivePlayers = true; // Found revivable player
                                break;
                            }
                        }
                        if (hasInactivePlayers) {
                            available = true; // Can block coup and revive someone
                            break;
                        }
                    }
                }
            } else if (button.action == "block_bribe") {
                // Find any active Judge player when there are players who have used bribe
                std::vector<Player*> allPlayers = game->getAllPlayers();
                available = false;
                for (Player* player : allPlayers) {
                    Judge* judge = dynamic_cast<Judge*>(player); // Check for Judge role
                    if (judge && player->isActive()) {
                        // Check if any player has used bribe that can be blocked
                        bool hasBribeUser = false;
                        for (Player* otherPlayer : allPlayers) {
                            if (otherPlayer->isBribeUsed()) {
                                hasBribeUser = true; // Found player who used bribe
                                break;
                            }
                        }
                        if (hasBribeUser) {
                            available = true; // Can block bribe action
                            break;
                        }
                    }
                }
            }
            // Additional role-specific actions can be added here
            
            button.setEnabled(available); // Apply availability state to button
            
            // Add visual feedback for unavailable actions
            if (!available) {
                button.setHovered(false); // Remove hover effect for disabled buttons
            }
        }
    }

    std::vector<Player*> GameGUI::getTargetablePlayers() const {
        std::vector<Player*> targets; // Initialize target list
        Player* currentPlayer = game->getCurrentPlayer(); // Get currently active player
        
        if (!currentPlayer) return targets; // Return empty if no current player
        
        std::vector<Player*> allPlayers = game->getActivePlayers(); // Get all active participants
        for (Player* player : allPlayers) {
            if (player != currentPlayer) { // Exclude current player from targets
                targets.push_back(player); // Add valid target to list
            }
        }
        
        return targets; // Return list of valid target players
    }

    std::vector<Player*> GameGUI::getActivePlayers() const {
        if (!game) {
            return std::vector<Player*>(); // Return empty vector if no game exists
        }
        return game->getActivePlayers(); // Return active players from game
    }

    void GameGUI::update() {
        playerNameInput.update(); // Update input field cursor animation
        updateAnimations(); // Update all visual animations
        
        if (currentState == GameState::PLAYING && game) { // Only update during active gameplay
            updatePlayerCards(); // Refresh player card displays with current information
            updateGameInfo(); // Update current player and game status information
            updateActionAvailability(); // Refresh button states based on current game state
        }
    }

    void GameGUI::updatePlayerCards() {
        std::vector<Player*> allPlayers = game->getAllPlayers(); // Get all game participants
        if (playerCards.size() != allPlayers.size()) {
            createPlayerCards(); // Recreate cards if player count changed
            return;
        }
        
        Player* currentPlayer = game->getCurrentPlayer(); // Get active player for highlighting
        
        // Update each player card with current information
        for (size_t i = 0; i < allPlayers.size() && i < playerCards.size(); ++i) {
            bool isCurrent = (currentPlayer && allPlayers[i] == currentPlayer); // Check if current player
            
            // Use the Player's getRoleType method for proper role detection
            RoleType displayRole = convertRoleType(allPlayers[i]->getRoleType()); // Convert role string to enum
            
            playerCards[i].updateInfo(allPlayers[i], isCurrent, displayRole); // Update card display
        }
    }

    void GameGUI::updateAnimations() {
        // Animate decorative coins with gentle floating motion
        static sf::Clock animClock; // Static clock to track animation time
        float time = animClock.getElapsedTime().asSeconds(); // Get elapsed time for smooth animation
        
        // Apply floating animation to each decorative coin
        for (size_t i = 0; i < decorativeCoins.size(); ++i) {
            float offset = sin(time + i) * 2; // Sine wave for smooth up-down motion
            sf::Vector2f pos = decorativeCoins[i].getPosition(); // Get current position
            decorativeCoins[i].setPosition(pos.x, pos.y + offset); // Apply vertical offset
        }
    }

    void GameGUI::render() {
        window.clear(theme.background); // Clear window with background color
        
        // Draw decorative elements for visual enhancement
        for (const auto& coin : decorativeCoins) {
            window.draw(coin); // Draw floating coin animations
        }
        
        // Draw interface based on current game state
        switch (currentState) {
            case GameState::MAIN_MENU:
                window.draw(titleText); // Draw game title
                window.draw(subtitleText); // Draw creator attribution
                for (const auto& button : menuButtons) {
                    button.draw(window); // Draw main menu buttons
                }
                break;
                
            case GameState::SETUP_PLAYERS:
                window.draw(instructionText); // Draw setup instructions
                playerNameInput.draw(window); // Draw player name input field
                addPlayerButton.draw(window); // Draw add player button
                startGameButton.draw(window); // Draw start game button
                backButton.draw(window); // Draw back to menu button
                
                // Draw player cards with delete buttons during setup
                for (const auto& card : playerCards) {
                    card.draw(window, true); // Show delete buttons during setup phase
                }
                break;
                
            case GameState::PLAYING: {
                // Draw organized game interface panels
                window.draw(gameInfoPanel); // Draw game information panel
                window.draw(actionPanel); // Draw action selection panel
                window.draw(playerPanel); // Draw player status panel
                window.draw(feedbackPanel); // Draw message feedback panel
                
                // Draw game information and status displays
                window.draw(currentPlayerText); // Draw current player indicator
                window.draw(gameStatusText); // Draw game status information
                
                // Draw all available action buttons
                for (const auto& button : actionButtons) {
                    button.draw(window); // Draw action buttons with current availability
                }
                
                // Draw player cards with target highlighting during selection
                std::vector<Player*> allPlayers = game->getAllPlayers(); // Get all players for highlighting
                for (size_t i = 0; i < playerCards.size() && i < allPlayers.size(); ++i) {
                    // Highlight targetable players when waiting for target selection
                    if (waitingForTarget && allPlayers[i] != game->getCurrentPlayer() && allPlayers[i]->isActive()) {
                        // Draw a selection highlight for valid targets
                        sf::RectangleShape highlight = playerCards[i].background; // Copy card background shape
                        highlight.setFillColor(sf::Color(255, 255, 0, 50)); // Semi-transparent yellow highlight
                        highlight.setOutlineColor(sf::Color::Yellow); // Bright yellow border
                        highlight.setOutlineThickness(3); // Thick border for visibility
                        window.draw(highlight); // Draw highlight behind card
                    }
                    playerCards[i].draw(window, false); // Don't show delete buttons during gameplay
                }
                
                // Draw action feedback for user responses
                window.draw(actionFeedbackText); // Display action results and messages
                break;
            }
                
            case GameState::GAME_OVER:
                // Draw game completion screen
                window.draw(gameOverTitle); // Draw "GAME OVER" title
                window.draw(winnerText); // Draw winner announcement
                returnToMenuButton.draw(window); // Draw return to menu button
                break;
        }
        
        // Draw reactive player selection overlay if waiting for selection
        if (waitingForReactivePlayer) {
            window.draw(selectionOverlay); // Draw semi-transparent overlay
            window.draw(selectionTitle); // Draw selection prompt title
            
            // Draw question text if it's a General decision for coup blocking
            if (pendingReactiveAction == "general_coup_decision") {
                window.draw(generalDecisionQuestion); // Draw coup blocking decision question
            }

            // Draw reactive player selection buttons
            for (const auto& button : reactivePlayerButtons) {
                button.draw(window); // Draw selection buttons for eligible players
            }
        }
        
        // Always draw message feedback at bottom of screen
        window.draw(messageText); // Display current message to user
        
        window.display(); // Present rendered frame to screen
    }

    sf::Vector2f GameGUI::getCenterPosition(sf::Vector2f size) const {
        return sf::Vector2f((WINDOW_WIDTH - size.x) / 2, (WINDOW_HEIGHT - size.y) / 2); // Calculate centered position
    }

    sf::Vector2f GameGUI::getButtonGridPosition(int index, int columns, sf::Vector2f startPos, sf::Vector2f spacing) const {
        int row = index / columns; // Calculate row based on index and column count
        int col = index % columns; // Calculate column position within row
        return sf::Vector2f(startPos.x + col * spacing.x, startPos.y + row * spacing.y); // Return grid position
    }

    void GameGUI::updateMessage(const std::string& message, bool isError, bool isWarning) {
        lastMessage = message; // Store message for reference
        
        // Set appropriate color based on message type
        if (isError) {
            messageText.setFillColor(theme.error); // Red for error messages
            actionFeedbackText.setFillColor(theme.error); // Red for action feedback
        } else if (isWarning) {
            messageText.setFillColor(theme.warning); // Orange for warning messages
            actionFeedbackText.setFillColor(theme.warning); // Orange for action feedback
        } else {
            messageText.setFillColor(theme.success); // Green for success messages
            actionFeedbackText.setFillColor(theme.success); // Green for action feedback
        }
        
        messageText.setString(message); // Update main message display
        actionFeedbackText.setString(message); // Update action feedback display
    }

    void GameGUI::createDecorativeElements() {
        decorativeCoins.clear(); // Clear existing decorative elements
        
        // Create floating coin decorations around the window for visual appeal
        for (int i = 0; i < 8; ++i) {
            sf::CircleShape coin(12); // Small circular coins
            coin.setFillColor(sf::Color(255, 215, 0, 150)); // Semi-transparent gold color
            coin.setOutlineThickness(2); // Subtle border
            coin.setOutlineColor(sf::Color(255, 255, 255, 100)); // Semi-transparent white outline
            
            // Position coins around the edges in circular pattern
            float angle = (i * 45) * 3.14159f / 180.0f; // Convert degrees to radians for positioning
            float radius = 300; // Distance from center
            float x = WINDOW_WIDTH/2 + cos(angle) * radius; // Calculate X position using trigonometry
            float y = WINDOW_HEIGHT/2 + sin(angle) * radius; // Calculate Y position using trigonometry
            
            coin.setPosition(x, y); // Apply calculated position
            decorativeCoins.push_back(coin); // Add coin to decoration list
        }
    }

    RoleType GameGUI::convertRoleType(const std::string& roleStr) const {
        // Convert string role names to enum values for consistent handling
        if (roleStr == "Governor") return RoleType::GOVERNOR;
        else if (roleStr == "Spy") return RoleType::SPY;
        else if (roleStr == "Baron") return RoleType::BARON;
        else if (roleStr == "General") return RoleType::GENERAL;
        else if (roleStr == "Judge") return RoleType::JUDGE;
        else if (roleStr == "Merchant") return RoleType::MERCHANT;
        else return RoleType::PLAYER; // Default role for unrecognized strings
    }

    std::vector<Player*> GameGUI::getEligibleReactivePlayers(const std::string& action) const {
        std::vector<Player*> eligiblePlayers; // Initialize list of eligible players
        if (!game) return eligiblePlayers; // Return empty if no game exists
        
        std::vector<Player*> allPlayers = game->getAllPlayers(); // Get all game participants
        
        // Check each player for eligibility based on action type and role
        for (Player* player : allPlayers) {
            if (action == "undo") {
                Governor* governor = dynamic_cast<Governor*>(player); // Check for Governor role
                if (governor && player->isActive()) { // Governor must be active to use undo
                    eligiblePlayers.push_back(player); // Add eligible Governor
                }
            }
            else if (action == "spy_on") {
                // Check if player is a Spy and is active for surveillance ability
                Spy* spy = dynamic_cast<Spy*>(player); // Check for Spy role
                if (spy && player->isActive()) { // Spy must be active to use spy ability
                    eligiblePlayers.push_back(player); // Add eligible Spy
                }
            }
            else if (action == "block_coup") {
                General* general = dynamic_cast<General*>(player); // Check for General role
                if (general && player->coins() >= 5) { // General needs 5+ coins to block coup
                    // Allow both active and inactive Generals (inactive can block their own coup)
                    eligiblePlayers.push_back(player); // Add eligible General
                }
            }
            else if (action == "block_bribe") {
                Judge* judge = dynamic_cast<Judge*>(player); // Check for Judge role
                if (judge && player->isActive()) { // Judge must be active to block bribes
                    eligiblePlayers.push_back(player); // Add eligible Judge
                }
            }
        }
        
        return eligiblePlayers; // Return list of players who can perform the reactive action
    }

    void GameGUI::showReactivePlayerSelection(const std::string& action, Player* target, const std::vector<Player*>& eligiblePlayers) {
        waitingForReactivePlayer = true; // Enable reactive player selection mode
        pendingReactiveAction = action; // Store action to be performed
        pendingReactiveTarget = target; // Store target of the action
        eligibleReactivePlayers = eligiblePlayers; // Store list of eligible players
        
        // Setup selection title based on action type
        std::string roleType = (action == "undo") ? "Governor" :
                               (action == "spy_on") ? "Spy" : 
                               (action == "block_coup") ? "General" : 
                               (action == "block_bribe") ? "Judge" : "Player";
        selectionTitle.setString("Choose " + roleType); // Set appropriate role title

        // "Who's gonna use it?" title for user guidance
        selectionTitle.setFont(mainFont); // Apply main font
        selectionTitle.setString("Who's gonna use it?"); // Clear instruction text
        selectionTitle.setCharacterSize(24); // Large readable size
        selectionTitle.setFillColor(theme.accent); // Gold color for prominence
        selectionTitle.setStyle(sf::Text::Bold); // Bold for emphasis
        sf::FloatRect bounds = selectionTitle.getLocalBounds(); // Get text bounds for centering
        selectionTitle.setPosition((WINDOW_WIDTH - bounds.width) / 2, 200); // Center horizontally
        
        // Create buttons for each eligible player with role information
        reactivePlayerButtons.clear(); // Clear existing buttons
        sf::Vector2f buttonSize(300, 50); // Increased width for better visibility
        sf::Vector2f startPos((WINDOW_WIDTH - buttonSize.x) / 2, 280); // Centered starting position
        int spacing = 60; // Vertical spacing between buttons
        
        // Generate selection buttons for each eligible player
        for (size_t i = 0; i < eligiblePlayers.size(); ++i) {
            sf::Vector2f pos(startPos.x, startPos.y + i * spacing); // Calculate button position
            std::string buttonText = eligiblePlayers[i]->getName() + " (" + roleType + ")"; // Display name and role
            
            // Create button with player identification and action
            reactivePlayerButtons.emplace_back(pos, buttonSize, buttonText, 
                "select_reactive_player_" + std::to_string(i), theme.primary);
            reactivePlayerButtons.back().setFont(mainFont); // Apply font to new button
        }
    }

    void GameGUI::hideReactivePlayerSelection() {
        waitingForReactivePlayer = false; // Disable reactive selection mode
        pendingReactiveAction = ""; // Clear pending action
        pendingReactiveTarget = nullptr; // Clear target reference
        eligibleReactivePlayers.clear(); // Clear eligible player list
        reactivePlayerButtons.clear(); // Clear selection buttons
    }

    void GameGUI::executeReactiveAction(const std::string& action, Player* reactivePlayer, Player* target) {
        try {
            // Execute Governor's undo tax ability
            if (action == "undo" && target) {
                Governor* governor = dynamic_cast<Governor*>(reactivePlayer); // Verify Governor role
                if (governor) {
                    governor->undo(*target); // Execute undo action on target
                    updateMessage(governor->getName() + " blocked " + target->getName() + "'s tax ability");
                } else {
                    updateMessage("Selected player is not a Governor!", true); // Role validation error
                }
            }
            // Execute Spy's surveillance ability
            else if (action == "spy_on" && target) {
                Spy* spy = dynamic_cast<Spy*>(reactivePlayer); // Verify Spy role
                if (spy) {
                    spy->spy_on(*target); // Execute spy action on target
                    updateMessage(spy->getName() + " spied on " + target->getName() + 
                                " (Coins: " + std::to_string(target->coins()) + ") and revoked their arrest ability for their next turn");
                } else {
                    updateMessage("Selected player is not a Spy!", true); // Role validation error
                }
            }
            // Execute General's coup blocking ability
            else if (action == "block_coup" && target) {
                General* general = dynamic_cast<General*>(reactivePlayer); // Verify General role
                if (general && reactivePlayer->coins() >= 5) { // Verify coin requirement
                    general->block_coup(*target); // Execute coup block and revive target
                    updateMessage(general->getName() + " blocked coup and revived " + target->getName());
                } else {
                    updateMessage("Selected player is not a General with 5+ coins!", true); // Requirements error
                }
            }
            // Execute Judge's bribe blocking ability
            else if (action == "block_bribe" && target) {
                Judge* judge = dynamic_cast<Judge*>(reactivePlayer); // Verify Judge role
                if (judge) {
                    judge->block_bribe(*target); // Execute bribe block on target
                    updateMessage(judge->getName() + " blocked " + target->getName() + "'s bribe");
                } else {
                    updateMessage("Selected player is not a Judge!", true); // Role validation error
                }
            }
            
            // Update game state after reactive action completion
            updatePlayerCards(); // Refresh player card displays
            updateGameInfo(); // Force update of game info to reflect changes
            createActionButtons(); // Recreate action buttons for current state
            addReactiveAbilityButtons(); // Refresh reactive ability availability
            
        } catch (const std::exception& e) {
            updateMessage("Error: " + std::string(e.what()), true); // Handle execution errors
        }
    }

    void GameGUI::showGeneralBlockCoupDecision() {
        if (!game) return; // Exit if no game exists
        
        // Find ANY General who can block coup (active or inactive with sufficient coins)
        Player* generalPlayer = nullptr;
        std::vector<Player*> allPlayers = game->getAllPlayers(); // Get all game participants
        
        // Search for eligible General to make coup blocking decision
        for (Player* player : allPlayers) {
            if (player->getRoleType() == "General" && player->coins() >= 5) { // General with coin requirement
                generalPlayer = player; // Found eligible General
                break; // Only need one General for decision
            }
        }
        
        if (!generalPlayer) return; // Exit if no eligible General found
        
        // Set up the decision overlay for General's coup blocking choice
        waitingForReactivePlayer = true; // Enable selection mode
        pendingReactiveAction = "general_coup_decision"; // Set special action type
        pendingReactiveTarget = generalPlayer; // Store General making decision
        
        // Setup selection title with General's name
        selectionTitle.setFont(mainFont); // Apply main font
        selectionTitle.setString(generalPlayer->getName() + " (General)"); // Display General's identity
        selectionTitle.setCharacterSize(32); // Large prominent size
        selectionTitle.setFillColor(theme.accent); // Gold color for importance
        selectionTitle.setStyle(sf::Text::Bold); // Bold for emphasis
        
        // Center the title horizontally
        sf::FloatRect bounds = selectionTitle.getLocalBounds(); // Get text dimensions
        selectionTitle.setPosition((WINDOW_WIDTH - bounds.width) / 2, 200); // Center positioning
        
        // Add subtitle asking the decision question
        sf::Text questionText; // Create question text element
        questionText.setFont(mainFont); // Apply main font
        questionText.setString("Choose a player to block coup on (or decline to end game):"); // Decision prompt
        questionText.setCharacterSize(20); // Medium readable size
        questionText.setFillColor(theme.textSecondary); // Secondary text color for subtlety
        sf::FloatRect questionBounds = questionText.getLocalBounds(); // Get question text bounds
        questionText.setPosition((WINDOW_WIDTH - questionBounds.width) / 2, 250); // Center question text
        
        // Store the question text for rendering
        generalDecisionQuestion = questionText; // Save question for display
        
        // Create buttons for each inactive player + decline option
        reactivePlayerButtons.clear(); // Clear existing buttons
        sf::Vector2f buttonSize(200, 45); // Compact button size for multiple options
        sf::Vector2f startPos((WINDOW_WIDTH - buttonSize.x) / 2, 300); // Centered starting position
        int spacing = 55; // Vertical spacing between buttons
        int buttonIndex = 0; // Track button position
        
        // Add buttons for each inactive player that can be revived
        for (Player* player : allPlayers) {
            if (!player->isActive()) { // Only show eliminated players as revival options
                sf::Vector2f pos(startPos.x, startPos.y + buttonIndex * spacing); // Calculate button position
                std::string buttonText = "Revive " + player->getName(); // Clear revival option text
                reactivePlayerButtons.emplace_back(pos, buttonSize, buttonText, 
                    "general_revive_" + std::to_string(buttonIndex), sf::Color(50, 205, 50)); // Green for positive action
                eligibleReactivePlayers.push_back(player); // Store the player for later reference
                buttonIndex++; // Move to next button position
            }
        }
        
        // Add decline button for General to refuse blocking coup
        sf::Vector2f declinePos(startPos.x, startPos.y + buttonIndex * spacing); // Position after revival options
        reactivePlayerButtons.emplace_back(declinePos, buttonSize, "Decline", 
            "general_block_no", sf::Color(220, 20, 60)); // Red for negative/ending action
        
        // Apply font to all created buttons for consistent typography
        for (auto& button : reactivePlayerButtons) {
            button.setFont(mainFont); // Ensure consistent typography across all buttons
        }
    }

} // namespace coup