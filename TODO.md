# TODO

Open items from a code review in October 2026, grouped by area and roughly
ordered by importance within each group.

## Tablebase tools (`egtb`, `egtb-stats`)

- [ ] **Stale table files give wrong results.** `egtb` opens its output file
  without `O_TRUNC`, and `setPos()` only ever sets bits. So any 1-bits in an
  existing `egtb-<size>-<line>.bit` survive, and an oversized file is never
  shrunk. Only the `rm *.bit` in `egtb.sh` hides this. Truncate the file, or
  clear bits explicitly.
- [ ] **Division by zero in the statistics pass.** In `egtb.cpp`, the
  per-parity `score = (...)/total` crashes with SIGFPE when a bucket is empty,
  e.g. `egtb 2 2`, `4 4` and `5 5`. The table is written before the crash, so it
  is correct, but the exit status is 136. `egtb-stats.cpp` already guards
  against this with `if (total)`.
- [ ] **`egtb.sh` is fragile.** It deletes the tracked `.bit` files and then
  regenerates them without `set -e`, so a failed run leaves a table missing or
  empty with no error.
- [ ] Free the `known[]` array in `egtb.cpp`. This is harmless, since the
  program exits right after.
- [ ] Build the tools with `-DNDEBUG`, or keep the asserts deliberately.
- [ ] Update the header comment in `egtb.cpp`: it still says one byte per
  position and 32 MB, but the table is now one bit per position (4 MB for 5×5).

## MikeBot (`NTTTPlayerMike`)

- [ ] **Tune the v5/v6 evaluation tables for line size 2.** Since the line
  counts were fixed for line sizes other than 3 (`5b76364`), v5 and v6 see
  "this move kills the board" correctly at line size 2, but they still use
  `values`/`values_v6`, which were generated from 5×5 boards with line size 3.
  In 1-round tournaments after the fix, v5 dropped from 83% to 67% on 2 boards
  of 5×5, and v6 from 75% to 25% on 2 boards of 6×6. Generate tables for line
  size 2 (e.g. with `egtb-stats`) and select them by line size.
- [ ] **Evaluation tables for other sizes.** More generally, the same tables
  are used for every board size and line size, and are only valid for 5×5 with
  line size 3.
- [ ] **Alpha-beta cutoff only leaves the innermost loop.** In `findMove()`,
  the `break` on `alpha >= beta` only exits the `y` loop, so the search
  continues over the remaining rows and boards. Results stay correct, but most
  of the pruning is lost with several boards.
- [ ] **The `almostDead` bonus grows quadratically.** In `evaluate()`,
  `sum += almostDead*2000` is inside the per-board loop while `almostDead`
  keeps accumulating. With about 10 boards it can exceed the win score.
- [ ] **Inconsistent win/loss scores.** The code uses 99999, 99990 + level and
  ±99990 in different places, so all forced losses score the same and the bot
  does not prefer slower losses.
- [ ] Combining per-board scores by addition has no theoretical basis. Misère
  Notakto needs a misère-quotient style combination of boards.
- [ ] Keep the loaded tablebase across games, like the hash table, and only
  reload it when the board size or line size changes. It is currently re-read
  for every game, which is 4 MB for 5×5.
- [ ] `evaluate()` is `const` but casts `this` to change state. Make it
  non-const, or make the cache `mutable`.

## IceBot (`NTTTPlayerIce`)

- [ ] It re-opens and reads its tablebase file for every lookup, which can be
  about 25 times per board per move. Load it once.
- [ ] Without a usable tablebase it silently guesses at random. Warn, like
  MikeBot does.
- [ ] It only uses tablebases for board sizes 3–5, although `egtb-2-*.bit`
  exists.
- [ ] `chooseOrder()` decides from board 0 only and ignores the board count.

## GUI

- [ ] **Text fields are hard to edit.** The cursor starts at position 0 even
  when the field is prefilled, and clicking does not move it. Typing "5" into a
  field showing "4" gives "54". Start the cursor at the end, and move it on
  click.
- [ ] The length-limit check for text (not number) fields is inverted. It is
  latent, because the only text field has no limit.
- [ ] **"Write log" always fails.** It writes to `logs/<name>`, but nothing
  creates `logs/`. Log names are not sanitised either, so `../x` escapes the
  directory.
- [ ] **"Load Game" and "Statistics" are not implemented.** They show a blank
  screen. `GUI/Screens/StatisticsScreen.h` is empty, and `onReadLogClick` is
  commented out.
- [ ] Pressing Exit only quits on the next event (e.g. a mouse movement),
  because `quit` is set in `input()` for the `EXIT` state.
- [ ] **Data races between threads.** The game thread modifies `m_moves` and
  the game state while the render thread reads them, without locking. The
  shared flags are plain `bool`s.
- [ ] In manual mode, undoing a move after the game is over leaves the old
  winner and game-over state in place.
- [ ] After the last move, the game thread sleeps for 3 s without checking for
  quit, so closing the window then takes up to about 4 s.
- [ ] The renderer is created with `SDL_RENDERER_ACCELERATED` only, with no
  software fallback, so the GUI fails without GPU acceleration.
- [ ] When `IMG_Load` or text rendering fails, `Texture` leaves its width and
  height uninitialised, and the failure goes unnoticed.
- [ ] `NewGameScreen::input()` passes `event.key.keysym` for `SDL_TEXTINPUT`
  events, which reads an inactive union member.
- [ ] `TextField::isInside()` uses the X padding for the height, so the
  bottom few pixels cannot be clicked. Buttons accept clicks one pixel past
  their right and bottom edges, and right and middle clicks also count.
- [ ] `srand()` is called before every move, so the bots get the same random
  sequence on every move.
- [ ] The title "IceBot v2 vs MikeBot v6" is cut off at the right edge of the
  game screen.
- [ ] A long number in the board count field is drawn past the edge of the
  field.

## Game core and tournament mode

- [ ] Move legality is only checked with `assert`, so a `-DNDEBUG` build
  accepts illegal bot moves.
- [ ] Uninitialised members before `init()`/`chooseOrder()`/`reset()`/
  `NewGame()` in `Board`, `NTTTPlayerIce`, `NTTTBoard` and `NTTTGame`.
  `NTTTGame::getBoardSize()` indexes `m_boards[0]` without checking.
- [ ] `Texture`, `Text`, `Button` and `TextField` own raw pointers but can be
  copied, which would free the same memory twice. Delete their copy
  operations, or use smart pointers.
- [ ] The tournament ignores the bots' `chooseOrder()` result.
- [ ] `main.cpp` never deletes the players. `dispResults()` takes the players
  vector by value.
- [ ] First/second-player percentages are truncated rather than rounded, so
  they do not add up to 100%.

## Logging (`trace.h`)

- [ ] It appends to an existing log instead of truncating it, so a second run
  doubles the log.
- [ ] It opens and closes the file every 199 bytes, which is slow for large
  logs.
- [ ] If `open` fails, output is silently dropped.
- [ ] `LOG`/`TRACE` are bare `if` statements, so a following `else` can bind
  to them. With `ENABLE_TRACE` the build fails to link, because `gpTrace` is
  never defined.

## Build, scripts and repository

- [ ] Object files do not depend on the `Makefile`, so changing `DEFINES` does
  not rebuild them.
- [ ] `make install` only installs `nottt`, not the tools.
- [ ] `stats.sh` runs `nottt` from `PATH` instead of `./nottt`, prints an
  error from `rm log-*.txt` when there are no logs, and assumes exactly 7
  players (`tail -n 11`).
- [ ] The program must be run from the repository root, because assets and
  tablebases are loaded from the current directory.
