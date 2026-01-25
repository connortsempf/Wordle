/**
 * @file GameConfig.hpp
 * @brief Game Config Constants for Wordle.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains compile-time constants that define the core
 * rules and constraints of the Wordle game.
 */


#pragma once


/**
 * @brief Core game configuration constants.
 */
namespace GameConfig {

    /**
     * @brief Number of letters in each word.
     */
    constexpr unsigned int WORD_LENGTH = 5;

    /**
     * @brief Maximum number of guess attempts allowed per game.
     */
    constexpr unsigned int NUM_GUESSES = 6;
}
