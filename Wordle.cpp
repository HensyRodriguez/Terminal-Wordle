#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <algorithm>
#include <string>
#include <filesystem>
#include <unordered_map>
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

inline void clearScreen() {std::cout << "\x1b[J";}
inline void topLeftCurs() {std::cout << "\x1b[H";}
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

    topLeftCurs();
    clearScreen();

    std::cout << std::setw(60) << std::setfill('=')
              << "|Welcome to Wordle!|" << std::setw(50) << std::setfill('=') << '\n';

    bool validFlag{false}; // Flag to determine if it's valid word or not.
    while(guessed != word && numGuessed > 0)
    {
        std::cout << "Enter your guess: ";
        std::getline(std::cin, user_guess);

        if(user_guess.size() != word.size())
        {
            std::cout << "Guess must be " << word.size() << " characters.\n";
            continue;
        }

        // Makes the user input be all lowercase
        std::transform(user_guess.begin(), user_guess.end(), user_guess.begin(), [](unsigned char c){ return std::tolower(c); });

        // Check the word vector to make sure it matches a word in the list
        for(const auto& eng_word: englishVector)
        {
            if(user_guess == eng_word)
            {
                validFlag = true;
                break;
            }
        }

        if(validFlag)
        {
            for(size_t i = 0; i < word.size(); i++)
            {
                if(user_guess[i] == word[i])
                {
                    guessed[i] = word[i];
                } else {
                    guessed[i] = '-';
                }
            }
            // rebuild frequency map each guess
            std::unordered_map<char, int> freq;
            for (char c : word) {
                freq[c]++; // Incr the key(int) per each value (char) found
            }

            // If a guess matches make that color green
            std::vector<std::string> colors(word.size(), "");
            for (size_t i = 0; i < word.size(); i++) {
                if (user_guess[i] == word[i]) {
                    colors[i] = MY_ANSI_GREEN; // Apply index to be green
                    freq[user_guess[i]]--; // If matches reduce key freq
                }
            }

            // Handle coloring orange or red if valid or duplicate
            for (size_t i = 0; i < word.size(); i++) {
                if (!colors[i].empty()) continue; // already green
                char c = user_guess[i];
                if (freq[c] > 0) { // If value key is more than 0, dup found
                    colors[i] = MY_ANSI_ORANGE;
                    freq[c]--; // Reduce the key's value
                } else {
                    colors[i] = MY_ANSI_RED; // No dup and no val thus red
                }
            }

            // Modify to index color, then char and then clear ANSI attrib.
            for (size_t i = 0; i < word.size(); i++) {
                std::cout << colors[i] << user_guess[i] << ANSI_CLEAR_ATTRIB;
            }

            if(guessed == word) {
                break;
            } else {
                --numGuessed;
                std::cout << "\nAttempts left: " << numGuessed << '\n';
            }

            // Set the flag back to 
            validFlag = false;
        } else { // If !validFlag
            std::cerr << "Word is not in the word bank" << '\n';
        }
        
    } // While loop

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
