#include <iostream>
#include "Matrix.h"
#include "FileLoader.h"
#include <string>
#include "Sorter.h"
#include "Iterators.h"
#include "Patterns.h"
#include "Decorator.h"
#include "ChainOfResponsibility.h"

template<typename T>
inline std::ostream& operator<<(std::ostream& output,const DynamicArray<matrix<T>>& data)
{
    for(unsigned int j = 0; j < data.size();j++)
    {
        data[j].convert_to_string();
        output << ',' << std::endl;
    }
    return output;
}
template <typename T>
inline std::ostream& operator<<(std::ostream& output,const DynamicArray<T>& data)
{
    output << '[';
    for(unsigned int j = 0; j < data.size();j++)
    {
        output << data[j] << ',';
    }
    output << ']';
    return output;
}

int main()
{
    /*//Singleton
    using namespace Game;
    Game::GetLevelLoad(2);
    Game::LoadGameState();
    
    //Facade
    AudioManager Scene1;
    std::cout << Scene1.PlayAudio();
    
    //Observer
    std::shared_ptr<EventManager> gameobject = std::make_shared<EventManager>();
    std::shared_ptr<Observer> observerA = std::make_shared<Observer1>();
    std::shared_ptr<Observer> observerB = std::make_shared<Observer2>();
    gameobject->attach(observerA);
    gameobject->attach(observerB);*/
    
    

    //Object Pool
    GameObject* score = new GameObject;
    score->SomeChange();
    score = GameManager::Aquire(); //adding to the pool
    score->SomeChange();

    GameManager::Release(score); //Reseting(optionally) & adding abbility to re-use it

    GameObject* ReleaseScore = GameManager::Aquire(); // Returning already existing object from pool as "new"
    ReleaseScore->SomeChange();
    
    //Decorator
    GamePills* Pill = new CharacterPills;
    Pill->Use(); // nothing happened
    GamePills* MilkyWayPill = new GreenPill(Pill);
    MilkyWayPill->Use(); // we've got +10 to stamina

    //Chain of Responsibility
    StateCheck quest1_check; //Exp check
    SoulsCheck quest1_souls_check; //Souls check
    quest1_check.SetNext(&quest1_souls_check); //Configur chain
    Character warrior; //actor
    std::cout << quest1_check.Handle(&warrior); //quest check

    
    delete ReleaseScore;
    delete Pill;
    delete MilkyWayPill;
    return 0; 
}












