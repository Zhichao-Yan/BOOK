#include <iostream>
#include "Sales_item.h"

int main()
{
    Sales_item total, book;
    if(std::cin >> total)
    {
        while(std::cin >> book)
        {
            if(total.same_isbn(book))
            {
                total += book;
            }else{
                std::cout << total << std::endl;
                total = book;
            }
        }
        std::cout << total << std::endl;
    }
    return 0;
}