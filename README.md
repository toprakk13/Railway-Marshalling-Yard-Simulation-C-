# Railway-Marshalling-Yard-Simulation-C-
This project is a C++-based management system that simulates a complex railway logistics hub (Classification Yard). The system processes incoming wagons, assembles trains according to predefined constraints (weight limits, cargo safety, etc.), and dispatches them. The project particularly focuses on custom linked list implementation and high-performance memory management using Move Semantics.

Project Features
	•	Custom Linked List (WagonList):
	•	Wagons are automatically sorted by weight (from heaviest to lightest).
	•	Using Move Semantics, lists containing thousands of wagons can be transferred in O(1) time without memory copying.
	•	Classification Yard:
	•	Wagons are organized in a 2D matrix structure based on their Destination and Cargo Type.
	•	Train Assembly:
	•	Coupler Stress: During train assembly, if the total weight of wagons exceeds the coupler capacity, the train is automatically split.
	•	Hazardous Materials: Special safety procedures are applied to wagons carrying hazardous cargo.

File Structure
	•	RailMarshal.h / .cpp: Main system controller. Processes commands (ADD_WAGON, ASSEMBLE_TRAIN) and manages yards.
	•	ClassificationYard.h / .cpp: Logistics center where wagons are grouped and assembled into trains.
	•	WagonList.h / .cpp: Pointer-based, dynamic, singly linked list implementation.
	•	TrainTrack.h / .cpp: Track where assembled trains wait before departure.
	•	main.cpp: Entry point that reads the input file and starts the simulation.

Supported Commands

The system processes a text-based command file:
	•	ADD_WAGON <id> <cargo> <dest> <weight>: Adds a new wagon to the system and places it in the appropriate yard.
	•	ASSEMBLE_TRAIN <dest>: Assembles wagons into a train for the specified destination.
	•	PRINT_YARD: Lists all wagons currently in the classification yard.
	•	DISPATCH_TRAIN <dest>: Dispatches the prepared train.
	•	AUTO_DISPATCH ON/OFF: Enables or disables automatic train dispatch when a predefined weight threshold is exceeded.

Build & Run

You can compile the project using a standard C++ compiler:
g++ -o HUTrain main.cpp RailMarshal.cpp ClassificationYard.cpp WagonList.cpp Wagon.cpp Train.cpp TrainTrack.cpp Enums.cpp
./HUTrain input.txt
