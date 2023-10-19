# CChess

Welcome to CChess, a simple chess game coded in C that allows you to play against an AI using the minmax algorithm or against another person on the same screen. The graphical interface is built using the ncurses library.


## Table of Contents

- [Screenshots](#screenshots)
  - [Main Menu](#main-menu)
  - [Gameplay](#gameplay)
- [Minmax Algorithm in Chess](#minmax-algorithm-in-chess)
- [Cloning the Project](#cloning-the-project)
- [Installing Dependencies](#installing-dependencies)
- [Compilation](#compilation)
- [Running the Game](#running-the-game)
- [License](#license)

## Screenshots

1. **Main Menu:**
   - Include a screenshot of the main menu where users can choose between playing against the AI or another person.

   ![Main Menu](screenshots/menu.png)

2. **Gameplay:**
   - Capture a screenshot during a game against the AI, showcasing the chessboard and current moves.

   ![Gameplay](screenshots/gameplay.png)

## Minmax Algorithm in Chess

The AI opponent in this game utilizes the minmax algorithm to make decisions. The minmax algorithm is a decision-making algorithm used in two-player games, such as chess. Here's a brief explanation:

1. **Maximizing Player (AI):**
   - The AI is the maximizing player, trying to get the highest possible score. It assumes that the opponent will make the best moves to minimize the AI's score.

2. **Minimizing Player (Opponent/You):**
   - The opponent, or the human player, is the minimizing player, trying to get the lowest possible score. It assumes that the AI will make the best moves to maximize the opponent's score.

3. **Recursive Exploration:**
   - The algorithm recursively explores possible moves, creating a tree of possible game states. At each level, the maximizing player chooses the move with the highest score, and the minimizing player chooses the move with the lowest score.

4. **Heuristic Evaluation:**
   - The algorithm uses a heuristic evaluation function to assess the desirability of a particular board state. This function assigns a score based on factors like piece values, board control, and positional advantage.

## Cloning the Project

To clone the repository, use the following command:
```git clone https://github.com/your-username/CChess.git```

## Installing Dependencies

Before compiling the project, make sure you have the ncurses library installed on your system.

On Debian-based systems, you can install it with:
```sudo apt-get install libncurses5-dev libncursesw5-dev```

On Red Hat-based systems, use:
```sudo yum install ncurses-devel```

On Manjaro-based systems, use:
```pamac install ncurses```

## Compilation

To Navigate, compile and execute the game, use the following command:
```cd CChess && make && make run```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Feel free to contribute and enjoy playing chess with CChess!
