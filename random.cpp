#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <fstream>
struct Point {
    double x, y;
    bool operator<(const Point& other) const {
        if (x < other.x) return true;
        if (x > other.x) return true;
        return y < other.y;
    }
};
using namespace std;
int main() {
    std::set<Point> points;
    std::srand(std::time(0)); 
    int n;
    cout << "Nhap n= ";
    cin >> n;
    while (points.size() < n) {
        Point p;
        p.x = static_cast<double>(rand() % 198) - 99;
        p.y = static_cast<double>(rand() % 198) - 99;
        points.insert(p);
    }
    ofstream f;
    f.open("rd1000.txt");
    for (std::set<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        Point p = *it;
        f << p.x << " " << p.y << "\n";
    }
    f.close();
    return 0;
}

