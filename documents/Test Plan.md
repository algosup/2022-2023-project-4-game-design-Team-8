# Test Plan

## Objectives

The software tested is the video game called Yul-Rememeber, made by the student group 8 of ALGOSUP.

Yul-Rememeber is a rogue like/lite game, which means that the player will have to progress through dungeons in order to collect Items, new Characters to play as, and acheivements. The core loop of the game is to explore, fight, die and retry, each death allowing the player to come back stronger either with the newly acquired knowledge or with bonuses. The final objective of the game is to collect all the Items "Memories" so that the main character "Yul" can remember his past life.

As all video game the main goal of the application is to entertain the user / player.

## Scope

### Features to be tested

The following features will be tested:
- Player
    - Player Moving using ZQSD
    - Player Looking at the cursor
    - Ability system
- Fight System
    - Moving enemies
    - Enemies and player can deal damage to each other
- Environment
    - Multiple rooms to explore
    - Procedural generation?

### Features not to be tested

The following features won't be tested because they are not required for the game to function properly and are not part of the main gameplay loop.
- Player
- Fight System
    - Player can block and/or deflect incomming projectiles 
- Environment
    - Breakable furnitures
    - Room Hazards

## Approach

Flow Chart

![Flow_Chart](/documents/Images/Flow_Chart.png)

## Assumptions

1. The member of the team that is assigned to testing will be present at any point during the project.
2. ...

## Risks

1. The team member assigned to testing is absent.
2. ...

## Contingency plan

1. The said team member will make templates for test procedures and will render his work accessible through GitHub to ensure that other team members could take on his work if the need were to rise.
2. ...

## Schedule

After each new version of the game tests will be conducted to ensure the newly implemented features are working correctly

## Defect Tracking

All bugs found will be saved and repertoriated in a database to facilitate the process of following which bugs are still currently in the game, what they exactly do and how to reproduce them.

[Bug Database](https://docs.google.com/spreadsheets/d/1uPonp1SBknfaEw47wkt7-kWAfdo2WUsXSYAGhkuUQrc/edit?usp=sharing)

## Test Environment

<b>Software:</b>
| | |
| - | - |
| OS | <span style="color:white"><b>macOS Ventura 13.2</b></span> |

<b>Hardware:</b>
| | |
| - | - |
| Processor | <span style="color:white"><b>Apple M1</b></span> |
| RAM | <span style="color:white"><b>8 GB</b></span> |
| GPU | <span style="color:white"><b>None</b></span> |

