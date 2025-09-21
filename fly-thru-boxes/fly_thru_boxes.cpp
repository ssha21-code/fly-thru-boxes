#include "raylib.h"
#include <iostream>
#include <vector>

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 800;

Texture2D textureBackground;
Texture2D textureRocket;
Texture2D texturePowerUpExtraLife;
Texture2D texturePowerUpExtraPoints;
Texture2D texturePowerUpExtraAmmo;
Texture2D textureCrate;
Texture2D textureGoldenCrate;
Texture2D textureTurret;
Texture2D textureTurretSpecial;
Texture2D textureUFO;

Sound soundRocketShoot;
Sound soundHit;
Sound soundPowerUpPickup;

double lastUpdateTime = 0;

bool UFOEventTriggered() {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= 1.5) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

void loadImages() {
    Image imageBackground = LoadImage("fly_thru_boxes_images/background.png");
    Image imageRocket = LoadImage("fly_thru_boxes_images/rocket.png");
    Image imagePowerUpExtraLife = LoadImage("fly_thru_boxes_images/extra_life.png");
    Image imagePowerUpExtraPoints = LoadImage("fly_thru_boxes_images/extra_point.png");
    Image imagePowerUpExtraAmmo = LoadImage("fly_thru_boxes_images/extra_ammo.png");
    Image imageCrate = LoadImage("fly_thru_boxes_images/crate.png");
    Image imageGoldenCrate = LoadImage("fly_thru_boxes_images/golden_crate.png");
    Image imageTurret = LoadImage("fly_thru_boxes_images/turret.png");
    Image imageTurretSpecial = LoadImage("fly_thru_boxes_images/turret_special.png");
    Image imageUFO = LoadImage("fly_thru_boxes_images/ufo.png");

    textureBackground = LoadTextureFromImage(imageBackground);
    textureRocket = LoadTextureFromImage(imageRocket);
    texturePowerUpExtraLife = LoadTextureFromImage(imagePowerUpExtraLife);
    texturePowerUpExtraPoints = LoadTextureFromImage(imagePowerUpExtraPoints);
    texturePowerUpExtraAmmo = LoadTextureFromImage(imagePowerUpExtraAmmo);
    textureCrate = LoadTextureFromImage(imageCrate);
    textureGoldenCrate = LoadTextureFromImage(imageGoldenCrate);
    textureTurret = LoadTextureFromImage(imageTurret);
    textureTurretSpecial = LoadTextureFromImage(imageTurretSpecial);
    textureUFO = LoadTextureFromImage(imageUFO);

    soundHit = LoadSound("fly_thru_boxes_audio/hit.wav");
    soundPowerUpPickup = LoadSound("fly_thru_boxes_audio/power_up_pickup.wav");
    soundRocketShoot = LoadSound("fly_thru_boxes_audio/rocket_shoot.wav");

    UnloadImage(imageBackground);
    UnloadImage(imageRocket);
    UnloadImage(imagePowerUpExtraLife);
    UnloadImage(imagePowerUpExtraPoints);
    UnloadImage(imagePowerUpExtraAmmo);
    UnloadImage(imageCrate);
    UnloadImage(imageGoldenCrate);
    UnloadImage(imageTurret);
    UnloadImage(imageTurretSpecial);
    UnloadImage(imageUFO);
}

class Rocket {
public: 
    float width = 100.0f;
    float height = 150.0f;
    Vector2 position = {(GetScreenWidth() - width) / 2, 600.0f};
    Rectangle rectangle = {position.x, position.y, width, height};
    Rectangle hitbox = {rectangle.x + 15, position.y, width - 30, height - 50};
    float velocityX = 8.0f;
    float velocityY = 3.0f;
    int lives = 3;
    int ammo = 3;

    bool hasReachedPosition = false;

    Rocket() {

    }

    void draw() {
        DrawTexture(textureRocket, position.x, position.y, WHITE);
    }

    void moveLeft() {
        if (position.x > 0) {
            position.x -= velocityX;
        }
    }

    void moveRight() {
        if (position.x + width < GetScreenWidth()) {
            position.x += velocityX;
        }
    }

    void update() {
        if (!hasReachedPosition) {
            position.y -= velocityY;
        }
        if (position.y < 500) {
            position.y = 500;
            hasReachedPosition = true;
        }
        if (IsKeyDown(KEY_A)) {
            moveLeft();
        }
        if (IsKeyDown(KEY_D)) {
            moveRight();
        }

        rectangle = {position.x, position.y, width, height};
        hitbox = {rectangle.x + 12, position.y, width - 24, height - 40};
    }
};

class Crate {
public: 
    Vector2 position = {};
    Rectangle rectangle = Rectangle();
    float velocityY = 4.0f;
    static const int size = 100;

    bool shouldBeRemoved = false;
    bool isGoldenCrate;

    Crate(Vector2 position, bool isGoldenCrate) {
        this->position = position;
        this->rectangle = {position.x, position.y, size, size};
        this->isGoldenCrate = isGoldenCrate;
    }

    void draw() {
        if (isGoldenCrate) {
            DrawTextureV(textureGoldenCrate, position, WHITE);
        } else {
            DrawTextureV(textureCrate, position, WHITE);
        }
    }

    void update() {
        position.y += velocityY;
        rectangle = {position.x, position.y, size, size};
    }
};

class PowerUp {
public: 
    Vector2 position = {};
    Rectangle rectangle = {};
    float velocityY = 4.0f;
    static const int size = 50;
    enum class PowerUpType {
        EXTRA_LIFE = 0,
        EXTRA_POINTS,
        EXTRA_AMMO,
    };
    PowerUpType powerUpType;

    bool shouldBeDestroyed = false;

    PowerUp(Vector2 position, PowerUpType powerUpType){
        this->position = position;
        this->rectangle = {position.x, position.y, size, size};
        this->powerUpType = powerUpType;
    }

    void draw() {
        switch (powerUpType) {
            case PowerUpType::EXTRA_LIFE: {
                DrawTextureV(texturePowerUpExtraLife, position, WHITE);
                break;
            }
            case PowerUpType::EXTRA_POINTS: {
                DrawTextureV(texturePowerUpExtraPoints, position, WHITE);
                break;
            }
            case PowerUpType::EXTRA_AMMO: {
                DrawTextureV(texturePowerUpExtraAmmo, position, WHITE);
                break;
            }
        }
    }

    void update() {
        position.y += velocityY;
        rectangle = {position.x, position.y, size, size};
    }
};

class Turret {
public: 
    float width = 10.0f;
    float height = 30.0f;
    Vector2 position = {};
    Rectangle hitbox = {};
    float velocity = 10.0f;

    bool shouldBeDestroyed = false;
    bool isSpecialTurret;
    bool isPlayerTurret;

    Turret(Vector2 position, bool isSpecialTurret, bool isPlayerTurret) {
        this->position = position;
        hitbox = {position.x, position.y, width, height};
        this->isSpecialTurret = isSpecialTurret;
        this->isPlayerTurret = isPlayerTurret;
    }

    void draw() {
        if (isSpecialTurret) {
            DrawTextureV(textureTurretSpecial, position, WHITE);
        } else {
            DrawTextureV(textureTurret, position, WHITE);
        }
    }

    void update() {
        if (isPlayerTurret) {
            position.y -= velocity;
        } else {
            position.y += velocity;
        }
        hitbox = {position.x, position.y, width, height};
    }
};

class UFO {
public: 
    constexpr static float width = 140;
    constexpr static float height = 104;
    Vector2 position = {};
    Rectangle hitbox = {};
    Rocket *rocket;

    int hp = 10;
    float velocityX = 4.0f;
    float velocityY = 4.0f;

    bool shouldSpawnTurret = false;
    bool hasReachedPosition = false;

    UFO(Vector2 position, Rocket *rocket) {
        this->position = position;
        hitbox = {position.x, position.y, width, height};
        this->rocket = rocket;
    }

    void draw() {
        DrawText(TextFormat("%i", hp), position.x + 50, position.y - 32, 32, WHITE);
        DrawTextureV(textureUFO, position, WHITE);
    }

    void update() {
        if (position.y < 300) {
            position.y += velocityY;
        }
        if (position.y >= 300) {
            position.y = 300;
            hasReachedPosition = true;
        }
        if (rocket->position.x <= position.x && position.x - rocket->position.x > 10) {
            position.x -= velocityX;
        } else if (rocket->position.x >= position.x && rocket->position.x - position.x > 10) {
            position.x += velocityX;
        }
        hitbox = {position.x, position.y, width, height};
    }
};

class Game {
public: 
    Rocket rocket = Rocket();
    UFO ufo = UFO({(GetScreenWidth() - UFO::width) / 2, 0}, &rocket);

    std::vector<Crate> crates = {};
    std::vector<PowerUp> powerUps = {};
    std::vector<Turret> turrets = {};

    bool gamePaused = true;
    bool shouldSpawnCrates = false;
    bool shouldSpawnPowerUp = false;
    bool isUFOActive = false;
    bool hasIncreasedPowerUpChance = false;

    int powerUpChance = 0;
    int goldenCrateChance = 0;
    int ufoChance = 0;
    int score = 0;

    Game() {
        
    }

    void draw() {
        DrawTexture(textureBackground, 0, 0, WHITE);
        rocket.draw();
        if (isUFOActive) {
            ufo.draw();
        }
        for (auto &crate: crates) {
            crate.draw();
        }
        for (auto &powerUp: powerUps) {
            powerUp.draw();
        }
        for (auto &turret: turrets) {
            turret.draw();
        }
        DrawText(TextFormat("Lives: %i", rocket.lives), 25, 25, 36, WHITE);        
        DrawText(TextFormat("Score: %i", score), 25, 70, 36, WHITE);        
        DrawText(TextFormat("Ammo: %i", rocket.ammo), 25, 115, 36, WHITE);        
    }

    void update() {
        if (gamePaused) {
            DrawText("Press Space to Start", 300, 300, 40, WHITE);
            if (IsKeyPressed(KEY_SPACE)) {
                gamePaused = false;
            }
        } else {
            if (crates.size() == 0) {
                if (rocket.hasReachedPosition) {
                    score++;
                }
                hasIncreasedPowerUpChance = false;
                goldenCrateChance++;
                ufoChance++;
                shouldSpawnCrates = true;
                if (powerUpChance >= 3) {
                    shouldSpawnPowerUp = true;
                    powerUpChance = 0;
                }
            }
            if (shouldSpawnCrates) {
                int posX = 50;
                int gap = 10;
                int space = gap + Crate::size;
                for (int i = -1; i < 9; i++) {
                    crates.push_back(Crate({(float)posX + (space * i), 25}, false));
                }
                int randomCrate = GetRandomValue(1, 6);
                crates.erase(crates.begin() + randomCrate);
                if (goldenCrateChance == 5) {
                    int randomGoldenCrate = GetRandomValue(1, 7);
                    crates.at(randomGoldenCrate).isGoldenCrate = true;
                    goldenCrateChance = 0;
                }
                shouldSpawnCrates = false;
            }

            if (shouldSpawnPowerUp) {
                float spawnY = rocket.position.y - 300;
                PowerUp::PowerUpType powerUpType = rocket.lives == 3 ? PowerUp::PowerUpType::EXTRA_POINTS : PowerUp::PowerUpType::EXTRA_LIFE;
                powerUps.push_back(PowerUp({(float)GetRandomValue(25, GetScreenWidth() - 125), spawnY}, powerUpType));
                shouldSpawnPowerUp = false;
            }

            if (rocket.lives <= 0) {
                rocket.lives = 3;
                rocket.position = {(GetScreenWidth() - rocket.width) / 2, 600.0f};
                rocket.ammo = 3;
                rocket.hasReachedPosition = false;
                score = 0;
                crates.clear();
                powerUps.clear();
                turrets.clear();
                ufo.hp = 10;
                isUFOActive = false;
                ufoChance = 0;
                gamePaused = true;
            }

            for (auto &crate: crates) {
                if (crate.position.y > rocket.position.y && !hasIncreasedPowerUpChance) {
                    powerUpChance++;
                    hasIncreasedPowerUpChance = true;
                }
            }

            if (IsKeyPressed(KEY_SPACE)) {
                float turretX = rocket.hitbox.x + rocket.hitbox.width / 2;
                float turretY = rocket.hitbox.y - 10;
                PlaySound(soundRocketShoot);
                turrets.push_back(Turret({turretX, turretY}, false, true));
            }

            if (IsKeyPressed(KEY_W)) {
                if (rocket.ammo > 0) {
                    float turretX = rocket.hitbox.x + rocket.hitbox.width / 2;
                    float turretY = rocket.hitbox.y - 10;
                    turrets.push_back(Turret({turretX, turretY}, true, true));
                    rocket.ammo--;
                }
                if (rocket.ammo < 0) {
                    rocket.ammo = 0;
                }
                PlaySound(soundRocketShoot);
            }

            if (powerUpChance == 3) {
                shouldSpawnPowerUp = true;
                powerUpChance = 0;
            }

            ufo.update();

            if (ufoChance == 11) {
                ufoChance = 0;
                ufo.hp = 10;
                ufo.position.y = 0;
                isUFOActive = true;
            }
            if (isUFOActive) {
                ufoChance = 0;
            }
            if (isUFOActive && ufo.hasReachedPosition) {
                if (UFOEventTriggered()) {
                    ufo.shouldSpawnTurret = true;
                }
            }
            if (ufo.hp <= 0 && isUFOActive) {
                for (int i = 0; i < 2; i++) {
                    powerUps.push_back(PowerUp({ufo.position.x + 10 + (20 * i), ufo.position.y + 20}, PowerUp::PowerUpType::EXTRA_POINTS));
                }
                for (int i = 0; i < 3 - rocket.lives; i++) {
                    powerUps.push_back(PowerUp({ufo.position.x + 10 + (15 * i), ufo.position.y + 60}, PowerUp::PowerUpType::EXTRA_POINTS));
                }
                powerUps.push_back(PowerUp({ufo.position.x + 10 + 23, ufo.position.y + 60}, PowerUp::PowerUpType::EXTRA_LIFE));
                for (int i = 0; i < 2; i++) {
                    powerUps.push_back(PowerUp({ufo.position.x + 10 + (20 * i), ufo.position.y + 70}, PowerUp::PowerUpType::EXTRA_AMMO));
                }
                ufo.hp = 10;
                isUFOActive = false;
                ufoChance = 0;
            }

            if (ufo.shouldSpawnTurret && isUFOActive && ufo.hasReachedPosition) {
                float turretX = ufo.hitbox.x + ufo.hitbox.width / 2;
                float turretY = ufo.hitbox.y + ufo.height + 10;
                turrets.push_back(Turret({turretX, turretY}, false, false));
                PlaySound(soundRocketShoot);
                ufo.shouldSpawnTurret = false;
            }

            for (auto &crate: crates) {
                crate.update();
            }

            for (auto &powerUp: powerUps) {
                powerUp.update();
            }

            for (auto &turret: turrets) {
                turret.update();
            }

            rocket.update();

            

            checkCollisions();
            checkRemoval();
            garbageCollect();
        }
    }

    void checkRemoval() {
        for (int i = 0; i < crates.size(); i++) {
            if (crates.at(i).position.y > GetScreenHeight()) {
                crates.at(i).shouldBeRemoved = true;
            }
        }
        for (int i = 0; i < powerUps.size(); i++) {
            if (powerUps.at(i).position.y > GetScreenHeight()) {
                powerUps.at(i).shouldBeDestroyed = true;
            }
        }
        for (int i = 0; i < turrets.size(); i++) {
            if (turrets.at(i).position.y < 0) {
                turrets.at(i).shouldBeDestroyed = true;
            }
        }
    }

    void garbageCollect() {
        for (int i = 0; i < crates.size(); i++) {
            if (crates.at(i).shouldBeRemoved) {
                crates.erase(crates.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < powerUps.size(); i++) {
            if (powerUps.at(i).shouldBeDestroyed) {
                powerUps.erase(powerUps.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < turrets.size(); i++) {
            if (turrets.at(i).shouldBeDestroyed) {
                turrets.erase(turrets.begin() + i);
                i--;
            }
        }
    }

    void checkCollisions() {
        for (auto &crate: crates) {
            if (CheckCollisionRecs(crate.rectangle, rocket.hitbox)) {
                crate.shouldBeRemoved = true;
                PlaySound(soundHit);
                if (crate.isGoldenCrate) continue;
                rocket.lives -= 1;
            }
        }
        for (auto &powerUp: powerUps) {
            if (CheckCollisionRecs(powerUp.rectangle, rocket.hitbox)) {
                switch (powerUp.powerUpType) {
                    case PowerUp::PowerUpType::EXTRA_LIFE: {
                        if (rocket.lives < 3) {
                            rocket.lives++;
                        }
                        break;
                    }
                    case PowerUp::PowerUpType::EXTRA_POINTS: {
                        score+=2;
                        break;
                    }
                    case PowerUp::PowerUpType::EXTRA_AMMO: {
                        rocket.ammo++;
                        break;
                    }
                }
                PlaySound(soundPowerUpPickup);
                powerUp.shouldBeDestroyed = true;
            }
        }
        for (auto &crate: crates) {
            for (auto &turret: turrets) {
                if (CheckCollisionRecs(crate.rectangle, turret.hitbox)) {
                    if (crate.isGoldenCrate) {
                        for (int i = 0; i < 2; i++) {
                            powerUps.push_back(PowerUp({crate.position.x + 10 + (20 * i), crate.position.y + 20}, PowerUp::PowerUpType::EXTRA_POINTS));
                        }
                        for (int i = 0; i < 3 - rocket.lives; i++) {
                            powerUps.push_back(PowerUp({crate.position.x + 10 + (15 * i), crate.position.y + 60}, PowerUp::PowerUpType::EXTRA_LIFE));
                        }
                        powerUps.push_back(PowerUp({crate.position.x + 40, crate.position.y + 70}, PowerUp::PowerUpType::EXTRA_AMMO));
                        crate.shouldBeRemoved = true;
                    }
                    if (turret.isSpecialTurret) {
                        crate.shouldBeRemoved = true;
                    }
                    PlaySound(soundHit);
                    turret.shouldBeDestroyed = true;
                }
            }
        }
        for (auto &turret: turrets) {
            if (CheckCollisionRecs(ufo.hitbox, turret.hitbox)) {
                if (turret.isPlayerTurret && isUFOActive) {
                    if (turret.isSpecialTurret) {
                        ufo.hp-=2;
                    } else {
                        ufo.hp--;
                    }
                    PlaySound(soundHit);
                    turret.shouldBeDestroyed = true;
                }
            }
            if (CheckCollisionRecs(turret.hitbox, rocket.hitbox)) {
                if (!turret.isPlayerTurret && isUFOActive) {
                    rocket.lives--;
                    PlaySound(soundHit);
                    turret.shouldBeDestroyed = true;
                }
            }
        }
    }
};

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fly Thru Boxes!");
    SetTargetFPS(60);
    InitAudioDevice();

    loadImages();

    Game game = Game();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        game.draw();
        game.update();

        EndDrawing();
    }

    CloseWindow();
    std::abort();
    return 0;
}