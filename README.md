<p align="center">
  <img src="https://github.com/VladWero08/SinisterEscape/assets/77508081/34f4012d-7b52-4e0d-8122-cdacb4b1ef32" height="130" width="650"/>
</p>

Sinister Escape is the matrix project that I have made for the **Introduction to Robotics** course, taken in my *3rd year* at the *Faculty of Mathematics and Computer Science, University of Bucharest*. It is a game about collecting *notes*, in order to escape from the house hounted by *Dr. Nocturne*. Simple, right?

<details closed>
<summary><h2>📝 Task requirements</h2></summary>

> *A game on a 16x16 logical matrix (it's OK to have only 1 physical matrix) that implements either multiple rooms, visibility / fog of war and/or multipls physical matrix.*

### Menu requirements
Create a menu for your game, emphasis on **the game**. You should scroll on the LCD with the joystick. Remember you have quite a lot of flexibility here, but do not confuse that with a free ticket to slack off. The menu should include the following functionality:

<hr>

1. **Intro Message** - when powering up a game, a greeting message should be shown for a few moments.

<hr>

2. **Categories** - should contain roughly the following:

    - **Start game**: starts the initial level of your game
    - **Highscore**:
      - initially, we have 0.
      - update it when the game is done; highest possible score should be achieved by starting at a higher level.
      - save the top 3+ values in EEPROM with name and score.
    - **Settings**:
      - *Enter name* - the name should be shown in highscore. Maybe somewhere else, as well? You decide.
      - *LCD brightness* control - mandatory, must change LED wire that’s directly connected to 5v. Save it to *EEPROM*.
      - *Matrix brightness* control - make sure to display something on the matrix when selecting it. Save it to *EEPROM*.
      - *Sounds* -  ON / OFF. Save it to *EEPROM*.
      - Extra stuff can include items specific to the game mechanics, or other settings such as chosen theme song etc. Again, save it to eeprom. You can even split the settings in 2: game settings and system settings.
    - **About**: should include details about the creator(s) of the game. At least game name, author and github link or user (use scrolling text?)
    - **How to play**: short and informative description

<hr>

3. **While playing the game** - display all relevent information: _lives_, _level_, _score_, _time?_, _player name?_, etc. 

<hr>

4. **Upon game ending**:
    - *Screen 1*: a message such as _"Congratulations on reaching level/score X"_ OR _"You did better than Y people"_ etc. Switches to screen 2 upon interaction (button press) or after a few moments.
    - *Screen 2*: display relevant game info: _score_, _time_, _lives left_ etc. **Must inform player if he/she beat the highscore**. This menu should only be closed by the player, pressing a button.

<hr>

### Game requirements

- **minimal components**: an LCD, a joystick, a buzzer and the led matrix;
- you must add basic sounds to the game (when _eating_ food, when dying, when finishing the level etc); **extra**: add theme songs.
- each level / instance should work on 16x16 matrix. You can apply the concept of visibility / fog of war (aka you only see 8x8 of the total 16x16 matrix, and you discover more as you move around) or you can use the concept of _rooms_. Basically you will have 4 rooms
that you need to go through on each level;
- it must be intuitive and **fun to play**;
- it must make sense in the current setup;
- you should have a feeling of progression in difficulty. Depending on the dynamic of the game, this is done in the same level or with multiple levels. You can make them progress dynamically or have a number of fixed levels with an endgame. Try to introduce some randomness, though.
</details>

<details closed>
<summary><h2>🔙 Backstory</h2></summary>

The first thing that got on my mind when I heard that we needed to configure _rooms_, in which the player should _move_, was **horror games**. Inspired by horror games like _Outlast_ and _Slenderman_, which involve **searching for clues** (in my case notes) to escape from the haunted house, I got hooked on the idea of creating my own spooky game.

Besides finding clues, I thought about implementing the idea of **jump scare**, but on the level of an 8x8 matrix. As you will be able to see in the following description, the villain in my game will reveal itself only when the player is close enough, similar with all the popular horror games.
  
</details>

<details closed>
<summary><h2>🏚️ Description</h2></summary>
Sinister Escape is a matrix game that is about traveling between 4 rooms and finding notes, to escape from a haunted house. Who is haunting the house? Dr. Nocturne. Before getting into how Dr. Nocturne will haunt you and make it harder to escape from the house, we should discuss how the house is configured.

### Rooms connection

Travelling between rooms is in the **advantage** of the player, because wherever you are, you have **4 directions** in which you can travel.

<p align="center">
  <img src="https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/9f16bf79-13df-4e8c-9e90-a6b28789008b" width="576" height="496"/>
</p>

<p align="center">
  E.G. from room 0 you can go up -> room 2, down -> room 2, left -> room 1, right -> room 1
</p>

<hr>

### Rooms configuration

On the matrix, this is how the rooms will be displayed:

<p align="center">
  <img src="https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/2cb3bc30-e8c1-49db-a001-2d9f9b466e99" width="576" height="496"/>
</p>

Moving between rooms will be made through some _"doors"_; the doors are positioned on each room in: _R1C4_, _R1C5_, _R4C1_, _R4C8_, _R5C1_, _R5C8_,  _R8C4_, _R8C5_. (R = row, C = column). 

### Player's / Note's representation 

In the matrix, you will be represented by a **fast-blinking red dot**, while the notes will be **slow-blinking red dots**. Besides this, the walls will be **static red dots**. As you might have guessed already, you will not be able to move through the walls, you need to move through **uncolored dots**.

<hr>

### Note

The notes will appear randomly in one of the rooms, which is also chosen randomly. After collecting one of them, it will randomly _spawn again_ in one of the rooms. The player will achieve a **WIN** only if he/she can collect **6 notes**. 

<hr>

### Dr. Nocturne

This is where things get complicated. Dr. Nocturne is on his pursuit to _kill you_. He will also be spawned randomly.

He is presented by a **slow-blinking red dot**, with a blinking frequency _equal_ to a note's blinking frequency. That means you cannot _distinguish between a note and Dr. Nocturne_.

Once you come close enough, you will be able to realize that the slow-blinking red dot you were approaching was the Dr. How? He will start chasing you:

- **LVL 1**: all notes are safe, you can collect them easily; leveling from 1 -> 2 occurs when the player collected **2 notes**;
- **LVL 2**: Dr. Nocturne spawns; he will start chasing the player when the **Euclidean distance** between him and the player is $\le$ 5; the speed with which he will approach the player is 1 move/s; leveling from 2 -> 3 occurs when the player collected **4 notes**;
- **LVL 3**: Dr. Nocturne gets faster and trickier; the **Euclidean distance** between him and the player needs to be $\le$ 3 in order to start chasing the player; the speed increased, 1 move/0.85s; this is his strength until the player reaches **6 notes** or **dies**.

If Dr. Nocturne catches the player, he will eat one of his lives, than dissapear.

<hr>

### Player

The player's main aim is to escape the haunted house as fast as possible while collecting **6 notes** and **staying alive**. In the beginning, he/she has 0 notes and 3 lives.

During the game, the player will see a timer, that is constantly increasing. The timer will tell how efficient and fast the player was, it will dictate **the score**. If he/she moves too fast, she might get killed by Dr. Nocturne. If he/she moves too slowly maybe their score will not be enough to reach the high scores. What is the point of playing if you are not one of the best? 🤔
</details>

<details closed>
<summary><h2>🔧 Set up</h2></summary>

### Components:
- LCD Display (_16 x 2_)
- 1088AS LED Matrix (_8 x 8_) & MAX7219 Driver to control it
- Joystick
- Buzzer
- Wires, resistors & capacitors as needed

<hr>

### EEPROM management

In EEPROM will be stored multiple values related to the settings set by the user: _LCD brightness_, _matrix brightness_, _sound (ON / OFF)_, _highscores_ & _how many_ highscores have been registered. From the total of **200 bytes** available in the EEPROM, I only used 24 bytes. The following scheme shows where I stored each information:

![MemoryAllocationEEPROM](https://github.com/VladWero08/SinisterEscape/assets/77508081/b8aa2998-e3d9-4e26-9680-d7cafb463bee)

The name of the players will contain maximum **3 chars**, each one of them stored on **1 byte**. The score will be stored as a **unsigned long int**, and it will represent how many _seconds_ the player escaped in. 

<hr>

### Photos

| From the top | From the side| From perspective |
| ---------- | ---------- | --------- |
| ![h08-top](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/2c6344a2-87a3-4d0a-8d2f-966e9f660626)| ![h08-side](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/705049a0-a803-4a22-a69c-8d56db1371ee) | ![h08-perspective](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/ac58c8ea-0ef3-476a-a3f3-c9280964df29) |

</details>

<details closed>
<summary><h2>📑 Menu</h2></summary>
  
The structure of the menu will be the following:

**Intro message**: which can be skipped by _pressing_ the joystick. 

**Main menu**:
  - **Start game**: after the game has finished, the user will need to choose from
    - Play again
    - Back (to the main menu)
  - **Highscores**: display a list of highscores with the scores and the player's name, who achieved them
  - **Settings**:
    - **Enter name**: the user will be prompted a "keyboard" with all the letters of the alphabet, an input where the name chosen will be displayed, and 3 buttons: delete (the whole input), save, and exit (without saving); the joystick will be used to create the name by moving through letters and pressing;
    - **LCD brightness setting**: the user will need to scroll **up** or **down** to modify the digits of the number that represents the LCD's brightness;
    - **Matrix brightness setting**: the user will need to scroll **up** or **down** to modify the digits of the number that represents the matrix's brightness;
    - **Reset scores**: all highscores will be reset
    - **Sound setting (ON/OFF)**: toggle the sound throughout the whole session
    - **Back**: (to the main menu)
  - **About**: a small story about the game, and the name of the creator (me), that corresponds to the name of my GitHub account

To enter a menu section, scroll through the current menu and press the menu you want to go to. To go back to the parent menu, from the current menu, use the _Back_ option or press the _exit arrow_, depending where you are currently navigating.
</details>

<details closed>
<summary><h2>🎮 How to play</h2></summary>

### Basics: 
After starting the game, use the joystick to move around: up, down, left, right. If you point diagonally, the player will stay in the same place; analogous when bumping into walls.

To move from one room to the other, use the _"doors"_ placed at the edges of the room (their position is explicitly defined at the beginning of the _description section_). 

<hr>

### Strategy:
Search for notes, go near them, and collect them. To collect a note, you need to be placed over it; be careful, when approaching a note, and do not go too fast, it might be Dr. Nocturne disguised as one of them.

Listen to the sounds during the game, you will hear a **special sound** when **collecting a note** or **losing a life**. Also, constantly check the LCD for the details of the game: _time_, _notes_, _lives_.

<hr>

### Pause
At any time, if you want to pause the game, just press the joystick, wherever you might be located.

**Good luck!**
  
</details>

Check out the <a href="https://youtu.be/WaORZJMfFRI">demo</a>. 
_NOTE!_ Some LEDs do not work in the matrix, in the video. More specifically, the LEDs which do not work are on the _first_ and _second column_ of the matrix.
