#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
#include <GL/glut.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

const int WIDTH = 1100;
const int HEIGHT = 720;
const int WINDOW_START_X = 80;
const int WINDOW_START_Y = 30;
const float PI = 3.14159265359f;
const int TIMER_INTERVAL_MS = 16;
const float DEFAULT_DELTA_TIME = 0.016f;
const float MIN_DELTA_TIME = 0.001f;
const float MAX_DELTA_TIME = 0.050f;

// ---- Core game rules -------------------------------------------------
const int TOTAL_LEVELS = 3;
const int STARTING_LIVES = 3;
const int FOOD_SCORE_VALUE = 10;
const int MEDIUM_LEVEL_SCORE = 800;
const int HARD_LEVEL_SCORE = 1200;
const float LEVEL_MESSAGE_DURATION = 1.80f;

// ---- Object-pool limits ---------------------------------------------
const int TOTAL_BUBBLES = 100;
const int TOTAL_GRASS = 10;
const int MAX_FOOD_FISH = 12;
const int MAX_ENEMY_FISH = 5;

// ---- Player movement -------------------------------------------------
// These are the main values to tune if you want the player to feel
// faster/slower or more/less responsive.
const float PLAYER_MIN_X = 95.0f;
const float PLAYER_MAX_X = 1000.0f;
const float PLAYER_MIN_Y = 155.0f;
const float PLAYER_MAX_Y = 555.0f;
const float PLAYER_START_X = 115.0f;
const float PLAYER_START_Y = 350.0f;
const float PLAYER_MAX_SPEED = 330.0f;              // pixels / second
const float PLAYER_ACCELERATION = 3200.0f;          // start moving quickly
const float PLAYER_DECELERATION = 3900.0f;          // stop cleanly on release
const float PLAYER_REVERSE_ACCELERATION = 5200.0f;  // fast left<->right response
const float PLAYER_VISUAL_TURN_SPEED = 20.0f;       // higher = faster direction flip
const float PLAYER_INPUT_DEAD_ZONE = 0.001f;

// ---- Player hit / recovery effect -----------------------------------
const float PLAYER_HIT_SHAKE_DURATION = 0.58f;
const float PLAYER_GAME_OVER_SHAKE_DURATION = 0.70f;
const float PLAYER_INVULNERABLE_DURATION = 1.35f;
const float PLAYER_GAME_OVER_INVULNERABLE_TIME = 9999.0f;
const float PLAYER_SHAKE_X_FREQUENCY = 82.0f;
const float PLAYER_SHAKE_Y_FREQUENCY = 68.0f;
const float PLAYER_SHAKE_X_AMOUNT = 7.0f;
const float PLAYER_SHAKE_Y_AMOUNT = 3.0f;

// ---- Visual sizes ----------------------------------------------------
// Required size order: Food < Player < Enemy.
const float FOOD_VISUAL_SCALE = 0.72f;
const float PLAYER_VISUAL_SCALE = 86.0f;
const float ENEMY_VISUAL_SCALE = 82.0f;

// ---- Collision boxes -------------------------------------------------
// Slightly smaller than decorative tails/fins for fair collision.
const float FOOD_COLLISION_HALF_W = 35.0f;
const float FOOD_COLLISION_HALF_H = 24.0f;
const float PLAYER_COLLISION_HALF_W = 43.0f;
const float PLAYER_COLLISION_HALF_H = 34.0f;
const float ENEMY_COLLISION_HALF_W = 52.0f;
const float ENEMY_COLLISION_HALF_H = 34.0f;

// ---- Food-fish spawning / movement variation -------------------------
const float FOOD_SPAWN_X_OFFSET_MIN = 75.0f;
const float FOOD_SPAWN_X_OFFSET_MAX = 190.0f;
const float FOOD_SPAWN_Y_MIN = 165.0f;
const float FOOD_SPAWN_Y_MAX = 550.0f;
const float FOOD_SPEED_FACTOR_MIN = 0.88f;
const float FOOD_SPEED_FACTOR_MAX = 1.12f;
const float FOOD_DESPAWN_X = -120.0f;
const float FOOD_SPAWN_INTERVAL_FACTOR_MIN = 0.82f;
const float FOOD_SPAWN_INTERVAL_FACTOR_MAX = 1.18f;
const float FOOD_FIRST_SPAWN_DELAY = 0.15f;
const float FOOD_AFTER_EAT_SPAWN_DELAY = 0.18f;
const int FOOD_COLOR_COUNT = 5;
const float FOOD_BOB_SPEED = 4.0f;
const float FOOD_BOB_AMOUNT = 2.5f;

// ---- Enemy-fish spawning / movement variation ------------------------
const float ENEMY_SPAWN_X_OFFSET_MIN = 95.0f;
const float ENEMY_SPAWN_X_OFFSET_MAX = 220.0f;
const float ENEMY_SPAWN_Y_MIN = 175.0f;
const float ENEMY_SPAWN_Y_MAX = 540.0f;
const float ENEMY_SPEED_FACTOR_MIN = 0.90f;
const float ENEMY_SPEED_FACTOR_MAX = 1.10f;
const float ENEMY_DESPAWN_X = -150.0f;
const float ENEMY_SPAWN_INTERVAL_FACTOR_MIN = 0.82f;
const float ENEMY_SPAWN_INTERVAL_FACTOR_MAX = 1.18f;
const float ENEMY_FIRST_SPAWN_DELAY = 0.85f;
const float ENEMY_MIN_VERTICAL_SEPARATION = 115.0f;
const float ENEMY_NEW_SPAWN_RIGHT_ZONE = 50.0f;
const int ENEMY_SPAWN_POSITION_ATTEMPTS = 6;
const float ENEMY_BOB_SPEED = 3.4f;
const float ENEMY_BOB_AMOUNT = 3.2f;

// ---- Bubble animation ------------------------------------------------
const float BUBBLE_INITIAL_Y_MAX = 560.0f;
const float BUBBLE_RADIUS_MIN = 5.0f;
const float BUBBLE_RADIUS_MAX = 14.0f;
const float BUBBLE_SPEED_MIN = 22.0f;
const float BUBBLE_SPEED_MAX = 58.0f;
const float BUBBLE_DRIFT_FREQUENCY = 1.7f;
const float BUBBLE_DRIFT_SPEED = 12.0f;
const float BUBBLE_HORIZONTAL_WRAP_MARGIN = 20.0f;

// ---- Aquarium floor / grass animation --------------------------------
const float SOIL_HEIGHT = 90.0f;
const float GRASS_TOP_Y = 180.0f;
const float GRASS_HALF_WIDTH = 8.0f;
const float GRASS_START_X = 55.0f;
const float GRASS_SPACING_X = 110.0f;
const float GRASS_SECOND_LEAF_OFFSET_X = 26.0f;
const float GRASS_PHASE_MAX = 10.0f;
const float GRASS_SWAY_SPEED = 2.0f;
const float GRASS_SWAY_AMOUNT = 15.0f;

// ---- Water-wave animation -------------------------------------------
const int WAVE_SAMPLE_STEP = 4;
const float WAVE_1_BASE_Y = 620.0f;
const float WAVE_1_A_FREQ = 0.018f;
const float WAVE_1_A_TIME = 1.5f;
const float WAVE_1_A_AMP = 10.0f;
const float WAVE_1_B_FREQ = 0.035f;
const float WAVE_1_B_TIME = 2.0f;
const float WAVE_1_B_AMP = 5.0f;
const float WAVE_1_C_FREQ = 0.008f;
const float WAVE_1_C_TIME = 0.8f;
const float WAVE_1_C_AMP = 4.0f;
const float WAVE_2_BASE_Y = 605.0f;
const float WAVE_2_A_FREQ = 0.020f;
const float WAVE_2_A_TIME = 1.2f;
const float WAVE_2_A_AMP = 7.0f;
const float WAVE_2_B_FREQ = 0.040f;
const float WAVE_2_B_TIME = 1.7f;
const float WAVE_2_B_AMP = 3.0f;

// ---- Decorative stone positions -------------------------------------
const float STONE_Y = 70.0f;
const float STONE_1_X = 155.0f;
const float STONE_2_X = 520.0f;
const float STONE_3_X = 955.0f;

// ---- HUD placement ---------------------------------------------------
const float HUD_TOP_Y = 680.0f;
const float HUD_LIFE_TEXT_X = 25.0f;
const float HUD_HEART_START_X = 100.0f;
const float HUD_HEART_SPACING_X = 40.0f;
const float HUD_SCORE_X = 500.0f;
const float HUD_LEVEL_RIGHT_MARGIN = 165.0f;
const float HUD_CONTROL_X = 360.0f;
const float HUD_CONTROL_Y = 650.0f;

// Fish drawing functions still contain vertex coordinates and RGB values.
// Those numbers describe artwork geometry/colors, not gameplay behavior.
// Keeping them beside the drawing code makes the shapes easier to edit.

// ------------------------------------------------------------
// LEVEL CONFIGURATION
// ------------------------------------------------------------
enum GameLevel {
    LEVEL_EASY = 0,
    LEVEL_MEDIUM = 1,
    LEVEL_HARD = 2
};

struct LevelConfig {
    const char* name;
    float foodSpeedPixelsPerSecond;
    float enemySpeedPixelsPerSecond;
    float foodSpawnInterval;
    float enemySpawnInterval;
    int maxActiveFood;
    int maxActiveEnemies;
    int enemyLevelUpBurst;
};

// Enemy difficulty now grows in THREE ways:
// 1) movement speed increases,
// 2) spawn interval becomes shorter (enemies enter more frequently),
// 3) more enemies may be active at the same time.
// enemyLevelUpBurst makes the increase visible immediately after a level-up.
const LevelConfig LEVELS[TOTAL_LEVELS] = {
    {"Easy",   115.0f, 140.0f, 0.86f, 3.20f, 5, 10, 0},
    {"Medium", 160.0f, 190.0f, 0.66f, 1.85f, 6, 15, 1},
    {"Hard",   210.0f, 245.0f, 0.50f, 1.10f, 7, 20, 2}
};

// ------------------------------------------------------------
// DATA STRUCTURES
// ------------------------------------------------------------
struct Bubble {
    float x;
    float y;
    float radius;
    float speed;
    float driftPhase;
};

struct FoodFish {
    bool active;
    float x;
    float y;
    float speedFactor;
    int color;
    float swimPhase;
};

struct EnemyFish {
    bool active;
    float x;
    float y;
    float speedFactor;
    float swimPhase;
    float aiOffset;
};

struct PlayerFish {
    float x;
    float y;
    float velocityX;
    float velocityY;
    int direction;       // desired facing: 1 = right, -1 = left
    float facingScaleX;  // smoothly moves between +1 and -1 while turning
    float shakeTime;
    float invulnerableTime;
};

struct InputState {
    bool left;
    bool right;
    bool up;
    bool down;
    int horizontalPriority; // -1 = last horizontal press was left, +1 = right
    int verticalPriority;   // -1 = last vertical press was down, +1 = up
};

struct GameState {
    int score;
    int lives;
    GameLevel level;
    bool gameOver;

    float foodSpawnTimer;
    float enemySpawnTimer;
    float levelMessageTimer;

    // Professional gameplay statistics.
    int fishEaten;
    int coinsCollected;
    int maxCombo;
    float survivalTime;
};

// ------------------------------------------------------------
// GLOBAL STATE
// ------------------------------------------------------------
float gameTime = 0.0f;
int lastUpdateMilliseconds = 0;

Bubble bubbles[TOTAL_BUBBLES];
float grassPhase[TOTAL_GRASS];
FoodFish foodFish[MAX_FOOD_FISH];
EnemyFish enemyFish[MAX_ENEMY_FISH];
PlayerFish player;
GameState gameState;
InputState inputState;

// ------------------------------------------------------------
// MENU / GAME STATE
// ------------------------------------------------------------
bool gameStarted = false;
bool gamePaused = false;

// Forward declarations for original helper/game functions defined later.
float randomFloat(float minValue, float maxValue);
float clampFloat(float value, float minValue, float maxValue);
void drawCircle(float x, float y, float radius, bool fill = true, int segments = 32);
void drawText(float x, float y, const char* text,
              void* font = GLUT_BITMAP_HELVETICA_18);
bool rectanglesOverlap(float ax, float ay, float aHalfW, float aHalfH,
                       float bx, float by, float bHalfW, float bHalfH);
void restartGame();
void drawMenuPanel(float left, float bottom, float right, float top);
void drawMenuPanel(float left, float bottom, float right, float top);
void updateMissionAndAchievements();
void drawMissionHUD();
void drawLevelProgressBar();
void drawGameStatisticsOverlay();
void unlockAchievement(int index);


bool showHowToPlay = false;
bool showMainMenu = true;

int countdownValue = 0;
float countdownTimer = 0.0f;
bool countdownActive = false;

int bestScore = 0;
const char* BEST_SCORE_FILE = "highscore.txt";

enum PowerUpType {
    POWER_NONE = 0,
    POWER_COIN,
    POWER_SHIELD,
    POWER_SPEED,
    POWER_LIFE
};

struct PowerUp {
    bool active;
    float x;
    float y;
    float speed;
    float phase;
    PowerUpType type;
};

struct ScorePopup {
    bool active;
    float x;
    float y;
    float timer;
    int value;
};

struct Particle {
    bool active;
    float x;
    float y;
    float vx;
    float vy;
    float life;
    float maxLife;
    float size;
};

const int MAX_POWERUPS = 4;
const int MAX_SCORE_POPUPS = 10;
const int MAX_PARTICLES = 80;

PowerUp powerUps[MAX_POWERUPS];
ScorePopup scorePopups[MAX_SCORE_POPUPS];
Particle particles[MAX_PARTICLES];

float powerUpSpawnTimer = 7.0f;
float comboTimer = 0.0f;
int comboCount = 0;
float shieldTimer = 0.0f;
float speedBoostTimer = 0.0f;
float gameOverTimer = 0.0f;
bool mouseDown = false;

const float POWERUP_DESPAWN_X = -100.0f;
const float POWERUP_SPEED = 95.0f;
const float POWERUP_RADIUS = 18.0f;
const float COMBO_WINDOW = 2.2f;
const float SHIELD_DURATION = 6.0f;
const float SPEED_BOOST_DURATION = 5.0f;
const float SPEED_BOOST_MULTIPLIER = 1.65f;
const float POWERUP_SPAWN_INTERVAL = 8.0f;
const float COUNTDOWN_STEP = 1.0f;


// ------------------------------------------------------------
// PROFESSIONAL MISSION / ACHIEVEMENT SYSTEM
// ------------------------------------------------------------
enum MissionType {
    MISSION_EAT_FISH = 0,
    MISSION_REACH_SCORE,
    MISSION_SURVIVE_TIME
};

struct MissionState {
    MissionType type;
    int target;
    bool completed;
};

MissionState currentMission;

const int ACHIEVEMENT_COUNT = 6;
const char* ACHIEVEMENT_NAMES[ACHIEVEMENT_COUNT] = {
    "FIRST CATCH",
    "COMBO MASTER",
    "SCORE 200",
    "SCORE 500",
    "SURVIVOR",
    "POWER HUNTER"
};

bool achievements[ACHIEVEMENT_COUNT] = {false, false, false, false, false, false};
float achievementPopupTimer = 0.0f;
int lastUnlockedAchievement = -1;

int totalPowerUpsCollected = 0;


void loadBestScore() {
    FILE* file = std::fopen(BEST_SCORE_FILE, "r");
    if (file) {
        if (std::fscanf(file, "%d", &bestScore) != 1) bestScore = 0;
        std::fclose(file);
    }
}

void saveBestScore() {
    FILE* file = std::fopen(BEST_SCORE_FILE, "w");
    if (file) {
        std::fprintf(file, "%d\n", bestScore);
        std::fclose(file);
    }
}

void resetPowerUpsAndEffects() {
    for (int i = 0; i < MAX_POWERUPS; ++i) powerUps[i].active = false;
    for (int i = 0; i < MAX_SCORE_POPUPS; ++i) scorePopups[i].active = false;
    for (int i = 0; i < MAX_PARTICLES; ++i) particles[i].active = false;
    powerUpSpawnTimer = POWERUP_SPAWN_INTERVAL;
    comboTimer = 0.0f;
    comboCount = 0;
    shieldTimer = 0.0f;
    speedBoostTimer = 0.0f;
}

void addScorePopup(float x, float y, int value) {
    for (int i = 0; i < MAX_SCORE_POPUPS; ++i) {
        if (!scorePopups[i].active) {
            scorePopups[i].active = true;
            scorePopups[i].x = x;
            scorePopups[i].y = y;
            scorePopups[i].timer = 1.0f;
            scorePopups[i].value = value;
            return;
        }
    }
}

void spawnParticles(float x, float y, int amount = 12) {
    int made = 0;
    for (int i = 0; i < MAX_PARTICLES && made < amount; ++i) {
        if (!particles[i].active) {
            float angle = randomFloat(0.0f, 2.0f * PI);
            float speed = randomFloat(35.0f, 115.0f);
            particles[i].active = true;
            particles[i].x = x;
            particles[i].y = y;
            particles[i].vx = std::cos(angle) * speed;
            particles[i].vy = std::sin(angle) * speed;
            particles[i].maxLife = randomFloat(0.35f, 0.75f);
            particles[i].life = particles[i].maxLife;
            particles[i].size = randomFloat(2.0f, 6.0f);
            ++made;
        }
    }
}

void updateEffects(float dt) {
    for (int i = 0; i < MAX_SCORE_POPUPS; ++i) {
        if (!scorePopups[i].active) continue;
        scorePopups[i].timer -= dt;
        scorePopups[i].y += 35.0f * dt;
        if (scorePopups[i].timer <= 0.0f) scorePopups[i].active = false;
    }

    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!particles[i].active) continue;
        particles[i].life -= dt;
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
        particles[i].vy -= 55.0f * dt;
        if (particles[i].life <= 0.0f) particles[i].active = false;
    }

    if (comboTimer > 0.0f) {
        comboTimer -= dt;
        if (comboTimer <= 0.0f) comboCount = 0;
    }

    if (shieldTimer > 0.0f) shieldTimer = std::max(0.0f, shieldTimer - dt);
    if (speedBoostTimer > 0.0f) speedBoostTimer = std::max(0.0f, speedBoostTimer - dt);
    if (achievementPopupTimer > 0.0f)
        achievementPopupTimer = std::max(0.0f, achievementPopupTimer - dt);
}

void drawEffects() {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!particles[i].active) continue;
        float alpha = clampFloat(particles[i].life / particles[i].maxLife, 0.0f, 1.0f);
        glColor4f(1.0f, 0.9f, 0.25f, alpha);
        drawCircle(particles[i].x, particles[i].y, particles[i].size, true, 12);
    }

    for (int i = 0; i < MAX_SCORE_POPUPS; ++i) {
        if (!scorePopups[i].active) continue;
        char text[30];
        std::sprintf(text, "+%d", scorePopups[i].value);
        float alpha = clampFloat(scorePopups[i].timer, 0.0f, 1.0f);
        glColor4f(1.0f, 0.95f, 0.20f, alpha);
        drawText(scorePopups[i].x, scorePopups[i].y, text, GLUT_BITMAP_HELVETICA_18);
    }
}

void drawPowerUpLocal(PowerUpType type) {
    if (type == POWER_COIN) {
        glColor3f(1.0f, 0.78f, 0.08f);
        drawCircle(0, 0, POWERUP_RADIUS, true, 28);
        glColor3f(0.55f, 0.30f, 0.02f);
        drawCircle(0, 0, POWERUP_RADIUS - 5, false, 28);
        glColor3f(0.25f, 0.15f, 0.02f);
        drawText(-5, -6, "$", GLUT_BITMAP_HELVETICA_18);
    } else if (type == POWER_SHIELD) {
        glColor3f(0.25f, 0.75f, 1.0f);
        drawCircle(0, 0, POWERUP_RADIUS, true, 28);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(0, 0, POWERUP_RADIUS - 5, false, 28);
        glBegin(GL_TRIANGLES);
        glVertex2f(0, 10); glVertex2f(-9, 2); glVertex2f(0, -11);
        glVertex2f(0, 10); glVertex2f(9, 2); glVertex2f(0, -11);
        glEnd();
    } else if (type == POWER_SPEED) {
        glColor3f(0.65f, 0.15f, 1.0f);
        drawCircle(0, 0, POWERUP_RADIUS, true, 28);
        glColor3f(1.0f, 0.95f, 0.20f);
        glBegin(GL_TRIANGLES);
        glVertex2f(2, 14); glVertex2f(-7, 1); glVertex2f(1, 1);
        glVertex2f(1, 1); glVertex2f(-2, -14); glVertex2f(8, 1);
        glEnd();
    } else if (type == POWER_LIFE) {
        glColor3f(1.0f, 0.15f, 0.25f);
        drawCircle(-6, 3, 8, true, 24);
        drawCircle(6, 3, 8, true, 24);
        glBegin(GL_TRIANGLES);
        glVertex2f(-14, 4); glVertex2f(14, 4); glVertex2f(0, -13);
        glEnd();
    }
}

void drawPowerUps() {
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (!powerUps[i].active) continue;
        float bob = std::sin(gameTime * 4.0f + powerUps[i].phase) * 4.0f;
        glPushMatrix();
        glTranslatef(powerUps[i].x, powerUps[i].y + bob, 0.0f);
        drawPowerUpLocal(powerUps[i].type);
        glPopMatrix();
    }
}

void spawnPowerUp() {
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (!powerUps[i].active) {
            powerUps[i].active = true;
            powerUps[i].x = WIDTH + randomFloat(50.0f, 180.0f);
            powerUps[i].y = randomFloat(190.0f, 535.0f);
            powerUps[i].speed = randomFloat(80.0f, 115.0f);
            powerUps[i].phase = randomFloat(0.0f, 2.0f * PI);
            int r = rand() % 100;
            if (r < 35) powerUps[i].type = POWER_COIN;
            else if (r < 60) powerUps[i].type = POWER_SHIELD;
            else if (r < 85) powerUps[i].type = POWER_SPEED;
            else powerUps[i].type = POWER_LIFE;
            return;
        }
    }
}

void updatePowerUps(float dt) {
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (!powerUps[i].active) continue;
        powerUps[i].x -= powerUps[i].speed * dt;
        if (powerUps[i].x < POWERUP_DESPAWN_X) powerUps[i].active = false;
    }

    powerUpSpawnTimer -= dt;
    if (powerUpSpawnTimer <= 0.0f) {
        spawnPowerUp();
        powerUpSpawnTimer = randomFloat(6.0f, 10.0f);
    }
}

void collectPowerUp(PowerUpType type, float x, float y) {
    spawnParticles(x, y, 18);
    ++totalPowerUpsCollected;

    if (type == POWER_COIN) {
        gameState.score += 50;
        ++gameState.coinsCollected;
        addScorePopup(x, y, 50);
    } else if (type == POWER_SHIELD) {
        shieldTimer = SHIELD_DURATION;
    } else if (type == POWER_SPEED) {
        speedBoostTimer = SPEED_BOOST_DURATION;
    } else if (type == POWER_LIFE) {
        gameState.lives = std::min(STARTING_LIVES, gameState.lives + 1);
        addScorePopup(x, y, 1);
    }

    if (gameState.score > bestScore) {
        bestScore = gameState.score;
        saveBestScore();
    }

    updateMissionAndAchievements();
}

void handlePowerUpCollisions() {
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (!powerUps[i].active) continue;
        if (rectanglesOverlap(player.x, player.y, PLAYER_COLLISION_HALF_W,
                               PLAYER_COLLISION_HALF_H,
                               powerUps[i].x, powerUps[i].y,
                               POWERUP_RADIUS, POWERUP_RADIUS)) {
            collectPowerUp(powerUps[i].type, powerUps[i].x, powerUps[i].y);
            powerUps[i].active = false;
        }
    }
}

void startCountdown() {
    countdownActive = true;
    countdownValue = 3;
    countdownTimer = COUNTDOWN_STEP;
}

void updateCountdown(float dt) {
    if (!countdownActive) return;
    countdownTimer -= dt;
    if (countdownTimer <= 0.0f) {
        --countdownValue;
        if (countdownValue <= 0) {
            countdownActive = false;
            gameStarted = true;
            showMainMenu = false;
            showHowToPlay = false;
            restartGame();
        } else {
            countdownTimer = COUNTDOWN_STEP;
        }
    }
}

void drawCountdown() {
    if (!countdownActive) return;
    char text[10];
    if (countdownValue > 0) std::sprintf(text, "%d", countdownValue);
    else std::sprintf(text, "GO!");
    glColor3f(1.0f, 0.9f, 0.15f);
    drawText(535, 365, text, GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawButton(float left, float bottom, float right, float top, const char* label) {
    glColor4f(0.02f, 0.18f, 0.32f, 0.90f);
    glBegin(GL_QUADS);
    glVertex2f(left, bottom); glVertex2f(right, bottom);
    glVertex2f(right, top); glVertex2f(left, top);
    glEnd();

    glColor3f(0.30f, 0.90f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(left, bottom); glVertex2f(right, bottom);
    glVertex2f(right, top); glVertex2f(left, top);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText((left + right) * 0.5f - 45.0f, (bottom + top) * 0.5f - 6.0f,
             label, GLUT_BITMAP_HELVETICA_18);
}

void drawProfessionalMainMenu() {
    drawMenuPanel(250, 110, 850, 625);

    glColor3f(1.0f, 0.85f, 0.15f);
    drawText(405, 545, "OCEAN HUNTER", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(0.70f, 0.95f, 1.0f);
    drawText(390, 505, "FISH EATING ADVENTURE");

    drawButton(400, 415, 700, 465, "PLAY GAME");
    drawButton(400, 340, 700, 390, "HOW TO PLAY");
    drawButton(400, 265, 700, 315, "EXIT");

    char bestText[50];
    std::sprintf(bestText, "Best Score: %d", bestScore);
    glColor3f(1.0f, 0.95f, 0.30f);
    drawText(465, 200, bestText);

    glColor3f(0.75f, 0.90f, 1.0f);
    drawText(375, 145, "WASD Move  |  P Pause  |  R Restart");
}

void drawHowToPlay() {
    drawMenuPanel(230, 105, 870, 635);

    glColor3f(1.0f, 0.85f, 0.15f);
    drawText(430, 550, "HOW TO PLAY", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(315, 495, "Eat colorful food fish to gain +10 score.");
    drawText(315, 455, "Avoid the purple/green monster fish.");
    drawText(315, 415, "Reach 200 and 400 points to increase difficulty.");
    drawText(315, 375, "Collect power-ups for special abilities.");

    glColor3f(1.0f, 0.85f, 0.20f);
    drawText(315, 325, "POWER-UPS");
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(345, 290, "Coin   = +50 score");
    drawText(345, 260, "Shield = temporary protection");
    drawText(345, 230, "Speed  = temporary speed boost");
    drawText(345, 200, "Heart  = restore one life");

    drawButton(430, 135, 670, 180, "BACK");
}

void drawInGamePowerStatus() {
    int x = 25;
    if (shieldTimer > 0.0f) {
        char text[40];
        std::sprintf(text, "Shield: %.1fs", shieldTimer);
        glColor3f(0.40f, 0.90f, 1.0f);
        drawText(x, 615, text, GLUT_BITMAP_HELVETICA_12);
        x += 120;
    }
    if (speedBoostTimer > 0.0f) {
        char text[40];
        std::sprintf(text, "Speed: %.1fs", speedBoostTimer);
        glColor3f(0.80f, 0.40f, 1.0f);
        drawText(x, 615, text, GLUT_BITMAP_HELVETICA_12);
    }
    if (comboCount > 1) {
        char text[40];
        std::sprintf(text, "COMBO x%d", comboCount);
        glColor3f(1.0f, 0.55f, 0.15f);
        drawText(500, 615, text, GLUT_BITMAP_HELVETICA_18);
    }
}

void drawBestScoreHUD() {
    char text[50];
    std::sprintf(text, "Best: %d", bestScore);
    glColor3f(1.0f, 0.95f, 0.35f);
    drawText(760, 650, text, GLUT_BITMAP_HELVETICA_12);
}

// Background music is optional.
// Put music.wav beside the EXE if you want music.
// If the file is missing, the game still works.

// ============================================================
// GENERIC HELPERS
// ============================================================
float randomFloat(float minValue, float maxValue) {
    return minValue +
           (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) *
           (maxValue - minValue);
}

float clampFloat(float value, float minValue, float maxValue) {
    return std::max(minValue, std::min(value, maxValue));
}

float moveTowards(float current, float target, float maxChange) {
    if (current < target) {
        return std::min(current + maxChange, target);
    }
    if (current > target) {
        return std::max(current - maxChange, target);
    }
    return target;
}

void clearMovementInput() {
    inputState.left = false;
    inputState.right = false;
    inputState.up = false;
    inputState.down = false;
    inputState.horizontalPriority = 0;
    inputState.verticalPriority = 0;
}

void drawCircle(float x, float y, float radius, bool fill, int segments) {
    glBegin(fill ? GL_POLYGON : GL_LINE_LOOP);

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * PI * static_cast<float>(i) /
                      static_cast<float>(segments);
        glVertex2f(x + std::cos(angle) * radius,
                   y + std::sin(angle) * radius);
    }

    glEnd();
}

void drawLocalCircle(float x, float y, float radius, int segments = 32) {
    glBegin(GL_POLYGON);

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * PI * static_cast<float>(i) /
                      static_cast<float>(segments);
        glVertex2f(x + std::cos(angle) * radius,
                   y + std::sin(angle) * radius);
    }

    glEnd();
}

void drawText(float x, float y, const char* text,
              void* font) {
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; ++i) {
        glutBitmapCharacter(font, text[i]);
    }
}

bool rectanglesOverlap(float ax, float ay, float aHalfW, float aHalfH,
                       float bx, float by, float bHalfW, float bHalfH) {
    return std::fabs(ax - bx) <= (aHalfW + bHalfW) &&
           std::fabs(ay - by) <= (aHalfH + bHalfH);
}

int countActiveFoodFish() {
    int count = 0;
    for (int i = 0; i < MAX_FOOD_FISH; ++i) {
        if (foodFish[i].active) {
            ++count;
        }
    }
    return count;
}

int countActiveEnemyFish() {
    int count = 0;
    for (int i = 0; i < MAX_ENEMY_FISH; ++i) {
        if (enemyFish[i].active) {
            ++count;
        }
    }
    return count;
}

// ============================================================
// PART 1 - TAMJID NILOY'S AQUARIUM BACKGROUND
// ============================================================
void drawWater() {
    glBegin(GL_QUADS);

    glColor3f(0.18f, 0.72f, 0.92f);
    glVertex2f(0, HEIGHT);
    glVertex2f(WIDTH, HEIGHT);

    glColor3f(0.02f, 0.35f, 0.65f);
    glVertex2f(WIDTH, 0);
    glVertex2f(0, 0);

    glEnd();
}

void drawWave() {
    glLineWidth(3.0f);
    glColor3f(0.75f, 0.95f, 1.0f);
    glBegin(GL_LINE_STRIP);

    for (int x = 0; x <= WIDTH; x += WAVE_SAMPLE_STEP) {
        const float xf = static_cast<float>(x);
        float y = WAVE_1_BASE_Y
                + std::sin(xf * WAVE_1_A_FREQ + gameTime * WAVE_1_A_TIME) * WAVE_1_A_AMP
                + std::sin(xf * WAVE_1_B_FREQ - gameTime * WAVE_1_B_TIME) * WAVE_1_B_AMP
                + std::sin(xf * WAVE_1_C_FREQ + gameTime * WAVE_1_C_TIME) * WAVE_1_C_AMP;
        glVertex2f(xf, y);
    }

    glEnd();

    glColor3f(0.45f, 0.82f, 0.95f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);

    for (int x = 0; x <= WIDTH; x += WAVE_SAMPLE_STEP) {
        const float xf = static_cast<float>(x);
        float y = WAVE_2_BASE_Y
                + std::sin(xf * WAVE_2_A_FREQ + gameTime * WAVE_2_A_TIME) * WAVE_2_A_AMP
                + std::sin(xf * WAVE_2_B_FREQ - gameTime * WAVE_2_B_TIME) * WAVE_2_B_AMP;
        glVertex2f(xf, y);
    }

    glEnd();
    glLineWidth(1.0f);
}

void initializeBubbles() {
    for (int i = 0; i < TOTAL_BUBBLES; ++i) {
        bubbles[i].x = randomFloat(0.0f, static_cast<float>(WIDTH));
        bubbles[i].y = randomFloat(0.0f, BUBBLE_INITIAL_Y_MAX);
        bubbles[i].radius = randomFloat(BUBBLE_RADIUS_MIN, BUBBLE_RADIUS_MAX);
        bubbles[i].speed = randomFloat(BUBBLE_SPEED_MIN, BUBBLE_SPEED_MAX);
        bubbles[i].driftPhase = randomFloat(0.0f, 2.0f * PI);
    }
}

void updateBubbles(float deltaTime) {
    for (int i = 0; i < TOTAL_BUBBLES; ++i) {
        bubbles[i].y += bubbles[i].speed * deltaTime;
        bubbles[i].x += std::sin(gameTime * BUBBLE_DRIFT_FREQUENCY + bubbles[i].driftPhase) *
                        BUBBLE_DRIFT_SPEED * deltaTime;

        if (bubbles[i].y > HEIGHT + bubbles[i].radius) {
            bubbles[i].y = -bubbles[i].radius;
            bubbles[i].x = randomFloat(0.0f, static_cast<float>(WIDTH));
        }

        if (bubbles[i].x < -BUBBLE_HORIZONTAL_WRAP_MARGIN) {
            bubbles[i].x = WIDTH + BUBBLE_HORIZONTAL_WRAP_MARGIN;
        } else if (bubbles[i].x > WIDTH + BUBBLE_HORIZONTAL_WRAP_MARGIN) {
            bubbles[i].x = -BUBBLE_HORIZONTAL_WRAP_MARGIN;
        }
    }
}

void drawBubbles() {
    glColor3f(0.80f, 0.95f, 1.0f);
    glLineWidth(1.0f);

    for (int i = 0; i < TOTAL_BUBBLES; ++i) {
        drawCircle(bubbles[i].x, bubbles[i].y, bubbles[i].radius, false, 28);
    }
}

void drawSoil() {
    glColor3f(0.70f, 0.50f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WIDTH, 0);
    glVertex2f(WIDTH, SOIL_HEIGHT);
    glVertex2f(0, SOIL_HEIGHT);
    glEnd();
}

void initializeGrass() {
    for (int i = 0; i < TOTAL_GRASS; ++i) {
        grassPhase[i] = randomFloat(0.0f, GRASS_PHASE_MAX);
    }
}

void drawGrassLeaf(float x, float move) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x - GRASS_HALF_WIDTH, SOIL_HEIGHT);
    glVertex2f(x + GRASS_HALF_WIDTH, SOIL_HEIGHT);
    glVertex2f(x + move, GRASS_TOP_Y);
    glEnd();
}

void drawGrass() {
    glColor3f(0.0f, 0.55f, 0.15f);

    for (int i = 0; i < TOTAL_GRASS; ++i) {
        float x = GRASS_START_X + static_cast<float>(i) * GRASS_SPACING_X;
        float move = std::sin(gameTime * GRASS_SWAY_SPEED + grassPhase[i]) *
                     GRASS_SWAY_AMOUNT;

        drawGrassLeaf(x, move);
        drawGrassLeaf(x + GRASS_SECOND_LEAF_OFFSET_X, move);
    }
}

void drawStone(float x, float y) {
    glColor3f(0.35f, 0.35f, 0.35f);

    glBegin(GL_POLYGON);
    glVertex2f(x - 40, y);
    glVertex2f(x - 30, y + 18);
    glVertex2f(x - 15, y + 28);
    glVertex2f(x + 15, y + 28);
    glVertex2f(x + 35, y + 15);
    glVertex2f(x + 40, y);
    glVertex2f(x + 25, y - 12);
    glVertex2f(x - 25, y - 12);
    glEnd();
}

void drawStones() {
    drawStone(STONE_1_X, STONE_Y);
    drawStone(STONE_2_X, STONE_Y);
    drawStone(STONE_3_X, STONE_Y);
}

// ============================================================
// PART 2 - FOOD FISH VISUAL + GAME SYSTEM
// ============================================================
void setFoodFishColor(int color) {
    if (color == 0) glColor3f(1.00f, 0.25f, 0.25f);
    else if (color == 1) glColor3f(1.00f, 0.82f, 0.10f);
    else if (color == 2) glColor3f(0.20f, 0.80f, 0.35f);
    else if (color == 3) glColor3f(0.20f, 0.55f, 1.00f);
    else glColor3f(0.35f, 0.18f, 0.70f);
}

void setFoodFinColor(int color) {
    if (color == 0) glColor3f(0.85f, 0.12f, 0.12f);
    else if (color == 1) glColor3f(0.95f, 0.60f, 0.05f);
    else if (color == 2) glColor3f(0.10f, 0.60f, 0.20f);
    else if (color == 3) glColor3f(0.10f, 0.35f, 0.85f);
    else glColor3f(0.25f, 0.08f, 0.50f);
}

void drawFoodEye() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-35, 7);
    glVertex2f(-32, 14);
    glVertex2f(-24, 15);
    glVertex2f(-19, 10);
    glVertex2f(-19, 4);
    glVertex2f(-24, -1);
    glVertex2f(-32, 0);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-29, 7);
    glVertex2f(-27, 11);
    glVertex2f(-23, 9);
    glVertex2f(-23, 5);
    glVertex2f(-27, 3);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glVertex2f(-26, 9);
    glEnd();
}

void drawFoodFishLocal(int color) {
    // Main body - based on teammate food-fish geometry.
    setFoodFishColor(color);
    glBegin(GL_POLYGON);
    glVertex2f(-45, 0);
    glVertex2f(-38, 16);
    glVertex2f(-22, 25);
    glVertex2f(0, 30);
    glVertex2f(25, 25);
    glVertex2f(42, 12);
    glVertex2f(48, 0);
    glVertex2f(42, -12);
    glVertex2f(25, -25);
    glVertex2f(0, -30);
    glVertex2f(-22, -25);
    glVertex2f(-38, -16);
    glEnd();

    // Tail.
    setFoodFinColor(color);
    glBegin(GL_TRIANGLES);
    glVertex2f(42, 0);
    glVertex2f(80, 32);
    glVertex2f(80, -32);
    glEnd();

    // Top fin.
    glBegin(GL_TRIANGLES);
    glVertex2f(-8, 24);
    glVertex2f(8, 45);
    glVertex2f(22, 20);
    glEnd();

    // Bottom fin.
    glBegin(GL_TRIANGLES);
    glVertex2f(-4, -23);
    glVertex2f(12, -43);
    glVertex2f(25, -18);
    glEnd();

    // Side fin.
    glBegin(GL_TRIANGLES);
    glVertex2f(-12, 3);
    glVertex2f(4, 13);
    glVertex2f(5, -2);
    glEnd();

    // Highlight.
    glColor3f(1.0f, 0.95f, 0.75f);
    glBegin(GL_POLYGON);
    glVertex2f(-12, 10);
    glVertex2f(5, 14);
    glVertex2f(20, 10);
    glVertex2f(12, 4);
    glVertex2f(-8, 4);
    glEnd();

    drawFoodEye();

    // Gill.
    glColor3f(0.15f, 0.15f, 0.15f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-15, 10);
    glVertex2f(-19, -9);
    glEnd();

    // Mouth.
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(-43, -7);
    glVertex2f(-33, -10);
    glEnd();
    glLineWidth(1.0f);
}

void drawFoodFish(const FoodFish& fish) {
    float bob = std::sin(gameTime * FOOD_BOB_SPEED + fish.swimPhase) *
                FOOD_BOB_AMOUNT;

    glPushMatrix();
    glTranslatef(fish.x, fish.y + bob, 0.0f);
    glScalef(FOOD_VISUAL_SCALE, FOOD_VISUAL_SCALE, 1.0f);
    drawFoodFishLocal(fish.color);
    glPopMatrix();
}

void initializeFoodFishSystem() {
    for (int i = 0; i < MAX_FOOD_FISH; ++i) {
        foodFish[i].active = false;
        foodFish[i].x = 0.0f;
        foodFish[i].y = 0.0f;
        foodFish[i].speedFactor = 1.0f;
        foodFish[i].color = 0;
        foodFish[i].swimPhase = 0.0f;
    }
}

void spawnFoodFish() {
    const LevelConfig& config = LEVELS[gameState.level];

    if (countActiveFoodFish() >= config.maxActiveFood) {
        return;
    }

    for (int i = 0; i < MAX_FOOD_FISH; ++i) {
        if (!foodFish[i].active) {
            foodFish[i].active = true;
            foodFish[i].x = WIDTH + randomFloat(FOOD_SPAWN_X_OFFSET_MIN, FOOD_SPAWN_X_OFFSET_MAX);
            foodFish[i].y = randomFloat(FOOD_SPAWN_Y_MIN, FOOD_SPAWN_Y_MAX);
            foodFish[i].speedFactor = randomFloat(FOOD_SPEED_FACTOR_MIN, FOOD_SPEED_FACTOR_MAX);
            foodFish[i].color = rand() % FOOD_COLOR_COUNT;
            foodFish[i].swimPhase = randomFloat(0.0f, 2.0f * PI);
            return;
        }
    }
}

void updateFoodFish(float deltaTime) {
    const LevelConfig& config = LEVELS[gameState.level];

    for (int i = 0; i < MAX_FOOD_FISH; ++i) {
        if (!foodFish[i].active) {
            continue;
        }

        foodFish[i].x -= config.foodSpeedPixelsPerSecond *
                            foodFish[i].speedFactor * deltaTime;

        if (foodFish[i].x < FOOD_DESPAWN_X) {
            foodFish[i].active = false;
        }
    }

    gameState.foodSpawnTimer -= deltaTime;

    if (gameState.foodSpawnTimer <= 0.0f) {
        spawnFoodFish();
        gameState.foodSpawnTimer = config.foodSpawnInterval *
                                   randomFloat(FOOD_SPAWN_INTERVAL_FACTOR_MIN, FOOD_SPAWN_INTERVAL_FACTOR_MAX);
    }
}

void drawAllFoodFish() {
    for (int i = 0; i < MAX_FOOD_FISH; ++i) {
        if (foodFish[i].active) {
            drawFoodFish(foodFish[i]);
        }
    }
}

// ============================================================
// PART 3 - PLAYER FISH VISUAL + MOVEMENT
// ============================================================
void drawPlayerFishLocal() {
    // Tail - converted from teammate player's normalized coordinates.
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.35f, 0.12f);
    glVertex2f(-0.32f, 0.10f);
    glVertex2f(-0.72f, 0.38f);
    glVertex2f(-0.65f, 0.08f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.48f, 0.15f);
    glVertex2f(-0.32f, 0.10f);
    glVertex2f(-0.65f, 0.08f);
    glVertex2f(-0.72f, -0.20f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.65f, 0.20f);
    glVertex2f(-0.37f, 0.10f);
    glVertex2f(-0.65f, 0.30f);
    glVertex2f(-0.58f, 0.10f);
    glEnd();

    // Body.
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.48f, 0.12f);
    glVertex2f(-0.40f, 0.10f);
    glVertex2f(-0.34f, 0.22f);
    glVertex2f(-0.20f, 0.31f);
    glVertex2f(0.02f, 0.34f);
    glVertex2f(0.20f, 0.30f);
    glVertex2f(0.34f, 0.21f);
    glVertex2f(0.43f, 0.08f);
    glVertex2f(0.44f, -0.02f);
    glVertex2f(0.35f, -0.13f);
    glVertex2f(0.18f, -0.21f);
    glVertex2f(-0.05f, -0.23f);
    glVertex2f(-0.26f, -0.18f);
    glVertex2f(-0.37f, -0.07f);
    glEnd();

    // Belly.
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.72f, 0.30f);
    glVertex2f(-0.27f, -0.08f);
    glVertex2f(-0.10f, -0.16f);
    glVertex2f(0.12f, -0.17f);
    glVertex2f(0.28f, -0.10f);
    glVertex2f(0.35f, -0.02f);
    glVertex2f(0.20f, -0.06f);
    glVertex2f(0.00f, -0.10f);
    glVertex2f(-0.18f, -0.08f);
    glEnd();

    // Top fin.
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.28f, 0.08f);
    glVertex2f(-0.08f, 0.31f);
    glVertex2f(0.00f, 0.58f);
    glVertex2f(0.16f, 0.31f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.60f, 0.18f);
    glVertex2f(-0.03f, 0.34f);
    glVertex2f(0.01f, 0.51f);
    glVertex2f(0.08f, 0.34f);
    glEnd();

    // Bottom fin.
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.30f, 0.08f);
    glVertex2f(0.00f, -0.20f);
    glVertex2f(0.10f, -0.46f);
    glVertex2f(0.22f, -0.17f);
    glEnd();

    // Side fin.
    glBegin(GL_TRIANGLES);
    glColor3f(0.95f, 0.35f, 0.10f);
    glVertex2f(-0.02f, 0.02f);
    glVertex2f(-0.18f, -0.15f);
    glVertex2f(0.12f, -0.08f);
    glEnd();

    // Scale marks.
    glColor4f(0.85f, 0.25f, 0.05f, 0.45f);
    glLineWidth(1.0f);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            float x = -0.18f + static_cast<float>(col) * 0.10f;
            float y = 0.10f - static_cast<float>(row) * 0.08f;

            glBegin(GL_LINE_STRIP);
            glVertex2f(x - 0.025f, y);
            glVertex2f(x, y - 0.025f);
            glVertex2f(x + 0.025f, y);
            glEnd();
        }
    }

    // Eye.
    glColor3f(1.0f, 1.0f, 1.0f);
    drawLocalCircle(0.27f, 0.17f, 0.065f, 40);

    glColor3f(0.02f, 0.02f, 0.02f);
    drawLocalCircle(0.285f, 0.17f, 0.035f, 30);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawLocalCircle(0.298f, 0.185f, 0.010f, 20);

    // Mouth and smile.
    glLineWidth(2.0f);
    glColor3f(0.55f, 0.08f, 0.03f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0.40f, 0.04f);
    glVertex2f(0.44f, 0.015f);
    glVertex2f(0.40f, -0.015f);
    glEnd();

    glColor3f(0.75f, 0.18f, 0.05f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0.37f, -0.01f);
    glVertex2f(0.40f, -0.025f);
    glVertex2f(0.42f, -0.018f);
    glEnd();
    glLineWidth(1.0f);
}

void resetPlayerAfterHit() {
    player.x = PLAYER_START_X;
    player.y = PLAYER_START_Y;
    player.velocityX = 0.0f;
    player.velocityY = 0.0f;
    player.direction = 1;
    player.facingScaleX = 1.0f;
    player.shakeTime = PLAYER_HIT_SHAKE_DURATION;
    player.invulnerableTime = PLAYER_INVULNERABLE_DURATION;
}

void initializePlayer() {
    player.x = PLAYER_START_X;
    player.y = PLAYER_START_Y;
    player.velocityX = 0.0f;
    player.velocityY = 0.0f;
    player.direction = 1;
    player.facingScaleX = 1.0f;
    player.shakeTime = 0.0f;
    player.invulnerableTime = 0.0f;
}

void updatePlayerTimers(float deltaTime) {
    if (player.shakeTime > 0.0f) {
        player.shakeTime = std::max(0.0f, player.shakeTime - deltaTime);
    }

    if (player.invulnerableTime > 0.0f) {
        player.invulnerableTime =
            std::max(0.0f, player.invulnerableTime - deltaTime);
    }
}

void updatePlayerMovement(float deltaTime) {
    // Build a direction vector from the currently held WASD keys.
    float inputX = 0.0f;
    float inputY = 0.0f;

    if (inputState.left && inputState.right) {
        inputX = static_cast<float>(inputState.horizontalPriority);
    } else if (inputState.left) {
        inputX = -1.0f;
    } else if (inputState.right) {
        inputX = 1.0f;
    }

    if (inputState.down && inputState.up) {
        inputY = static_cast<float>(inputState.verticalPriority);
    } else if (inputState.down) {
        inputY = -1.0f;
    } else if (inputState.up) {
        inputY = 1.0f;
    }

    // Diagonal movement is normalized so it is not faster than horizontal
    // or vertical movement.
    const float inputLength = std::sqrt(inputX * inputX + inputY * inputY);
    if (inputLength > 1.0f) {
        inputX /= inputLength;
        inputY /= inputLength;
    }

    // Facing reacts immediately to horizontal intent. The visible model then
    // performs a very short smooth 2D turn instead of popping/jittering.
    if (inputX < -PLAYER_INPUT_DEAD_ZONE) {
        player.direction = -1;
    } else if (inputX > PLAYER_INPUT_DEAD_ZONE) {
        player.direction = 1;
    }

    const bool movementLocked = gameState.gameOver || player.shakeTime > 0.0f;

    if (movementLocked) {
        // A hit/game-over must not leave residual momentum that slides the
        // fish while it is supposed to be recovering or frozen.
        player.velocityX = 0.0f;
        player.velocityY = 0.0f;

        const float targetFacingScale = static_cast<float>(player.direction);
        player.facingScaleX = moveTowards(
            player.facingScaleX, targetFacingScale,
            PLAYER_VISUAL_TURN_SPEED * deltaTime);
        return;
    }

    const float currentMaxSpeed = PLAYER_MAX_SPEED *
        (speedBoostTimer > 0.0f ? SPEED_BOOST_MULTIPLIER : 1.0f);
    const float targetVelocityX = inputX * currentMaxSpeed;
    const float targetVelocityY = inputY * currentMaxSpeed;

    const bool hasHorizontalInput = std::fabs(inputX) > PLAYER_INPUT_DEAD_ZONE;
    const bool hasVerticalInput = std::fabs(inputY) > PLAYER_INPUT_DEAD_ZONE;
    const bool reversingX = hasHorizontalInput &&
                            player.velocityX * targetVelocityX < 0.0f;
    const bool reversingY = hasVerticalInput &&
                            player.velocityY * targetVelocityY < 0.0f;

    const float horizontalRate = !hasHorizontalInput
        ? PLAYER_DECELERATION
        : (reversingX ? PLAYER_REVERSE_ACCELERATION : PLAYER_ACCELERATION);
    const float verticalRate = !hasVerticalInput
        ? PLAYER_DECELERATION
        : (reversingY ? PLAYER_REVERSE_ACCELERATION : PLAYER_ACCELERATION);

    player.velocityX = moveTowards(
        player.velocityX, targetVelocityX, horizontalRate * deltaTime);
    player.velocityY = moveTowards(
        player.velocityY, targetVelocityY, verticalRate * deltaTime);

    player.x += player.velocityX * deltaTime;
    player.y += player.velocityY * deltaTime;

    const float clampedX = clampFloat(player.x, PLAYER_MIN_X, PLAYER_MAX_X);
    const float clampedY = clampFloat(player.y, PLAYER_MIN_Y, PLAYER_MAX_Y);

    if (clampedX != player.x) {
        player.velocityX = 0.0f;
        player.x = clampedX;
    }
    if (clampedY != player.y) {
        player.velocityY = 0.0f;
        player.y = clampedY;
    }

    const float targetFacingScale = static_cast<float>(player.direction);
    player.facingScaleX = moveTowards(
        player.facingScaleX, targetFacingScale,
        PLAYER_VISUAL_TURN_SPEED * deltaTime);
}

void updatePlayer(float deltaTime) {
    updatePlayerTimers(deltaTime);
    updatePlayerMovement(deltaTime);
}

void drawPlayerFish() {
    float shakeX = 0.0f;
    float shakeY = 0.0f;

    if (player.shakeTime > 0.0f) {
        shakeX = std::sin(gameTime * PLAYER_SHAKE_X_FREQUENCY) *
                 PLAYER_SHAKE_X_AMOUNT;
        shakeY = std::cos(gameTime * PLAYER_SHAKE_Y_FREQUENCY) *
                 PLAYER_SHAKE_Y_AMOUNT;
    }

    glPushMatrix();
    glTranslatef(player.x + shakeX, player.y + shakeY, 0.0f);

    // Smoothly compresses through X=0 then expands in the opposite
    // direction, which visually reads as a quick fish turn.
    glScalef(player.facingScaleX, 1.0f, 1.0f);
    glScalef(PLAYER_VISUAL_SCALE, PLAYER_VISUAL_SCALE, 1.0f);
    drawPlayerFishLocal();
    glPopMatrix();
}

// ============================================================
// PART 4 - ENEMY / MONSTER FISH VISUAL + GAME SYSTEM
// ============================================================
void drawEnemyFishLocal() {
    // Top fin.
    glBegin(GL_TRIANGLES);
    glColor3f(0.18f, 0.05f, 0.38f);
    glVertex2f(-0.20f, 0.20f);
    glVertex2f(-0.02f, 0.50f);
    glVertex2f(0.15f, 0.20f);
    glEnd();

    // Bottom fin.
    glBegin(GL_TRIANGLES);
    glColor3f(0.13f, 0.03f, 0.28f);
    glVertex2f(-0.20f, -0.20f);
    glVertex2f(-0.02f, -0.48f);
    glVertex2f(0.15f, -0.20f);
    glEnd();

    // Side fin.
    glBegin(GL_TRIANGLES);
    glColor3f(0.25f, 0.07f, 0.50f);
    glVertex2f(-0.25f, -0.02f);
    glVertex2f(-0.50f, -0.30f);
    glVertex2f(-0.06f, -0.13f);
    glEnd();

    // Monster tail from teammate source.
    glBegin(GL_POLYGON);
    glColor3f(0.16f, 0.04f, 0.32f);
    glVertex2f(0.34f, 0.12f);
    glColor3f(0.38f, 0.12f, 0.62f);
    glVertex2f(0.92f, 0.34f);
    glColor3f(0.28f, 0.07f, 0.50f);
    glVertex2f(0.70f, 0.04f);
    glColor3f(0.18f, 0.03f, 0.34f);
    glVertex2f(0.62f, 0.00f);
    glColor3f(0.24f, 0.05f, 0.44f);
    glVertex2f(0.70f, -0.04f);
    glColor3f(0.34f, 0.10f, 0.56f);
    glVertex2f(0.92f, -0.34f);
    glColor3f(0.13f, 0.025f, 0.26f);
    glVertex2f(0.34f, -0.12f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.56f, 0.24f, 0.80f);
    glVertex2f(0.39f, 0.09f);
    glVertex2f(0.84f, 0.28f);
    glVertex2f(0.69f, 0.04f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.42f, 0.14f, 0.68f);
    glVertex2f(0.39f, -0.09f);
    glVertex2f(0.84f, -0.28f);
    glVertex2f(0.69f, -0.04f);
    glEnd();

    // Horns.
    glBegin(GL_TRIANGLES);
    glColor3f(0.12f, 0.02f, 0.20f);
    glVertex2f(-0.48f, 0.15f);
    glVertex2f(-0.60f, 0.36f);
    glVertex2f(-0.34f, 0.19f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.18f, 0.03f, 0.28f);
    glVertex2f(-0.29f, 0.23f);
    glVertex2f(-0.20f, 0.45f);
    glVertex2f(-0.08f, 0.22f);
    glEnd();

    // Main monster body.
    glBegin(GL_POLYGON);
    glColor3f(0.05f, 0.20f, 0.12f);
    glVertex2f(-0.62f, 0.00f);
    glColor3f(0.08f, 0.29f, 0.17f);
    glVertex2f(-0.51f, 0.17f);
    glColor3f(0.15f, 0.45f, 0.24f);
    glVertex2f(-0.25f, 0.28f);
    glColor3f(0.22f, 0.56f, 0.30f);
    glVertex2f(0.05f, 0.28f);
    glColor3f(0.16f, 0.44f, 0.24f);
    glVertex2f(0.26f, 0.21f);
    glColor3f(0.12f, 0.35f, 0.20f);
    glVertex2f(0.39f, 0.12f);
    glColor3f(0.10f, 0.28f, 0.17f);
    glVertex2f(0.43f, 0.00f);
    glColor3f(0.09f, 0.27f, 0.17f);
    glVertex2f(0.39f, -0.12f);
    glColor3f(0.07f, 0.31f, 0.18f);
    glVertex2f(0.26f, -0.21f);
    glColor3f(0.05f, 0.23f, 0.14f);
    glVertex2f(0.05f, -0.28f);
    glColor3f(0.04f, 0.20f, 0.12f);
    glVertex2f(-0.25f, -0.28f);
    glColor3f(0.035f, 0.15f, 0.10f);
    glVertex2f(-0.51f, -0.17f);
    glEnd();

    // Monster eye.
    glColor3f(0.10f, 0.01f, 0.03f);
    drawLocalCircle(-0.34f, 0.105f, 0.085f, 40);
    glColor3f(1.00f, 0.86f, 0.20f);
    drawLocalCircle(-0.34f, 0.105f, 0.063f, 40);
    glColor3f(1.00f, 0.34f, 0.02f);
    drawLocalCircle(-0.34f, 0.105f, 0.043f, 40);
    glColor3f(0.01f, 0.01f, 0.01f);
    drawLocalCircle(-0.34f, 0.105f, 0.020f, 30);

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.90f);
    glVertex2f(-0.355f, 0.125f);
    glEnd();

    // Mouth.
    glBegin(GL_POLYGON);
    glColor3f(0.07f, 0.01f, 0.03f);
    glVertex2f(-0.59f, 0.02f);
    glVertex2f(-0.53f, -0.06f);
    glVertex2f(-0.47f, -0.10f);
    glVertex2f(-0.39f, -0.08f);
    glVertex2f(-0.41f, -0.01f);
    glVertex2f(-0.51f, 0.05f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.58f, 0.03f, 0.06f);
    glVertex2f(-0.54f, -0.005f);
    glVertex2f(-0.40f, -0.075f);
    glVertex2f(-0.47f, -0.032f);
    glEnd();

    // Teeth.
    glBegin(GL_TRIANGLES);
    glColor3f(1.00f, 0.94f, 0.76f);
    glVertex2f(-0.55f, 0.00f);
    glVertex2f(-0.51f, -0.015f);
    glVertex2f(-0.53f, -0.075f);
    glVertex2f(-0.51f, -0.015f);
    glVertex2f(-0.47f, -0.035f);
    glVertex2f(-0.49f, -0.085f);
    glVertex2f(-0.47f, -0.035f);
    glVertex2f(-0.43f, -0.055f);
    glVertex2f(-0.45f, -0.090f);
    glVertex2f(-0.43f, -0.055f);
    glVertex2f(-0.40f, -0.065f);
    glVertex2f(-0.415f, -0.092f);
    glEnd();

    // Gills / body marks.
    glLineWidth(3.0f);
    glColor3f(0.50f, 0.82f, 0.58f);
    glBegin(GL_LINES);
    glVertex2f(-0.14f, 0.08f);
    glVertex2f(-0.14f, -0.08f);
    glVertex2f(-0.09f, 0.07f);
    glVertex2f(-0.09f, -0.07f);
    glEnd();
    glLineWidth(1.0f);
}

void drawEnemyFish(const EnemyFish& fish) {
    float bob = std::sin(gameTime * ENEMY_BOB_SPEED + fish.swimPhase) *
                ENEMY_BOB_AMOUNT;

    glPushMatrix();
    glTranslatef(fish.x, fish.y + bob, 0.0f);
    glScalef(ENEMY_VISUAL_SCALE, ENEMY_VISUAL_SCALE, 1.0f);
    drawEnemyFishLocal();
    glPopMatrix();
}

void initializeEnemyFishSystem() {
    for (int i = 0; i < MAX_ENEMY_FISH; ++i) {
        enemyFish[i].active = false;
        enemyFish[i].x = 0.0f;
        enemyFish[i].y = 0.0f;
        enemyFish[i].speedFactor = 1.0f;
        enemyFish[i].swimPhase = 0.0f;
        enemyFish[i].aiOffset = 0.0f;
    }
}

bool enemySpawnPositionTooClose(float y) {
    // Keep newly spawned enemies vertically separated so that the screen
    // remains playable, especially on Medium/Hard.
    for (int i = 0; i < MAX_ENEMY_FISH; ++i) {
        if (enemyFish[i].active && std::fabs(enemyFish[i].y - y) < ENEMY_MIN_VERTICAL_SEPARATION &&
            enemyFish[i].x > WIDTH - ENEMY_NEW_SPAWN_RIGHT_ZONE) {
            return true;
        }
    }
    return false;
}

void spawnEnemyFish() {
    const LevelConfig& config = LEVELS[gameState.level];

    if (countActiveEnemyFish() >= config.maxActiveEnemies) {
        return;
    }

    for (int i = 0; i < MAX_ENEMY_FISH; ++i) {
        if (!enemyFish[i].active) {
            float spawnY = randomFloat(ENEMY_SPAWN_Y_MIN, ENEMY_SPAWN_Y_MAX);

            for (int attempt = 0; attempt < ENEMY_SPAWN_POSITION_ATTEMPTS && enemySpawnPositionTooClose(spawnY);
                 ++attempt) {
                spawnY = randomFloat(ENEMY_SPAWN_Y_MIN, ENEMY_SPAWN_Y_MAX);
            }

            enemyFish[i].active = true;
            enemyFish[i].x = WIDTH + randomFloat(ENEMY_SPAWN_X_OFFSET_MIN, ENEMY_SPAWN_X_OFFSET_MAX);
            enemyFish[i].y = spawnY;
            enemyFish[i].speedFactor = randomFloat(ENEMY_SPEED_FACTOR_MIN, ENEMY_SPEED_FACTOR_MAX);
            enemyFish[i].swimPhase = randomFloat(0.0f, 2.0f * PI);
            enemyFish[i].aiOffset = randomFloat(0.0f, 2.0f * PI);
            return;
        }
    }
}

void updateEnemyFish(float deltaTime) {
    const LevelConfig& config = LEVELS[gameState.level];

    for (int i = 0; i < MAX_ENEMY_FISH; ++i) {
        if (!enemyFish[i].active) {
            continue;
        }

        const float horizontalSpeed =
            config.enemySpeedPixelsPerSecond * enemyFish[i].speedFactor;

        // Easy + Medium:
        // Monster fish move normally from right to left.
        // Hard (final) level:
        // Monster fish use AI and gradually chase the player's vertical position.
        enemyFish[i].x -= horizontalSpeed * deltaTime;

        if (gameState.level == LEVEL_HARD) {
            float desiredY = player.y +
                std::sin(gameTime * 2.0f + enemyFish[i].aiOffset) * 70.0f;

            float dy = desiredY - enemyFish[i].y;
            float steering = clampFloat(dy * 0.55f, -85.0f, 85.0f);

            enemyFish[i].y += steering * deltaTime;
            enemyFish[i].y = clampFloat(
                enemyFish[i].y,
                ENEMY_SPAWN_Y_MIN,
                ENEMY_SPAWN_Y_MAX
            );
        }

        if (enemyFish[i].x < ENEMY_DESPAWN_X) {
            enemyFish[i].active = false;
        }
    }

    gameState.enemySpawnTimer -= deltaTime;

    if (gameState.enemySpawnTimer <= 0.0f) {
        spawnEnemyFish();
        gameState.enemySpawnTimer = config.enemySpawnInterval *
                                    randomFloat(ENEMY_SPAWN_INTERVAL_FACTOR_MIN,
                                                ENEMY_SPAWN_INTERVAL_FACTOR_MAX);
    }
}

void drawAllEnemyFish() {
    for (int i = 0; i < MAX_ENEMY_FISH; ++i) {
        if (enemyFish[i].active) {
            drawEnemyFish(enemyFish[i]);
        }
    }
}

// ============================================================

// ============================================================
// PROFESSIONAL MISSION / ACHIEVEMENT LOGIC
// ============================================================
void unlockAchievement(int index) {
    if (index < 0 || index >= ACHIEVEMENT_COUNT || achievements[index])
        return;

    achievements[index] = true;
    lastUnlockedAchievement = index;
    achievementPopupTimer = 3.0f;
    spawnParticles(player.x, player.y, 24);
}

void updateMissionAndAchievements() {
    if (!currentMission.completed &&
        currentMission.type == MISSION_EAT_FISH &&
        gameState.fishEaten >= currentMission.target) {
        currentMission.completed = true;
        gameState.score += 50;
        if (gameState.score > bestScore) {
            bestScore = gameState.score;
            saveBestScore();
        }
        addScorePopup(player.x, player.y, 50);
    }
    else if (!currentMission.completed &&
             currentMission.type == MISSION_REACH_SCORE &&
             gameState.score >= currentMission.target) {
        currentMission.completed = true;
        gameState.score += 50;
        addScorePopup(player.x, player.y, 50);
    }
    else if (!currentMission.completed &&
             currentMission.type == MISSION_SURVIVE_TIME &&
             gameState.survivalTime >= static_cast<float>(currentMission.target)) {
        currentMission.completed = true;
        gameState.score += 75;
        addScorePopup(player.x, player.y, 75);
    }

    if (gameState.fishEaten >= 1)
        unlockAchievement(0);

    if (gameState.maxCombo >= 5)
        unlockAchievement(1);

    if (gameState.score >= 200)
        unlockAchievement(2);

    if (gameState.score >= 500)
        unlockAchievement(3);

    if (gameState.survivalTime >= 60.0f)
        unlockAchievement(4);

    if (totalPowerUpsCollected >= 5)
        unlockAchievement(5);
}

void drawMissionHUD() {
    char text[100];

    if (currentMission.type == MISSION_EAT_FISH) {
        std::sprintf(text, "MISSION: Eat %d fish  [%d/%d]",
                     currentMission.target,
                     std::min(gameState.fishEaten, currentMission.target),
                     currentMission.target);
    }
    else if (currentMission.type == MISSION_REACH_SCORE) {
        std::sprintf(text, "MISSION: Reach %d score  [%d/%d]",
                     currentMission.target,
                     std::min(gameState.score, currentMission.target),
                     currentMission.target);
    }
    else {
        int seconds = static_cast<int>(gameState.survivalTime);
        std::sprintf(text, "MISSION: Survive %ds  [%ds/%ds]",
                     currentMission.target,
                     std::min(seconds, currentMission.target),
                     currentMission.target);
    }

    glColor3f(0.80f, 0.95f, 1.0f);
    drawText(25, 585, text, GLUT_BITMAP_HELVETICA_12);

    if (currentMission.completed) {
        glColor3f(1.0f, 0.88f, 0.20f);
        drawText(25, 565, "MISSION COMPLETE!  BONUS AWARDED",
                 GLUT_BITMAP_HELVETICA_12);
    }
}

void drawLevelProgressBar() {
    float progress = 0.0f;

    if (gameState.level == LEVEL_EASY) {
        progress = clampFloat(
            static_cast<float>(gameState.score) / MEDIUM_LEVEL_SCORE,
            0.0f, 1.0f);
    }
    else if (gameState.level == LEVEL_MEDIUM) {
        progress = clampFloat(
            static_cast<float>(gameState.score - MEDIUM_LEVEL_SCORE) /
            static_cast<float>(HARD_LEVEL_SCORE - MEDIUM_LEVEL_SCORE),
            0.0f, 1.0f);
    }
    else {
        progress = 1.0f;
    }

    const float left = 400.0f;
    const float right = 700.0f;
    const float bottom = 625.0f;
    const float top = 635.0f;

    glColor4f(0.0f, 0.04f, 0.08f, 0.65f);
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();

    glColor3f(0.20f, 0.90f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(left + (right - left) * progress, bottom);
    glVertex2f(left + (right - left) * progress, top);
    glVertex2f(left, top);
    glEnd();

    char text[60];
    if (gameState.level == LEVEL_HARD)
        std::sprintf(text, "MAX LEVEL");
    else
        std::sprintf(text, "NEXT LEVEL");

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(705, 624, text, GLUT_BITMAP_HELVETICA_10);
}

void drawAchievementPopup() {
    if (achievementPopupTimer <= 0.0f ||
        lastUnlockedAchievement < 0)
        return;

    float alpha = clampFloat(achievementPopupTimer / 3.0f, 0.0f, 1.0f);

    glColor4f(0.02f, 0.08f, 0.12f, 0.90f * alpha);
    glBegin(GL_QUADS);
    glVertex2f(350, 500);
    glVertex2f(750, 500);
    glVertex2f(750, 555);
    glVertex2f(350, 555);
    glEnd();

    glColor4f(1.0f, 0.85f, 0.20f, alpha);
    drawText(380, 532, "ACHIEVEMENT UNLOCKED!",
             GLUT_BITMAP_HELVETICA_18);

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    drawText(380, 510, ACHIEVEMENT_NAMES[lastUnlockedAchievement],
             GLUT_BITMAP_HELVETICA_12);
}

void drawGameStatisticsOverlay() {
    if (!gameState.gameOver)
        return;

    glColor4f(0.0f, 0.0f, 0.0f, 0.76f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WIDTH, 0);
    glVertex2f(WIDTH, HEIGHT);
    glVertex2f(0, HEIGHT);
    glEnd();

    glColor3f(1.0f, 0.30f, 0.22f);
    drawText(430, 515, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

    char text[80];

    std::sprintf(text, "Final Score: %d", gameState.score);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(410, 465, text);

    std::sprintf(text, "Best Score: %d", bestScore);
    drawText(410, 430, text);

    std::sprintf(text, "Fish Eaten: %d", gameState.fishEaten);
    drawText(410, 395, text);

    std::sprintf(text, "Max Combo: x%d", gameState.maxCombo);
    drawText(410, 360, text);

    std::sprintf(text, "Coins Collected: %d", gameState.coinsCollected);
    drawText(410, 325, text);

    int survivalSeconds = static_cast<int>(gameState.survivalTime);
    std::sprintf(text, "Survival Time: %02d:%02d",
                 survivalSeconds / 60, survivalSeconds % 60);
    drawText(410, 290, text);

    glColor3f(1.0f, 0.85f, 0.20f);
    drawText(410, 245, "ACHIEVEMENTS", GLUT_BITMAP_HELVETICA_18);

    int y = 215;
    glColor3f(0.85f, 0.95f, 1.0f);

    for (int i = 0; i < ACHIEVEMENT_COUNT; ++i) {
        if (achievements[i]) {
            std::sprintf(text, "[OK] %s", ACHIEVEMENT_NAMES[i]);
            drawText(410, y, text, GLUT_BITMAP_HELVETICA_12);
            y -= 22;
        }
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(675, 145, "R = Restart", GLUT_BITMAP_HELVETICA_12);
    drawText(675, 120, "ESC = Exit", GLUT_BITMAP_HELVETICA_12);
}

// GAMEPLAY LOGIC
// ============================================================
GameLevel calculateLevelFromScore(int score) {
    if (score >= HARD_LEVEL_SCORE) {
        return LEVEL_HARD;
    }

    if (score >= MEDIUM_LEVEL_SCORE) {
        return LEVEL_MEDIUM;
    }

    return LEVEL_EASY;
}

void updateLevelFromScore() {
    GameLevel newLevel = calculateLevelFromScore(gameState.score);

    if (newLevel != gameState.level) {
        gameState.level = newLevel;
        gameState.levelMessageTimer = LEVEL_MESSAGE_DURATION;

        // Do not force the player to wait for old spawn timers after
        // difficulty rises. Existing fish also immediately use the new
        // level's movement speed on the next update.
        const LevelConfig& config = LEVELS[gameState.level];
        gameState.foodSpawnTimer =
            std::min(gameState.foodSpawnTimer, config.foodSpawnInterval);
        gameState.enemySpawnTimer =
            std::min(gameState.enemySpawnTimer, config.enemySpawnInterval);

        // Make the higher enemy density noticeable immediately.
        // spawnEnemyFish() is pool-bounded and respects maxActiveEnemies,
        // so these extra level-up spawns cannot overflow the enemy array.
        for (int i = 0; i < config.enemyLevelUpBurst; ++i) {
            spawnEnemyFish();
        }
    }
}

void handleFoodCollisions() {
    for (int i = 0; i < MAX_FOOD_FISH; ++i) {
        if (!foodFish[i].active) {
            continue;
        }

        if (rectanglesOverlap(player.x, player.y,
                              PLAYER_COLLISION_HALF_W,
                              PLAYER_COLLISION_HALF_H,
                              foodFish[i].x, foodFish[i].y,
                              FOOD_COLLISION_HALF_W,
                              FOOD_COLLISION_HALF_H)) {
            foodFish[i].active = false;
            int earned = FOOD_SCORE_VALUE;
            if (comboTimer > 0.0f) {
                ++comboCount;
            } else {
                comboCount = 1;
            }
            comboTimer = COMBO_WINDOW;

            if (comboCount >= 3) earned += 5 * (comboCount - 2);
            gameState.score += earned;
            ++gameState.fishEaten;
            gameState.maxCombo = std::max(gameState.maxCombo, comboCount);

            addScorePopup(foodFish[i].x, foodFish[i].y, earned);
            spawnParticles(foodFish[i].x, foodFish[i].y, 14);

            updateMissionAndAchievements();

            if (gameState.score > bestScore) {
                bestScore = gameState.score;
                saveBestScore();
            }

            updateLevelFromScore();

            // Replenish eaten food quickly without creating an immediate
            // pile-up of fish.
            gameState.foodSpawnTimer =
                std::min(gameState.foodSpawnTimer, FOOD_AFTER_EAT_SPAWN_DELAY);
        }
    }
}

void handlePlayerHit() {
    if (player.invulnerableTime > 0.0f || gameState.gameOver) {
        return;
    }

    if (shieldTimer > 0.0f) {
        shieldTimer = 0.0f;
        player.shakeTime = 0.18f;
        spawnParticles(player.x, player.y, 22);
        return;
    }

    --gameState.lives;

    if (gameState.lives <= 0) {
        gameState.lives = 0;
        gameState.gameOver = true;
        player.shakeTime = PLAYER_GAME_OVER_SHAKE_DURATION;
        player.invulnerableTime = PLAYER_GAME_OVER_INVULNERABLE_TIME;
        return;
    }

    // Requirement: shake/recover at the far left and lose exactly one life.
    resetPlayerAfterHit();
}

void handleEnemyCollisions() {
    if (player.invulnerableTime > 0.0f) {
        return;
    }

    for (int i = 0; i < MAX_ENEMY_FISH; ++i) {
        if (!enemyFish[i].active) {
            continue;
        }

        if (rectanglesOverlap(player.x, player.y,
                              PLAYER_COLLISION_HALF_W,
                              PLAYER_COLLISION_HALF_H,
                              enemyFish[i].x, enemyFish[i].y,
                              ENEMY_COLLISION_HALF_W,
                              ENEMY_COLLISION_HALF_H)) {
            handlePlayerHit();
            return; // One collision can remove only one life per update.
        }
    }
}

void updateGameplay(float deltaTime) {
    updatePlayer(deltaTime);
    updateFoodFish(deltaTime);
    updateEnemyFish(deltaTime);
    updatePowerUps(deltaTime);

    handleFoodCollisions();
    handlePowerUpCollisions();
    handleEnemyCollisions();

    if (gameState.levelMessageTimer > 0.0f) {
        gameState.levelMessageTimer =
            std::max(0.0f, gameState.levelMessageTimer - deltaTime);
    }
}

void restartGame() {
    gameState.score = 0;
    gameState.lives = STARTING_LIVES;
    gameState.level = LEVEL_EASY;
    gameState.gameOver = false;
    gameState.foodSpawnTimer = FOOD_FIRST_SPAWN_DELAY;
    gameState.enemySpawnTimer = ENEMY_FIRST_SPAWN_DELAY;
    gameState.levelMessageTimer = 0.0f;
    gameState.fishEaten = 0;
    gameState.coinsCollected = 0;
    gameState.maxCombo = 0;
    gameState.survivalTime = 0.0f;

    currentMission.type = MISSION_EAT_FISH;
    currentMission.target = 10;
    currentMission.completed = false;

    totalPowerUpsCollected = 0;
    achievementPopupTimer = 0.0f;
    lastUnlockedAchievement = -1;

    clearMovementInput();
    initializePlayer();
    initializeFoodFishSystem();
    initializeEnemyFishSystem();
    resetPowerUpsAndEffects();
    gameOverTimer = 0.0f;
}

// ============================================================
// HUD / STATUS
// ============================================================
void drawHeart(float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(x - 7, y + 5, 8, true, 26);
    drawCircle(x + 7, y + 5, 8, true, 26);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 15, y + 5);
    glVertex2f(x + 15, y + 5);
    glVertex2f(x, y - 18);
    glEnd();
}

void drawLives() {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(HUD_LIFE_TEXT_X, HUD_TOP_Y, "Life:");

    for (int i = 0; i < gameState.lives; ++i) {
        drawHeart(HUD_HEART_START_X + static_cast<float>(i) * HUD_HEART_SPACING_X,
                  HUD_TOP_Y);
    }
}

void drawScore() {
    char text[40];
    std::sprintf(text, "Score: %d", gameState.score);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(HUD_SCORE_X, HUD_TOP_Y, text);
}

void drawLevel() {
    char text[50];
    std::sprintf(text, "Level: %s", LEVELS[gameState.level].name);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(WIDTH - HUD_LEVEL_RIGHT_MARGIN, HUD_TOP_Y, text);
}

void drawControlHint() {
    glColor3f(0.92f, 0.98f, 1.0f);
    drawText(HUD_CONTROL_X, HUD_CONTROL_Y,
             "WASD: Move   P: Pause   R: Restart   ESC: Exit",
             GLUT_BITMAP_HELVETICA_12);
}

void drawLevelMessage() {
    if (gameState.levelMessageTimer <= 0.0f) {
        return;
    }

    char text[60];
    std::sprintf(text, "%s Level! Speed Increased",
                 LEVELS[gameState.level].name);

    glColor4f(0.0f, 0.0f, 0.0f, 0.38f);
    glBegin(GL_QUADS);
    glVertex2f(355, 565);
    glVertex2f(745, 565);
    glVertex2f(745, 615);
    glVertex2f(355, 615);
    glEnd();

    glColor3f(1.0f, 0.95f, 0.25f);
    drawText(435, 585, text);
}

void drawGameOverOverlay() {
    // Detailed professional statistics are drawn by drawGameStatisticsOverlay().
}

// ============================================================
// START / PAUSE MENU
// ============================================================
void drawMenuPanel(float left, float bottom, float right, float top)
{
    glColor4f(0.0f, 0.05f, 0.12f, 0.78f);
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();
}

void drawStartMenu()
{
    drawMenuPanel(245, 145, 855, 600);

    glColor3f(1.0f, 0.85f, 0.15f);
    drawText(350, 515, "FISH EATING GAME", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(0.3f, 0.95f, 1.0f);
    drawText(405, 465, "START GAME", GLUT_BITMAP_HELVETICA_18);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(375, 420, "Press ENTER or SPACE");
    drawText(390, 375, "W - Up");
    drawText(390, 345, "A - Left");
    drawText(390, 315, "S - Down");
    drawText(390, 285, "D - Right");
    drawText(390, 245, "R - Restart");
    drawText(390, 215, "P - Pause / Resume");
    drawText(390, 185, "ESC - Exit");

    glColor3f(0.65f, 0.95f, 1.0f);
    drawText(355, 155, "Eat food fish and avoid monster fish!");
}

void drawPauseOverlay()
{
    if (!gamePaused || gameState.gameOver)
        return;

    drawMenuPanel(330, 285, 770, 440);

    glColor3f(1.0f, 0.90f, 0.20f);
    drawText(465, 385, "GAME PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(430, 340, "Press P to Resume");
    drawText(430, 305, "Press R to Restart");
}

// ============================================================
// DISPLAY PIPELINE
// ============================================================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawWater();
    drawWave();
    drawBubbles();

    if (gameStarted)
    {
        drawAllFoodFish();
        drawAllEnemyFish();
        drawPowerUps();
        drawPlayerFish();
        drawEffects();
    }

    drawSoil();
    drawGrass();
    drawStones();

    if (!gameStarted)
    {
        if (showHowToPlay)
            drawHowToPlay();
        else
            drawProfessionalMainMenu();
    }
    else
    {
        drawLives();
        drawScore();
        drawLevel();
        drawControlHint();
        drawLevelMessage();
        drawBestScoreHUD();
        drawInGamePowerStatus();
        drawMissionHUD();
        drawLevelProgressBar();
        drawAchievementPopup();
        drawGameOverOverlay();
        drawGameStatisticsOverlay();
        drawPauseOverlay();
    }

    drawCountdown();

    glutSwapBuffers();
}

// ============================================================
// INPUT
// ============================================================
// Standard GLUT is used. WASD is read continuously with Windows
// GetAsyncKeyState(), so no FreeGLUT key-release callback is needed.
void updateKeyboardInput()
{
    bool leftPressed  = (GetAsyncKeyState('A') & 0x8000) != 0;
    bool rightPressed = (GetAsyncKeyState('D') & 0x8000) != 0;
    bool upPressed    = (GetAsyncKeyState('W') & 0x8000) != 0;
    bool downPressed  = (GetAsyncKeyState('S') & 0x8000) != 0;

    inputState.left = leftPressed;
    inputState.right = rightPressed;
    inputState.up = upPressed;
    inputState.down = downPressed;

    if (leftPressed && !rightPressed)
    {
        inputState.horizontalPriority = -1;
        player.direction = -1;
    }
    else if (rightPressed && !leftPressed)
    {
        inputState.horizontalPriority = 1;
        player.direction = 1;
    }
    else if (!leftPressed && !rightPressed)
    {
        inputState.horizontalPriority = 0;
    }

    if (upPressed && !downPressed)
    {
        inputState.verticalPriority = 1;
    }
    else if (downPressed && !upPressed)
    {
        inputState.verticalPriority = -1;
    }
    else if (!upPressed && !downPressed)
    {
        inputState.verticalPriority = 0;
    }
}

void keyboard(unsigned char key, int, int)
{
    if (key == 27) // ESC
    {
        exit(0);
    }

    if (!gameStarted)
    {
        if (showHowToPlay)
        {
            if (key == 'b' || key == 'B' || key == 8)
            {
                showHowToPlay = false;
                glutPostRedisplay();
            }
            return;
        }

        if (key == 13 || key == ' ')
        {
            startCountdown();
            glutPostRedisplay();
        }
        return;
    }

    if (countdownActive)
        return;

    if (key == 'r' || key == 'R')
    {
        gamePaused = false;
        restartGame();
        glutPostRedisplay();
        return;
    }

    if (key == 'p' || key == 'P')
    {
        if (!gameState.gameOver)
        {
            gamePaused = !gamePaused;
            clearMovementInput();
            glutPostRedisplay();
        }
    }
}


bool pointInside(float mx, float my, float left, float bottom, float right, float top)
{
    return mx >= left && mx <= right && my >= bottom && my <= top;
}

void mouse(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    int viewportW = glutGet(GLUT_WINDOW_WIDTH);
    int viewportH = glutGet(GLUT_WINDOW_HEIGHT);

    float gameX = (static_cast<float>(x) / std::max(1, viewportW)) * WIDTH;
    float gameY = HEIGHT - (static_cast<float>(y) / std::max(1, viewportH)) * HEIGHT;

    if (!gameStarted && !showHowToPlay)
    {
        if (pointInside(gameX, gameY, 400, 415, 700, 465))
        {
            startCountdown();
        }
        else if (pointInside(gameX, gameY, 400, 340, 700, 390))
        {
            showHowToPlay = true;
        }
        else if (pointInside(gameX, gameY, 400, 265, 700, 315))
        {
            exit(0);
        }
    }
    else if (!gameStarted && showHowToPlay)
    {
        if (pointInside(gameX, gameY, 430, 135, 670, 180))
            showHowToPlay = false;
    }

    glutPostRedisplay();
}

// ============================================================
// UPDATE LOOP
// ============================================================
void timer(int)
{
    int now = glutGet(GLUT_ELAPSED_TIME);

    float deltaTime = DEFAULT_DELTA_TIME;

    if (lastUpdateMilliseconds != 0)
    {
        deltaTime = static_cast<float>(now - lastUpdateMilliseconds) / 1000.0f;
    }

    lastUpdateMilliseconds = now;

    deltaTime = clampFloat(deltaTime, MIN_DELTA_TIME, MAX_DELTA_TIME);

    gameTime += deltaTime;

    updateEffects(deltaTime);
    updateCountdown(deltaTime);

    // Aquarium background remains animated on the start screen.
    updateBubbles(deltaTime);

    if (gameStarted && !gamePaused)
    {
        updateKeyboardInput();

        if (!gameState.gameOver)
        {
            gameState.survivalTime += deltaTime;
            updateMissionAndAchievements();
            updateGameplay(deltaTime);
        }
        else
        {
            updatePlayer(deltaTime);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(TIMER_INTERVAL_MS, timer, 0);
}

// ============================================================
// WINDOW / OPENGL INITIALIZATION
// ============================================================
void reshape(int windowWidth, int windowHeight) {
    if (windowHeight <= 0) {
        windowHeight = 1;
    }

    const float gameAspect = static_cast<float>(WIDTH) /
                             static_cast<float>(HEIGHT);
    const float windowAspect = static_cast<float>(windowWidth) /
                               static_cast<float>(windowHeight);

    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = windowWidth;
    int viewportHeight = windowHeight;

    if (windowAspect > gameAspect) {
        viewportWidth = static_cast<int>(static_cast<float>(windowHeight) * gameAspect);
        viewportX = (windowWidth - viewportWidth) / 2;
    } else {
        viewportHeight = static_cast<int>(static_cast<float>(windowWidth) / gameAspect);
        viewportY = (windowHeight - viewportHeight) / 2;
    }

    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void initializeOpenGL() {
    glClearColor(0.0f, 0.15f, 0.30f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void initializeGame() {
    loadBestScore();
    initializeBubbles();
    initializeGrass();
    restartGame();
}

// ============================================================
// MAIN
// ============================================================
int main(int argc, char** argv) {
    std::srand(static_cast<unsigned int>(std::time(0)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(WINDOW_START_X, WINDOW_START_Y);
    glutCreateWindow("Fish Eating Game - Merged Team Project");

    initializeOpenGL();
    initializeGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(TIMER_INTERVAL_MS, timer, 0);

    glutMainLoop();
    return 0;
}
