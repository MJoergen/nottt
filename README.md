# nottt — No Tic Tac Toe

A C++ implementation of *No Tic Tac Toe* (also known as Notakto): a
misère variant of tic-tac-toe played on one or more boards, where both
players place the same mark (X). A board "dies" as soon as it contains a
complete line, and the player who kills the **last** live board loses.

The project contains:

* An SDL2 GUI for playing against the computer or another human.
* Several computer players ("bots"), including an alpha-beta searcher backed
  by endgame tablebases.
* A command-line tournament mode for pitting the bots against each other.
* Tools for generating endgame tablebases by retrograde analysis.

## Building

Dependencies: a C++11 compiler, SDL2, SDL2_image and SDL2_ttf.
On Debian/Ubuntu:

```sh
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

Then:

```sh
make
```

Note that the `Makefile` currently moves the resulting `nottt` binary to
`/home/mike/bin`; edit or remove that line to suit your setup. Setting
`TARGET = windows` cross-compiles with MinGW (`i686-w64-mingw32-g++`).

The tablebase tools are not part of the `Makefile` and are built directly:

```sh
g++ -O3 -std=c++11 -o egtb egtb.cpp
g++ -O3 -std=c++11 -o egtb-stats egtb-stats.cpp
```

## Running

Run the program from the repository root, since it loads its assets
(`*.png`, `Junicode-Regular.ttf`) and tablebases (`egtb-*.bit`) from the
current directory.

### GUI

```sh
nottt
```

### Bot tournament

```sh
nottt <boardCount> <boardSize> <lineSize> <numGames> [logFile]
```

Plays a round robin between all bots (Mike v1–v6 and Ice) on
`boardCount` boards of size `boardSize`×`boardSize`, where `lineSize` marks
in a row kill a board. Results are printed after each round; if `logFile`
is given, every game is traced to it. `stats.sh` runs a series of such
tournaments.

## Bots

| Bot | Source | Description |
|-----|--------|-------------|
| Mike v1–v6 | `NTTTPlayerMike.cpp` | Bitboard-based alpha-beta search with a transposition table. The version number selects the evaluation function; v6 is the newest. When only one board is alive it plays perfectly from the tablebase. |
| Ice | `NTTTPlayerIce.cpp` | Heuristic player. |

New bots derive from the abstract `NTTTPlayer` class (`NTTTPlayer.h`) and
implement `chooseOrder`, `performMove` and `getName`.

## Endgame tablebases

`egtb <boardSize> <lineSize>` solves every position on a single board by
retrograde analysis and writes `egtb-<boardSize>-<lineSize>.bit`, one bit per
position (bit set = the side to move wins). Boards up to 5×5 are supported.
`egtb.sh` regenerates all tablebases from 2×2 to 5×5, and `egtb.log` holds
the resulting per-move-count statistics. `egtb-stats` reads a generated
tablebase and prints further statistics.

Some results (see the comment at the top of `egtb.cpp`):

* The empty 4×4 board (line length 3) is a win for the first player, but only
  when starting in a corner.
* The empty 5×5 board (line length 3) is a loss for the first player, no matter
  where they play.

## Repository layout

```
main.cpp              Entry point: GUI or tournament mode
NTTTGame.h            Game state: a set of boards
NTTTBoard.{h,cpp}     A single board and its alive/dead state
NTTTMove.h            A move (board, x, y)
NTTTPlayer.h          Abstract bot interface
NTTTPlayerMike.*      Alpha-beta bot
NTTTPlayerIce.*       Heuristic bot
NTTTManager.*         SDL2 setup and main loop
GUI/                  Widgets and screens
egtb.cpp, egtb.sh     Tablebase generator
egtb-stats.cpp        Tablebase statistics
trace.h               Logging helper
```

## License

The bundled Junicode font is distributed under the SIL Open Font License
(see `SIL Open Font License.txt`).
