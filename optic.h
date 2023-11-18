#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
#include "nanoflann.hpp"

using namespace std;

#define mxPoints 4
#define eps 0.5

class Point {
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

const int dimension = 7;

// Definir el adaptador de datos para nanoflann
struct PointCloud {
    std::vector<Point> pts;

    // Métodos requeridos por nanoflann
    inline size_t kdtree_get_point_count() const { return pts.size(); }

    inline double kdtree_distance(const double* p1, const size_t idx_p2, size_t /*size*/) const {
        double distance = 0.0;
        for (int d = 0; d < dimension; ++d) {
            const double diff = p1[d] - pts[idx_p2].data[d];
            distance += diff * diff;
        }
        return distance;
    }

    inline double kdtree_get_pt(const size_t idx, int dim) const {
        return pts[idx].data[dim];
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }
};

pair<vector<bool>, vector<double>> optical(const vector<Point> & );

vector<Point> epsilonCluster(const Point &, const vector<Point> &);

pair<set<Point>, vector<double>> updateQueue(const Point &, vector<Point> , const vector<bool> &, set<Point> , vector<double>, const vector<Point> &, nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, PointCloud>, PointCloud, dimension, size_t> &);

vector<int> opticsCluster(const vector<bool> & ordering, const vector<double> &rd);