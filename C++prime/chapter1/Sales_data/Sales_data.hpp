#pragma once

#include <string>
#include <iostream>
#include <istream>


class Sales_data{
    friend class std::hash<Sales_data>;
    friend std::istream &read(std::istream &is,Sales_data &item);
    friend std::ostream &print(std::ostream &os,const Sales_data &item);
    friend Sales_data add(const Sales_data &lhs,const Sales_data &rhs);
    friend std::ostream &operator<<(std::ostream &os,const Sales_data &item);
    friend std::istream &operator>>(std::istream &is,Sales_data &item);
    friend bool operator==(const Sales_data &lhs,const Sales_data &rhs);
    friend bool operator!=(const Sales_data &lhs,const Sales_data &rhs);
public:
    Sales_data(std::string s = ""):bookNo(s){}
    Sales_data(const std::string &s):bookNo(s){}
    Sales_data(const std::string &s, unsigned n,double p):
    bookNo(s),units_sold(n),revenue(p * n){}
    Sales_data(std::istream &is);
    Sales_data &operator+=(const Sales_data &rhs);
    std::string isbn() const { return bookNo;}
    Sales_data &combine(const Sales_data &rhs);
    double avg_price() const;
private:
    std::string bookNo;
    unsigned units_sold = 0;
    double  revenue = 0.0;
};

std::istream &read(std::istream &is,Sales_data &item);
std::ostream &print(std::ostream &os,const Sales_data &item);
Sales_data add(const Sales_data &lhs,const Sales_data &rhs);
std::ostream &operator<<(std::ostream &os,const Sales_data &item);
std::istream &operator>>(std::istream &is,Sales_data &item);
bool operator==(const Sales_data &lhs,const Sales_data &rhs);
bool operator!=(const Sales_data &lhs,const Sales_data &rhs);


namespace std
{
template<>
class hash<Sales_data>
{
public:
    typedef size_t return_type;
    typedef Sales_data argument_type;
    size_t operator()(const Sales_data &s) const;
};
size_t hash<Sales_data>::operator()(const Sales_data &s) const
{
    return hash<string>()(s.bookNo) ^ hash<unsigned>()(s.units_sold) ^ hash<double>()(s.revenue);
}
}