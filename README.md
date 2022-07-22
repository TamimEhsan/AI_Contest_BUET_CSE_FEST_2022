# Save The Flag

"Capture opponent's flag and take it to your base before them!"

**Competition**: Save the flag is the game given for the AI contest of BUET CSE Fest 2022. 

**Result**: `3rd`

<hr>

Read below to see different variations of my codes. 

The final submission is  **`TamimEhsanFinalSubmission.cpp`**  and the polished and clean version is **`Strategy_8.cpp`**

Details about the game can be found [Link will be updated soon]. You can read the details there

## Final Submission

There are 3 or 4 winning condition. So, instead of capturing the flag I went for All Opponent Dead. Which was the final iteration! But lastly Masrhrur created an anti strat ( The jiggle strat ) to counter it and many copied it. Which led to me downfall.

### Code Structure

Create a better structure to store all the variables and data

namely

1. **Coord**: Stores 2D coordinate points. And has some important functions to calculate distance, visibility etc.
2. **Power**: For easy storage of power moves
3. **Bot**: Storing information about minions
4. **Player**: For storing information about players

### Pre-calculations:

1. **calculatePairWiseDistance**: calculates pair wise distance of cells (Using bfs). This distance will be used to predict enemy seeker path, assigning keepers etc.

### Strategies:

#### Coin collection

This for me was the most crucial partt! As I just spam fire. So i need more coins. So at first bfs is called with radius 1 and when no coin is found crease search radius by two times untill there are no coins left. And each time update the status of coin present

#### Bot responsibility:

1. No seeker bot. (no one will go for enemy bot)
2. 1 keeper bot. Will protect my flag. But only when the flag is under attack or the timer crossed a definite value.
3. Beater bot. Works as coin collector and attacker. And suppliments keeper.

Always assign the closest one to flag as keeper.

#### Attack

**Mine**: At first drop a mine in my flag base. As maximum targets the flag. And many others did the same! 

**Freeze**: No freeze. Waste of time

**Fire**: Rain hell fire! Saw any opponent? boom. except for the opponent carrier. As I can predict its path. So wont fire if it goes out of vision the next move. This saved some good number of coins.



And that's it! Simple and nice <3



Now read previous iterations of my thinking! Enjoy



## Versions



### Bot responsibilities

1. Seeker: There must always be a seeker who will target enemy flag and bring it near base. If seeker dies one beater will take its place
2. Keeper: There must always be a keeper if possible. It will remain near its own flag. and try to kill or sabotage enemy. If keeper dies one beater will take its place.
3. Beater: Remaining bots will be beaters. They will collect coins and fight enemy bots.



### Power move strategies common 

1. Probable score gather = 20*30/2/2 = 150

2. plant 2 bombs, one near my base, another at enemy base: cost 15*2 = 30

3. Use freeze then use fire 4 times : cost = 12 + 7*4  = 40 ; hp decrease : 96 = 24 * 4P

   





## Game Strategies

### Strategy 1

Send everyone to enemy flag position. And once anyone have captured the flag send everyone to home base. Pretty basic. but starts the journey nicely. Jumped to Rank 10 within first few hours



### Strategy 2

It's a huge improvement than that of strategy 1. Huge improvement in rank too! went to 4 within 12 hours

Algorithms:

1. Use BFS for finding closest bot

Movement:

1. make the closest bot to the enemy flag (not flag base btw) the seeker
2. make the closest bot to my flag (not the flag base) the keeper
3. Others (name beater) will move randomly around the maze collecting coins and stuff.
4. A seeker will never use any power moves. It will just move towards its goal
5. A keeper will always follow own flag. 

Power Move:

1. Seeker wont use power moves
2. If any non seeker (keeper or beater) when encounters an enemy use fire
3. If any non seeker (keeper or beater) when encounters the enemy seeker use 1 freeze then fire 3 fire



### Strategy 3 [Stable]

It's just a polished strategy 2. Top in leaderboard at the end of first day. ie 14 hours

Movement:

1. For beaters Instead of randomly choosing every non wall cell , here only reachable cells are chosen randosmly.

Power Move:

1. Don't waste moves on non  seeker enemies
2. Use freeze, fire fire fire combo if the hp of enemy seeker is greater than twice of fire damage. Else use fire

### Strategy 4

Somehow this works worse than strategy 3 :/

Movement:

1. For beaters instead of choosing cell randomly a 20 moves bfs is used to determine the best possible coin collection scenario. At first a initial 20 radius search is made then the radius is doubled if no coin is found. 

Power Move:

Most probably I have made this too complicated. Need to rethink 

1. Seeker will attack anyone with fire
2. Keeper will use freeze, fire , fire, fire combo on enemy seeker only if it has enough score
3. Keeper will use fire if 2 if a non carrier is seen nearby
4. Keeper will use fire on enemy seeker if it has low score



### Strategy 5 [Stable]

Hybrid of 3 and 4. Working better than 4. Finished second day at 2nd position.

Bot responsibility:

Make one bot keeper when when flag is under attack

Movement:

Like Strategy 4

Power Move:

Like Strategy 3

Bug fix: If misfire of freeze occurs it will be counted for and following fires won't be applied

bug: 

1. A bot may get trapped [fixed]

 ![image-20220717122957693](C:\Users\User\AppData\Roaming\Typora\typora-user-images\image-20220717122957693.png)

2. Seeker leaves its combo if another one is close. Instead of distance we need to think of visual too. [will be fixed in strategy 6]
3. power move misses if the attacker turns at the exact moment. [will be fixed in strategy 6]
4. We need to target enemy keeper too! (We can check if the keeper is under attack or not). What will be the best move idk still. [will be fixed in strategy 6]
4. Keeper assignment is faulty. closest one is not set as keeper [will be fixed in strategy 6]

Strategy for next iteration:

1. Don't  fire if the next location is unreachable 
2. Check under fire by how many bots // how will that help?
3. Check how many bots are under fire? // how will that help?

### Strategy 6:

Is an upgrade of strategy 5.

Coin collection:

Changing initial search radius from 20 to 1 improves the coin collection to a huge leap! Like 1.5 times!



### Strategy 7: [Stable] [Final Submission]

Hell with everything 🙂 ! Just spam fire. Target is full knockout :) Finished day 3 at first position

### Strategy 8: [Cleaner version of Final Submission]

Upgrade of strategy 7 ()
