#include "optic.h"

double distanceEuclidian( const Point & p1 , const Point & p2 ){
    double distancia = 0;
    for( int i = 0 ; i < p1.data.size() ; i++ ){
        distancia += pow(abs(p1.data[i]-p2.data[i]),2);
    }
    distancia = sqrt(distancia);
    return distancia;
}

double coreDist(const Point & p){
    return 1;
}

pair<vector<bool>, vector<double>> optical( const vector<Point> & dataSet ){
    vector<double> rDist( dataSet.size() );
    vector<bool> ordering( dataSet.size() );
    for( int i = 0 ; i < dataSet.size() ; i++ ){
        if(ordering[dataSet[i].indice]) continue;
        ordering[dataSet[i].indice] = 1;
        vector<Point> neighbors = epsilonCluster( dataSet[i] , dataSet );
        if ( neighbors.size() >= mxPoints ){
            set<Point> toProcess;
            pair<set<Point>, vector<double>> result1 = updateQueue(dataSet[i],neighbors, ordering,toProcess,rDist, dataSet);
            toProcess = result1.first;
            rDist = result1.second;
            while( !toProcess.empty() ){
                Point q = *(toProcess.begin());
                toProcess.erase(begin(toProcess));
                ordering[q.indice] = 1;
                pair<set<Point>, vector<double>> result2 = updateQueue(dataSet[i],vector<Point>(), ordering,toProcess,rDist, dataSet);
            }
        }
    }

    return make_pair( ordering , rDist );
}

pair<set<Point>, vector<double>> updateQueue(const Point & p, vector<Point> neighbors, const vector<bool> & ordering, set<Point> process, vector<double> rd, const vector<Point> & dataSet){
    if(neighbors.empty()){
        neighbors = epsilonCluster( p , dataSet );
    }   

    if(neighbors.size() < mxPoints){
        return make_pair(process, rd); 
    }
    for(int i = 0; i < neighbors.size(); i++){
        if(ordering[neighbors[i].indice]) continue;
        double newRDist = max(coreDist(p), distanceEuclidian(p,neighbors[i]));
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