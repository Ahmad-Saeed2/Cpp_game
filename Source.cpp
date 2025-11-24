#include "raylib.h"
#include <vector>
#include <cmath>

struct Entity {
    Rectangle rect;
    Vector2 velocity;
    int health;
    bool isPlayer;
    int frame = 0;
    float animTimer = 0;
};

struct Projectile {
    Rectangle rect;
    Vector2 velocity;
    bool isBlade = false;
    bool isBigBlast = false;
    bool isHoming = false;
    int lifetime = 0;
};

void DrawHealthBar(Vector2 pos, int health, int maxHealth, Color color) {
    DrawRectangle((int)pos.x, (int)pos.y, 100, 10, GRAY);
    DrawRectangle((int)pos.x, (int)pos.y, (int)((float)health / maxHealth * 100), 10, color);
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Epic Boss Fight");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D playerSheet = LoadTexture("charmulti.png");
    Texture2D bossTextures[3] = {
        LoadTexture("bos1.png"),
        LoadTexture("bos2.png"),
        LoadTexture("bos3.png")
    };
    Texture2D background = LoadTexture("Backgorund.png");
    Sound jumpSound = LoadSound("jump.wav");
    Sound attackSound = LoadSound("attack.wav");
    Sound hitSound = LoadSound("hit.wav");
    Sound damage = LoadSound("damage.wav");
    Music bgm = LoadMusicStream("background.wav");
    PlayMusicStream(bgm);
    Music menu = LoadMusicStream("menu.wav");
    PlayMusicStream(menu);

    bool gameStarted = false;
    int titleOffset = 0;
    bool goingUp = false;

    while (!WindowShouldClose() && !gameStarted) {
        UpdateMusicStream(menu);
        if (IsKeyPressed(KEY_ENTER)) {
            gameStarted = true;
            StopMusicStream(menu);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("HOLLOW KNIGHT LITE", screenWidth / 2 - MeasureText("Hollow Knight LITE", 60) / 2, 150 + titleOffset, 50, WHITE);
        DrawText("Press [ENTER] to Start", screenWidth / 2 - MeasureText("Press [ENTER] to Start", 30) / 2, 300, 30, GRAY);
        EndDrawing();
    }

    const float groundY = 480;
    Entity player = { {100, groundY, 80, 120}, {0, 0}, 150, true };
    Entity bosses[3] = {
        { {600, groundY, 120, 180}, {1.5f, 0}, 200, false },
        { {600, 200, 100, 100}, {2, 0}, 150, false },
        { {screenWidth - 200, 200, 140, 140}, {0, 1.5f}, 200, false }
    };

    Rectangle door = { screenWidth - 40, groundY, 30, 60 };
    bool doorActive[2] = { false, false };

    int level = 1, frameCounter = 0, playerDir = 1;
    bool canJump = false, isGameOver = false, boss1Charging = false;
    int boss1ChargeFrames = 0, attackTimer = 0;
    bool showAttack = false;
    Vector2 attackPoints[3] = {};
    std::vector<Projectile> projectiles;
    const float gravity = 0.24f;

    while (!WindowShouldClose()) {
        frameCounter++;
        UpdateMusicStream(bgm);
        if (player.health <= 0) isGameOver = true;
        if (!isGameOver) {
            player.velocity.x = 0;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) { player.velocity.x = -3; playerDir = -1; }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { player.velocity.x = 3; playerDir = 1; }
            if (IsKeyPressed(KEY_SPACE) && canJump) {
                player.velocity.y = -10; canJump = false; PlaySound(jumpSound);
            }
            player.velocity.y += gravity;
            player.rect.x += player.velocity.x;
            player.rect.y += player.velocity.y;
            if (player.rect.y >= groundY) {
                player.rect.y = groundY; player.velocity.y = 0; canJump = true;
            }

            if (player.velocity.x != 0) {
                player.animTimer += GetFrameTime();
                if (player.animTimer >= 0.12f) {
                    player.frame = (player.frame + 1) % 4;
                    player.animTimer = 0;
                }
            }
            else player.frame = 0;
            // First Boss Physics
            if (level == 1 && bosses[0].health > 0) {
                if (!boss1Charging) {
                    bosses[0].rect.x += bosses[0].velocity.x;
                    if (bosses[0].rect.x <= 100 || bosses[0].rect.x >= screenWidth - 150)
                        bosses[0].velocity.x *= -1;
                }
                if (frameCounter % 150 == 0) {
                    for (int i = -1; i <= 1; i++) {
                        Vector2 dir = { (player.rect.x - bosses[0].rect.x) / 60.0f, (player.rect.y - bosses[0].rect.y + i * 30) / 60.0f };
                        projectiles.push_back({ { bosses[0].rect.x + 30, bosses[0].rect.y, 10, 10 }, dir });
                    }
                }
                // Function Which makes boss Dash
                if (frameCounter % 300 == 0 && !boss1Charging) {
                    boss1Charging = true;
                    boss1ChargeFrames = 30;
                    bosses[0].velocity.x = (player.rect.x < bosses[0].rect.x) ? -8 : 8;
                }
                if (boss1Charging) {
                    bosses[0].rect.x += bosses[0].velocity.x;
                    boss1ChargeFrames--;
                    if (boss1ChargeFrames <= 0) {
                        boss1Charging = false;
                        bosses[0].velocity.x = (bosses[0].rect.x < screenWidth / 2) ? 1.5f : -1.5f;
                    }
                }
                if (frameCounter % 500 == 0) {
                    for (int i = 0; i < 5; i++)
                        projectiles.push_back({ { (float)(rand() % (screenWidth - 20)), -20, 10, 20 }, { 0, 6 }, true });
                }
            }
            // Second Boss Physics
            if (level == 2 && bosses[1].health > 0) {
                bosses[1].rect.y = 300 + 40 * sinf(frameCounter * 0.05f); // Boss moves in a Sine Wave type of Path
                bosses[1].rect.x += bosses[1].velocity.x;
                if (bosses[1].rect.x <= 100 || bosses[1].rect.x >= screenWidth - 120)
                    bosses[1].velocity.x *= -1;
                if (frameCounter % 90 == 0)
                    for (int i = 0; i < 3; i++)
                        projectiles.push_back({ { bosses[1].rect.x + 30, bosses[1].rect.y + 40, 10, 10 }, { 0, 2.5f }, false, false, true, 60 });
                if (frameCounter % 360 == 0)
                    projectiles.push_back({ { bosses[1].rect.x + 35, bosses[1].rect.y + 60, 20, 20 }, { 0, 5 }, false, true });
            }
            if (bosses[0].health <= 0) doorActive[0] = true;
            if (bosses[1].health <= 0) doorActive[1] = true;

            if (level == 1 && doorActive[0] && CheckCollisionRecs(player.rect, door)) {
                level = 2;
                player.rect = { 250, groundY, 80, 120 };
                projectiles.clear();
            }
            if (level == 2 && doorActive[1] && CheckCollisionRecs(player.rect, door)) {
                level = 3;
                player.rect = { 250, groundY, 80, 120 };
                projectiles.clear();
            }

            // Third Boss Physics
            if (level == 3 && bosses[2].health > 0) {
                //Makes Boss move in x and y directions
                bosses[2].rect.y += bosses[2].velocity.y;
                bosses[2].rect.x += (sinf(frameCounter * 0.03f) * 2);   // Slight left-right sway
                if (bosses[2].rect.y < 200 || bosses[2].rect.y > groundY - bosses[2].rect.height)
                    bosses[2].velocity.y *= -1;


                if (frameCounter % 100 == 0)
                    for (int i = 0; i < 8; i++) {
                        float angle = i * 2 * PI / 8;
                        Vector2 vel = { cosf(angle) * 3, sinf(angle) * 3 };
                        projectiles.push_back({ { bosses[2].rect.x + 50, bosses[2].rect.y + 50, 10, 10 }, vel });
                    }


                if (frameCounter % 200 == 0)
                    projectiles.push_back({ { player.rect.x, -20, 20, 20 }, { 0, 6 }, false, true });
            }

            for (int i = projectiles.size() - 1; i >= 0; i--) {
                if (projectiles[i].isHoming && projectiles[i].lifetime > 0) {
                    Vector2 dir = { player.rect.x - projectiles[i].rect.x, player.rect.y - projectiles[i].rect.y };
                    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
                    if (len != 0) dir = { dir.x / len, dir.y / len };
                    projectiles[i].velocity = { dir.x * 2.5f, dir.y * 2.5f };
                    projectiles[i].lifetime--;
                }
                projectiles[i].rect.x += projectiles[i].velocity.x;
                projectiles[i].rect.y += projectiles[i].velocity.y;
                if (CheckCollisionRecs(player.rect, projectiles[i].rect)) {
                    player.health -= projectiles[i].isBigBlast ? 10 : 5;
                    PlaySound(damage);
                    projectiles.erase(projectiles.begin() + i);
                }
                else if (projectiles[i].rect.y > screenHeight || projectiles[i].rect.x < 0 || projectiles[i].rect.x > screenWidth)
                    projectiles.erase(projectiles.begin() + i);
            }
            //Player Hitting Mechanic
            if (IsKeyPressed(KEY_Z)) {
                Rectangle attack = { player.rect.x + (playerDir == 1 ? player.rect.width : -30), player.rect.y, 30, 60 };
                for (int i = 0; i < 3; i++)
                    if (level == i + 1 && bosses[i].health > 0 && CheckCollisionRecs(attack, bosses[i].rect)) {
                        bosses[i].health -= 20;
                        PlaySound(hitSound);
                    }
                PlaySound(attackSound);
                attackPoints[0] = { attack.x, attack.y + 10 };
                attackPoints[1] = { attack.x + 25 * playerDir, attack.y + 30 };
                attackPoints[2] = { attack.x, attack.y + 50 };
                showAttack = true;
                attackTimer = 10;
            }
            if (attackTimer > 0 && --attackTimer == 0) showAttack = false;
        }

        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        DrawText("Level", 10, 10, 30, WHITE);
        DrawText(TextFormat("%d", level), 90, 10, 30, YELLOW);

        Rectangle src = { player.frame * (float)(playerSheet.width / 3), 0, (float)playerSheet.width / 3, (float)playerSheet.height };
        if (playerDir == -1) src.width *= -1;
        DrawTexturePro(playerSheet, src, player.rect, { 0, 0 }, 0, WHITE);

        for (int i = 0; i < 3; i++)
            if (level == i + 1 && bosses[i].health > 0)
                DrawTexturePro(bossTextures[i], { 0, 0, (float)bossTextures[i].width, (float)bossTextures[i].height }, bosses[i].rect, { 0, 0 }, 0.0f, WHITE);

        for (auto& p : projectiles)
            DrawRectangleRec(p.rect, p.isBigBlast ? PURPLE : p.isBlade ? ORANGE : p.isHoming ? PINK : WHITE);

        if (showAttack) DrawTriangle(attackPoints[0], attackPoints[1], attackPoints[2], YELLOW);
        DrawHealthBar({ 10, 50 }, player.health, 150, BLUE);
        if (level >= 1 && bosses[level - 1].health > 0) DrawHealthBar({ screenWidth - 120, 50 }, bosses[level - 1].health, 150 + level * 10, RED);
        if (player.health <= 0) DrawText("YOU DIED", screenWidth / 2 - 100, screenHeight / 2, 40, RED);
        if (level == 3 && bosses[2].health <= 0) DrawText("VICTORY!", screenWidth / 2 - 100, screenHeight / 2, 40, GREEN);
        if ((level == 1 && doorActive[0]) || (level == 2 && doorActive[1]))
            DrawRectangleRec(door, GOLD);
        EndDrawing();
    }

    UnloadTexture(playerSheet);
    for (int i = 0; i < 3; i++) UnloadTexture(bossTextures[i]);
    UnloadTexture(background);
    UnloadSound(jumpSound);
    UnloadSound(attackSound);
    UnloadSound(hitSound);
    UnloadMusicStream(bgm);
    UnloadMusicStream(menu);
    CloseAudioDevice();
    CloseWindow();

    return 0;
} 