#include "TrainTrack.h"
#include <iostream>

bool TrainTrack::autoDispatch = false;

TrainTrack::TrainTrack()
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(Destination::OTHERS),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::TrainTrack(Destination _dest)
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(_dest),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::~TrainTrack()
{
    // TODO: If track is deconstucting, 
    // depart all of the trains
    // Handle pointers as necessary
    Train *p=firstLocomotive;
    while(p!=nullptr){
        Train* q=p->getNext();
        departTrain();
        delete p;
        p=q;
    }
    firstLocomotive=nullptr;
    lastLocomotive=nullptr;
    totalWeight=0;
}

// Given to you ready
std::string TrainTrack::generateTrainName()
{
    ++trainCounter;
    return "Train_" + destinationToString(destination) + "_" + std::to_string(trainCounter);
}

void TrainTrack::addTrain(Train *train)
{
    // TODO: Add a train to the end (rear) of this track
    train->setNext(nullptr);
    
    if(firstLocomotive==nullptr){
        firstLocomotive=train;
        lastLocomotive=train;
    }
    else{
        lastLocomotive->setNext(train);
        
        lastLocomotive=train;
        
    }
    totalWeight+=train->getTotalWeight();
    if(autoDispatch){
        while(totalWeight>AUTO_DISPATCH_LIMIT){
            Train* departed=departTrain();
            if (departed == nullptr) {break; }// Önlem
            std::cout << "Auto-dispatch: departing " << departed->getName() << " to make room.\n";
            delete departed;
        }
    }
    // TODO: Hadle Auto-dispatch rule:
    //   If adding this train causes AUTO_DISPATCH_LIMIT to overflow
    //   and auto-dispatch is enabled, repeatedly dispatch (depart) trains
    //   from the front until there is enough capacity.
    //      use: std::cout << "Auto-dispatch: departing " << departed->getName() << " to make room.\n";

}

Train *TrainTrack::departTrain()
{
    // TODO: Remove the first train (front of the track) and return it.
    // use: std::cout << "Train " << removed->name << " departed from Track " << destinationToString(destination) << "." << std::endl;
    if(firstLocomotive==nullptr)
    {return nullptr;}
    Train* removeTrain=firstLocomotive;
    totalWeight-=removeTrain->getTotalWeight();
    firstLocomotive=removeTrain->getNext();
    if(firstLocomotive==nullptr){
        lastLocomotive=nullptr;
    }
    
    removeTrain->setNext(nullptr);
    
    std::cout << "Train " << removeTrain->getName() << " departed from Track " << destinationToString(destination) << "." << std::endl;
    return removeTrain;
}

bool TrainTrack::isEmpty() const
{
    // TODO: Return true if there are no trains on this track.
    if(firstLocomotive==nullptr)
{
    return true;
}    return false;
}


Train *TrainTrack::findTrain(const std::string &name) const
{
    // TODO: Search for a train by name.
    // Return pointer to train if found, nullptr otherwise.
    Train *p=firstLocomotive;
    while(p!=nullptr){
        if(p->getName()==name){break;}
     else{p=p->getNext();}   
    }
    if(p==nullptr)
    {return nullptr;}
    else{
        return p;
    }
}

// Given to you ready
void TrainTrack::printTrack() const
{

    if (isEmpty())
        return;

    Train *current = firstLocomotive;

    std::cout << "[Track " << static_cast<int>(firstLocomotive->destination) << "] ";
    while (current)
    {
        std::cout << current->getName() << "(" << current->getTotalWeight() << "ton)-" << current->wagons << " -> ";
        current = current->getNext();
    }
    std::cout << std::endl;
    return;
}
void TrainTrack::setTrainCounter(int newCounter) 
{ 
    this->trainCounter = newCounter; 
}