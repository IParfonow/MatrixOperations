#pragma once
#include <iostream>
#include <string>
#include <vector>

using String = std::string;


struct BackGroundMusic
{
    String Play() const
    {
        return "Playing Background track\n";
    }
    String AbruptStop() const
    {
        return "Stopped Background track\n";
    }
};
struct SoundEffects
{
    String Play() const
    {
        return "Turning on sound effects\n";
    }
    String AdjustVolume() const
    {
        return "Adjusting volume sound effects\n";
    }
};
struct Dialogues
{
    String Play() const
    {
        return "Playing Dialogue\n";
    }
    String AbruptStop() const
    {
        return "Stopped dialogue track\n";
    }
};


class AudioManager
{
public:
    AudioManager()
    {
        background = new BackGroundMusic;
        soundeffects = new SoundEffects;
        dialogue = new Dialogues;
    }
    ~AudioManager()
    {
        delete background;
        delete soundeffects;
        delete dialogue;
    }
    
    String PlayAudio()
    {
        String result;
        result += background->Play();
        result += soundeffects->Play();
        result += background ->AbruptStop();
        result += dialogue->Play();
        return result;
    }
protected:
    BackGroundMusic* background;    
    SoundEffects* soundeffects;    
    Dialogues* dialogue;    
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

class EventManager {
public:
    void attach(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }

    void detach(std::shared_ptr<Observer> observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify() {
        for (const auto& observer : observers) {
            observer->update();
        }
    }
private:
    std::vector<std::shared_ptr<Observer>> observers; // Vector to store attached observers
};

class GameObject
{
public:
    GameObject()
    {
        event = nullptr;
    }
    void SomeChange()
    {
        std::cout << "Game Object has changed the values\n";
        event = new EventManager;
        event->notify();
    }
    bool IsUsed() const{return m_IsUsed;}
    void SetUsedState(bool used)
    {
        m_IsUsed = used;
    }
    void Reset()
    {
        std::cout << "Reseting the gameobject...\n";
    }
private:
    bool m_IsUsed{true};
    EventManager* event;  
};

class Observer1 : public Observer {
public:
    void update() override {
        std::cout << "Observer1: Notified\n";
    }
};

class Observer2 : public Observer {
public:
    void update() override {
        std::cout << "Observer2: Notified\n";
    }
};

class GameManager
{
public:
    inline static GameManager& GetInstance()
    {
        static GameManager instance;
        return instance;
    }
    
    void LevelLoad(const int& level) const
    {
        std::cout << "Loading " << level << " level...\n";
    }
    void GetConfigue()
    {
        std::cout << "Loading " << GameState << '\n';
    }
    static GameObject* Aquire()
    {
        for(GameObject*& it : ObjectPool)
        {
            if(!it->IsUsed())
            {
                std::cout << "Returningn an existing object [POOL]\n";
                it->SetUsedState(true);
                it->Reset();
                return it;
            }
        }
        std::cout << "Creating GameObject [POOL]\n";
        GameObject* it = new GameObject();
        ObjectPool.push_back(it);
        return it;
    }
    static void Release(GameObject* ObjectToRelease)
    {
        for(GameObject*& it : ObjectPool)
        {
            if(it == ObjectToRelease)
            {
                it->SetUsedState(false);
                it->Reset(); //optinally
            }
        }
    }
    
        
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    GameManager(GameManager&&) = delete;
    GameManager& operator=(GameManager&&) = delete;
        
private:
    GameManager(){}
    inline static std::vector<GameObject*> ObjectPool{};
    String GameState = "Game Data";
    static GameManager instance;
};

namespace Game
{
    static void LoadGameState()
    {
        GameManager::GetInstance().GetConfigue();
    }
    inline static void GetLevelLoad(const int& level)
    {
        GameManager::GetInstance().LevelLoad(level);
    };
}