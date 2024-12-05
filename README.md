Poker Hand Odds Calculator
==========================
This program calculates the odds of a poker hand winning against other poker hands. It uses a Monte Carlo simulation to simulate the odds of each hand winning. The program can calculate the odds of a hand winning against another opponent.

To start, input your hand such as "As Kd", where the first character is the rank and the second character is the suit. Then, input the opponent's hand the same way. After that, pick the stage of the game which are preflop, flop, turn, and river. The program will then simulate the odds of your hand winning against the opponent's hand.

Since this program uses a Monte Carlo simulation, the more simulations, the more accurate the odds will be. The default number of simulations is 100,000, but you can change that by changing the number of simulations you want to run.

This program also includes an automated simulator, which will run a specified amount of trials and output the data into a CSV file to create a dataset. An example of such a CVS file ("PokerOddsDataset.cvs") with 100,000 data points is included in the GitHub file.
