# Help_Library

A modern C++ utility library with colors, input, keyboard, math, and more!

## Features

-  **Colors** – ANSI color support for terminal output
-  **Keyboard** – Real-time keyboard input detection | Uses of Windows API
-  **Input** – Beautiful input fields with validation
-  **Random** – Random number generation
-  **Math** – Arithmetic operations and calculators
-  **Files** – File and folder management
-  **Time** – Timers and waiting functions
-  **Menus** – Interactive terminal menus

##  Quick Start

```cpp
#include "hp/help.hpp"

int main() {
    hp::cls();
    hp::printCl("Hello, World!", hp::GREEN);
    hp::wait(1);
    return 0;
}
```

## Demo

```cpp
#include "hp/help.hpp"

int main() {
    hp::cls(); // Clear terminal
    
    // Colored text
    hp::printCl("Hello, World!", hp::GREEN);
    hp::printlnCl("This is a test", hp::CYAN);
    
    // Random number
    int num = hp::Random::rand(1, 100);
    hp::printlnCl("Random number: " + std::to_string(num), hp::YELLOW);
    
    // Centered box
    hp::centeredBox(10, "Welcome to hp!", hp::MAGENTA);
    
    // Input field
    std::string name = hp::CenteredInput(14, 30, "Enter your name");
    
    hp::wait(2); 
    return 0;
}
```

## Functions

### Colors
- `hp::printCl(msg, color)` – Print colored text
- `hp::printlnCl(msg, color)` – Print colored text with newline
- `hp::getColorCode(color)` – Get ANSI color code

### Input
- `hp::inputField(row, col, width, placeholder)` – Input field
- `hp::CenteredInput(row, width, msg)` – Centered input field
- `hp::inputInt()`, `hp::inputFloat()`, `hp::inputAlpha()`, `hp::inputAlphaNum()`

### Keyboard
- `hp::updateKeyboard()` – Update keyboard states
- `hp::KeyIsPressed::Key` – Check if key was pressed
- `hp::KeyIsHold::Key` – Check if key is held

### Random
- `hp::Random::rand(min, max)` – Random number
- `hp::Random::choice(vector)` – Random item from vector
- `hp::Random::shuffle(vector)` – Shuffle vector

### Math
- `hp::add(a, b)` – Add two numbers
- `hp::calc<T>("5+3")` – Calculate expression
- `hp::calcAdvanced<T>("5+3*2")` – Advanced calculation

### Menus
- `hp::arrowMenu(title, options)` – Arrow-key navigable menu
- `hp::CenteredMenu(title, options)` – Centered menu
- `hp::CenteredFieldMenu(startRow, width, options)` – Field-based menu + Arrow-key navigable menu

### Time
- `hp::wait(time)` - Wait in seconds
- `hp::wait_ms(time)` - Wait in milliseconds

## Installation
Copy the include/hp/ folder to your project and include hp/help.hpp.

## How to Compile

```bash
g++ -std=c++20 -Iinclude your_file.cpp -o your_program.exe
```

## Notes
This was made by me to improve my C++ skills and experience.

## License
MIT License
Check LICENSE file for details

## Requirements
- **Windows** (uses Windows API for keyboard input)
- **C++20** or later (uses concepts, ranges, fold expressions)
- **C++ compiler** (GCC, Clang, or MSVC)

## Coming Soon...
- **2D Terminal Graphics Library** – Draw shapes, sprites(not sure), and animations(not sure) in the console
- **GUI Elements** – Progress bars, loading spinners, tables
- **Data Structures** – Stack, Queue, LinkedList implementations
- **Encryption** – MD5, Base64 encoding
- **Sounds** - Sounds effect
- **Buttons** - Clickable buttons whit colors and sound effects

## Contacts
- Discord: xxavi640
- Email: rzm664477@gmail.com 
