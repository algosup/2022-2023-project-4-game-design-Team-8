# Technical Specifications

<details>
<summary>Table of Content</summary>

- [Introduction](#introduction)
- [Project Overview](#project-overview)
- [Functional Requirements](#functional-requirements)
- [Non-Functional Requirements](#non-functional-requirements)
- [Architecture and Design](#architecture-and-design)
  - [Component-Based Architecture](#component-based-architecture)
  - [Game Systems](#game-systems)
  - [Procedural Generation](#procedural-generation)
  - [Random Neighbor Function](#random-neighbor-function)
  - [Level Generation Function](#level-generation-function)
- [Game Balancing](#game-balancing)
  - [Stats](#stats)
  - [Yul and Rim Stat Comparison](#yul-and-rim-stat-comparison)
  - [Explanation for Each Stat](#explanation-for-each-stat)
    - [Speed](#speed)
    - [Damage](#damage)
    - [Fire Rate](#fire-rate)
    - [Health](#health)
  - [Stats Ups Formula](#stats-ups-formula)
  - [Example for Yul](#example-for-yul)
- [Technologies Used](#technologies-used)
- [Testing Plan](#testing-plan)
  - [Unit Testing](#unit-testing)
  - [Integration Testing](#integration-testing)
  - [System Testing](#system-testing)
  - [Acceptance Testing](#acceptance-testing)
- [Deployment Plan](#deployment-plan)
  - [Distribution Channel](#distribution-channel)
  - [Installation Process](#installation-process)
- [Project Management](#project-management)
- [Quality Assurance](#quality-assurance)
- [Project Risks](#project-risks)
- [Performance Metrics](#performance-metrics)
- [Accessibility](#accessibility)
- [Future Plans](#future-plans)


</details>

## Introduction

This document provides the technical specifications for the project managed by Mathis Kakal. The project aims to develop a new video game using Unreal Engine 4.27.2 and C++ programming language. The team comprises Mathieu Chaput as the software engineer, Leo Chartier as the program manager, Arthur Lemoine as the QA, and David Cuahonte Cuevas as the Tech Lead.

## Project Overview

The project aims to develop a new video game that incorporates various game elements such as adventure and action. The game will be developed for PC, and the target audience is gamers aged between 16-35 years. The game will feature a simple storyline with several playable characters, each with their unique abilities and gameplay mechanics.

## Functional Requirements

1. The game should support a single-player mode with a simple storyline that immerses the player in the game world.
2. The game should support various playable characters, each with their unique abilities and gameplay mechanics.
3. The game should feature multiple environments with different levels of difficulty, each with its unique challenges and obstacles.
4. The game should allow the player to customize their character's abilities and equipment.
5. The game should feature a dynamic camera system that adapts to the player's movements and actions.
6. The game should provide the player with various resources and items that help them progress through the game.
8. The game should feature a combat system that incorporates various elements such as melee combat, ranged combat.

## Non-Functional Requirements

1. The game should load quickly and provide a smooth gaming experience without any significant performance issues.
2. The game should be stable and free of bugs and glitches.
3. The game should have a user-friendly interface that is easy to navigate and use.
4. The game should support various input devices such as keyboard, mouse, and gamepad.
5. The game should support various screen resolutions and aspect ratios.
6. The game should have an engaging soundtrack and sound effects that enhance the gaming experience.
7. The game should have visually appealing graphics and animations that immerse the player in the game world.

## Architecture and Design

The game will be developed using Unreal Engine 4.27.2 and will be programmed primarily using C++. The game will follow a component-based architecture that allows for modular and scalable code. The game will feature various systems such as the player system, the combat system and the "dungeon" system. These systems will interact with each other to create a seamless and engaging gameplay experience.

## Game Balancing

Game balancing is an essential aspect of any game development process. In this game, balancing is done by managing various character stats, weapons, and items to ensure a fair and enjoyable experience for the player.

### Stats
There are four primary stats in the game: speed, damage, fire rate, and health. Each stat has a base value for Yul, one of the two playable characters. The other character, Rim, has a different unique multiplier for each stat.

### Yul and Rim Stat Comparison

| Stat | Yul | Rim |
| --- | --- | --- |
| Speed | 1.0 (Ingame/3) | 0.8 (ingame/2,5) |
| Damage | 5 (ingame/2) | 8 (ingame/3) |
| Fire Rate | 1.0 (ingame/2) | 1.0 (Ingame/2) |
| Health | 10(Ingame/3) | 8(ingame/2,5) |
| Multiplier | Speed: 1.2, Damage: 0.8, Fire rate: 1.0, Health: 1.0 | Speed: 0.8, Damage: 1.2, Fire rate: 1.0, Health: 1.0 |

The maximum stats that a player can attain are as follows:

- Speed: 2.0
- Damage: 20
- Fire rate: 2.0
- Health: 20

### Explanation for Each Stat

#### Speed

Speed can range from 0.1 (minimum) to 2.0 (maximum). Weapons and items increase the speed of the player, with the maximum being double the current speed of Yul.

#### Damage

Damage ranges from 0 (minimum) to 20 (maximum).

#### Fire Rate

The base fire rate is 1.0, meaning one bullet per second for the character. However, when the player picks up a weapon, the character multiplier should be the current stat of the player. For example, if the player picks up a fire rate up with items and has a 1.4 fire rate as a stat, the weapon base fire rate * Stat fire rate.

#### Health

Health is the quantity of damage that the player can receive. The base health for Yul is 10, which can be increased to double.

## Stats Ups Formula

The formula for stats ups is as follows:

`EffectiveStat = BaseStat * (1 + FlatStatUps * StatUpMultiplier)`

- EffectiveStat is the final value of the stat used in-game.
- BaseStat is the character's base value for the given stat.
- FlatStatUps is the total of all stat ups that are excluded from the above (for example, items that don't apply to other categories).
- TotalStatUps is the total of all regular stat ups collected (not including special exceptions or multipliers).
- StatUpMultiplier is a constant multiplier value that determines how much each individual stat up affects the total value of the stat. For now, a value of 0.2 is used.

### Example for Yul

Suppose Yul has collected one flat speed boost and one flat damage boost in addition to the regular stat ups. The base speed value is 10, and the unique multiplier for Yul is 0.2. Here's how you would calculate Yul's effective speed:

`EffectiveSpeed = 10 * (1 + (0 * 0.2)) + 1 + 1 = 12`
`EffectiveDamage = 12`



## Procedural Generation

The game uses a procedural generation algorithm to generate levels. The algorithm takes which rooms you want and their number and creates a matrix or rooms. Each of these rooms will be modified by the algorithm to include metadata such as door placements, items, etc.

### Level Generation Function

The level generation function generates a matrix of rooms (R) by selecting a starting room (r0) from a list of available rooms (L) and making it the spawn room. It then iteratively selects a random neighbor (r1) of a previously placed room (from R) and adds a new room (r2) from the list of available rooms (L) as a neighbor on a randomly selected side (s). If a randomly generated probability p is greater than or equal to the specified probability of having a special room (ps), the new room is marked as special. A door is then created between the two rooms.

Finally, the algorithm selects a room (r3) in the R matrix that is the farthest away from the spawn room (R(0,0)) and marks it as the boss room.

Note that the algorithm generates N rooms, where N is a specified parameter that is greater than or equal to 2 since there must be at least a spawn room and a boss room.

### Random Neighbor Function

The random neighbor selects a room and a side of this room on which you can add a new one. It does this by randomly selecting an index from the R matrix and checking if the room at that index is not special and has no neighbor on the specified side (s). If these conditions are met, the function returns the selected room and side.

### Flowchart

![room example](Level%20Generation/Level%20Generation.png)


## Technologies Used

The game will be developed using the following technologies:

1. Unreal Engine 4.27.2
2. C++ programming language


## Testing Plan

The game will be thoroughly tested throughout the development process to ensure  that it meets the functional and non-functional requirements. The testing plan will include unit testing, integration testing, system testing, and acceptance testing. The testing plan will be continuously updated to reflect the progress of the development process.

## Deployment Plan

The deployment plan outlines the steps required to make the game available to the players.

### Distribution Channel
The game will be distributed through the popular code repository website, GitHub. This platform will allow players to access the game's source code, as well as to download compiled versions of the game. GitHub is a popular and reliable platform that allows for easy distribution, updates, and version control.

### Installation Process
Players will be able to download the compiled version of the game from GitHub. The installation process will vary depending on the operating system used by the player. The game's README file will include detailed instructions on how to download, install, and run the game on various operating systems, including Windows, MacOS, and Linux.

### Update Process
The game will be updated periodically to fix bugs, add new features, and improve the gameplay experience. Players will be notified of new updates through GitHub's notification system. The game's README file will include instructions on how to update the game to the latest version.

### Support
Players can reach out to the development team via the GitHub issue tracker. The development team will be actively monitoring these channels to provide support and address any issues or concerns that arise.


## Project Management

Mathis Kakal will be the project manager for the development of the game. The team will follow an agile methodology with regular sprint cycles to ensure that the project progresses according to the timeline. The project management plan will include the following:

1. Develop a project schedule that outlines the tasks and milestones of the project.
2. Assign tasks to team members based on their expertise and availability.
3. Hold regular meetings with the team to discuss progress and address any issues or concerns.
4. Use project management tools such as ClickUP to track progress and manage tasks.
5. Continuously evaluate the project's progress and adjust the project plan as needed.

## Security Requirements
The game is intended to be played locally and does not include any online features. Therefore, security risks are minimal. However, measures will be taken to ensure that user data is protected in the event of any unforeseen circumstances.

## Performance Requirements
The game will be developed in 2D format and will not require a dedicated graphics card. As such, it is expected to have minimal performance requirements. Further testing will be conducted to determine specific metrics such as framerate, load times, and response times.

## Localization Requirements
The game will initially be available in English only, but provisions will be made for additional languages in the future.

## Accessibility Requirements
Currently, no specific features for accessibility have been implemented. However, future plans include a colorblind mode, descriptive subtitles, and font size options to ensure accessibility for all players.

## Version Control
The project will utilize Github for version control, allowing for efficient collaboration and development management.

## Data Management
The game will include a single save file initially, with the potential for additional save files in the future. As the game is currently local, there are no plans for cloud-based data management.

## Performance Optimization
Efforts will be made to optimize the game's performance, including minimizing resource usage and maximizing compatibility across a variety of hardware configurations.

## User Feedback
While direct, in-person feedback will be collected during development, future plans include implementing a system for collecting feedback from players post-release.

## Maintenance and Support
Development will continue post-release, with plans to add additional zones and levels to the game. Ongoing maintenance and support will be provided to ensure the game remains up-to-date and bug-free.

## Quality Assurance

Quality assurance is a critical aspect of game development. It ensures that the game is functioning as intended and is free from bugs and other issues that could negatively impact the player's experience. At our studio, we take quality assurance very seriously and have a specific philosophy when it comes to testing our game.

Our philosophy is to "play the game and see how it functions." This means that we rely on our tester to play the game extensively and report any issues they encounter. We typically release a new version of the game every week, and our tester is responsible for thoroughly testing each new version to ensure that it meets our quality standards.

Our quality standards require that the game is playable with essential features bug-free. This means that the game must be functional and playable without any major issues. Of course, there may be some minor bugs or issues that slip through the cracks, but we make every effort to minimize these as much as possible.

In addition to testing each new version of the game, we also conduct regular playtesting sessions with external players. This allows us to gather feedback and identify issues that we may have missed during our internal testing. By continuously testing and improving our game, we aim to deliver a high-quality product that meets the expectations of our players.

## Project Risks

There are several risks associated with this project, including:

### Technical Risks

- Unreal Engine may not work as expected
- Bugs in the code
- Compatibility issues
- Lack of support for specific features

### Schedule Risks

- The project may take longer than expected to complete, causing delays in the release date.

### Resource Risks

- The team may face a shortage of resources (e.g., developers, testers, hardware) or an unexpected cost increase.

### Quality Risks

- The game may not meet quality standards or may have bugs that affect gameplay or user experience.

### Market Risks

- The game may not be well-received by the target audience, or may not generate enough revenue to cover development costs.

### Legal Risks

- There may be copyright or trademark issues with certain aspects of the game, or the game may be subject to legal action.

### Security Risks

- The game may be vulnerable to hacking, data breaches, or other security threats that compromise user data or the game's integrity.

To mitigate these risks, it's important to have contingency plans in place and to stay vigilant throughout the development process. This could include:

- Building in time for testing and troubleshooting to identify and fix technical issues before the release.
- Creating a detailed project schedule and monitoring progress to stay on track.
- Maintaining open lines of communication with team members to ensure that everyone is aware of any resource constraints or cost overruns.
- Implementing a robust QA process to catch bugs and ensure that the game meets quality standards.
- Conducting market research and user testing to ensure that the game is meeting the needs and expectations of the target audience.
- Consulting with legal experts to ensure that the game is free of legal issues and compliant with all relevant laws and regulations.
- Implementing strong security measures, such as encryption and secure data storage, to protect user data and the game's integrity.

## Performance Metrics

The game should achieve the following performance metrics:

- 60 frames per second (FPS)
- Fast loading times
- Low memory usage because the game is not demanding.

## Accessibility

Accessibility is an important aspect of game development. The goal is to make the game playable and enjoyable for as many people as possible, regardless of their physical abilities or disabilities. In the case of our game, we have taken some initial steps to ensure that it is accessible to a wide range of players.

Firstly, we have ensured that the game is not completely inaccessible for those who have color-blindness or are deaf. While our game may not be optimized for color-blind players or those who are deaf, we have made efforts to avoid making it entirely unplayable for these groups. However, we recognize the importance of catering to these groups and are planning to implement a colorblind mode in the future. This will involve modifying the game's color scheme to ensure that it is distinguishable for those with color blindness.

In addition to colorblindness and deafness, there are other physical disabilities that can affect gameplay. For example, those with mobility issues may struggle with using traditional keyboard and mouse controls. We have not yet implemented any specific accessibility features for these groups, but we will be exploring ways to make our game more accessible to a wider range of players in the future.

## Future Plans

Our future plans for the game include:

- Adding a colorblind mode and descriptive audio.
- Creating cutscenes (mainly at the beginning and at the end of the game).
- Adding more levels (for now, we only have 1 big one, but we have thought of 6 total levels)
- Adding more enemies.
- Adding more weapons.


## Glossary

- C++: A programming language widely used for developing video games.
- Component-Based Architecture: An architecture that allows for modular and scalable code by breaking down the software into reusable and interchangeable parts.
- Integration Testing: Testing that checks the interactions between different software modules or systems.
- PC: Personal Computer, a type of computer designed for personal use.
- System Testing: Testing that verifies the entire software system or application.
- Unit Testing: Testing that checks individual units or components of the software.
- Unreal Engine: A game engine developed by Epic Games, widely used for developing video games.
- Acceptance testing: A software testing technique that involves testing a software system to ensure that it meets the requirements of its stakeholders.
- Agile methodology: An iterative approach to software development that emphasizes flexibility and collaboration between cross-functional teams.
- ClickUP: A project management tool that can be used to track progress, manage tasks, and collaborate with team members.
- Security risks: Potential threats to the security of a software system, including unauthorized access, data breaches, and malware.
- Performance metrics: Quantitative measures of the performance of a software system, including metrics such as framerate, load times, and response times.
- Localization: The process of adapting a software system to support different languages, cultures, and regions.
- Colorblind mode: A feature that adjusts the colors of a software system to make it easier to use for people with color vision deficiencies.
- Descriptive subtitles: Subtitles that include additional information about the audio content of a software system, such as descriptions of sound effects and music.
- Font size options: A feature that allows users to adjust the size of text in a software system to make it easier to read.