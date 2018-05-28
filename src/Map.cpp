#include <iostream>
#include <map>
#include <iterator>
using namespace std;

bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp {
    bool operator() (const char& lhs, const char& rhs) const
    {return lhs<rhs;}
};

int main ()
{
    map <char,int> first;

    first['a']=10;
    first['b']=30;
    first['c']=50;
    first['d']=70;

    //Printing Map First
    map <char, int> :: iterator itr;
    for (itr = first.begin(); itr != first.end(); ++itr)
    {
        cout  <<  '\t' << itr->first
              <<  '\t' << itr->second << '\n';
    }
    cout << endl;

    std::map<char,int> second (first.begin(),first.end());

    //Printing Map Second
    for (itr = second.begin(); itr != second.end(); ++itr)
    {
        cout  <<  '\t' << itr->first
              <<  '\t' << itr->second << '\n';
    }
    cout << endl;

    std::map<char,int> third (second);

    //Printing Map Third
    for (itr = third.begin(); itr != third.end(); ++itr)
    {
        cout  <<  '\t' << itr->first
              <<  '\t' << itr->second << '\n';
    }
    cout << endl;

    return 0;
}

