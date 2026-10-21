# CLAUDE.md

Notes for working on this repository. See README.md for what the project is,
and TODO.md for known issues. When an item in TODO.md is fixed, remove it.

## Workflow

- Show the change and how it was tested, then wait to be asked before
  committing. One logical change per commit, with a message that explains the
  cause.
- Use LF line endings. Edit with the Edit tool rather than Python text-mode
  rewrites, which silently change line endings.
- Indent C++ with 4 spaces, never tabs. The `Makefile` must keep its tabs.

## Building and running

- `make` only lists the targets; `make all` builds `nottt`, `egtb` and
  `egtb-stats`.
- Run everything from the repository root: `assets/` and `tables/` are found
  relative to the current directory.
- Never run `./nottt` without arguments outside Xvfb: it opens the GUI on the
  user's desktop. Use the tournament mode
  (`./nottt <boardCount> <boardSize> <lineSize> <numGames>`) for bot tests.
- Run `egtb` in a scratch directory. It writes `egtb-<size>-<line>.bit` to the
  current directory, and the tracked tables in `tables/` must not change.
  `tools/egtb.sh` deletes and regenerates the tracked tables.

## Testing

There are no automated tests. Instead:

- **Bot changes:** save tournament output before the change and `cmp` it
  afterwards. Output is deterministic (fixed seeds), so anything that should
  not change behaviour must give byte-identical results. Cover several board
  sizes (2–6), line sizes and board counts. Results for line size 3 are the
  reference; the v5/v6 evaluation tables were tuned for 5×5 with line size 3.
- **Tablebase changes:** regenerated `.bit` files must be identical to the
  tracked ones.
- **GUI changes:** drive the app on a virtual display and take screenshots:
  - Start `Xvfb :99 -screen 0 1024x768x24` detached, then run
    `DISPLAY=:99 SDL_RENDER_DRIVER=software ./nottt < /dev/null`.
  - Move the window to 0,0 with `xdotool windowmove`, click with
    `xdotool mousemove X Y click 1`, and capture it with
    `import -window root -crop 900x600+0+0 shot.png`.
  - Turn on Manual Mode to step through a game with the → key.
  - Text fields keep the cursor where it was, so clear one with →×15 and
    then Backspace×15 before typing.
  - Stop the app with `pkill -x nottt`, never `pkill -f nottt`: the repository
    path contains "nottt", so `-f` also kills your own shell. Make sure no old
    instance is still running before starting a new one.
