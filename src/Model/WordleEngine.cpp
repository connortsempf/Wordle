/**
 * @file WordleEngine.cpp
 * @brief Wordle Engine Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the WordleEngine class, which
 * provides the core functionality for the Wordle word guessing game.
 */


#include "Model/WordleEngine.hpp"


/**
 * @brief Starts a new game by selecting a new target word and resetting attempts.
 */
void WordleEngine::startNewGame() {
    gameActive = true;
    numAttempts = 0;
    for (auto& correctLetter : hardModeRequirements.correctLetters) {
        correctLetter = "";
    }
    hardModeRequirements.presentLetters.clear();
    pickTargetWord();
}


/**
 * @brief Get whether the game is active.
 */
bool WordleEngine::getIsActive() const {
    return gameActive;
}


/**
 * @brief Set the difficulty mode of the game.
 */
void WordleEngine::setDifficulty(WordleEngine::Difficulty difficulty) {
    if (numAttempts > 0 && this->difficulty == Difficulty::NORMAL_MODE && difficulty == Difficulty::HARD_MODE) return;
    this->difficulty = difficulty;
}


/**
 * @brief Makes a guess and returns whether it was successful.
 *
 * @param guess A 5-character array representing the guessed word.
 */
WordleEngine::GuessResult WordleEngine::makeGuess(std::string guess) {
    // Convert Guess to lowercase //
    std::transform(guess.begin(), guess.end(), guess.begin(), [](unsigned char c){ return static_cast<char>(std::tolower(c)); });

    GuessResult guessResult;
    guessResult.guess = guess;
    if (!gameActive) {
        guessResult.guessValidity = GuessValidity::GameInactive;
        guessResult.message = "";
    }
    else if (guess.length() < GameConfig::WORD_LENGTH) {
        guessResult.guessValidity = GuessValidity::GuessTooShort;
        guessResult.message = "Not enough letters";
    }
    else if (guess.length() > GameConfig::WORD_LENGTH) {
        guessResult.guessValidity = GuessValidity::GuessTooLong;
        guessResult.message = "Too many letters";
    }
    else if (!isValidWord(guess)) {
        guessResult.guessValidity = GuessValidity::GuessWordInvalid;
        guessResult.message = "Not in word list";
    }
    else if ((difficulty == WordleEngine::Difficulty::HARD_MODE && isHardModeAdhered(guess, guessResult)) || difficulty == WordleEngine::Difficulty::NORMAL_MODE) {
        std::array<LetterState, GameConfig::WORD_LENGTH> letterStates = evaluateGuess(guess);
        guessResult.numAttempts = numAttempts;

        bool correctGuess = isCorrectGuess(letterStates);
        if (correctGuess) {
            std::vector<std::string> successMessages = { "Genius", "Magnificent", "Impressive", "Splendid", "Great", "Phew" };
            guessResult.guessValidity = GuessValidity::GameWon;
            guessResult.message = successMessages[numAttempts - 1];
        }
        else if (!correctGuess) {
            if (difficulty == WordleEngine::Difficulty::HARD_MODE) updateHardModeRequirements(guess, letterStates);
            if (numAttempts < GameConfig::NUM_GUESSES) guessResult.guessValidity = GuessValidity::GuessIncorrect;
            else if (numAttempts >= GameConfig::NUM_GUESSES) {
                guessResult.guessValidity = GuessValidity::GameLost;
                std::string correctWord = targetWord;
                std::transform(correctWord.begin(), correctWord.end(), correctWord.begin(), [](unsigned char c){ return static_cast<char>(std::toupper(c)); });
                guessResult.message = correctWord;
            }
        }
        guessResult.letterStates = letterStates;
    }

    return guessResult;
}


/**
 * @brief Get the target word.
 *
 * @return The target word as a const reference to a std::string.
 */
const std::string& WordleEngine::getTargetWord() const {
    return targetWord;
}


/**
 * @brief Get the game difficulty mode.
 *
 * @return The difficulty as a const reference to a Difficulty enum.
 */
const WordleEngine::Difficulty& WordleEngine::getDifficulty() const {
    return difficulty;
}


/**
 * @brief Picks a target word for the game.
 */
void WordleEngine::pickTargetWord() {
    std::ifstream file("../assets/wordlists/target-words.txt");

    // Fallback Word //
    if (!file.is_open()) {
        targetWord = "apple";
        return;
    }

    std::vector<std::string> words;
    std::string word;
    while (std::getline(file, word)) {
        if (!word.empty()) {
            words.push_back(word);
        }
    }
    file.close();

    // Fallback Word //
    if (words.empty()) {
        targetWord = "apple";
        return;
    }

    // Seed RNG for Random Word //
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, words.size() - 1);
    targetWord = words[dist(gen)];
}


/**
 * @brief Checks if a word is valid 5-letter word (exists in english language).
 *
 * @param word The word to check.
 *
 * @return True if the word exists, false otherwise.
 */
bool WordleEngine::isValidWord(const std::string& word) {
    std::ifstream file("../assets/wordlists/valid-words.txt");

    // Error Opening File //
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line == word) {
            file.close();
            return true;
        }
    }

    // Word Not Found in words.txt -- Invalid //
    file.close();
    return false;
}


/**
 * @brief Checks if a guess word adheres to hard mode requirements.
 *
 * @param guess The guess word to check for adherence.
 * @param guessResult The guess result info to update if the guess is not adhered.
 *
 * @return True if the guess adheres to hard mode requirements, false otherwise.
 */
bool WordleEngine::isHardModeAdhered(const std::string& guess, GuessResult& guessResult) {
    const std::array<std::string, GameConfig::WORD_LENGTH> indexOrdinals = { "1st", "2nd", "3rd", "4th", "5th" };

    for (int i = 0; i < GameConfig::WORD_LENGTH; i++) {
        const std::string guessedLetter(1, guess[i]);
        const std::string previousCorrectLetter = hardModeRequirements.correctLetters[i];

        // Previously Correct Letter Not Reused //
        if (previousCorrectLetter != "" && guessedLetter != hardModeRequirements.correctLetters[i]) {
            guessResult.guessValidity = GuessValidity::PreviousCorrectLetterWordInvalid;
            guessResult.message = indexOrdinals[i] + " letter must be " + std::string(1, std::toupper(previousCorrectLetter[i]));
            return false;
        }

        // Previously Present Letter Not Reused //
        for (const auto& presentLetter : hardModeRequirements.presentLetters) {
            if (guess.find(presentLetter) == std::string::npos) {
                guessResult.guessValidity = GuessValidity::PreviousPresentLetterWordInvalid;
                guessResult.message = "Guess must contain " + std::string(1, std::toupper(static_cast<unsigned char>(presentLetter[0])));
                return false;
            }
        }
    }

    return true;
}


/**
 * @brief Check if a guess is correct based on its letter states result.
 */
bool WordleEngine::isCorrectGuess(std::array<LetterState, GameConfig::WORD_LENGTH> letterStates) {
    bool correctGuess = true;
    for (const LetterState& letterState : letterStates) {
        if (letterState != LetterState::CORRECT) return false;
    }
    return true;
}


/**
 * @brief Update the hard mode requirements struct.
 *
 * @param guess The current guess.
 * @param letterStates The letter states from the current guess's evaluation.
 */
void WordleEngine::updateHardModeRequirements(const std::string& guess, std::array<LetterState, GameConfig::WORD_LENGTH> letterStates) {
    hardModeRequirements.presentLetters.clear();
    for (auto& letter : hardModeRequirements.correctLetters) letter = "";

    for (int i = 0; i < guess.size(); i++) {
        if (letterStates[i] == LetterState::CORRECT) hardModeRequirements.correctLetters[i] = guess[i];
        else if (letterStates[i] == LetterState::PRESENT) hardModeRequirements.presentLetters.push_back(std::string(1, guess[i]));
    }
}


/**
 * @brief Evaluates a guess against the target word.
 */
std::array<LetterState, GameConfig::WORD_LENGTH> WordleEngine::evaluateGuess(std::string guess) {
    std::array<LetterState, GameConfig::WORD_LENGTH> letterStates;
    std::array<bool, GameConfig::WORD_LENGTH> targetUsed = {false, false, false, false, false};

    // First Pass -- Mark All Correct (Green) Letters //
    for (size_t i = 0; i < GameConfig::WORD_LENGTH; ++i) {
        if (guess[i] == targetWord[i]) {
            letterStates[i] = LetterState::CORRECT;
            targetUsed[i] = true;
        }   else {
            letterStates[i] = LetterState::ABSENT; // Temporary //
        }
    }

    // Second Pass -- Mark Present (Yellow) Letters //
    for (size_t i = 0; i < GameConfig::WORD_LENGTH; ++i) {
        // Already Marked as Correct //
        if (letterStates[i] == LetterState::CORRECT) continue;

        // Look for Unused Matching Letter in Target //
        for (size_t j = 0; j < GameConfig::WORD_LENGTH; ++j) {
            if (!targetUsed[j] && guess[i] == targetWord[j]) {
                letterStates[i] = LetterState::PRESENT;
                targetUsed[j] = true;
                break;
            }
        }
    }

    ++numAttempts;
    if (guess == targetWord) gameActive = false;
    return letterStates;
}
