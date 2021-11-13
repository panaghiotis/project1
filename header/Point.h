#ifndef PROJECTEMIRI_POINT_H
#define PROJECTEMIRI_POINT_H

#include <iostream>

using namespace std;


struct Point{
    unsigned int hashed_ID;
    bool hashed;
    int pos;
    string id;
    vector<double> *coords;
    Point(vector<double> *coordinates, string id = "", int pos = -1) :
        id(id), coords(coordinates), hashed_ID(-1), hashed(false), pos(pos) {}
    ~Point() {
        delete coords;
    }
    void print() const { cout << "id: " << id << " - 1st coord: " << coords->at(0) << endl; }
    friend ostream& operator<<(ostream& os, const Point& p);
};

#endif
