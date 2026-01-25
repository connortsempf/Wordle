/**
 * @file main.cpp
 * @brief Wordle Application Program Entry Point.
 * @author Connor Sempf
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the main entry point for the application.
 * It creates the application object and runs it indefinitely.
 */


#include <QApplication>
#include "ViewModel/WordleApp.hpp"


/**
 * @brief The main function to start up and run the application.
 *
 * @return The exit status code as an int.
 */
int main(int argc, char *argv[]) {
    QApplication wordle(argc, argv);
    WordleApp wordleApp;
    return wordle.exec();
}
