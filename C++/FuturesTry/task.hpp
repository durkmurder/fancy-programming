#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <vector>
#include <utility>


class Task
{
public:
    Task() { }
    Task( std::string owner, std::string title, std::string description) :
        m_owner( std::move( owner ) ),
        m_title( std::move( title ) ),
        m_description( std::move( description ) )
    { }

    inline std::string getOwner() const
    { return m_owner; }
    inline std::string getTitle() const
    { return m_title; }
    inline std::string getDescription() const
    { return m_description; }

    friend std::istream & operator>>(std::istream &stream, Task &obj)
    {
        stream >> obj.m_owner >> obj.m_title >> obj.m_description;
        return stream;
    }


private:
    std::string m_owner;
    std::string m_title;
    std::string m_description;
};

typedef std::vector< Task > TasksVector;

#endif // TASK_HPP

