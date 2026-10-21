# TODO

Open items from a code review in October 2026, grouped by area and roughly
ordered by importance within each group.

Items are numbered within each section, so they can be referred to as, for
example, "GUI 3". The numbers are permanent: when an item is removed, the
others keep their numbers, and new items get the next unused number in their
section.

## MikeBot (`NTTTPlayerMike`)

- [ ] 1. **Tune the v5/v6 evaluation tables for line size 2.** Since the line
  counts were fixed for line sizes other than 3 (`5b76364`), v5 and v6 see
  "this move kills the board" correctly at line size 2, but they still use
  `values`/`values_v6`, which were generated from 5×5 boards with line size 3.
  In 1-round tournaments after the fix, v5 dropped from 83% to 67% on 2 boards
  of 5×5, and v6 from 75% to 25% on 2 boards of 6×6. Generate tables for line
  size 2 (e.g. with `egtb-stats`) and select them by line size.
- [ ] 2. **Evaluation tables for other sizes.** More generally, the same tables
  are used for every board size and line size, and are only valid for 5×5 with
  line size 3.
- [ ] 7. Keep the loaded tablebase across games, like the hash table, and only
  reload it when the board size or line size changes. It is currently re-read
  for every game, which is 4 MB for 5×5.
- [ ] 8. `evaluate()` is `const` but casts `this` to change state. Make it
  non-const, or make the cache `mutable`.

## IceBot (`NTTTPlayerIce`)

- [ ] 2. Without a usable tablebase it silently guesses at random. Warn, like
  MikeBot does.
- [ ] 4. `chooseOrder()` decides from board 0 only and ignores the board count.

## GUI

- [ ] 3. **"Write log" always fails.** It writes to `logs/<name>`, but nothing
  creates `logs/`. Log names are not sanitised either, so `../x` escapes the
  directory.
- [ ] 4. **"Load Game" and "Statistics" are not implemented.** They show a blank
  screen. `GUI/Screens/StatisticsScreen.h` is empty, and `onReadLogClick` is
  commented out.
- [ ] 6. **Data races between threads.** The game thread modifies `m_moves` and
  the game state while the render thread reads them, without locking. The
  shared flags are plain `bool`s.
- [ ] 7. In manual mode, undoing a move after the game is over leaves the old
  winner and game-over state in place.
- [ ] 9. The renderer is created with `SDL_RENDERER_ACCELERATED` only, with no
  software fallback, so the GUI fails without GPU acceleration.
- [ ] 10. When `IMG_Load` or text rendering fails, `Texture` leaves its width and
  height uninitialised, and the failure goes unnoticed.
- [ ] 11. `NewGameScreen::input()` passes `event.key.keysym` for `SDL_TEXTINPUT`
  events, which reads an inactive union member.
- [ ] 12. `TextField::isInside()` uses the X padding for the height, so the
  bottom few pixels cannot be clicked. Buttons accept clicks one pixel past
  their right and bottom edges, and right and middle clicks also count.
- [ ] 13. `srand()` is called before every move, so the bots get the same random
  sequence on every move.

## Build, scripts and repository

- [ ] 1. `make install` only installs `nottt`, not the tools.
- [ ] 2. `stats.sh` runs `nottt` from `PATH` instead of `./nottt`, prints an
  error from `rm log-*.txt` when there are no logs, and assumes exactly 7
  players (`tail -n 11`).
- [ ] 3. The program must be run from the repository root, because assets and
  tablebases are loaded from the current directory.
