//
//  main.cpp
//  ThinkCell
//
//  Created by Micheli Knechtel on 05/11/2017.
//  Copyright © 2017 Micheli Knechtel. All rights reserved.
//

#include <map>
#include <iostream>
#include <assert.h>

using namespace std;

template<class K, class V> class interval_map
{
    friend void IntervalMapTest();
    
private:
    std::map<K, V> m_map;
    
public:
    
    interval_map(V const& val)
    {
        m_map.insert(m_map.begin(), std::make_pair(std::numeric_limits<K>::lowest(), val));
    };
    
    void assign(K const& keyBegin, K const& keyEnd, const V& val)
    {
        if (!(keyBegin < keyEnd))
            return;
        
        typename std::map<K, V>::iterator itBegin       = m_map.lower_bound(keyBegin);
        typename std::map<K, V>::iterator itEnd         = m_map.upper_bound(keyEnd);
        typename std::map<K, V>::iterator itNext        = itBegin;
        typename std::map<K, V>::iterator itPenultimate = itEnd;
        typename std::map<K, V>::iterator itPrevious;
        
        V currentValue;
        bool flag = true;
        if(itBegin != m_map.end() && itBegin->first == keyBegin)
        {
            currentValue = itBegin->second;
            itBegin->second = val;
            ++itNext;
        }
        else
        {
            itPrevious = itBegin;
            --itPrevious;
            currentValue = itPrevious->second;
            
            if (itPrevious->second != val)
            {
                typename std::map<K, V>::iterator it = m_map.insert(itBegin, std::make_pair(keyBegin, val));
                itBegin = it;
                itNext = ++it;
            }
        }
        
        if (itEnd == m_map.end() || itEnd->first != keyEnd)
        {
            itPrevious = itEnd;
            --itPrevious;
            
            if (itPrevious != itBegin)
                currentValue = itPrevious->second;
            
            if (currentValue != val)
            {
                typename std::map<K, V>::iterator it = m_map.insert(itEnd, std::make_pair(keyEnd, currentValue));
                itPenultimate = it;
            
                
                if (it!= m_map.end() && it->second == currentValue)
                {
                    if (itBegin!= m_map.end() && itEnd!= m_map.end())
                    {
                        ++itBegin;
                        m_map.erase(itBegin,itEnd);
                        
                        if(itEnd->first > keyEnd+1 )
                        {
                           throw 1;
                        }
                        
                    }
                    else
                        m_map.erase(it);
                    
                    flag = false;
                }
            }
        }
        
        if (flag && itNext != m_map.end() && itPenultimate != m_map.end() && itNext->first < itPenultimate->first)
            m_map.erase(itNext, itPenultimate);
    }
    
    V const& operator[](K const& key) const
    {
        return (--m_map.upper_bound(key))->second;
    }
};

void print(typename std::map<unsigned int, unsigned int>::iterator itBegin, typename std::map<unsigned int, unsigned int>::iterator itEnd)
{

    for (itBegin; itBegin != itEnd; ++itBegin)
    {
        cout <<"key = "<< itBegin->first << " value = " << itBegin->second<< endl;
    }
}

void IntervalMapTest()
{
    interval_map<unsigned int, unsigned int> myMap(10);
    
    try {
        cout <<"interval[0, 0) value (10)" <<endl;
        print(myMap.m_map.begin(), myMap.m_map.end());
        
       
        cout <<endl<<"interval[3, 6) value (20)" <<endl;
        myMap.assign(3, 6, 20);
        print(myMap.m_map.begin(), myMap.m_map.end());
        
        cout <<endl<<"interval[2, 5) value (30)" <<endl;
        myMap.assign(2, 5, 30);
        print(myMap.m_map.begin(), myMap.m_map.end());
        
        cout <<endl<<"interval[5, 7) value (60)" <<endl;
        myMap.assign(5, 7, 60);
        print(myMap.m_map.begin(), myMap.m_map.end());
        
        cout <<endl<<"interval[0, 4) value (10)" <<endl;
        myMap.assign(0, 4, 10);
        print(myMap.m_map.begin(), myMap.m_map.end());
        
        cout <<endl<<"interval[0, 3) value (150)" <<endl;
        myMap.assign(0, 3, 150);
        print(myMap.m_map.begin(), myMap.m_map.end());
    }
    catch (int exception)
    {
        if (exception ==1 )
        cout << endl <<  "There is a gap in the specified interval !!!";
    }
}

int main(int argc, char* argv[])
{
    IntervalMapTest();
    return 0;
}
