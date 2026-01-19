#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
        
        Destination dest = static_cast<Destination>(i);
        
        departureYard[i]=TrainTrack(dest);
    }
   
    TrainTrack::autoDispatch = false;
}
    
    
   
    
    


RailMarshal::~RailMarshal()
{ 
    // TODO: Cleanup remaining trains, prevent memory leaks
    
    for(int i=0;i<NUM_DESTINATIONS_INT;++i){
      while(!departureYard[i].isEmpty()){
        departureYard[i].departTrain();
      }
    }
    
    
    
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{   
    // TODO: Parse user commands from input lines.
    std::stringstream ss(line);
    std::string command;
    ss >> command;
    if (command.empty()) {return;}
    // if ADD_WAGON
    // Use: std::cout << "Error: Invalid ADD_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << *w << " added to yard." << std::endl;
    if (command == "ADD_WAGON") {
        int id, weight, maxCouplerLoad;
        std::string cargoStr, destStr;
        
        if (!(ss >> id >> cargoStr >> destStr >> weight >> maxCouplerLoad)) {
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }
        CargoType cargo = parseCargo(cargoStr);
        Destination dest = parseDestination(destStr);
        
        
        if (cargo == CargoType::OTHER || dest == Destination::OTHERS || weight <= 0 || maxCouplerLoad <= 0) {
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }

        Wagon *w = new Wagon(id, cargo, dest, weight, maxCouplerLoad);
        
        
        classificationYard.insertWagon(w); 
        
        std::cout << "Wagon " << *w << " added to yard." << std::endl;

    }
    else if (command == "REMOVE_WAGON") {
        int id;
        if (!(ss >> id)) {
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
        }

        
        Wagon* removedWagon = nullptr;
        
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
            for (int j = 0; j < NUM_CARGOTYPES_INT; ++j) {
                removedWagon = classificationYard.getBlockTrain(i, j).detachById(id);
                if (removedWagon) break;
            }
            if (removedWagon) break;
        }

        if (removedWagon == nullptr) {
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
        } else {
            std::cout << "Wagon " << id << " removed." << std::endl;
            delete removedWagon; 
        }

    }
    else if (command == "ASSEMBLE_TRAIN") {
        std::string destStr;
        if (!(ss >> destStr)) {
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }
        
        Destination dest = parseDestination(destStr);
        if (dest == Destination::OTHERS) {
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }
        
        TrainTrack& track = departureYard[static_cast<int>(dest)];
        
        
        WagonList* blocks = classificationYard.getBlocksFor(dest);
        bool hasWagons = false;
        for(int i = 0; i < NUM_CARGOTYPES_INT; ++i) {
            if (!blocks[i].isEmpty()) {
                hasWagons = true;
                break;
            }
        }
        if (!hasWagons) {
            std::cout << "No wagons to assemble for " << destStr << std::endl;
            return;
        }

        
        std::string trainName = track.generateTrainName(); 
        
        Train *newTrain = classificationYard.assembleTrain(dest, trainName);

        if (newTrain) {
            
            std::cout << "Train " << newTrain->getName() << " assembled with " 
                      << newTrain->getWagons() << " wagons." << std::endl;
            
            
            Train* frontTrain = newTrain;
            int splitCount = 1;

            while (true) {
                Train* splitTrain = frontTrain->verifyCouplersAndSplit(splitCount);
                
                if (splitTrain == nullptr) {
                    
                    track.addTrain(frontTrain);
                    break;
                } else {
                    
                    track.addTrain(splitTrain);
                    
                    splitCount++;
                }
            }
        }

    }
    else if (command == "DISPATCH_TRAIN") {
        std::string destStr;
        if (!(ss >> destStr)) {
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }
        
        Destination dest = parseDestination(destStr);
        if (dest == Destination::OTHERS) {
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }
        
        
        dispatchFromTrack(dest);
        
    }
    else if (command == "PRINT_YARD") {
        
        
        if (ss.peek() != EOF) {
             std::cout << "Error: Invalid PRINT_YARD parameters.\n";
             return;
        }

        std::cout << "--- classification Yard ---\n";
        classificationYard.print();

    }
    else if (command == "PRINT_TRACK") {
        std::string destStr;
        if (!(ss >> destStr)) {
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        
        Destination dest = parseDestination(destStr);
        if (dest == Destination::OTHERS) {
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        
        departureYard[static_cast<int>(dest)].printTrack();

    }
    else if (command == "AUTO_DISPATCH") {
        std::string statusStr;
        if (!(ss >> statusStr)) {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            return;
        }
        
        std::transform(statusStr.begin(), statusStr.end(), statusStr.begin(), ::toupper);
        
        
        if (statusStr == "ON") {
            TrainTrack::autoDispatch = true;
            std::cout << "Auto dispatch enabled\n";
        } else if (statusStr == "OFF") {
            TrainTrack::autoDispatch = false;
            std::cout << "Auto dispatch disabled\n";
        } else {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }

    } else if (command == "CLEAR") {
        
        if (ss.peek() != EOF) {
             
             std::cout << "Error: Invalid CLEAR parameters.\n";
             return;
        }
        
        
        
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
            TrainTrack& currentTrack = departureYard[i];
            
            
            while (!currentTrack.isEmpty()) {
                Train* t = currentTrack.departTrain();
                if (t != nullptr) {
                    delete t;
                    t = nullptr;
                }
                
            }
            classificationYard.clear();
            
            
            currentTrack.setTrainCounter(0); 
        }
        
        
        TrainTrack::autoDispatch = false; 
        
        std::cout << "System cleared." << std::endl;

    } else {
        
        std::cout << "Error: Unknown command '" << command << "'" << std::endl;
    }

    // if REMOVE_WAGON
    // Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;

    // if ASSEMBLE_TRAIN
    //  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;

    // if DISPATCH_TRAIN
    //  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;

    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";

    // if PRINT_TRACK
    //  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";

    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"

    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;

    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    // TODO: Dispatch the next train (frontmost) from the specified track.
    // std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
    /*std::cout << "Train " << t->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";
     */
    int destIndex = static_cast<int>(track);
    
    TrainTrack& trackToDispatch = departureYard[destIndex]; 
    
    
    if (trackToDispatch.isEmpty()) {
        
        std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n"; 
        return;
    }
    Train* t = trackToDispatch.departTrain(); 
    
    std::cout << "Dispatching " << t->getName() 
              << " (" << t->getTotalWeight() << " tons)." << std::endl;
    
    delete t;
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(NUM_DESTINATIONS_INT); ++i)
    {
        departureYard[i].printTrack();
    }
}
