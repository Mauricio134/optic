#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

#define mxPoints 4
#define eps 0.3

class Point{
public:
    string sex;
    vector<double> data;
    int rings;
    int indice;
    double RDist = 0.0;
    bool operator<(const Point& other) const {
        return RDist < other.RDist;
    }
};

pair<vector<bool>, vector<double>> optical(const vector<Point> & );

vector<Point> epsilonCluster(const Point &, const vector<Point> &);

pair<set<Point>, vector<double>> updateQueue(const Point &, vector<Point> , const vector<bool> &, set<Point> , vector<double>, const vector<Point> &);

vector<int> opticsCluster(const vector<bool> & ordering, const vector<double> &rd);