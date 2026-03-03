#pragma once
#include <string>
#include <vector>
#include <queue>
#include <variant>

struct Event
{
    std::string name; // am setat ca numele eventului sa fie acelasi cu functia apelata de lua

    using Arg = std::variant<int, float, std::string>; 
    std::vector<Arg> args; // lista de argumente (care pot fi int, float sau string) trimise catre lua
};

class EventBus
{
public:
    static void Push(const Event& e)
    {
        GetQueue().push(e);
    }

    static void DispatchAll(void (*handler)(const Event&))
    {
        auto& q = GetQueue();
        while (!q.empty())
        {
            Event e = q.front();
            q.pop();
            handler(e);
        }
    }

    static void Clear()
    {
        auto& q = GetQueue();
        while (!q.empty()) q.pop();
    }

private:
    static std::queue<Event>& GetQueue()
    {
        static std::queue<Event> q;
        return q;
    }
};
