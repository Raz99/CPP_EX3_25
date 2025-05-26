// email: razcohenp@gmail.com

/**
 * GameGUI.hpp
 * 
 * This file contains the GUI code for the Coup card game.
 * Uses SFML library for graphics and input handling.
 * Has code for all the game screens and player actions.
 */

#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Game.hpp"
#include "Player.hpp"

namespace coup
{
    /**
     * Different screens/states of the game interface.
     * Controls which view is shown and what interactions are available.
     */
    enum class GameState
    {
        MAIN_MENU, // Main menu screen with game options
        SETUP_PLAYERS, // Player creation and role selection screen
        PLAYING, // Main gameplay screen
        GAME_OVER // End game screen showing results
    };

    /**
     * Character roles available in the game.
     * Each role has different abilities and skills.
     */
    enum class RoleType
    {
        PLAYER, // Default player type
        GOVERNOR, // Can collect taxes
        SPY, // Can perform coups at lower cost
        BARON, // Can steal coins from other players
        GENERAL, // Can force other players to give coins
        JUDGE, // Can block stealing actions
        MERCHANT // Can gain extra coins during income
    };

    /**
     * Enhanced Button with hover effects and icons.
     * A button component for the game UI that can be clicked to perform actions.
     */
    struct EnhancedButton
    {
        sf::RectangleShape shape; // Button background shape
        sf::Text text; // Button label text
        sf::CircleShape icon; // Optional icon for visual enhancement
        std::string action; // Action identifier triggered when clicked
        bool enabled; // Whether button is interactive
        bool hovered; // Tracks mouse hover state
        sf::Color normalColor; // Default button color
        sf::Color hoverColor; // Color when mouse hovers over button
        sf::Color disabledColor; // Color when button is disabled
        EnhancedButton(sf::Vector2f position, sf::Vector2f size, const std::string &label,
                       const std::string &actionType, sf::Color normal = sf::Color(70, 130, 180)); // Constructor for a new button
        bool contains(sf::Vector2f point) const; // Checks if a point is inside the button
        void setEnabled(bool enable); // Sets button as enabled or disabled
        void setHovered(bool hover); // Sets the hover state of the button
        void draw(sf::RenderWindow &window) const; // Draws the button to the window
        void setFont(const sf::Font &font); // Sets the font for the button text
    };

    /**
     * Player card with role and status.
     * Shows player info like name, coins, and current state visually.
     */
    struct PlayerCard
    {
        sf::RectangleShape background; // Card background
        sf::RectangleShape roleIcon; // Visual indicator of player role
        sf::Text nameText; // Player's name
        sf::Text coinsText; // Current coin count
        sf::Text statusText; // Active/inactive status text
        sf::Text roleText; // Text label of player's role
        sf::CircleShape playerAvatar; // Visual representation of player
        sf::RectangleShape coinIcon; // Icon for coin display
        bool isCurrentPlayer; // Whether this is the active player
        bool isActive; // Whether player is still in the game
        RoleType role; // Player's character role
        PlayerCard(sf::Vector2f position, sf::Vector2f size); // Constructor for a new player card
        void updateInfo(const Player *player, bool current, 
            RoleType playerRole = RoleType::PLAYER); // Updates card with current player information
        void draw(sf::RenderWindow &window) const; // Draws the player card to the window.
        void setFont(const sf::Font &font); // Sets the font for all text elements
    };

    /**
     * Text input component for collecting user text
     * 
     * Provides a way for users to input text with visual feedback
     * including cursor animation and placeholder text
     */
    struct InputField
    {
        sf::RectangleShape background; // Input field background
        sf::Text displayText; // Text entered by the user
        sf::Text placeholder; // Hint text shown when empty
        std::string content; // Actual text content
        bool active; // Whether field is receiving input
        bool showCursor; // Controls cursor visibility
        sf::Clock cursorClock; // For cursor blinking animation
        InputField(sf::Vector2f position, sf::Vector2f size,
            const std::string &placeholderText); // Constructs a new input field
        void handleInput(sf::Uint32 unicode); // Processes text input from keyboard
        void setActive(bool isActive); // Sets the field's active state for input focus
        bool contains(sf::Vector2f point) const; // Checks if a point is within the field boundaries
        void update(); // Updates visual state, including cursor animation
        void draw(sf::RenderWindow &window) const; // Renders the input field to the window
        void setFont(const sf::Font &font); // Sets the font for text elements
        std::string getText() const { return content; } // Gets the current text content
        
        // Clears all text content
        void clear() {
            content.clear();
            displayText.setString("");
        }
    };

    /**
     * Main class handling the game's graphical user interface
     * 
     * Manages the game's visual representation, user interactions,
     * state transitions, and integration with the game logic
     */
    class GameGUI
    {
        private:
            sf::RenderWindow window; // Main rendering window
            sf::Font mainFont; // Standard font for UI elements
            sf::Font titleFont; // Font used for titles and headers
            Game *game; // Pointer to the game logic
            std::vector<std::unique_ptr<Player>> players; // Player objects
            GameState currentState; // Current UI state

            // UI text elements for different game states
            sf::Text titleText; // Main title display
            sf::Text subtitleText; // Secondary title/information
            sf::Text instructionText; // Instructions for the player
            sf::Text messageText; // General message display
            sf::Text gameInfoText; // Game status information
            sf::Text gameOverTitle; // Game over screen title
            sf::Text winnerText; // Displays winner information
            sf::Text currentPlayerText; // Current player's turn indicator
            sf::Text gameStatusText; // Game state information
            sf::Text actionFeedbackText; // Feedback on player actions
            sf::Text actionHistoryText; // History of recent actions

            // UI panel containers for organized layout
            sf::RectangleShape gameInfoPanel; // Panel for game information
            sf::RectangleShape actionPanel; // Panel for action buttons
            sf::RectangleShape playerPanel; // Panel for player cards
            sf::RectangleShape feedbackPanel; // Panel for action feedback

            // Visual elements for backgrounds and decoration
            sf::RectangleShape backgroundGradient; // Background color gradient
            sf::CircleShape coinDecoration; // Coin decoration element
            std::vector<sf::CircleShape> decorativeCoins; // Animated coin elements

            /// Button collections for different UI sections
            std::vector<EnhancedButton> menuButtons; // Main menu buttons
            std::vector<EnhancedButton> actionButtons; // Game action buttons
            std::vector<EnhancedButton> targetButtons; // Player target selection buttons
            std::vector<EnhancedButton> roleButtons; // Role selection buttons

            /// Player creation and management UI elements
            std::vector<PlayerCard> playerCards; // Visual cards for each player
            InputField playerNameInput; // Text input for player names
            EnhancedButton addPlayerButton; // Button to add new player
            EnhancedButton startGameButton; // Button to start the game
            EnhancedButton backButton; // Navigation back button
            EnhancedButton returnToMenuButton; // Return to main menu button

            /// Game action state tracking
            std::string currentAction; // Currently selected action
            std::string lastMessage; // Last feedback message
            bool waitingForTarget; // Waiting for player to select target
            bool waitingForRole; // Waiting for player to select role
            Player *selectedTarget; // Currently targeted player
            RoleType selectedRole; // Currently selected role

            /**
             * Color theme and visual styling for the UI
             * 
             * Defines consistent colors across the interface
             */
            struct Theme
            {
                sf::Color background; // Main background color
                sf::Color primary; // Primary UI element color
                sf::Color secondary; // Secondary UI element color
                sf::Color accent; // Accent color for highlights
                sf::Color text; // Main text color
                sf::Color textSecondary; // Secondary text color
                sf::Color success; // Color for success messages
                sf::Color error; // Color for error messages
                sf::Color warning; // Color for warning messages
            } theme; // Current theme instance

            /// Layout dimension constants
            static const int WINDOW_WIDTH = 1400; // Main window width
            static const int WINDOW_HEIGHT = 900; // Main window height
            static const int BUTTON_WIDTH = 180; // Standard button width
            static const int BUTTON_HEIGHT = 50; // Standard button height
            static const int SMALL_BUTTON_WIDTH = 120; // Small button width
            static const int SMALL_BUTTON_HEIGHT = 35; // Small button height
            static const int CARD_WIDTH = 220; // Player card width
            static const int CARD_HEIGHT = 140; // Player card height

            /// Section header text elements
            sf::Text actionSectionHeader; // Header for action section
            sf::Text playerSectionHeader; // Header for player section
            sf::Text gameInfoHeader; // Header for game info section

        public:
            GameGUI(); // Constructor that sets up the game interface
            ~GameGUI(); // Destructor that cleans up resources

            // Main methods
            bool initialize(); // Sets up fonts, window and initial UI elements
            void run(); // Main game loop that keeps running until the game ends
            void handleEvents(); // Processes all user input events like clicks and key presses
            void update(); // Updates game state and UI elements each frame
            void render(); // Draws everything to the screen

            // State management
            void changeState(GameState newState); // Switches between different game screens
            void setupMainMenu(); // Prepares the main menu screen
            void setupPlayerSetup(); // Sets up the player creation screen
            void setupGameScreen(); // Prepares the main gameplay UI
            void setupGameOver(); // Sets up the game over screen with results

            // Event handling
            void handleMouseClick(sf::Vector2f mousePos); // Processes mouse button clicks
            void handleMouseMove(sf::Vector2f mousePos); // Handles mouse movement for hover effects
            void handleKeyPress(sf::Keyboard::Key key); // Processes keyboard input
            void handleTextInput(sf::Uint32 unicode); // Handles text input for name fields

            // UI creation methods
            void createMenuButtons(); // Creates buttons for the main menu
            void createActionButtons(); // Creates action buttons for gameplay
            void addRoleSpecificButtons(RoleType role, sf::Vector2f startPos, sf::Vector2f buttonSize,
                int spacing, int startIndex); // Adds role-specific action buttons
            void createRoleButtons(); // Creates role selection buttons
            void createPlayerCards(); // Creates visual cards for each player
            void updatePlayerCards(); // Updates player cards with current game state

            // Game actions
            void addNewPlayer(); // Adds a new player with the name from the input field
            void startNewGame(); // Starts the game with the current players
            void executeAction(const std::string &action, Player *target = nullptr); // Performs a game action
            void selectTarget(Player *target); // Sets the target player for an action
            void selectRole(RoleType role); // Selects a role for a new player

            // Helper methods
            Player *getCurrentPlayer() const; // Gets the player with the current turn
            std::vector<Player *> getActivePlayers() const; // Gets all players still in the game
            std::vector<Player *> getTargetablePlayers() const; // Gets players that can be targeted
            bool isActionAvailable(const std::string &action) const; // Checks if an action can be performed
            RoleType getPlayerRole(Player *player) const; // Gets the role type of a player
            std::string getRoleName(RoleType role) const; // Converts role enum to display name
            sf::Color getRoleColor(RoleType role) const; // Gets the color associated with a role
            void updateMessage(const std::string &message, bool isError = false, bool isWarning = false); // Shows feedback message
            void createDecorativeElements(); // Creates visual decorations
            void setupGamePanels(); // Sets up the UI layout panels
            void updateGameInfo(); // Updates game status display
            void updateActionAvailability(); // Enables/disables action buttons based on game state

            // Layout helpers
            sf::Vector2f getCenterPosition(sf::Vector2f size) const; // Gets the center position for an element
            sf::Vector2f getButtonGridPosition(int index, int columns, sf::Vector2f startPos, sf::Vector2f spacing) const; // Calculates position in a grid
            void centerText(sf::Text &text, sf::Vector2f containerPos, sf::Vector2f containerSize) const; // Centers text in container

            // Animation and effects
            void updateAnimations(); // Updates all visual animations
            void createCoinAnimation(sf::Vector2f position); // Creates a coin animation effect
    };
} // namespace coup

#endif