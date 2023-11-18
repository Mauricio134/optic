#include <iostream>
#include <vector>
#include "nanoflann.hpp"

using namespace std;

// Definir el tipo de datos y la dimensión
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

int main() {
    // Crear datos de ejemplo utilizando la estructura Point
    vector<Point> fullPoints = {
        {"Male", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0}, 3, 0},
        {"Female", {20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0}, 4, 1},
        {"", {0.8, 0.9, 2.1, 3.2, 4.5, 5.5, 6.5}, 0, 0},
        // Agregar más puntos según sea necesario
    };

    // Crear datos de ejemplo utilizando solo el vector data
    PointCloud cloud;
    for (const auto& point : fullPoints) {
        cloud.pts.push_back(point);
    }

    // Construir el árbol KD utilizando solo el vector data
    nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, PointCloud>, PointCloud, dimension, size_t> kdtree(
        dimension, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* leaf max size */));
    kdtree.buildIndex();
    Point query_point = {"", {0.8, 0.9, 2.1, 3.2, 4.5, 5.5, 6.5}, 0, 0};
    // Punto de consulta
            // Punto de consulta
    const double search_radius = static_cast<double>(20);
    vector<nanoflann::ResultItem<size_t, double>> ret_matches;

    const size_t nMatches = kdtree.radiusSearch(query_point.data.data(), search_radius, ret_matches);

        cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches
             << " matches\n";
        for (size_t i = 0; i < nMatches; i++)
            cout << "idx[" << i << "]=" << ret_matches[i].first << " dist[" << i
                 << "]=" << ret_matches[i].second << endl;
        cout << "\n";
    return 0;
}
