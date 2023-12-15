#pragma once

#include <iostream>

class GamePills
{
public:
    virtual ~GamePills(){}
    virtual void Use() = 0;
};

class CharacterPills : public GamePills
{
public:
    void Use() override
    {
        std::cout << "\nPill has been eaten\n";
    }
};

class PillsEffect : public GamePills
{
public:
    PillsEffect(GamePills* pill)
        : effect(pill){}
    void Use() override
    {
        this->effect->Use();
    }
protected:
    GamePills* effect;
};

class GreenPill : public PillsEffect
{
public:
    GreenPill(GamePills* pill) : PillsEffect(pill){}
    void Use() override
    {
        PillsEffect::Use();
        std::cout << "+10 to stamina\n";
    }
};

class RedPill : public PillsEffect
{
public:
    RedPill(GamePills* Pill) : PillsEffect(Pill){}
    void Use()
    {
        std::cout << "+5 to power\n";
    }
};
