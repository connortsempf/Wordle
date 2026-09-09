# Wordle #
**A word-guessing game desktop application as a clone for the popular New York Times game written with Qt6 and C++20.**


---
<br>


## 🧰 Build and Install ##
Build from source and install to **Windows**, **macOS**, and **Linux** using `CMake`.
- Follow the build and install instructions for **Windows** [here](#-build-and-install-for-windows).
- Follow the build and install instructions for **macOS** [here](#-build-and-install-for-macos).
- Follow the build and install instructions for **Linux** [here](#-build-and-install-for-linux).


---
<br>


## 🚀 Features ##
- **🎨 Clean, Intuitive GUI:** Minimalist design focused on gameplay with responsive buttons, clear feedback, and smooth animations.
- **⏱️ Real-Time Game Feedback:** Instant visual feedback on letter matches, including color-coded tiles (green for correct position, yellow for wrong position, gray for not in word).
- **🧩 Sustained Player Statistics and Settings:** Save your game progress, win streak, and personal statistics. Customize difficulty levels and game preferences that persist between sessions.
- **🖼️ Responsive Window Layouts:** Adapts seamlessly to different window sizes and screen resolutions without breaking the user interface.
- **🛡️ Full Application Support:** Proper native OS integration for a polished desktop experience.
- **🖥️ Cross-Platform Support (Windows, macOS, Linux):** Built with Qt6 to run natively on all major operating systems with consistent behavior.
- **📚 Fully Documented:** Well-commented source code, inline documentation, and developer guides for anyone wanting to contribute or modify the project.


---
<br>


## 🔥 Screenshots ##
<p align="left">
  <img src="media/intro-screen.png" width="450">
  <img src="media/gameplay.png" width="450">
  <img src="media/game-directions.png" width="450">
  <img src="media/settings-menu.png" width="450">
</p>


---
<br>


## 🛠️🪟 Build and Install for Windows ##
### 🔹 Build from Source ###
#### 🔸 Install Build Requirements ####
To build **Wordle** from source for Windows, install the following as they are all required.
- **Qt 6.10** or higher development libraries. Install Qt from [qt.io](https://www.qt.io/development/download)
- **C++ 20+** capabilities. Qt6 will come with the necessary compiler to build the project.
- **CMake 3.16+** for building the project. Install CMake from [cmake.org](https://cmake.org/download)
- **Git** to clone the Wordle repository. Install Git from [git-scm.com](https://git-scm.com/install/windows)
#### 🔸 Clone the Repository ####
```bash
cd /path/to/your/desired/directory
git clone https://github.com/connortsempf/wordle.git
cd wordle
```
#### 🔸 Create the Build Directory ####
```bash
mkdir build
cd build
```
#### 🔸 Build the Project Using CMake ####
```bash
## Configure the Build ##
cmake ..

## Only If The Qt Package isn't Visisble in the Path and Configuration Fails, Point to It Manually ##
cmake .. -DQT6_DIR="/path/to/your/Qt/package/6.10.x/compiler/"

## Generate the Application ##
cmake --build .
```
### 🔹 Install Directly (Installation Option 1.) ###
#### 🔸 Install Directly using CMake ####
```bash
cmake --install .
```
### 🔹 Package for Installation (Installation Option 2.) ###
#### 🔸 Install Packaging Requirements ####
To package Wordle for traditional windows application installation, install the following as they are required.
- **WiX** packaging system. Install WiX from [github.com/wixtoolset](https://github.com/wixtoolset/wix/releases)
- **NSIS** packaging system. Install NSIS from [nsis.sourceforge.io](https://nsis.sourceforge.io/Download)
#### 🔸 Navigate to the Build Directory ####
```bash
cd /path/to/your/desired/directory/build
```
#### 🔸 Package the Application ####
```bash
cpack
```
#### 🔸 Navigate to the Packages Directory ####
```bash
cd ../packages
```
#### 🔸 Run the Desired Installer ####
Both installers are valid, choose your preferred installer and run it as normal to install Wordle to your machine.


---
<br>


## 🛠️🍎 Build and Install for macOS ##
### 🔹 Build from Source ###
#### 🔸 Install Build Requirements ####
To build **Wordle** from source for macOS, install the following as they are all required.
- **Qt 6.10** or higher development libraries. Install Qt with Homebrew from the command line with...
```bash
brew install qt
```
- **C++ 20+** capabilities. Qt6 will come with the necessary compiler to build the project.
- **CMake 3.16+** for building the project. Install CMake with Homebrew from the command line with...
```bash
brew install cmake
```
- **Git** to clone the Wordle repository. Install Git with Homebrew from the command line with...
```bash
brew install git
```
#### 🔸 Clone the Repository ####
```bash
cd /path/to/your/desired/directory
git clone https://github.com/connortsempf/wordle.git
cd wordle
```
#### 🔸 Create the Build Directory ####
```bash
mkdir build
cd build
```
#### 🔸 Build the Project Using CMake ####
```bash
## Configure the Build ##
cmake ..

## Generate the Application ##
cmake --build .
```
### 🔹 Install Directly (Installation Option 1.) ###
#### 🔸 Install Directly with the Generated .app File ####
Simply drag and drop the `Wordle.app` file generated within the `/bin` directory to your applications
### 🔹 Package for Installation (Installation Option 2.) ###
#### 🔸 Navigate to the Build Directory ####
```bash
cd /path/to/your/desired/directory/build
```
#### 🔸 Package the Application ####
```bash
cpack
```
#### 🔸 Navigate to the Packages Directory ####
```bash
cd ../packages
```
#### 🔸 Run the Installer ####
Run the `.dmg` installer as normal to install Wordle to your machine.


---
<br>


## 🛠️🐧 Build and Install for Linux ##
### 🔹 Install Project Requirements ###
To build **Wordle** from source for Linux, install the following as they are all required.
- **Qt 6.10** or higher development libraries.
- **C++ 20+** capabilities. Qt6 will come with the necessary compiler to build the project.
- **CMake 3.16+** for building the project.
- **Git** to clone the Wordle repository.
These can all be installed from the command line with the following command
```bash
## Ubuntu / Debian ##
sudo apt install qt6-base-dev qt6-tools-dev cmake git

## Fedora ##
sudo dnf install qt6-base-dev qt6-tools-dev cmake git

## Arch ##
sudo pacman install qt6-base-dev qt6-tools-dev cmake git

## Change your Package Manager Command as Needed ##
```
### 🔸 Clone the Repository ####
```bash
cd /path/to/your/desired/directory
git clone https://github.com/connortsempf/wordle.git
cd wordle
```
#### 🔸 Create the Build Directory ####
```bash
mkdir build
cd build
```
#### 🔸 Build the Project Using CMake ####
```bash
## Configure the Build ##
cmake ..

## Generate the Application ##
cmake --build .
```
### 🔹 Install Directly (Installation Option 1.) ###
#### 🔸 Install Directly using CMake ####
```bash
cmake --install .
```
### 🔹 Package for Installation (Installation Option 2.) ###
#### 🔸 Install Packaging Requirements ####
To package Wordle for traditional Linux application installation, install the following as it is required.
```bash
## Change your Package Manager Command as Needed ##
sudo apt install rpm-build
```
#### 🔸 Navigate to the Build Directory ####
```bash
cd /path/to/your/desired/directory/build
```
#### 🔸 Package the Application ####
```bash
sudo cpack
```
#### 🔸 Navigate to the Packages Directory ####
```bash
cd ../packages
```
#### 🔸 Execute the Desired Installer ####
```bash
## Change your Package Manager Command as Needed ##
sudo apt install ./<package-name.deb>
sudo apt install ./<package-name.rpm>
```


---
<br>


## 📄 License & Attribution ##
This is an unofficial fan recreation of Wordle. The original game concept is owned by the New York Times Company. This project is licensed under the MIT License - see the LICENSE file for details.

- Built with [Qt](https://www.qt.io/)
- Inspired by [Wordle](https://www.nytimes.com/games/wordle/index.html)
