//
//  main.cpp
//  Mule_MTh
//
//  Created by Eihab Salah on 2/13/17.
//  Copyright © 2017 Eihab Salah. All rights reserved.
//


#include <cstdlib>
#include <iostream>
#include<fstream>
#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <assert.h>
#include "pthread.h"
#include "Pattern.h"

using namespace std;
struct arg_struct {
    //    Pattern* P;
    //    int minsup;
    //    vector<Pattern* > & MFS;
    int id;
};

int counter_threads = 0;
vector<vector<Pattern*> > mfs_s;

// Number of lines starting from 1 to use it like reality
int numberOfLines = 0;
//put the elements of the map in vector (as patterns) to be used in ECLAT algorithm
vector <Pattern*> levelOne;
//vector will contain the final answer (maximal patterns)
vector< Pattern* > MFS;

// the map witch the elements of file with IDs will stored in
vector<vector<int> > v_attributes; // matrix-like to put the attributes in
long n_nodes ;
vector< pair<int, int> > edges;
int maxx =0;
int minsup = 0;
int minsize = 0;
int counter = 0 ;
int p_counter = 0 ;
pthread_mutex_t mutexx = PTHREAD_MUTEX_INITIALIZER;

vector<int > D;
//#############printVector#################

void printVector(vector <int> x){
    for (int i=0;i<x.size();i++){
        cout << x[i]<< " ";
    }
    cout <<endl;
}

//##############findMax##############

int findMax (vector<int> x){
    int max = -1;
    for (int i = 0;i<x.size(); i++) {
        if(max < x[i]){
            max = x[i];
        }
    }
    return max;
}


//##############checkIsSubset################

bool checkIsSubset(Pattern* P,vector<Pattern* >& F){
    bool maxi = false;
    for (int k=0;k<F.size();k++){
        vector<int> v ;
        set_intersection(P->item.begin(), P->item.end(),
                         F[k]->item.begin(), F[k]->item.end(),
                         back_inserter(v));
        if(v.size() == P->item.size()){
            return true;
        }
    }
    return maxi;
}

//##############checkIsSubsetOrSuperset################

pair<bool, bool> checkIsSubsetOrSuperset(int i, Pattern* P,vector<Pattern* >& F){
    bool sub = false;
    bool sup = false;
    for (int k=0;k<F.size();k++){
        if(k == i){
            continue;
        }
        vector<int> v ;
        set_intersection(P->item.begin(), P->item.end(),
                         F[k]->item.begin(), F[k]->item.end(),
                         back_inserter(v));
        if(v.size() == P->item.size()){
            sub =  true;
        }else if(v.size() == F[k]->item.size()){
            sup = true;
        }
    }
    return make_pair(sub, sup);
}

//##############Union Nodes###############

vector <int> UnionEdges(vector <int> X1, int X2){
    X1.push_back(X2);
    return X1;
    
}

//##############updateCandidates###############

vector <int> updateCandidates(vector <int> candidates,vector <int> new_candidate,vector <int> visited){
    vector <int> result,result1;
    std::sort(candidates.begin(), candidates.end());
    std::sort(new_candidate.begin(), new_candidate.end());
    std::sort(visited.begin(), visited.end());
    
    std::set_union(candidates.begin(), candidates.end(),
                   new_candidate.begin(), new_candidate.end(),
                   std::back_inserter(result));
    
    std::set_difference(result.begin(), result.end(), visited.begin(), visited.end(),
                        std::inserter(result1, result1.end()));
    
    return result1;
}
//############logical_And##################

pair<vector<int>, int> logical_And(vector<int> X1, vector<int> X2){
    vector<int> result;
    int counter = 0;
    for (int i=0;i<X1.size();i++){
        int sdf = X1[i] * X2[i];
        result.push_back(sdf);
        if(sdf == 1){
            counter++;
        }
    }
    return make_pair(result, counter);
}

//##############vectorToString################

string vectorToString(vector<int> x){
    sort(x.begin(), x.end());
    std::stringstream ss;
    for(int i = 0; i < x.size(); ++i)
    {
        if(i != 0)
            ss << ",";
        ss << x[i];
    }
    return ss.str();
}

//###################vecStrToInt#####################

vector<int> vecStrToInt(vector<string>& s){
    
    vector<int> intNumbers;
    for (int i=0; i< s.size(); i++)
    {
        int num = atoi(s[i].c_str());
        intNumbers.push_back(num);
    }
    return intNumbers;
}

//###################split###########################
// The function "split" to to split the line coming from the file into tokens  (split by space)

vector <string> split(string line) {
    vector<string> tokens;
    istringstream iss(line);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    
    return tokens;
}

//##################loadAttributes###################
//Load the file and call split to split the line and store it in attributes vector

void loadAttributes(string path) {
    
    vector<string> list;
    ifstream in_stream;
    std::ifstream input(path);
    
    for (std::string line; getline(input, line);) {
        if (line.empty() == false && line.at(0) != '#' && line.at(0) != '%' && line.at(0) != '@') {
            vector <string> v_string_attributes = split(line);
            v_attributes.push_back(vecStrToInt(v_string_attributes));
        }
    }
    input.close();
}

//####################loadEdges#####################
//Load the file and call split to split the line and store it in a map

void loadEdges(string path){
    
    vector<string> list;
    ifstream in_stream;
    std::ifstream input(path);
    
    for (std::string line; getline(input, line);) {
        if (line.empty() == false && line.at(0) != '#' && line.at(0) != '%' && line.at(0) != '@') {
            vector <string> v_string_edges = split(line);
            vector<int> v_int =vecStrToInt(v_string_edges);
            if(v_int[0] > maxx){
                maxx = v_int[0];
            }else if(v_int[1] > maxx){
                maxx = v_int[1];
            }
            edges.push_back(make_pair(v_int[0], v_int[1]));
        }
    }
    input.close();
}
//####################fillLevelOne#####################

void fillLevelOne(){
    vector<int> neighbors;
    vector<int> visited;
    
    for (int i=0;i<edges.size();i++){
        int mycount = count (v_attributes[i].begin(), v_attributes[i].end(), 1);
        if (mycount >= minsup){
            vector<int> new_x;
            new_x.push_back(i);
            Pattern *pp = new Pattern(new_x, neighbors, neighbors,v_attributes[i],visited);
            levelOne.push_back(pp);
        }else{
            levelOne.push_back(NULL);
        }
    }
    
    vector<int> vis;
    for (int i=0;i<levelOne.size();i++){
        if(levelOne[i] !=NULL){
            vector<int> cand;
            vis.push_back(i);
            if(i % 1000 == 0){
                cout << i << endl;
                
            }
            
            int current_id = levelOne[i]->item[0];
            //cout << "C_id " << current_id << endl;
            for (int j=i+1;j<levelOne.size();j++){
                if( levelOne[j] != NULL){
                    int next_id = levelOne[j]->item[0];
                    //cout << "N_id " << next_id << endl;
                    
                    if(edges[current_id].first == edges[next_id].first || edges[current_id].first == edges[next_id].second
                       || edges[current_id].second == edges[next_id].first || edges[current_id].second == edges[next_id].second ){
                        cand.push_back(next_id);
                    }
                }
                levelOne[i]->candidates = cand;
                levelOne[i]->visited = vis;
            }
        }
    }
    
    for (int i=0;i<levelOne.size();i++){
        if(levelOne[i] !=NULL){
            vector<int> neigh;
            
            int current_id = levelOne[i]->item[0];
            //cout << "C_id " << current_id << endl;
            for (int j=0;j<levelOne.size();j++){
                if( levelOne[j] != NULL){
                    if( i ==j){
                        continue;
                    }
                    int next_id = levelOne[j]->item[0];
                    //cout << "N_id " << next_id << endl;
                    
                    if(edges[current_id].first == edges[next_id].first || edges[current_id].first == edges[next_id].second
                       || edges[current_id].second == edges[next_id].first || edges[current_id].second == edges[next_id].second ){
                        neigh.push_back(next_id);
                    }
                }
                levelOne[i]->neighbors = neigh;
            }
        }
    }
}
//####################fillLevelOneV2#####################

void fillLevelOneV2(){
    vector<int> neighbors;
    vector<int> visited;
    
    for (int i=0;i<edges.size();i++){
        int mycount = count (v_attributes[i].begin(), v_attributes[i].end(), 1);
        if (mycount >= minsup){
            vector<int> new_x;
            new_x.push_back(i);
            Pattern *pp = new Pattern(new_x, neighbors, neighbors,v_attributes[i],visited);
            levelOne.push_back(pp);
        }else{
            levelOne.push_back(NULL);
        }
    }
    vector<int> vis;

    // populate neighbors
    for (int i=0;i<levelOne.size();i++){
        if(levelOne[i] !=NULL){
            vis.push_back(i);
            int current_id = levelOne[i]->item[0];
            //cout << "C_id " << current_id << endl;
            for (int j=0;j<levelOne.size();j++){
                if( levelOne[j] != NULL){
                    if( i ==j){
                        continue;
                    }
                    int next_id = levelOne[j]->item[0];
                    //cout << "N_id " << next_id << endl;
                    
                    if(edges[current_id].first == edges[next_id].first || edges[current_id].first == edges[next_id].second
                       || edges[current_id].second == edges[next_id].first || edges[current_id].second == edges[next_id].second ){
                        pair<vector<int>, int> v3;
                        v3 =logical_And(levelOne[i]->attributes,levelOne[j]->attributes);
                        if(v3.second >= minsup){
                            levelOne[i]->neighbors.push_back(next_id);
                            if(j>i){
                                levelOne[i]->candidates.push_back(next_id);
                            }
                        }
                        
                    }
                }
                levelOne[i]->visited = vis;
            }
        }
    }
//
//    for (int i=0;i<levelOne.size();i++){
//        if(levelOne[i] !=NULL){
//            vector<int> cand;
//            vis.push_back(i);
//            int current_id = levelOne[i]->item[0];
//            //cout << "C_id " << current_id << endl;
//            for (int j=i+1;j<levelOne.size();j++){
//                if( levelOne[j] != NULL){
//                    int next_id = levelOne[j]->item[0];
//                    //cout << "N_id " << next_id << endl;
//                    
//                    if(edges[current_id].first == edges[next_id].first || edges[current_id].first == edges[next_id].second
//                       || edges[current_id].second == edges[next_id].first || edges[current_id].second == edges[next_id].second ){
//                        cand.push_back(next_id);
//                    }
//                }
//                levelOne[i]->candidates = cand;
//                levelOne[i]->visited = vis;
//            }
//        }
//    }
    
    }

//#############MinePathways###################

void MinePathways(Pattern* P, int minsup, vector<Pattern* > & MFS) {
    bool isMaximal = true;
    for (int j = 0; j < P->candidates.size(); j++) {
        int idx = P->candidates[j];
        P->visited.push_back(idx);
        D.push_back(idx);
        pair<vector<int>, int> v3;
        v3 =logical_And(v_attributes[idx],P->attributes);
        int mycount = v3.second;
        vector<int> att = v3.first;
        if (mycount >= minsup){
            vector <int> W_items;
            W_items = UnionEdges(P->item,idx);
            sort(W_items.begin(), W_items.end());
            isMaximal = false;
            vector <int> W_candidates;
            W_candidates =updateCandidates(P->candidates, levelOne[idx]->neighbors, P->visited);
            Pattern* new_p = new Pattern (W_items,W_candidates,P->neighbors,att,P->visited);
            MinePathways(new_p, minsup, MFS);
        }
    }
    if (isMaximal){
        if (!checkIsSubset(P, MFS)){
            MFS.push_back(P);
        }
        else{
            if(P->getItem().size() >1){
                delete P;
            }
        }
    }
    else{
        if(P->getItem().size() >1){
            delete P;
        }
    }
    
}

//#############Ccalling multithreading###################

void* callMTh (void *arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    //int cc = * (int*) arg;
    while (true){
        if(p_counter >= levelOne.size()){
            break;
        }
        //start critical section
        pthread_mutex_lock (&mutexx);
        //        cout << "Thread: " << (args -> id) << " working on this node_id: " << p_counter << endl;
        if(levelOne[p_counter] != NULL){
            cout << "Thread "<< args -> id << " working on: " << p_counter << " out of " << levelOne.size() << endl;
            MinePathways(levelOne[p_counter], minsup, mfs_s[args -> id]);
            //End critical section
        }
        p_counter  += 1;
        //End critical section
        pthread_mutex_unlock (&mutexx);
    }
    printf( "Thread %d has completed\n", args -> id );
    pthread_exit( &args -> id);

}

//#############CreateThreads###################
void createThreads (int counter_threads){
    pthread_t threads[ counter_threads ];
    int result_code;
    unsigned index;
    
    
    
    struct arg_struct argus[counter_threads];
    
    for ( index = 0; index < counter_threads; ++index ){
        argus[index].id = index;
      //  printf("Creating thread %d\n", index);
        result_code = pthread_create( &threads[index], NULL, callMTh, &argus[index] );
        assert( !result_code );
        

    }
    for( index = 0; index < counter_threads; ++index )
    {
        // block until thread 'index' completes
        result_code = pthread_join( threads[ index ], NULL );
        assert( !result_code );
    //    printf( "Thread %d has been joined\n", index );
    }
    
    printf( "::All threads completed successfully::\n" );
    
}

//#############excludeRecurrences###################
void excludeRecurrences(){
    vector<Pattern*> mfs_general;
    for (int i = 0;i< mfs_s.size() ; i++){
        for (int j = 0;j< mfs_s[i].size() ; j++){
            mfs_general.push_back(mfs_s[i][j]);
        }
    }
    vector<int> flags;
    pair<bool, bool> sub_sup;
    for(int i =0 ;i < mfs_general.size() ; i++){
        sub_sup = checkIsSubsetOrSuperset(i, mfs_general[i], mfs_general);
        if (sub_sup.first){
            flags.push_back(-1);
        }else{
            flags.push_back(1);
        }
    }
    
    for (int i = 0;i< mfs_general.size()  ; i++){
        if(flags[i] == 1){
            MFS.push_back(mfs_general[i]);
        }
    }
    
}

//#############printOutput###################
void printOutput(string path){
    ofstream myfile1;
    myfile1.open (path, std::ofstream::out | std::ofstream::trunc);
    // myfile1.open ("output.txt", std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < MFS.size(); i++) {
        if(MFS[i]->getItem().size() < minsize)
            continue;
        for (int m = 0; m < MFS[i]->getItem().size(); m++) {
            myfile1 << MFS[i]->getItem()[m] << " ";
        }
        myfile1 << endl;
    }
    myfile1.close();
    
}
//###########################################
//    summaryAttributeThr20.txt
//    summaryGraphThr20.txt
//    hs_datasetSup10AttributeMatrix.txt
//    hs_datasetSup10SummaryGraphS1.txt
//    summaryAttributeThr20.txt
//    summaryGraphThr20.txt
int main(int argc, const char * argv[]) {
    
    time_t t = time(0);
    vector<int> temp = *new vector<int>(0);
    cout << ">>loading attributes..." << endl;
        loadAttributes(argv[2]);
    cout << v_attributes.size() <<endl;

//    loadAttributes("hs_datasetSup10AttributeMatrix.txt");
    cout << "::attributes loaded::" << endl;
    
    time_t t_22 = time(0);
    cout << "time: " << t_22-t << endl;
    
    cout << ">>loading edges..." << endl;
        loadEdges(argv[1]);
//    loadEdges("hs_datasetSup10SummaryGraphS1.txt");
    cout << "::edges loaded::" << endl;
    
        minsup = atoi(argv[4]);
        minsize = atoi(argv[6]);
    counter_threads =atoi(argv[5]);
//
//    minsize = 2;
//    counter_threads = 4;
//    minsup = 20;

    mfs_s.resize(counter_threads);
    
    
    //    fill levelOne
    cout << ">>fill levelOne..." << endl;
    fillLevelOneV2();
    cout << "::levelOne filled::" << endl;
    
    time_t t_2 = time(0);
    cout << "time: " << t_2-t << endl;
    int counter = 0;
    for (int i = 0; i < levelOne.size(); i++) {
        if(levelOne[i] != NULL){
            counter ++;
        }
    }
    cout << "size of levelone: " <<counter<<endl;
    cout << ">>Working on threads..." << endl;

    //######starting the pthread things
    createThreads(counter_threads);
    //######ending the pthread things
    excludeRecurrences();
    cout << "::Done::" << endl;
    time_t t_3 = time(0);
    cout << "Algorithm's time: " << t_3-t_2 << endl;

    cout << "*********Final Result***********"<<endl;
    string path = argv[3];
    printOutput(path);
    cout << "number of patterns: "<<MFS.size()<<endl;
    
    time_t t_4 = time(0);
    
    cout << "time: " << t_4-t_3 << endl;
    cout << "Total time: " << t_4-t << endl;

    
    return 0;
}
