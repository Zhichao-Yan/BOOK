#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main()
{
    cout << " default bool value:" << true << '-' << false << endl;
    cout << boolalpha;
    cout << " alpha   bool value:" << true << '-' << false << endl;
    cout << noboolalpha;

    cout << "without showbase" << endl;
    cout << "defaut:" << 20 << " " << 1024 << endl;
    cout << "oct:" << oct << 20 << " " << 1024 << endl;
    cout << "hex:" << hex << 20 << " " << 1024 << endl;
    cout << "dec:" << dec << 20 << " " << 1024 << endl;

    cout << showbase;
    cout << "with showbase" << endl;
    cout << "oct:" << oct << 20 << " " << 1024 << endl;
    cout << "hex:" << hex << 20 << " " << 1024 << endl;
    cout << "dec:" << dec << 20 << " " << 1024 << endl;
    cout << noshowbase;
    cout << "showbase cancelled" << endl;

    cout << showbase << uppercase;
    cout << "with showbase uppercase" << endl;
    cout << "oct:" << oct << 20 << " " << 1024 << endl;
    cout << "hex:" << hex << 20 << " " << 1024 << endl;
    cout << "dec:" << dec << 20 << " " << 1024 << endl;
    cout << noshowbase << nouppercase;
    cout << "showbase uppercase cancelled" << endl;
    
    // 设置精度，此时的精度指打印的数字总数，不包含小数点
    cout << "precision:" << cout.precision() << "-value:" << sqrt(2.0) << endl;
    cout.precision(12);
    cout << "precision:" << cout.precision() << "-value:" << sqrt(2.0) << endl;
    cout << setprecision(3);
    cout << "precision:" << cout.precision() << "-value:" << sqrt(2.0) << endl;
    cout << setprecision(6);
    
    cout << "default format:" << 100 * sqrt(2.0) << endl;
    cout << "fixed:" << fixed << 100 * sqrt(2.0) << endl;
    cout << "scientific:" << scientific << 100 * sqrt(2.0) << endl;
    cout << "hexfloat:" << hexfloat << 100 * sqrt(2.0) << endl;
    cout << "default format:" << defaultfloat << 100 * sqrt(2.0) << endl;

    cout << 10.0 << endl;
    cout << showpoint << 10.0 << noshowpoint << endl;

    int i = -16;
    double d = 3.14159;
    // 默认右对齐
    cout << "i: " << setw(12) << i << " next col\n";
    cout << "d: " << setw(12) << d << " next col\n";
    cout << left;//变成左对齐
    cout << "i: " << setw(12) << i << " next col\n";
    cout << "d: " << setw(12) << d << " next col\n";
    cout << right;//变回默认的右对齐
    cout << "i: " << setw(12) << i << " next col\n";
    cout << "d: " << setw(12) << d << " next col\n";
    cout << internal;//设置在符号和值之间留白
    cout << "i: " << setw(12) << i << " next col\n";
    cout << "d: " << setw(12) << d << " next col\n";
    cout << setfill('$');//设置填充空白的字符
    cout << "i: " << setw(12) << i << " next col\n";
    cout << "d: " << setw(12) << d << " next col\n";
    cout << setfill(' ');//设置填充空白的字符
    cout << right;
    cout << "i: " << setw(12) << i << " next col\n";
    cout << "d: " << setw(12) << d << " next col\n";
}