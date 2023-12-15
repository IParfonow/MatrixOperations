#pragma once

#include <iostream>

class Character
{
public:
    Character() : Expirience(15000), Souls(17.2f){}
    int Expirience;
    float Souls;
};

class QuestAudit
{
public:
    virtual ~QuestAudit() = default;
    virtual std::string Handle(Character* actor) const = 0;
    virtual QuestAudit* SetNext(QuestAudit* handler) = 0;
};

class AbstractQuestAudit : public QuestAudit
{
public:
    AbstractQuestAudit() : next_handler(nullptr){}
    QuestAudit* SetNext(QuestAudit* handler) override
    {
        this->next_handler = handler;
        return this;
    }
    std::string Handle(Character* actor) const override
    {
        if(next_handler)
        {
            return this->next_handler->Handle(actor);
        }
        return {};
    }
protected:
    QuestAudit* next_handler;
};

class StateCheck : public AbstractQuestAudit
{
public:
    std::string Handle(Character* actor) const override
    {
        std::cout << "Checking state progress...\n";
        if(actor->Expirience >= 15000)
        {
            if(next_handler)
                return next_handler->Handle(actor);
            else
            {
                return "Passed check";
            }
        }
        else
        {
            return "Not enough exp\n";
        }
    }
};

class SoulsCheck : public AbstractQuestAudit
{
public:
    std::string Handle(Character* actor) const override
    {
        std::cout << "Checking Souls...\n";
        if(actor->Souls >= 15.0f)
        {
            if(next_handler)
                return next_handler->Handle(actor);
            else
            {
                return "Passed check";
            }
        }
        else
        {
            return "Not enought souls\n";
        }
    }
};

