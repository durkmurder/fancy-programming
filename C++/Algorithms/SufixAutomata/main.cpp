#include <iostream>
#include <string>

#include "sufixautomata.h"

using namespace std;

string test(SufixAutomata *automata, const string &str)
{
    for(int i = 0; i < str.length(); ++i)
        automata->extend(str[i]);
    string s = "abbb";
    int st;
    for(int i = 0; i < s.size(); ++i)
    {
        st = automata->mStates[i].next[s[i]];
        cout << st << endl;
    }
    if(st <= automata->mLast)
    {
        cout << "contains" << endl;
    }
    else
    {
        cout << "isn`t sufix" << endl;
    }
    return string("ololo");
}
int main()
{
    SufixAutomata *automata = new SufixAutomata;
    cout << test(automata, "abbb") << endl;
    return 0;
}

