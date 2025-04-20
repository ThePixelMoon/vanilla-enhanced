# PvZ: Vanilla Enhanced
[![Build Status](https://github.com/ThePixelMoon/vanilla-enhanced/actions/workflows/build.yml/badge.svg)](https://github.com/ThePixelMoon/vanilla-enhanced/actions/workflows/build.yml) ![Repo size](https://img.shields.io/github/repo-size/ThePixelMoon/vanilla-enhanced) ![Last commit](https://img.shields.io/github/last-commit/ThePixelMoon/vanilla-enhanced)

An edit of PvZ:QOTL, which aims to add features useful for both players, and for coders trying to mod the game.

Precompiled builds are made using GitHub Actions.

## Building

### Requirements
- CMake
- Visual Studio 2022

### Windows
In order to build, run `config.cmd` and then go to `build/` directory and open the .sln file.

Inside Visual Studio, open the solution, and open the folder called `CMakePredefinedTargets`. Right click on `ALL_BUILD`, and click Build. Later, after you've builded everything, right click on `INSTALL` and select Build too. This will install the .exe file to the `out` directory.

### Linux
Currently there's no way to build this on Linux.

## Features
- CMake support
- Updated BASS, libpng, libjpeg, ogg and Vorbis

## Tasks
- 64 bit support
- DirectX 11 instead of DirectX 8
- Usage of SDL3 instead of the Windows API

# Credits:
PvZ:QOTL, for providing the base
