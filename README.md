# TWo - The Journey

T and W are best friends. W is a woman. One day W disappears. T wakes up alone in a strange forest with no memory of how he got there.

## Story

Find W and uncover the mystery behind her disappearance. Travel through different episodes, solve puzzles, defeat bosses of episode, and save your friend.


- 4 Episodes with unique maps
- Collect and use items
- each episode have mini game
- Timer-based challenges
- Auto-save progress with SQLite

## Demo
![TWo Gameplay](/demo.gif)

## Controls

| Key | Action |
|-----|--------|
| Arrow Keys / hjkl | Move character |
| e | Open/close inventory |
| 1 - 5 | Use items from inventory |
| Q | Quit game |

## Characters

| Symbol | Character |
|--------|-----------|
| T | Player (You) |
| W | Missing woman (best friend) |
| P | Boss Episode 1 |
| D | Boss Episode 2 |
| S | Boss Episode 3 |
| U | Final Boss |

## Requirements

**Debian/Ubuntu:**
$ sudo apt install libncurses-dev libsqlite3-dev

**Fedora:**
$ sudo dnf install ncurses-devel sqlite-devel

**Arch Linux:**
$ sudo pacman -S ncurses sqlite

**Void Linux:**
$ sudo xbps-install ncurses-devel sqlite-devel

**Alpine Linux:**
$ sudo apk add ncurses-dev sqlite-dev

## Build & Run

$ make
$ ./TWo
