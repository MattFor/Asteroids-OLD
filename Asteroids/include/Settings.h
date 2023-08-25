#define STRICT

// Miscellaneous
#define DEBUG true
#define DEBUG_PLAYER_INFO false

// Constants
#define DECAY 0.9975f
#define RADIANS 3.14159f / 180.0f

#define MOVEMENT_CONSTANT 1000.0f

#define MAX_SPEED 500.0f
#define MAX_ROTATION 180.0f

// Messages
#define STARTUP_ERROR 0
#define STARTUP_SUCCESS 1

#define LOADING_TEXTURE_FAILURE 0
#define LOADING_TEXTURE_SUCCESS 1

// Kebinds
#define EXIT sf::Keyboard::Escape
#define UP sf::Keyboard::W
#define DOWN sf::Keyboard::S
#define LEFT sf::Keyboard::A
#define RIGHT sf::Keyboard::D
#define SHOOT sf::Keyboard::Space

// Rendering
enum class RenderMode : int
{
	VECTORS,
	TEXTURES
};

#define RENDER_MODE RenderMode::VECTORS

#define FULLSCREEN false // Make the game fullscreen
#define OLD_SCHOOL true // Make it pixelated!

#define WIDTH 0
#define HEIGHT 0

// Gameplay
#define SLIDE_ROTATION false
