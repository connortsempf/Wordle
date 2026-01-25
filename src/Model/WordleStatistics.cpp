/**
 * @file WordleStatistics.cpp
 * @brief Wordle Statistics Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the WordleStatistics class, which
 * manages the wordle game statistics over indefinite play-throughs.
 */


#include "Model/WordleStatistics.hpp"


/**
 * @brief Load the old stats for recurring tracking during runtime.
 */
void WordleStatistics::loadStats(WordleStatistics::Statistics statistics) {
    this->statistics = statistics;
}


/**
 * @brief Add the results of a wordle game to the current stats.
 */
void WordleStatistics::addGameStats(bool gameWon, unsigned int numGuesses) {
    unsigned int oldGamesPlayed = statistics.gamesPlayed;
    unsigned int oldGamesWon = 0;
    for (const auto& distributedWins : statistics.guessDistribution) oldGamesWon += distributedWins;

    ++statistics.gamesPlayed;
    if (gameWon) {
        statistics.winPercentage = static_cast<unsigned int>((static_cast<float>(++oldGamesWon) / statistics.gamesPlayed) * 100);
        ++statistics.currentStreak;
        statistics.maxStreak = statistics.currentStreak > statistics.maxStreak ? statistics.currentStreak : statistics.maxStreak;
        if (numGuesses > 0 && numGuesses < 7) ++(statistics.guessDistribution[numGuesses - 1]);
    }   else if (!gameWon) {
        statistics.winPercentage = static_cast<unsigned int>((static_cast<float>(oldGamesWon) / statistics.gamesPlayed) * 100);
        statistics.currentStreak = 0;
    }
}


/**
 * @brief Get the current state of the tracked stats.
 */
WordleStatistics::Statistics WordleStatistics::getStats() {
    return statistics;
}
