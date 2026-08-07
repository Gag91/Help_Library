#include "hp/help.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <thread>


// ----- THIS IS A TEST FILE

int main() {
    hp::cls();
    hp::enableUTF8();

    std::cout << "========== TESTING hp LIBRARY ==========\n\n";

    // ============================================================
    // 1. TEST COLORS
    // ============================================================
    std::cout << "1. TESTING COLORS:\n";
    hp::printCl("This is RED text\n", hp::RED);
    hp::printCl("This is GREEN text\n", hp::GREEN);
    hp::printCl("This is BLUE text\n", hp::BLUE);
    hp::printlnCl("This is YELLOW text with newline", hp::YELLOW);
    std::cout << hp::getColorCode(hp::CYAN) << "This is CYAN text" << hp::getColorCode(hp::RESET) << "\n\n";

    // ============================================================
    // 2. TEST REPEAT STRING
    // ============================================================
    std::cout << "2. TESTING REPEAT STRING:\n";
    std::cout << hp::repeatString("=", 20) << "\n";
    std::cout << hp::repeatString("abc", 5) << "\n\n";

    // ============================================================
    // 3. TEST CENTERED BOX
    // ============================================================
    std::cout << "3. TESTING CENTERED BOX:\n";
    hp::centeredBox(12, "HELLO WORLD", hp::CYAN);
    hp::centeredBox(16, "ASCII STYLE", hp::GREEN, hp::ASCII);
    hp::centeredBox(20, "EXTENDED STYLE", hp::YELLOW, hp::EXTENDED);
    hp::centeredBox(24, "DOUBLE STYLE", hp::RED, hp::DOUBLE);
    std::cout << "\n";

    // ============================================================
    // 4. TEST STRING UTILITIES
    // ============================================================
    std::cout << "4. TESTING STRING UTILITIES:\n";
    std::string testStr = "Hello World";
    std::cout << "starts_with('Hello World', 'Hello'): " << hp::starts_with(testStr, "Hello") << "\n";
    std::cout << "ends_with('Hello World', 'World'): " << hp::ends_with(testStr, "World") << "\n";
    std::cout << "contains('Hello World', 'lo W'): " << hp::contains(testStr, "lo W") << "\n\n";

    // ============================================================
    // 5. TEST RANDOM
    // ============================================================
    std::cout << "5. TESTING RANDOM:\n";
    std::cout << "Random int (1-10): " << Random::rand(1, 10) << "\n";
    std::cout << "Random double (0.0-1.0): " << Random::rand(0.0, 1.0) << "\n";

    std::vector<int> vec = { 10, 20, 30, 40, 50 };
    std::cout << "Random choice from vector: " << Random::choice(vec) << "\n";
    std::cout << "Random choice from list: " << Random::choice({ 1, 2, 3, 4, 5 }) << "\n";

    Random::shuffle(vec);
    std::cout << "Shuffled vector: ";
    for (int v : vec) std::cout << v << " ";
    std::cout << "\n\n";

    // ============================================================
    // 6. TEST FIELD
    // ============================================================
    std::cout << "6. TESTING FIELD:\n";
    hp::Field(28, 10, 30, "Field Test", hp::GREEN, true);
    std::cout << "\n\n";

    // ============================================================
    // 7. TEST TIMER
    // ============================================================
    std::cout << "7. TESTING TIMER:\n";
    auto start = hp::startTimer();
    hp::wait(0.5);
    double elapsed = hp::stopTimer(start);
    std::cout << "Elapsed time (0.5s wait): " << elapsed << "s\n";
    hp::displayTimer(start, "Timer test");
    std::cout << "\n";

    // ============================================================
    // 8. TEST SUMMING
    // ============================================================
    std::cout << "8. TESTING SUMMING:\n";
    std::vector<int> nums = { 1, 2, 3, 4, 5 };
    std::cout << "sumAll({1,2,3,4,5}): " << hp::sumAll(nums) << "\n";
    std::cout << "\n";

    // ============================================================
    // 9. TEST CONTAINS
    // ============================================================
    std::cout << "9. TESTING CONTAINS:\n";
    std::vector<int> nums2 = { 1, 2, 3, 4, 5 };
    std::cout << "contains({1,2,3,4,5}, 3): " << hp::containsAny(nums2, 3) << "\n";
    std::cout << "contains({1,2,3,4,5}, 6): " << hp::containsAny(nums2, 6) << "\n";
    std::cout << "contains({1,2,3,4,5}, 2, 4): " << hp::containsAny(nums2, 2, 4) << "\n\n";

    // ============================================================
    // 10. TEST INDEX OF
    // ============================================================
    std::cout << "10. TESTING INDEX OF:\n";
    std::vector<int> nums3 = { 10, 20, 30, 40, 50 };
    std::cout << "indexOf({10,20,30,40,50}, 30): " << hp::indexOf(nums3, 30) << "\n";
    std::cout << "indexOf({10,20,30,40,50}, 100): " << hp::indexOf(nums3, 100) << "\n\n";

    // ============================================================
    // 11. TEST MENU
    // ============================================================
    std::cout << "11. TESTING MENU (arrow keys to navigate, Enter to select):\n";
    std::vector<std::string> options = { "Option 1", "Option 2", "Option 3", "Exit" };
    int choice = hp::arrowMenu("TEST MENU", options);
    std::cout << "You selected: " << choice << " (" << options[choice] << ")\n\n";

    // ============================================================
    // 12. TEST CALCULATOR
    // ============================================================
    std::cout << "12. TESTING CALCULATOR:\n";
    std::cout << "calc<int>('5+3'): " << hp::calc<int>("5+3") << "\n";
    std::cout << "calc<double>('5.5+3.2'): " << hp::calc<double>("5.5+3.2") << "\n";
    std::cout << "calc<double>('2^3'): " << hp::calc<double>("2^3") << "\n\n";

    // ============================================================
    // 13. TEST TITLE & SP
    // ============================================================
    std::cout << "13. TESTING TITLE & SP:\n";
    hp::title("TEST TITLE", 30);
    hp::sp("TEST SP", 30);
    std::cout << "\n";

    // ============================================================
    // 14. TEST CLS CLEAR
    // ============================================================
    std::cout << "14. TESTING CLS CLEAR (press Enter to clear screen):\n";
    hp::waitForEnter();
    hp::cls();

    std::cout << "Screen cleared! Testing complete.\n";
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
    }