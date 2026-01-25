/**
 * @file LetterState.hpp
 * @brief Letter State Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the Letter and Tile validity states, which
 * represents a letter or tile's validity in the Wordle UI.
 */


#pragma once


/**
 * @brief Represents the validity state of a letter or tile in the Wordle UI.
 */
enum class LetterState {
    ABSENT,
    PRESENT,
    CORRECT
};
