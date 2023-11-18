#include <iostream>
#include <vector>
#include "nanoflann.hpp"

using namespace std;

// Definir el tipo de datos y la dimensión
const int dimension = 7;  // Cambiar según la dimensión de tus datos

// Definir el adaptador de datos para nanoflann
struct PointCloud {
    std::vector<vector<double>> pts;  // Utilizar solo el vector data

    // Métodos requeridos por nanoflann
    inline size_t kdtree_get_point_count() const { return pts.size(); }

    inline double kdtree_distance(const double* p1, const size_t idx_p2, size_t /*size*/) const {
        double distance = 0.0;
        for (int d = 0; d < dimension; ++d) {
            const double diff = p1[d] - pts[idx_p2][d];
            distance += diff * diff;
        }
        return distance;
    }

    inline double kdtree_get_pt(const size_t idx, int dim) const {
        return pts[idx][dim];
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }
};

// Estructura original con la información completa
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

int main() {
    // Crear datos de ejemplo utilizando la estructura Point
    vector<Point> fullPoints = {
        {"Male", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0}, 3, 0},
        {"Female", {20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0}, 4, 1},
        {"", {0.8, 0.9, 2.1, 3.2, 4.5, 5.5, 6.5}, 0, 0},
    };

    // Crear datos de ejemplo utilizando solo el vector data
    PointCloud cloud;
    for (const auto& point : fullPoints) {
        cloud.pts.push_back(point.data);
    }

    // Construir el árbol KD utilizando solo el vector data
    nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, PointCloud>, PointCloud, dimension, size_t> kdtree(
        dimension, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* leaf max size */));
    kdtree.buildIndex();

    // Puntos de consulta utilizando la estructura Point
    Point query_point_full = {"", {0.8, 0.9, 2.1, 3.2, 4.5, 5.5, 6.5}, 0, 0};

    // Puntos de consulta utilizando solo el vector data
    vector<double> query_point_data = {0.8, 0.9, 2.1, 3.2, 4.5, 5.5, 6.5};

    // Realizar una búsqueda de k vecinos más cercanos con la estructura Point
    const size_t num_results_full = 2;
    std::vector<size_t> indices_full(num_results_full);
    std::vector<double> distances_full(num_results_full);

    kdtree.knnSearch(query_point_full.data.data(), num_results_full, indices_full.data(), distances_full.data());

    // Realizar una búsqueda de k vecinos más cercanos con solo el vector data
    const size_t num_results_data = 2;
    std::vector<size_t> indices_data(num_results_data);
    std::vector<double> distances_data(num_results_data);

    kdtree.knnSearch(query_point_data.data(), num_results_data, indices_data.data(), distances_data.data());

    // Imprimir resultados para la estructura Point
    cout << "Resultados para la estructura Point:\n";
    for (size_t i = 0; i < num_results_full; ++i) {
        cout << "    Vecino " << i + 1 << ": {";
        for (int d = 0; d < dimension; ++d) {
            cout << query_point_full.data[d];
            if (d < dimension - 1) {
                cout << ", ";
            }
        }
        cout << "} -> Vecino más cercano: {";
        for (int d = 0; d < dimension; ++d) {
            cout << cloud.pts[indices_full[i]][d];
            if (d < dimension - 1) {
                cout << ", ";
            }
        }
        cout << "} (Distancia: " << distances_full[i] << ")\n";
    }

    // Imprimir resultados para solo el vector data
    cout << "Resultados para solo el vector data:\n";
    for (size_t i = 0; i < num_results_data; ++i) {
        cout << "    Vecino " << i + 1 << ": {";
        for (int d = 0; d < dimension; ++d) {
            cout << query_point_data[d];
            if (d < dimension - 1) {
                cout << ", ";
            }
        }
        cout << "} -> Vecino más cercano: {";
        for (int d = 0; d < dimension; ++d) {
            cout << cloud.pts[indices_data[i]][d];
            if (d < dimension - 1) {
                cout << ", ";
            }
        }
        cout << "} (Distancia: " << distances_data[i] << ")\n";
    }

    return 0;
}
