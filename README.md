<b>Just finished writing my fully working and functional game-engine (Lumberjack) in C++ and objective-C++.
   I am a huge fan of C++ and love incorporating physics into my programming.
   I plan to expand and evolve this code to add more features.
   <br>Tool: Xcode Library: SFML<br><br><b>

1. Created shapes (tree, branches, player, clouds, bee, axe, log, gravestone) using SFML Texture, Sprite and draw and added movement to the shapes.<br>
2. Added a time bar to keep track of player’s remaining time as well as to respond when the player runs out of time.<br>
3. Handled player’s input (keyboard input, mouse positions and inputs) and player’s death.<br>
4. Added sound effects for log chopping, player ran out of time and player’s death using SFML sound class.<br>
5. Added score and message text using Text and Font class of SFML.<br>
6. Handled growing and updating branches, pausing and restarting the game and collision detection with screen and objects.
<br><br>

Demo <br><br>
![](timberguy.gif)

<br><br>

<b>Learnings and why use C++:<b> With any new project, the goal should always be to learn and understand the concepts of programming, i.e. the principles such as memory-management, object-oriented programming OOP, compiling & linking. The goal should not be to get the results as fast as possible, as it interferes with creating good codes. C++, in a way, forces you to create clean solutions and care for memory or otherwise the game just won’t work. This is the benefit of working with purely compiled languages as the compiler tells you to do it correctly or things will crash. This could be hard in the beginning but you will learn a lot of stuff in a short time. Also, Garbage-collection is nice, it's convenient to not care for the memory but with game programming, garbage-collection could lead to unpredictable performance glitches (short pausing of the application). C++ makes you the master of construction and destruction and you are able to control when memory is allocated and freed. Lastly, performance is always relevant, and applications should be platform-independent. 
