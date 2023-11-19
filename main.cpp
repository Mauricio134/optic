#include <iostream>
#include <sstream>
#include <fstream>
#include "optic.cpp"
#include "optic.h"


#define NOMBRE_ARCHIVO "abalone_csv.csv"

using namespace std;

vector<Point> obtDataset(){
    vector<Point> obtData;
    ifstream archivo(NOMBRE_ARCHIVO);
    string linea;
    char delimitador = ',';
    getline(archivo, linea);
    int ind = 0;
    int count = 0;
    while (getline(archivo, linea) && count < 500)
    {
        Point punto;
        stringstream stream(linea);
        string Sex,Length,Diameter,Height,Whole_weight,Shucked_weight,Viscera_weight,Shell_weight,Class_number_of_rings;
        float dato;
        int ring;
        getline(stream, Sex, delimitador);
        punto.sex = Sex;
        getline(stream, Length, delimitador);
        istringstream(Length) >> dato;
        punto.data.push_back(dato);
        getline(stream, Diameter, delimitador);
        istringstream(Diameter) >> dato;
        punto.data.push_back(dato);
        getline(stream, Height, delimitador);
        istringstream(Height) >> dato;
        punto.data.push_back(dato);
        getline(stream, Whole_weight, delimitador);
        istringstream(Whole_weight) >> dato;
        punto.data.push_back(dato);
        getline(stream, Shucked_weight, delimitador);
        istringstream(Shucked_weight) >> dato;
        punto.data.push_back(dato);
        getline(stream, Viscera_weight, delimitador);
        istringstream(Viscera_weight) >> dato;
        punto.data.push_back(dato);
        getline(stream, Shell_weight, delimitador);
        istringstream(Shell_weight) >> dato;
        punto.data.push_back(dato);
        getline(stream, Class_number_of_rings, delimitador);
        istringstream(Class_number_of_rings) >> ring;
        punto.rings = ring;
        punto.indice = ind;
        ind++;
        obtData.push_back(punto);
        count++;
    }

    archivo.close();
    return obtData;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    vector<Point> dataSet = obtDataset();

    pair<vector<bool>, vector<double>> Optical = optical(dataSet);

    vector<int> clusters = opticsCluster(Optical.first, Optical.second);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Tiempo de ejecucion: " << duration.count() << " microsegundos." << std::endl;

    ofstream archivo("clusters.txt");

    for(int i = 0; i < dataSet.size(); i++){
        archivo << dataSet[i].data[0] << " " << dataSet[i].data[1] << " " << clusters[i] <<  endl;
    }
    return 0;
}