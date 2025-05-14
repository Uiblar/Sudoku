# Sudoku C Project

This is a command-line Sudoku game implemented in C.

## Features

- [x] Generate Sudoku puzzles with varying difficulty levels (Easy, Medium, Hard, Brutal).
- [x] Interactive gameplay via command-line input (e.g., "A5:9").
- [x] Board validation to check for errors.
- [x] Hint system (Naked Singles, Hidden Singles, Locked Candidates).
- [x] Automatic puzzle solving.
- [x] Save and load game progress to/from `savegame.txt`.
- [x] Solution submission and validation.
- [x] Basic command-line interface for interaction.
- [x] **Input Validation:** Enhance input validation to handle more edge cases and provide clearer error messages (e.g., invalid coordinates, non-numeric input).

## TODO

- [ ] **More Hint Techniques:** Implement additional Sudoku solving techniques for hints (e.g., Naked/Hidden Pairs/Triples/Quads, X-Wing, Swordfish).
- [ ] **Difficulty Algorithm:** Refine the puzzle generation difficulty algorithm. Currently, it might rely heavily on the number of clues removed; consider incorporating the complexity of solving techniques required.
- [ ] **User Interface:** Add terminal compatibility checks for existing ANSI color codes used for UI presentation.
- [ ] **Error Handling:** Add more robust error handling, especially around file I/O (saving/loading). Check return values of `fopen`, `fprintf`, `fscanf`, etc.
- [ ] **Code Structure:** Refactor parts of the code for better modularity or clarity. For example, the input parsing logic in `sudoku_cli.c` could potentially be separated.
- [ ] **Memory Management:** Review memory management (if dynamic allocation is used, though it seems less likely in this structure) for potential leaks or errors.
- [ ] **Manual Testing:** Create a checklist of core functionalities to test manually (e.g., puzzle generation for each difficulty, move input variations, board validation, each hint type, save/load functionality, automatic solving, solution submission).
- [ ] **Configuration File:** Instead of hardcoding `savegame.txt`, allow specifying the save file via command-line arguments or a configuration file.
- [ ] **Cross-Platform Compatibility:** Ensure the code compiles and runs on both Windows and Linux. Verify that game saves (`savegame.txt`) created on one OS can be successfully loaded on the other.
- [ ] **Undo/Redo Functionality:** Implement a mechanism to undo the last move(s).
- [x] **Terminal Display Enhancement**: Screen Refresh

## Save and Load Functionality:

The current implementation saves and loads the game state to a file named `savegame.txt`.

**Critique of Current Implementation:**

- **Fixed Location:** Saves `savegame.txt` directly in the current working directory, which is non-standard and inconvenient.
- **Fixed Filename:** Only supports a single save slot, overwriting previous saves.
- **Limited Data Saved:** Stores only the `workingBoard` (current player progress), not the `initialBoard` (original puzzle) or difficulty level.
- **Fragile Format:** Uses simple space-separated integers, making it difficult to extend for saving additional data.
- **Incomplete Error Handling:** Lacks checks for potential errors during file writing (`fprintf`) and reading (`fscanf`), which could lead to data corruption.

**Proposed Enhancements:**

- **Standard Save Locations:** Utilize platform-specific user directories:
  - Windows: `%USERPROFILE%\Saved Games\Sudoku\` or `%APPDATA%\Sudoku\Saves\`
  - Linux: `$HOME/.local/share/Sudoku/saves/` (preferred) or `$HOME/.config/Sudoku/saves/`
  - Requires OS detection and path construction logic, including directory creation.
- **Multiple Save Slots/Named Saves:** Allow users to manage multiple save files (e.g., `save slot1`, `load my_puzzle.sudoku`).
- **Structured Save Format:** Adopt a more robust format:
  - _Option 1 (Text Key-Value):_ Human-readable, extensible (e.g., `Difficulty: Medium\nInitialBoard: 0 0 3 ...\nWorkingBoard: 5 1 3 ...`).
  - _Option 2 (Binary):_ More compact but requires careful handling of data types and potential cross-platform issues (endianness).
- **Comprehensive Data Storage:** Save essential game state:
  - `initialBoard`
  - `workingBoard`
  - `difficulty` level
  - Optionally: timer, hints used, etc.
- **Robust Error Handling:** Implement thorough checks for all file I/O operations (`fopen`, `fprintf`/`fwrite`, `fscanf`/`fread`, `fclose`) and provide clear user feedback on failures.

## Terminal Display Enhancement: Screen Refresh

Currently, after each command (like making a move), the updated game board is printed to the console below the previous output. This leads to a long scrollback history during gameplay.

**Proposed Enhancement:**

Implement a screen refresh mechanism where, before displaying the updated board state, the terminal screen is cleared. This creates the effect of the board updating in place, providing a cleaner user experience.

**Implementation Considerations:**

- **Clearing Method:**
  - _Platform-Specific Commands:_ Use `system("cls")` on Windows or `system("clear")` on Linux/macOS. This is simpler but less portable and can have security implications if not used carefully.
  - _ANSI Escape Codes:_ Use codes like `\033[2J` (clear screen) and `\033[H` (move cursor to top-left). This is more portable but requires ensuring the user's terminal supports ANSI codes (linking back to the UI TODO item).
- **Hint Persistence:** A challenge with screen clearing is ensuring that messages like hints remain visible. When a hint is requested, it's displayed. If the screen is cleared immediately upon the _next_ user input (e.g., a move), the hint disappears instantly. To address this:
  - The screen clear should only happen _before_ redrawing the board due to actions like making a move, starting a new game, or loading a game.
  - The `hint` command itself should _not_ trigger a screen clear. It should print the hint below the current board display.
  - Alternatively, the hint message could be stored temporarily and redisplayed below the board after each refresh, until a valid move is made.

## Potential Refinement: Technique-Based Difficulty Rating

The current puzzle generation determines difficulty based primarily on the number of clues remaining after removing cells from a solved grid. While ensuring a unique solution, this metric may not accurately reflect the logical complexity required to solve the puzzle.

A more sophisticated approach involves analyzing the specific solving techniques required for a given puzzle:

1.  **Technique Implementation:** Expand the solver's capabilities to include a wider range of standard Sudoku solving techniques (e.g., Naked/Hidden Subsets, Pointing Pairs/Triples, Intersection Removal, Fish patterns like X-Wing/Swordfish).
2.  **Technique Hierarchy:** Assign a relative difficulty score or level to each implemented solving technique.
3.  **Solve Path Analysis:** For a candidate puzzle, simulate the solving process step-by-step, applying techniques in order of increasing difficulty. Record the most complex technique (highest score/level) required at any point to fully solve the puzzle.
4.  **Difficulty Assignment:** Classify the puzzle's difficulty (Easy, Medium, Hard, etc.) based on the highest-scoring technique identified during the analysis.
5.  **Generation Integration:** Modify the puzzle generation process. Instead of targeting a specific clue count, generate puzzles by removing clues while ensuring unique solvability and periodically analyzing the required techniques. Stop removing clues when the puzzle's technique-based rating reaches the desired difficulty threshold.

## Game Startup Flow: New vs. Load

**Current Flow:**

Upon launching the application:

1.  The user is immediately prompted to select a difficulty level (Easy, Medium, Hard, Brutal).
2.  A new puzzle corresponding to the selected difficulty is generated and displayed.
3.  Only after this initial puzzle is presented does the main command prompt appear, offering options like `save(s)` and `load(l)` alongside gameplay commands.

This flow forces the user to start a new game initially, and loading a saved game requires using the `load` command _after_ this initial setup, potentially overwriting the just-generated puzzle if not saved first.

**Proposed Enhancement:**

A more intuitive startup sequence would be:

1.  **Check for Save File:** Upon starting the application, check if a default save file (e.g., `savegame.txt` or a platform-specific equivalent) exists.
2.  **Prompt User (if save exists):** If a save file is found, prompt the user: "Load existing game from [savefile]? (Y/n)".
    - If 'Y' (or Enter), load the game and proceed.
    - If 'N', proceed to the new game setup.
3.  **New Game Setup (if no save or user chose 'N'):** Prompt the user to start a new game, by asking for the desired difficulty level (e.g., "Enter difficulty (Easy, Medium, Hard, Brutal)?").
4.  **Explicit Commands:** Retain the explicit `load [filename]` and `save [filename]` commands for managing potentially multiple save files.
5.  **Implement a `new`:** command to allow users to start a new game at any time, even if they are currently in a game. This would be useful if they want to switch puzzles without quitting the application.
6.  **Prevent Accidental Overwrites:** If the user attempts to start a new game while a game is in progress, prompt them to save their current game first (e.g., "You have an unsaved game. Save it before starting a new one? (Y/n)").
