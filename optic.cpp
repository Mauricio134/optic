#include "optic.h"

double distanceEuclidian( const Point & p1 , const Point & p2 ){
    double distancia = 0;
    for( int i = 0 ; i < p1.data.size() ; i++ ){
        distancia += pow(abs(p1.data[i]-p2.data[i]),2);
    }
    distancia = sqrt(distancia);
    return distancia;
}

double coreDist(const Point & p, nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, PointCloud>, PointCloud, dimension, size_t> & kdtree){
    std::vector<size_t> indices_data(mxPoints-1);
    std::vector<double> distances_data(mxPoints-1);

    kdtree.knnSearch(p.data.data(), mxPoints-1, indices_data.data(), distances_data.data());
    return distances_data[mxPoints-2];
}

pair<vector<bool>, vector<double>> optical( const vector<Point> & dataSet ){
    PointCloud cloud;
    for (const auto& point : dataSet) {
        cloud.pts.push_back(point);
    }
    nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, PointCloud>, PointCloud, dimension, size_t> kdtree(dimension, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* leaf max size */));
    kdtree.buildIndex();

    vector<double> rDist( dataSet.size() );
    vector<bool> ordering( dataSet.size() );
    for( int i = 0 ; i < dataSet.size() ; i++ ){
        if(ordering[dataSet[i].indice]) continue;
        ordering[dataSet[i].indice] = 1;
        vector<Point> neighbors;
        vector<nanoflann::ResultItem<size_t, double>> ret_matches;
        const size_t nMatches = kdtree.radiusSearch(dataSet[i].data.data(), eps, ret_matches);

        for(int i = 0; i < nMatches; i++){
            neighbors.push_back(dataSet[ret_matches[i].first]);
        }
        if ( neighbors.size() >= mxPoints ){
            set<Point> toProcess;
            pair<set<Point>, vector<double>> result1 = updateQueue(dataSet[i],neighbors, ordering,toProcess,rDist, dataSet, kdtree);
            toProcess = result1.first;
            rDist = result1.second;
            while( !toProcess.empty() ){
                Point q = *(toProcess.begin());
                toProcess.erase(begin(toProcess));
                ordering[q.indice] = 1;
                pair<set<Point>, vector<double>> result2 = updateQueue(dataSet[i],vector<Point>(), ordering,toProcess,rDist, dataSet, kdtree);
            }
        }
    }

    return make_pair( ordering , rDist );
}

pair<set<Point>, vector<double>> updateQueue(const Point & p, vector<Point> neighbors, const vector<bool> & ordering, set<Point> process, vector<double> rd, const vector<Point> & dataSet, nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, PointCloud>, PointCloud, dimension, size_t> & kdtree){
    if(neighbors.empty()){
        vector<nanoflann::ResultItem<size_t, double>> ret_matches;
        const size_t nMatches = kdtree.radiusSearch(p.data.data(), eps, ret_matches);

        for(int i = 0; i < nMatches; i++){
            neighbors.push_back(dataSet[ret_matches[i].first]);
        }
    }   

    if(neighbors.size() < mxPoints){
        return make_pair(process, rd); 
    }
    for(int i = 0; i < neighbors.size(); i++){
        if(ordering[neighbors[i].indice]) continue;
        double newRDist = max(coreDist(p, kdtree), distanceEuclidian(p,neighbors[i]));
        if(rd[neighbors[i].indice] == 0){
            rd[neighbors[i].indice] = newRDist;
            neighbors[i].RDist = newRDist;
            process.insert(neighbors[i]);
        }
        else if(newRDist < rd[neighbors[i].indice]){
            process.erase(neighbors[i]);
            rd[neighbors[i].indice] = newRDist;
            neighbors[i].RDist = newRDist;
            process.insert(neighbors[i]);
        }
    }
    return make_pair(process, rd);
}

vector<Point> epsilonCluster( const Point & p , const vector<Point> & puntos ){
    vector<Point> groupEpsilon;
    for( int i = 0 ; i < puntos.size(); i++ ){
        if(i != p.indice){
            double distancia = distanceEuclidian( p , puntos[i] );
            if( distancia <= eps ){
                groupEpsilon.push_back(puntos[i]);
            }
        }
    }
    return groupEpsilon;
}

vector<int> opticsCluster(const vector<bool> & ordering, const vector<double> &rd){
    vector<int> clusterIndices(rd.size(), -1);
    int current = 0;
    bool incrementCurrentIndex = false;
    for(int i = 0; i < ordering.size(); i++){
        if(rd[i] == 0.0 || rd[i] > eps){
            incrementCurrentIndex = true;
        }
        else{
            if(incrementCurrentIndex){
                current = current + 1;
                clusterIndices[i-1] = current;
            }
            clusterIndices[i] = current;
            incrementCurrentIndex = false;
        }
    }
    return clusterIndices;
}