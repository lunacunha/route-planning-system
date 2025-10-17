# Route Planning System

Course project for Algorithms and Data Structures (L.EIC - FEUP, 2023/2024)

## Project

A C++ route planning application that supports both driving-only and combined driving-walking routes with customizable constraints.

## Features

### Driving-Only Routes
- **Best Route**: Finds the fastest driving route and an independent alternative route
- **Restricted Route**: Calculates routes while avoiding specific nodes/segments or requiring passage through mandatory nodes

### Driving & Walking Routes
- **Combined Routes**: Finds optimal routes that combine driving to a parking location and walking to the destination
- **Walking Time Constraints**: Respects maximum walking time limits
- **Approximate Solutions**: Suggests alternative routes when no exact match is found

## Datasets

The application supports three datasets:
1. **Small Dataset** - For testing and demonstration
2. **Large Dataset** - Full-scale routing network
3. **PDF Dataset** - Additional test cases

## Input Format

### Driving Mode
```
Source: <location_id>
Destination: <location_id>
AvoidNodes: <node1>,<node2>,...
AvoidSegments: (id1,id2),(id3,id4),...
IncludeNode: <location_id>
```

### Driving-Walking Mode
```
Source: <location_id>
Destination: <location_id>
MaxWalkTime: <minutes>
AvoidNodes: <node1>,<node2>,...
AvoidSegments: (id1,id2),(id3,id4),...
```

## Output Format

### Driving Routes
```
BestDrivingRoute: <id1>,<id2>,...,<idN> (time)
AlternativeDrivingRoute: <id1>,<id2>,...,<idN> (time)
```

### Driving-Walking Routes
```
DrivingRoute: <id1>,<id2>,...,<parking> (time)
ParkingNode: <parking_id>
WalkingRoute: <parking>,<id2>,...,<destination> (time)
TotalTime: (total_time)
```

## File Structure
```
├── Graph.h                          # Graph data structure
├── Location.cpp/h                   # Location node representation
├── RoutePlanning.cpp/h              # Driving route algorithms
├── DrivingWalkingRoutePlanning.cpp/h # Combined route algorithms
├── GraphInterface.cpp/h             # Data loading interface
├── InputValidation.cpp/h            # Input parsing and validation
├── Menu.cpp/h                       # User interface
└── main.cpp                         # Entry point
```

## Building and Running
```bash
# Compile
g++ -std=c++17 *.cpp -o route_planner

# Run
./route_planner
```

## Algorithm Details

- **Dijkstra's Algorithm**: Used for shortest path calculation
- **Edge Weights**: Separate driving and walking time weights
- **Constraint Handling**: Temporary graph modification for avoid constraints
- **Parking Optimization**: Evaluates all parking nodes to minimize total time

## Input/Output Files

- `input.txt` - Stores the last query parameters
- `output.txt` - Contains the computed route results

## Error Handling

The system validates:
- Location existence in the graph
- Parking availability at source/destination (not allowed)
- Adjacent source/destination (not allowed for combined routes)
- Route feasibility under given constraints

## Notes

- All location IDs are case-insensitive and automatically converted to uppercase
- Times are rounded to the nearest integer for display
- When no exact solution exists, the system automatically searches for approximate routes with relaxed constraints
