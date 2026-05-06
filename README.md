# Snake Game

A classic Snake game running in the terminal, written in C using the ncurses library.

## Features

- Smooth terminal rendering with ncurses
- Score tracking with **highscore saved to a file** — persists between sessions
- **Two levels** — at score 50 the food starts moving randomly
- Increasing speed as you eat food
- Wall and self-collision detection
- Head rendered as `O`, body as `o`

## Getting Started

### Requirements

- GCC compiler
- ncurses library

On Debian/Ubuntu:
```bash
sudo apt install libncurses5-dev
```

### Build & Run

```bash
git clone https://github.com/jakubjalcak1/snake-game.git
cd snake-game
gcc -o snake program.c -lncurses
./snake
```

## Controls

| Key | Action |
|-----|--------|
| `↑` | Move up |
| `↓` | Move down |
| `←` | Move left |
| `→` | Move right |
| `Q` | Quit |

## Scoring

| Event | Points |
|-------|--------|
| Eat food | +10 |
| Reach 50 points | Level 2 — food starts moving |

## Built With

- **C** — core game logic
- **ncurses** — terminal rendering

## Author

**Jakub Jalčak** — [jakubjalcak1.github.io](https://jakubjalcak1.github.io/portfolio)
