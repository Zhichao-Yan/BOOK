#include <iostream>
#include "Sales_item.h"

int main()
{
    Sales_item currVal, val;
    if(std::cin >> currVal)
    {
        int cnt = 1;
        while(std::cin >> val)
        {
            if(val.same_isbn(currVal))
            {
                ++cnt;
            }else{
                std::cout << currVal.isbn << " occurs " << cnt << " times" << std::endl;
                currVal = val;
                cnt = 1;
            }
        }
        std::cout << currVal.isbn << " occurs " << cnt << " times" << std::endl;
    }
    return 0;
}