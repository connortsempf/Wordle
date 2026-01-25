/**
 * @file WordleEngine.hpp
 * @brief Wordle Engine Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the WordleEngine class, which
 * provides the core functionality for the Wordle word guessing game.
 */


#pragma once
#include <string>
#include <array>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cctype>
#include <ranges>
#include <fstream>
#include "Model/LetterState.hpp"
#include "Model/GameConfig.hpp"


/**
 * @brief Provides the core functionality for the Wordle word guessing game.
 */
class WordleEngine {

    public:
        /**
         * @brief Difficulty mode for each game.
         */
        enum class Difficulty { NORMAL_MODE, HARD_MODE };

        /**
         * @brief Represents the validity of a letter in a guess.
         */
        enum class GuessValidity {
            GameInactive,
            GuessTooShort,
            GuessTooLong,
            GuessWordInvalid,
            PreviousCorrectLetterWordInvalid,
            PreviousPresentLetterWordInvalid,
            GuessIncorrect,
            GameWon,
            GameLost
        };

    public:
        /**
         * @brief Struct describing the outcome of a guess attempt.
         */
        struct GuessResult {
            /**
             * @brief The guess word for the attempt.
             */
            std::string guess = "";

            /**
             * @brief Number of attempts used so far (including this one).
             */
            unsigned int numAttempts = 0;

            /**
             * @brief The validity of the guess.
             */
            GuessValidity guessValidity;

            /**
             * @brief Message associated guess validity for the guess result.
             */
            std::string message = "";

            /**
             * @brief The letter feedback for this guess (correct/present/absent).
             */
            std::array<LetterState, GameConfig::WORD_LENGTH> letterStates;
        };

    public:
        /**
         * @brief Default constructor for WordleEngine.
         */
        WordleEngine() = default;

        /**
         * @brief Starts a new game by selecting a new target word and resetting attempts.
         */
        void startNewGame();

        /**
         * @brief Get whether the game is active.
         */
        bool getIsActive() const;

        /**
         * @brief Set the difficulty mode of the game.
         */
        void setDifficulty(Difficulty difficulty);

        /**
         * @brief Makes a guess and returns whether it was successful.
         *
         * @param guess A 5-character array representing the guessed word.
         */
        GuessResult makeGuess(std::string guess);

        /**
         * @brief Get the target word.
         *
         * @return The target word as a const reference to a std::string.
         */
        const std::string& getTargetWord() const;

        /**
         * @brief Get the game difficulty mode.
         *
         * @return The difficulty as a const reference to a Difficulty enum.
         */
        const Difficulty& getDifficulty() const;

    private:
        /**
         * @brief Reuired letter slots for hard mode.
         */
        struct HardModeRequirements {
            /**
             * @brief Slots for letters that must be reused in an exact position.
             */
            std::array<std::string, GameConfig::WORD_LENGTH> correctLetters = {"", "", "", "", ""};

            /**
             * @brief Slots for letters that must be reused in any position.
             */
            std::vector<std::string> presentLetters;
        };

    private:
        /**
         * @brief Indicates whether a game is currently active.
         */
        bool gameActive = false;

        /**
         * @brief The difficulty mode tracking variable.
         */
        Difficulty difficulty = Difficulty::NORMAL_MODE;

        /**
         * @brief The current guess attempt number that the user is CURRENTLY ON.
         */
        unsigned int numAttempts = 0;

        /**
         * @brief The target word to be guessed.
         */
        std::string targetWord = "";

        /**
         * @brief Hard mode requirements tracking variable.
         */
        HardModeRequirements hardModeRequirements;

    private:
        /**
         * @brief Picks a target word for the game.
         */
        void pickTargetWord();

        /**
         * @brief Checks if a word is valid 5-letter word (exists in english language).
         *
         * @param word The word to check.
         *
         * @return True if the word exists, false otherwise.
         */
        bool isValidWord(const std::string& word);

        /**
         * @brief Checks if a guess word adheres to hard mode requirements.
         *
         * @param guess The guess word to check for adherence.
         * @param guessResult The guess result info to update if the guess is not adhered.
         *
         * @return True if the guess adheres to hard mode requirements, false otherwise.
         */
        bool isHardModeAdhered(const std::string& guess, GuessResult& guessResult);

        /**
         * @brief Check if a guess is correct based on its letter states result.
         */
        bool isCorrectGuess(std::array<LetterState, GameConfig::WORD_LENGTH> letterStates);

        /**
         * @brief Update the hard mode requirements struct.
         *
         * @param guess The current guess.
         * @param letterStates The letter states from the current guess's evaluation.
         */
        void updateHardModeRequirements(const std::string& guess, std::array<LetterState, GameConfig::WORD_LENGTH> letterStates);

        /**
         * @brief Evaluates the most recent guess and updates the guess results.
         */
        std::array<LetterState, GameConfig::WORD_LENGTH> evaluateGuess(std::string guess);
};
