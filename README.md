# 🕹️ Hollow Knight LITE – Raylib C++ Game

A fast-paced action platformer inspired by **Hollow Knight**, built completely in **C++** using the **Raylib** library.  
Fight through **3 unique boss battles**, dodge complex attack patterns, unlock levels, and enjoy smooth animation & SFX!

---

## 🎮 Gameplay Features

### ✅ Player Mechanics
- Smooth movement with left/right keys  
- Jumping with **SPACE**  
- Basic melee attack using **Z**  
- Animated player sprite sheet  
- Health bar UI

### 🛡️ Boss Battles
There are **3 different bosses**, each with unique mechanics:

#### **Boss 1 – The Charger**
- Dashes aggressively across the arena  
- Shoots triple projectiles aimed at the player  
- Spawns falling blades  

#### **Boss 2 – The Sentry**
- Moves in a **sine-wave** pattern  
- Fires **homing projectiles**  
- Uses a heavy **blast attack**  

#### **Boss 3 – The Void Beast**
- Floats and moves vertically  
- Emits circular projectile rings  
- Creates large falling shockwaves  

Beat a boss → the door opens → enter to reach the next level.

---

## 🔊 Audio & Visual Assets
- Background music, menu music, jump, hit, damage, and attack sounds  
- Player animation sheet  
- Three boss textures  
- Background image  
- Menu animation with floating title  

(All assets are loaded at runtime from the project directory.)

---

## 🕹️ Controls
| Action | Key |
|--------|-----|
| Move left | **A / Left Arrow** |
| Move right | **D / Right Arrow** |
| Jump | **SPACE** |
| Attack | **Z** |
| Start Game | **ENTER** |

---

## 🧾 Requirements

### Install Raylib
You must have **Raylib** installed and linked.  
Download from:  
https://www.raylib.com

### Compiler
- g++, MinGW, or MSVC  
- C++17 or later

---

###Authors :
-Muhammad Ahmed Saeed 

-Muhammad Mutaal Khan

## ▶️ How to Run

### **Windows (Visual Studio project included)**  
1. Open **Raylib Project.vcxproj**  
2. Press **Build → Run**  

OR

### **Manual g++ Compile**
```bash
g++ Source.cpp -o game.exe -lraylib -lopengl32 -lgdi32 -lwinmm
Run:

./game.exe

📁 Project Structure
/assets
  jump.wav
  attack.wav
  menu.wav
  background.wav
  hit.wav
  damage.wav
  charmulti.png
  bos1.png
  bos2.png
  bos3.png
  Backgorund.png

Source.cpp        → Main game logic  (boss fights, player physics)
main.cpp          → Bomb defusal mini-game build (unused in main exe)
Raylib Project.vcxproj   → Visual Studio configuration

🏆 Goal of the Game

✔️ Survive
✔️ Defeat all 3 bosses
✔️ Reach the end door
✔️ Claim victory!

💡 Inspiration

Hollow Knight LITE is a learning project inspired by Team Cherry’s Hollow Knight, rebuilt using Raylib to practice:

game loops

sprite animation

boss AI

collision systems

physics

sound & music streaming

📜 License

This game is for learning and personal use.
Do not redistribute copyrighted Hollow Knight assets.


