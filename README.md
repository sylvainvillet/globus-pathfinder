# Globus Pathfinder

## Project Summary
Globus Pathfinder is a Qt-based pathfinding tool that loads grid-based maps from JSON, allows interactive editing of the start/target positions, and visualizes the optimal route directly on the map.

This project is part of a take-home assignment.

## Demo
![Demo](images/demo.gif)

## Build instructions
The `src` folder contains a standard Qt 6 / CMake project that can be builded with QtCreator or from command line following these instructions:
https://doc.qt.io/qt-6/cmake-build-on-cmdline.html

## Usage
Use the left click to set starting position and the left click to set target position.
The file menu lets you import or export a map and export a path.

## Project Structure
- src/ — Application source code
    - CMakeLists.txt — CMake project file
    - main.cpp — Qt application entry point
    - resources.qrc - Qt resource file to embed the JSON map
    - assets/
        - sample_map.json — JSON Map loaded at the app startup
    - logic/
        - pathfinder.{cpp,h} — Pathfinding algorithm implementation (A*).
    - models/
        - gamemap.{cpp,h} - Model defining a map with start and target points
        - tile{cpp,h} - Model defining a single tile of the map
    - ui/
        - mainwindow.{cpp,h} — Main application window
        - mapview.{cpp,h} — Widget that renders the grid and path.
        - tileitem{cpp,h} - Single tile UI view with click signals
- .gitignore - Git ignore file
- README.md — Project documentation and usage (this file).

## Design Decisions
### Global architecture
I decided to split my code in 3 subfolders: UI, models and logic. This makes it easy to understand and maintain.
The UI is also splitted in 3 classes: 
- MainWindow for the File menu actions, the status bar and as a map container
- MapView for drawing the map and path
- TileItem for drawing a single tile and sending signals when a tile is clicked

### UI/UX
The UI/UX is pretty straightforward, I decided to interact with left and right clicks on tiles without adding buttons for the different actions.
There's no "start" button either, just click and the path updates automatically.

The goal here was to showcase the shortest path from start to target which is the output of the algorithm. 

Another approach could have been to move the unit to the target position with an animation, which would be how a game would react but then it's harder to see the actual path.

### Pathfinding algorithm
For finding the path to the target position, I thought about 2 approaches:
- "GPS" approach, meaning we know the full map and we can find the quickest path before starting the travel
- "fog" approach, meaning we only know the tiles around us and where the target is. This is often the case in RTS games where the player discovers the map as it progresses
I chose to pick the first approach because we can easily know the shortest path to the target this way.
For the algorithm, A* is a classic for a reason, it's very effective