# CS30-IceMan 
Ice Man Game CS30

# Description
This was the final project that I made in my Advanced Programming in C++ class called Ice Man. This game was targeted towards my knowledge of data structures and algorithms, use of pointers, and classes. It encorporated multiple inheritance, algorithms, and data strucutures to create a working video games. 

This tested my ability to work in a team setting, developing and creating different code as well as utilize different code as well as utilize different code bases in order to create functional working code. 

## Game Description

The game "Iceman" involves the player controlling an Iceman character to dig through underground tunnels in an oil field to collect barrels of oil. The player starts with three lives and must complete multiple levels, each with a unique configuration. The Iceman is placed at the top-middle of the oil field at the start of each level with full health, water for the squirt gun, and a sonar charge.

# Game Setup and Objective:
The player must dig and find all hidden oil barrels to progress to the next level.
Each level has boulders, gold nuggets, and barrels of oil randomly distributed. Only boulders are visible initially.

# Iceman's Actions:
The Iceman can move using arrow keys, dig through ice, and avoid boulders.
The squirt gun can be used to stun protesters.
Gold nuggets can be used to bribe protesters.
Sonar charges reveal hidden items within a radius.

# Enemies (Protesters):
Regular Protesters: Easier to deal with, can be bribed or stunned.
Hardcore Protesters: More challenging, cannot be bribed, and track the Iceman's location if close enough.
Both types of protesters roam the field and irritate the Iceman by shouting, which decreases his health.

# Game Mechanics:
Iceman loses health when shouted at by protesters or hit by falling boulders.
Lives are lost when health reaches zero; the game ends when all lives are lost.
Water pools occasionally appear for refilling the squirt gun.
Friends drop sonar kits to help find hidden items.
Points are earned for various actions such as squirting protesters, dropping boulders on them, and collecting items.

# Scoring:
Regular Protester gives up: 100 points
Hardcore Protester gives up: 250 points
Boulder bonks Protester: 500 points
Bribing Regular Protester: 25 points
Bribing Hardcore Protester: 50 points
Barrel of oil: 1000 points
Gold Nugget: 10 points
Water: 100 points
Sonar Kit: 75 points

# Game Play:
Game play is divided into ticks, with each tick allowing the Iceman and other objects to perform actions.
The Iceman can:
move using WSAD or the Arrow Keys
shoot using space bar
use sonar using ___
restart the level with the escape key.
Protesters and other objects have their own actions and interactions within each tick.
