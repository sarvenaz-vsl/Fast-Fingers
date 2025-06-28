# Fast Fingers

A C++ Typing Game for Practice and Speed!

## Overview

**Fast Fingers** is a command-line typing game designed to help users improve their typing speed and accuracy under time pressure. Players are challenged to type as many words correctly as possible within a limited time. After the first round, the program provides targeted practice on the letters that were most frequently mistyped.

---

## Features

- **Timed Typing Game:** Type randomly selected words within a set duration.
- **Difficulty Modes:** Choose between Normal (any word) or Difficult (longer words).
- **Accuracy Feedback:** See which letters you mistype the most and practice them.
- **Leaderboard:** Track the average performance of all players.
- **Focused Practice Round:** Optional second round focusing on your most-mistyped letters.
- **Result Logging:** Game statistics and results are saved for review.

## How to Run

1. **Run the game executable:**

2. **Follow the on-screen menu:**

   - **1:** Start Game
   - **2:** Show Leaderboard/Statistics
   - **3:** About
   - **4:** Exit

3. **Gameplay:**
   - Enter your name (cannot be empty).
   - Choose your preferred difficulty.
   - Type each displayed word as fast and accurately as possible.
   - Optionally practice your most mistyped letters in a focused round.
   - Your results will be saved to the leaderboard.

---

## Controls & Inputs

- All inputs are via the keyboard.
- Press `Enter` after each word you type.
- Follow prompts for difficulty and practice rounds.

---

## File Structure

- `Fast_Fingers.cpp` — Main source code.
- `dataset.txt` — Word list (must be present in the same folder).
- `result.txt` — Stores leaderboard data (created/overwritten automatically).
- `full_results.txt` — Detailed log of all rounds.
- `README.md` — This file.

---

## Notes

- All input is validated to prevent crashes (e.g., invalid menu options, empty names).
- No third-party libraries required; standard C++ only.
- If you encounter issues, ensure your `dataset.txt` is formatted with one word per line and contains at least 20 words.

---

## Example Usage

```text
For <Start> Enter 1
For <Statistics> Enter 2
For <About> Enter 3
For <Exit> Enter 4
Please enter your choice (1-4): 1
Enter your name: Sarvenaz
Select Difficulty:
1. Normal
2. Difficult
Enter your choice (1 or 2): 1
...
========= Your Typing Summary =========
Name: Sarvenaz
Total Words Attempted: 9
Correct Words: 7
Incorrect Words: 2
Typing Speed (words/min): 19
...
```

---
