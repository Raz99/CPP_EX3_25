// email: razcohenp@gmail.com
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

namespace coup {

// Game states for different screens
enum class GameState {
    MAIN_MENU,
    SETUP_PLAYERS,
    PLAYING,
    GAME_OVER
};

// Role types for visual representation
enum class RoleType {
    PLAYER,
    GOVERNOR,
    SPY,
    BARON,
    GENERAL,
    JUDGE,
    MERCHANT
};

// Enhanced Button with hover effects and icons
struct EnhancedButton {
    sf::RectangleShape shape;
    sf::Text text;
    sf::CircleShape icon;
    std::string action;
    bool enabled;
    bool hovered;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color disabledColor;
    
    EnhancedButton(sf::Vector2f position, sf::Vector2f size, const std::string& label, 
                   const std::string& actionType, sf::Color normal = sf::Color(70, 130, 180));
    bool contains(sf::Vector2f point) const;
    void setEnabled(bool enable);
    void setHovered(bool hover);
    void draw(sf::RenderWindow& window) const;
    void setFont(const sf::Font& font);
};

// Player card with role and status
struct PlayerCard {
    sf::RectangleShape background;
    sf::RectangleShape roleIcon;
    sf::Text nameText;
    sf::Text coinsText;
    sf::Text statusText;
    sf::Text roleText;
    sf::CircleShape playerAvatar;
    sf::RectangleShape coinIcon;
    bool isCurrentPlayer;
    bool isActive;
    RoleType role;
    
    PlayerCard(sf::Vector2f position, sf::Vector2f size);
    void updateInfo(const Player* player, bool current, RoleType playerRole = RoleType::PLAYER);
    void draw(sf::RenderWindow& window) const;
    void setFont(const sf::Font& font);
};

// Input field for player names
struct InputField {
    sf::RectangleShape background;
    sf::Text displayText;
    sf::Text placeholder;
    std::string content;
    bool active;
    bool showCursor;
    sf::Clock cursorClock;
    
    InputField(sf::Vector2f position, sf::Vector2f size, const std::string& placeholderText);
    void handleInput(sf::Uint32 unicode);
    void setActive(bool isActive);
    bool contains(sf::Vector2f point) const;
    void update();
    void draw(sf::RenderWindow& window) const;
    void setFont(const sf::Font& font);
    std::string getText() const { return content; }
    void clear() { content.clear(); displayText.setString(""); }
};

class GameGUI {
private:
    sf::RenderWindow window;
    sf::Font mainFont;
    sf::Font titleFont;
    Game* game;
    std::vector<std::unique_ptr<Player>> players;
    GameState currentState;
    
// UI Elements for different states
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text instructionText;
    sf::Text messageText;
    sf::Text gameInfoText;
    sf::Text gameOverTitle;
    sf::Text winnerText;
    sf::Text currentPlayerText;
    sf::Text gameStatusText;
    sf::Text actionFeedbackText;
    sf::Text actionHistoryText;
    
// Panels for organized layout
    sf::RectangleShape gameInfoPanel;
    sf::RectangleShape actionPanel;
    sf::RectangleShape playerPanel;
    sf::RectangleShape feedbackPanel;
    
// Backgrounds and decorations
    sf::RectangleShape backgroundGradient;
    sf::CircleShape coinDecoration;
    std::vector<sf::CircleShape> decorativeCoins;
    
// Menu system
    std::vector<EnhancedButton> menuButtons;
    std::vector<EnhancedButton> actionButtons;
    std::vector<EnhancedButton> targetButtons;
    std::vector<EnhancedButton> roleButtons;
    
// Player management
    std::vector<PlayerCard> playerCards;
    InputField playerNameInput;
    EnhancedButton addPlayerButton;
    EnhancedButton startGameButton;
    EnhancedButton backButton;
    EnhancedButton returnToMenuButton;
    
// Game state
    std::string currentAction;
    std::string lastMessage;
    bool waitingForTarget;
    bool waitingForRole;
    Player* selectedTarget;
    RoleType selectedRole;
    
// Visual themes and colors
    struct Theme {
        sf::Color background;
        sf::Color primary;
        sf::Color secondary;
        sf::Color accent;
        sf::Color text;
        sf::Color textSecondary;
        sf::Color success;
        sf::Color error;
        sf::Color warning;
    } theme;
    
// Layout constants
    static const int WINDOW_WIDTH = 1400;
    static const int WINDOW_HEIGHT = 900;
    static const int BUTTON_WIDTH = 180;
    static const int BUTTON_HEIGHT = 50;
    static const int SMALL_BUTTON_WIDTH = 120;
    static const int SMALL_BUTTON_HEIGHT = 35;
    static const int CARD_WIDTH = 220;
    static const int CARD_HEIGHT = 140;
    
// Action section headers
    sf::Text actionSectionHeader;
    sf::Text playerSectionHeader;
    sf::Text gameInfoHeader;
    
public:
    GameGUI();
    ~GameGUI();
    
// Main methods
    bool initialize();
    void run();
    void handleEvents();
    void update();
    void render();
    
// State management
    void changeState(GameState newState);
    void setupMainMenu();
    void setupPlayerSetup();
    void setupGameScreen();
    void setupGameOver();
    
// Event handling
    void handleMouseClick(sf::Vector2f mousePos);
    void handleMouseMove(sf::Vector2f mousePos);
    void handleKeyPress(sf::Keyboard::Key key);
    void handleTextInput(sf::Uint32 unicode);
    
// UI creation methods
    void createMenuButtons();
    void createActionButtons();
    void addRoleSpecificButtons(RoleType role, sf::Vector2f startPos, sf::Vector2f buttonSize, 
                               int spacing, int startIndex);
    void createRoleButtons();
    void createPlayerCards();
    void updatePlayerCards();
    
// Game actions
    void addNewPlayer();
    void startNewGame();
    void executeAction(const std::string& action, Player* target = nullptr);
    void selectTarget(Player* target);
    void selectRole(RoleType role);
    
// Helper methods
    Player* getCurrentPlayer() const;
    std::vector<Player*> getActivePlayers() const;
    std::vector<Player*> getTargetablePlayers() const;
    bool isActionAvailable(const std::string& action) const;
    RoleType getPlayerRole(Player* player) const;
    std::string getRoleName(RoleType role) const;
    sf::Color getRoleColor(RoleType role) const;
    void updateMessage(const std::string& message, bool isError = false, bool isWarning = false);
    void createDecorativeElements();
    void setupGamePanels();
    void updateGameInfo();
    void updateActionAvailability();
    
// Layout helpers
    sf::Vector2f getCenterPosition(sf::Vector2f size) const;
    sf::Vector2f getButtonGridPosition(int index, int columns, sf::Vector2f startPos, sf::Vector2f spacing) const;
    void centerText(sf::Text& text, sf::Vector2f containerPos, sf::Vector2f containerSize) const;
    
// Animation and effects
    void updateAnimations();
    void createCoinAnimation(sf::Vector2f position);
};

} // namespace coup

#endif