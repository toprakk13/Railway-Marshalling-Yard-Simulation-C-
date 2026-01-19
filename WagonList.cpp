#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    // TODO: Delete all Wagon objects in this list and reset pointers.
    Wagon *p=front;
    if(p==nullptr){
        return;
    }
    while(p!=nullptr){
        Wagon *temp=p;
        p=p->getNext();
        delete temp;
        temp=nullptr;
    }
    front=nullptr;
    rear=nullptr;

    totalWeight=0;
   

}

WagonList::WagonList(WagonList &&other) noexcept
{
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state.
    front = other.front;
    rear = other.rear;
    totalWeight = other.totalWeight;
    other.front=nullptr;
    other.rear=nullptr;
    other.totalWeight=0;
}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    // Operation version of the move constructor.
    // TODO: Implement it.
    
    if(this==&other){return *this;}
    else{
        clear();
        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
        other.front = nullptr;
        other.rear = nullptr;
        other.totalWeight = 0;
        return *this;
    }
}

Wagon *WagonList::findById(int id)
{
    // TODO: Find and return the Wagon with given ID.
    Wagon *p=front;
    while(p!=nullptr){
        if(p->getID()==id){
            return p;
            
        }
        p=p->getNext();
    }
    // Return nullptr if not found.
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight
    w->setNext(nullptr);
    w->setPrev(nullptr);
    if(front==nullptr){
        front=w;
        rear=w;
    }
    else {
        rear->setNext(w);
        w->setPrev(rear);
        rear=w;
    }
    totalWeight+=w->getWeight();
    
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{   if(front==nullptr){
    return true;
}
    // TODO: Return true if the list has no wagons.
    return false;
}

void WagonList::insertSorted(Wagon *wagon)
{
    // TODO: Insert wagon into this list in descending order of weight.
    Wagon *p=front;
    wagon->setNext(nullptr);
    wagon->setPrev(nullptr);
    if(front==nullptr){
        front=wagon;
        rear=wagon;
        
    }
    else if(front->getWeight()<wagon->getWeight()){
        wagon->setNext(front);
        front->setPrev(wagon);
        
        front=wagon;
        
    }
    else{
        while (p->getNext() != nullptr && p->getNext()->getWeight() > wagon->getWeight()) {
            p = p->getNext();   
        }
        wagon->setNext(p->getNext());
        wagon->setPrev(p);
        p->setNext(wagon);
        if (wagon->getNext() != nullptr) {
            wagon->getNext()->setPrev(wagon);
        } else {
            
            rear = wagon;
        }
        
        

   
    }
totalWeight += wagon->getWeight();
}
    


void WagonList::appendList(WagonList &&other)
{
   // TODO: Append another WagonList to this one (merge them).
   // Use move semantics to avoid deep copies. (Double && at the parameter already makes it so)
   // 'other' should end up empty after this operation
   // At merge lists (blocks) will be protected 
   // But the one with heavier wagon at the front will be before the other list
   if (other.isEmpty()) {return;}
   if (this->isEmpty()) {
    *this = std::move(other);
       return;
 }
    else{ if(other.front->getWeight()<front->getWeight())
    {rear->setNext(other.front);
    other.front->setPrev(rear);
    rear=other.rear;
    }
        else{
            other.rear->setNext(this->front);
            this->front->setPrev(other.rear);
            this->front=other.front;
            
        }
        totalWeight+=other.totalWeight;
        other.rear=nullptr;
        other.front=nullptr;
        other.totalWeight=0;
        
    }


}

Wagon *WagonList::detachById(int id)
{
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.
    Wagon* x=findById(id);
    if(x==nullptr){return nullptr;}
    else{
        if(x==rear&& x==front){front=nullptr;
        rear=nullptr;

        }
        else if(x->getPrev()!=nullptr && x->getNext()!=nullptr){
            x->getPrev()->setNext(x->getNext());
            x->getNext()->setPrev(x->getPrev());
        }
        else if(x->getPrev()==nullptr && x->getNext()!=nullptr){
            front=x->getNext();
            x->getNext()->setPrev(nullptr);
            
            
        }
        else{
            rear=x->getPrev();
            x->getPrev()->setNext(nullptr);
            
            
            
        }
        x->setPrev(nullptr);
        x->setNext(nullptr);
        totalWeight-=x->getWeight();
        std::cout << "Wagon " << x->getID() << " detached from Wagon List. " << std::endl;
        
        return x;
        
        
        
    }
}


WagonList WagonList::splitAtById(int id)
{   Wagon* n=findById(id);
    WagonList newList; // return-by-value (will be moved automatically)
    if(n==nullptr){
        return newList;
    }
    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.
    else if(n==front){
        newList.front=front;
        newList.rear=rear;
        newList.totalWeight=totalWeight;
        front=nullptr;
        rear=nullptr;
        totalWeight=0;

    }
    else {
        Wagon* newRear = n->getPrev();
        newList.rear=rear;
        newRear->setNext(nullptr);
        
        rear=newRear;
        n->setPrev(nullptr);
        newList.front=n;
        
        Wagon *p=n;
        while(p!=nullptr){
            newList.totalWeight+=p->getWeight();
            p=p->getNext();
        }
        totalWeight=totalWeight-newList.totalWeight;
        


    }
    
    
    



    return newList; // moved, not copied
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
