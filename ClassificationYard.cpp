#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{   Destination dest = w->getDestination();
    CargoType cargo = w->getCargoType();
    int destIndex = static_cast<int>(dest);
    int cargoIndex = static_cast<int>(cargo);
    
    WagonList &targetList = blockTrains[destIndex][cargoIndex];
    targetList.insertSorted(w);
    
    
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    // TODO: Collect wagons of the same destination and assemble them into a single Train.

    /**
     * - Blocks of the same cargo type must remain grouped together.
     * - These groups must be appended to the train in descending order
     *   based on their heaviest wagon.
     * - Hazardous cargo (e.g., OIL) must always be placed at the very end of the train,
     *   and only one hazardous block can be included per train.*/


    Train* newTrain = new Train(trainName, dest);
    WagonList hazardousBlock;
    int destIndex = static_cast<int>(dest);
    for(int i=0;i<NUM_DESTINATIONS_INT;++i){
        if (!blockTrains[destIndex][i].isEmpty()) {
            CargoType type = static_cast<CargoType>(i);
            if (type == CargoType::OIL||type == CargoType::HAZARDOUS) {
                if(hazardousBlock.isEmpty()){
                hazardousBlock = std::move(blockTrains[destIndex][i]);}
                else{continue;}
            } 
            else {
                
                newTrain->getWagons().appendList(std::move(blockTrains[destIndex][i]));
            }
            
        }
    }
   
    if (!hazardousBlock.isEmpty()) {
        newTrain->getWagons().getRear()->setNext(hazardousBlock.getFront());
        hazardousBlock.getFront()->setPrev(newTrain->getWagons().getRear());
        newTrain->getWagons().setRear(hazardousBlock.getRear());
    }

    return newTrain;

    
}

bool ClassificationYard::isEmpty() const
{
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */
    for(int i=0;i<NUM_DESTINATIONS_INT;++i){
        for(int j=0;j<NUM_CARGOTYPES_INT;++j){
            if(!blockTrains[i][j].isEmpty()){
                return false;
            }
        }
    }

    return true;
}

void ClassificationYard::clear()
{
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */
    for(int i=0;i<NUM_DESTINATIONS_INT;++i){
        for(int j=0;j<NUM_CARGOTYPES_INT;++j){
            blockTrains[i][j].clear();
        }
    }
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}