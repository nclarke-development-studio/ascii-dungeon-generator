# Procedural Dungeon & Organic Map Generator

A simple **procedural, room-based dungeon generator** with support for **organic (cellular-style) maps**.  
Includes region connection via graph algorithms and a demo application.

Inspired by and based on:
- [Procedural Dungeon Generation Algorithm (Gamasutra)](https://www.gamasutra.com/blogs/AAdonaac/20150903/252889/Procedural_Dungeon_Generation_Algorithm.php)
- [Reddit explanation](https://www.reddit.com/r/gamedev/comments/1dlwc4/procedural_dungeon_generation_algorithm_explained/)
- [YouTube series](https://www.youtube.com/watch?v=eJEpeUH1EMg)

---

## Features

- Room-based dungeon generation  
- Organic / cellular-automata map generation  
- Region connectivity using graph algorithms (MST)  
- Configurable generation parameters  
- Demo executable included  

---

## To-Do

- Package as a reusable library

---

## Project Structure

```
.
├── CMakeLists.txt
├── data
├── demo
│   ├── CMakeLists.txt
│   ├── DungeonGeneratorDemo.exe
│   ├── main.cpp
│   └── samples
├── include
│   ├── core
│   ├── map
│   ├── physics
│   └── utils
├── lib
│   └── libAsciiMapGenerator.a
├── README.md
└── src
```

---

## Compilation

### Using CMake (MSYS2 MinGW-w64)

```bash
cmake . -G "MSYS Makefiles" -Dbuild_demo=TRUE
make
```

---

## Usage

```bash
main.exe --help
```

### Command-line Flags

| Flag | Description |
|-----|-------------|
| `-d`, `--dimensions <w> <h>` | **Required**. Map size |
| `-f`, `--fillpercentage <int>` | Floor fill percentage |
| `-n`, `--organic` | Organic generation mode |
| `-c`, `--connect` | Fully connect regions |
| `-nr`, `--norandom` | Disable random seed |
| `-s`, `--smoothing <int>` | Smoothing iterations |
| `-dn`, `--dense` | Dense non-organic map |
