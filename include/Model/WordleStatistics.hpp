/**
 * @file WordleStatistics.hpp
 * @brief Wordle Statistics Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the WordleStatistics class, which
 * manages the wordle game statistics over indefinite play-throughs.
 */


#pragma once
#include <array>
#include "Model/GameConfig.hpp"


/**
 * @brief
 */
class WordleStatistics {

    public:
        /**
         * @brief Statistics struct that contains all necessary stats data for wordle games.
         */
        struct Statistics {
            /**
             * @brief The number of total games played.
             */
            unsigned int gamesPlayed = 0;

            /**
             * @brief The percentage of games won as a [0, 100] int (not a [0, 1] float).
             */
            unsigned int winPercentage = 0;

            /**
             * @brief The current consecutive win streak.
             */
            unsigned int currentStreak = 0;

            /**
             * @brief The maximum consecutive win streak ever.
             */
            unsigned int maxStreak = 0;

            /**
             * @brief The total number of games won for each number of guesses.
             */
            std::array<unsigned int, GameConfig::NUM_GUESSES> guessDistribution = { 0, 0, 0, 0, 0, 0 };
        };

    public:
        /**
         * @brief Construct a statistics object.
         */
        WordleStatistics() = default;

        /**
         * @brief Load the old stats for recurring tracking during runtime.
         */
        void loadStats(Statistics statistics);

        /**
         * @brief Add the results of a wordle game to the current statistics.
         */
        void addGameStats(bool gameWon, unsigned int numGuesses);

        /**
         * @brief Get the current state of the tracked statistics.
         */
        Statistics getStats();

    private:
        /**
         * @brief Statistics tracking variable.
         */
        Statistics statistics;

};
