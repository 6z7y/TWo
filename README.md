# TWo - The Journey

**A Terminal UI (TUI) Adventure Game / Visual Novel**

T and W are best friends. W is a woman. One day W disappears. T wakes up alone in a strange forest with no memory of how he got there.

## Demo
![TWo Gameplay](/demo.gif)

## Story

Find W and uncover the mystery behind her disappearance. Travel through different episodes, solve puzzles, defeat bosses of episode, and save your friend.

- 4 Episodes with unique maps
- Visual Novel style dialogue with character portraits
- Collect and use items
- each episode have mini game
- Timer-based challenges
- Auto-save progress with SQLite


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
| P | Boss Episode 1 Pixel |
| G | Boss Episode 2 Gfed|
| S | Boss Episode 3 Shallot|
| U | Boss Episode 4 Ultim |

## Requirements

**Debian/Ubuntu:**
$ `sudo apt install libncurses-dev libsqlite3-dev`

**Fedora:**
$ `sudo dnf install ncurses-devel sqlite-devel`

**Arch Linux:**
$ `sudo pacman -S ncurses sqlite`

**Void Linux:**
$ `sudo xbps-install ncurses-devel sqlite-devel`

**Alpine Linux:**
$ `sudo apk add ncurses-dev sqlite-dev`

## Build & Run

$ `make`<br>
$ `./TWo`

## Note

This Project is my first game, made for practice. My father told me to make a game with a story. So here it is.

Thank you for playing.
