#include "raylib.h"
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>

struct Wire {
    Color color;
    Rectangle rect;
    bool cut;
};

Color GetRandomColor() {
    Color colors[] = { RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE };
    return colors[GetRandomValue(0, 5)];
}

std::string GetRandomWord() {
    const char* words[] = { "ALPHA", "DELTA", "OMEGA", "EAGLE", "BLAZE", "THUNDER", "NOVA", "STRIKE", "BOMB", "FIRE" };
    return words[GetRandomValue(0, 9)];
}

int main() {
    const int screenWidth = 900;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "ULTIMATE Bomb Defusal Final Build");
    SetTargetFPS(60);
    srand(time(NULL));

    float timer = 40.0f;
    bool defused = false;
    bool failed = false;

    int currentScreen = 0; // 0 = Bomb View, 1 = Assistant Terminal

    bool preWireCut = false;
    bool postWireCut = false;
    bool wordsEntered = false;
    bool codeSolved = false;

    // --- 3-word passphrase ---
    std::vector<std::string> words = { GetRandomWord(), GetRandomWord(), GetRandomWord() };
    std::string requiredPhrase = words[0] + " " + words[1] + " " + words[2];
    std::string userInput = "";

    // --- Trivia Code ---
    std::map<std::string, std::string> trivia = {
        {"Capital of France", "PARIS"},
        {"Largest mammal", "BLUEWHALE"},
        {"Fastest land animal", "CHEETAH"},
        {"Capital of Japan", "TOKYO"},
        {"Black and white bear", "PANDA"},
        {"National bird of USA", "EAGLE"}
    };
    auto it = std::next(trivia.begin(), GetRandomValue(0, trivia.size() - 1));
    std::string triviaHint = it->first;
    std::string triviaAnswer = it->second;
    std::string userCodeInput = "";

    // --- Wires ---
    std::vector<Wire> wires;
    int wireCount = 6;
    int yStart = 230;
    int spacing = 45;
    int wireToCutBefore = GetRandomValue(0, wireCount - 1);
    int wireToCutAfter = GetRandomValue(0, wireCount - 1);
    while (wireToCutAfter == wireToCutBefore) wireToCutAfter = GetRandomValue(0, wireCount - 1);

    for (int i = 0; i < wireCount; i++) {
        wires.push_back({ GetRandomColor(), { 100, (float)(yStart + i * spacing), 500, 30 }, false });
    }

    SetExitKey(KEY_NULL);

    while (!WindowShouldClose()) {
        // Timer ticking
        if (!defused && !failed) {
            timer -= GetFrameTime();
            if (timer <= 0) {
                timer = 0;
                failed = true;
            }
        }

        // Press O to switch screens
        if (IsKeyPressed(KEY_ONE)) {
            currentScreen = (currentScreen + 1) % 2;
        }

        // --------------------------
        // MAIN LOGIC: BOMB SCREEN
        // --------------------------
        if (currentScreen == 0 && !defused && !failed) {
            // Cut wires
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                for (int i = 0; i < wires.size(); i++) {
                    if (!wires[i].cut && CheckCollisionPointRec(mouse, wires[i].rect)) {
                        wires[i].cut = true;

                        if (!preWireCut) {
                            if (i == wireToCutBefore) preWireCut = true;
                            else failed = true;
                        }
                        else if (codeSolved && !postWireCut) {
                            if (i == wireToCutAfter) postWireCut = true;
                            else failed = true;
                        }
                    }
                }
            }

            // Type phrase
            if (preWireCut && !wordsEntered) {
                int key = GetCharPressed();
                while (key > 0) {
                    if (key >= 32 && key <= 125) userInput += (char)key;
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && !userInput.empty()) userInput.pop_back();

                if (userInput.length() == requiredPhrase.length()) {
                    if (userInput == requiredPhrase) wordsEntered = true;
                    else failed = true;
                }
            }

            // Enter trivia code
            if (wordsEntered && !codeSolved) {
                int key = GetCharPressed();
                while (key > 0) {
                    if (key >= 32 && key <= 125) userCodeInput += (char)key;
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && !userCodeInput.empty()) userCodeInput.pop_back();

                if (userCodeInput.length() == triviaAnswer.length()) {
                    if (userCodeInput == triviaAnswer) codeSolved = true;
                    else failed = true;
                }
            }

            if (preWireCut && wordsEntered && codeSolved && postWireCut) defused = true;
        }

        // --------------------------
        // DRAWING START
        // --------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("? Time Left: %.1f", timer), 20, 20, 40, WHITE);
        DrawText("Press 'O' to switch screen", 20, 70, 20, GRAY);

        if (currentScreen == 0) {
            // BOMB SCREEN - NO INSTRUCTIONS
            DrawText("?? BOMB VIEW", 20, 110, 30, RED);
            for (int i = 0; i < wires.size(); i++) {
                Color color = wires[i].cut ? DARKGRAY : wires[i].color;
                DrawRectangleRec(wires[i].rect, color);
                DrawText(TextFormat("#%d", i + 1), wires[i].rect.x - 40, wires[i].rect.y + 5, 20, WHITE);
            }

            // Inputs only
            DrawText(userInput.c_str(), 20, 560, 25, WHITE);
            DrawText(userCodeInput.c_str(), 20, 590, 25, WHITE);

        }
        else {
            // ASSISTANT TERMINAL VIEW
            ClearBackground(DARKBLUE);
            DrawText("?? COMPUTER ASSISTANT", 20, 40, 35, YELLOW);
            DrawText("Press '1' to return to bomb screen", 20, 80, 20, GRAY);

            DrawText(TextFormat("STEP 1: Cut wire #%d", wireToCutBefore + 1), 40, 150, 30, GREEN);
            DrawText(TextFormat("STEP 2: Type the phrase: %s", requiredPhrase.c_str()), 40, 200, 25, SKYBLUE);
            DrawText("STEP 3: Solve the following riddle:", 40, 250, 25, ORANGE);
            DrawText(TextFormat("?? %s", triviaHint.c_str()), 60, 280, 28, LIGHTGRAY);
            DrawText(TextFormat("STEP 4: Cut wire #%d", wireToCutAfter + 1), 40, 330, 30, GREEN);
        }

        if (defused) {
            DrawText("?? Bomb Defused! You're a legend!", 180, 500, 35, GREEN);
        }
        else if (failed) {
            DrawText("?? BOOM! Mission Failed!", 250, 500, 40, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}




