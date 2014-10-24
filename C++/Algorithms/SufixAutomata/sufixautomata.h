#ifndef SUFIXAUTOMATA_H
#define SUFIXAUTOMATA_H

#include <map>

struct state
{
    int len;
    int link;
    std::map<char, int> next;
};

const int MAXLEN = 100000;

class SufixAutomata
{
public:
    SufixAutomata();
    void extend(char c);
    state mStates[MAXLEN];
    int mSize;
    int mLast;

};

#endif // SUFIXAUTOMATA_H
