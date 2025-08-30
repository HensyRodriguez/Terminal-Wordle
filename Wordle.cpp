#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <string>
#include <string_view>
#include <filesystem>
#include <vector>

const std::string_view ANSI_CLEAR_ATTRIB {"\x1b[0m"};
const std::string_view MY_ANSI_RED       {"\x1b[38;2;255;0;0m"};
const std::string_view MY_ANSI_ORANGE    {"\x1b[38;2;255;200;0m"};
const std::string_view MY_ANSI_GREEN     {"\x1b[38;2;0;255;0m"};

std::vector<std::string> englishVector{};

/**
 * 1. word = plums
 * 2. result = ""
 * 3. wordGuessed = boats
 * 4. print("Doesn't match") attempts go down by 1 max 5 attempts
 * 5.result = ----s
 * 6. Go back to 3 but make a new guess
 **/

static void clearScreen() {std::cout << "\x1b[J";}
static void topLeftCurs() {std::cout << "\x1b[H";}
bool fileToVector (const std::filesystem::path& filename);


int main(void)
{
    std::random_device rd{};
    std::mt19937 gen(rd());
    
    if(!fileToVector("words.txt")) {
        return -1;
    }

    std::uniform_int_distribution<int> dist(0, englishVector.size());

    std::string user_guess{};
    std::string word{englishVector[dist(gen)]};
    std::string guessed(word.size(), '-');

    unsigned int numGuessed{5};

    std::vector<std::string> colors(guessed.size(), "");

    topLeftCurs();
    clearScreen();

    std::cout << std::setw(60) << std::setfill('=')
              << "|Welcome to Wordle!|" << std::setw(50) << std::setfill('=') << '\n';

    while(guessed != word && numGuessed > 0)
    {
        std::cout << "Enter your guess: ";
        std::getline(std::cin, user_guess);

        if(user_guess.size() != word.size())
        {
            std::cout << "Guess must be " << word.size() << " characters.\n";
            continue;
        }
        
        for(size_t i = 0; i < word.size(); i++)
        {
            if(user_guess[i] == word[i])
            {
                guessed[i] = word[i];
            } else {
                guessed[i] = '-';
            }
        }

        
        for(size_t i{0}; i < guessed.size(); i++)
        {
            if(user_guess[i] == guessed[i])
            {
                std::cout << MY_ANSI_GREEN << user_guess[i]
                          << ANSI_CLEAR_ATTRIB;
            } else {
                if(word.find(user_guess[i]) != std::string::npos) {
                             std::cout << MY_ANSI_ORANGE << user_guess[i]
                              << ANSI_CLEAR_ATTRIB;
                             
                } else {
                    std::cout << MY_ANSI_RED << user_guess[i]
                              << ANSI_CLEAR_ATTRIB;
                }
                
            }
        }

        std::cout << "\nGuessed: " << guessed << '\n';

        if(guessed == word) {
            break;
        } else {
            --numGuessed;
            std::cout << "\nAttempts left: " << numGuessed << '\n';
        }
        
    }

    if(numGuessed == 0)
    {
        std::cout << "You lose! Word was: " << word << std::endl;
    }
    else if(guessed == word) {
        std::cout << "\nYou won! Word was: " << word << std::endl;
    }

     return 0;
}

bool fileToVector (const std::filesystem::path& filename)
{
    std::ifstream file{filename, std::ios::in};
    if(!file.is_open()) {
        std::cerr << "Failed to open " << filename.string() << std::endl;
        return false;
    }

    std::string line{};
    while(std::getline(file, line)) {
        englishVector.push_back(line);
    }
    
    file.close();
    return true;
}
