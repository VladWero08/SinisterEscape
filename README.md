<p align="center">
  <img src="https://github.com/VladWero08/SinisterEscape/assets/77508081/34f4012d-7b52-4e0d-8122-cdacb4b1ef32" height="130" width="650"/>
</p>

Sinister Escape is my matrix project that I have made for the **Introduction to Robotics** course, taken in my *3rd year* at the *Faculty of Mathematics and Computer Science, University of Bucharest*. It is a game about collecting *notes*, in order to escape from the house hounted by *Dr. Nocturne*. Simple, right?

<details open>
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

<details open>
<summary><h2>🏚️ Description</h2></summary>
Sinister Escape is a matrix game that is about travelling between **4 rooms** and finding notes, to escape from the haunted house. Who is hounting the house? Dr. Nocturne. Before getting into how Dr. Nocturne will haunt you and make it harder to escape from the house, we should discuss about how the house is configured.

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
</details>

<details open>
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

</details>

<details open>
<summary><h2>📑 Menu</h2></summary>
</details>

<details open>
<summary><h2>🎮 How to play</h2></summary>
</details>

<img src="https://github.com/VladWero08/SinisterEscape/assets/77508081/1bc21c3b-48e9-4fb5-a016-246cacaf8e39"/>
<img src="https://github.com/VladWero08/SinisterEscape/assets/77508081/ad24c3f7-9d7f-43cf-822b-417ad2c54208"/>
