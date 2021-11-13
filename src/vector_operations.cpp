#include <cmath>
#include <vector>
#include "../header/vector_operations.h"

using namespace std;


/***
 * Wrapper to use for distances elsewhere.
 * If we want to change the distance used we can do this here.
 */
long double used_distance(const Point &p1, const Point &p2) {
    return L2_distance(p1, p2);   // use Euclidean distance
}

long double L2_distance(const Point &p1, const Point &p2) {
    long double sum = 0.0;
    for (int i = 0 ; i < p1.coords->size() && i < p2.coords->size(); i++) {
        sum += pow(p1.coords->at(i) - p2.coords->at(i), 2);
    }
    return sqrt(sum);
}

long double dot_product(const Point &p1, const Point &p2) {
    long double sum = 0.0;
    for (int i = 0 ; i < p1.coords->size() && i < p2.coords->size(); i++) {
        sum += p1.coords->at(i) * p2.coords->at(i);
    }
    return sum;
}

long unsigned int int_dot_product(const vector<unsigned int> &v1, const vector<unsigned int> &v2) {
    long unsigned int sum = 0;
    for (int i = 0 ; i < v1.size() && i < v2.size(); i++) {
        sum += v1.at(i) * v2.at(i);
    }
    return sum;
}

Point *vec_add(const Point &p1, const Point &p2) {
    vector<double> *coords = new vector<double>();
    for (int i = 0 ; i < p1.coords->size() && i < p2.coords->size(); i++) {
        coords->push_back(p1.coords->at(i) + p2.coords->at(i));
    }
    return new Point(coords);
}

Centroid *vec_avg(vector<Point *> &points, unsigned int dim) {
    vector<double> *sums = new vector<double>(dim);
    for (int j = 0 ; j < points[0]->coords->size() ; j++) {
        (*sums)[j] = 0.0;
    }
    if (points.empty()){          // empty cluster
        cerr << "Warning: cluster with no points assigned" << endl;
        return NULL;
    }
    for (int i = 0 ; i < points.size() ; i++) {
        for (int j = 0 ; j < dim ; j++) {
            (*sums)[j] += points[i]->coords->at(j);
        }
    }
    for (int j = 0 ; j < dim ; j++) {
        (*sums)[j] /= ((double) points.size());
    }
    return new Centroid(sums);
}

ostream& operator<<(ostream& os, const Point& p) {
    os << "[ ";
    for (int i = 0 ; i < p.coords->size() ; i++) {
        os << p.coords->at(i) << ", ";
    }
    os << "]";
    return os;
}
