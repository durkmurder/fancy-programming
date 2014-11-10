#include "sufixautomata.h"

SufixAutomata::SufixAutomata()
{
    mSize = mLast = 0;
    mStates[0].len = 0;
    mStates[0].link = -1;
    ++mSize;
}

void SufixAutomata::extend(char c)
{
    int cur = mSize++;
    mStates[cur].len = mStates[mLast].len + 1;
    int p;
    for (p = mLast; p != -1 && !mStates[p].next.count(c); p = mStates[p].link)
        mStates[p].next[c] = cur;
    if (p == -1)
        mStates[cur].link = 0;
    else
    {
        int q = mStates[p].next[c];
        if (mStates[p].len + 1 == mStates[q].len)
            mStates[cur].link = q;
        else
        {
            int clone = mSize++;
            mStates[clone].len = mStates[p].len + 1;
            mStates[clone].next = mStates[q].next;
            mStates[clone].link = mStates[q].link;
            for (; p != -1 && mStates[p].next[c] == q; p = mStates[p].link)
                mStates[p].next[c] = clone;
            mStates[q].link = mStates[cur].link = clone;
        }
    }
    mLast = cur;
}
