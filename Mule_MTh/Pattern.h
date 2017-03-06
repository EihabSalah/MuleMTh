//
//  Pattern.h
//  Mule_MTh
//
//  Created by Eihab Salah on 2/13/17.
//  Copyright © 2017 Eihab Salah. All rights reserved.

// Include any system framework and library headers here that should be included in all compilation units.
// You will also need to set the Prefix Header build setting of one or more of your targets to reference this file.
#include <cstdlib>
#include <iostream>
#include<fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>



using namespace std;

class Pattern {
public:
    
    
    vector<int>  item;
    vector<int> candidates;
    vector<int> neighbors;
    vector<bool> attributes;
    vector<int> visited;
    
    
    Pattern(vector<int> my_item, vector<int> my_candidates, vector<int> my_neighbors, vector<bool> my_attributes,
            vector<int> my_visited){
        item = vector<int>(my_item.begin(),my_item.end());
        candidates = vector<int>(my_candidates.begin(),my_candidates.end());
        neighbors =  vector<int>(my_neighbors.begin(),my_neighbors.end());
        attributes = vector<bool>(my_attributes.begin(),my_attributes.end());
        visited = vector<int>(my_visited.begin(),my_visited.end());
        
    }
    
    ~Pattern(){
        item.clear();
        item.resize(0); //item.shrink_to_fit();
        candidates.clear();
        candidates.resize(0);
        neighbors.clear();
        neighbors.resize(0);
        attributes.clear();
        attributes.resize(0);
        visited.clear();
        visited.resize(0);
        //        cout << "###Pattern Deleted###" << endl;
    }
    
    
    vector<int> getItem(){
        return item;
    }
    
    vector<int> getCandidates(){
        return candidates;
    }
    
    vector<int> getNeighbors(){
        return neighbors;
    }
    
    vector<bool> getAttributes(){
        return attributes;
    }
    
    vector<int> getvisited(){
        return visited;
    }
    
    
    void print(){
        cout << "###Pattern###" << endl;
        cout << "item: ";
        copy(item.begin(), item.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "candidates: ";
        copy(candidates.begin(), candidates.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "neighbors: ";
        copy(neighbors.begin(), neighbors.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "attributes: ";
        copy(attributes.begin(), attributes.end(), ostream_iterator<bool>(cout, " "));
        cout << endl;
        cout << "visited: ";
        copy(visited.begin(), visited.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        
        
    }
};
