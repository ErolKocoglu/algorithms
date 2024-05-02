/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 1
 * Erol Koçoğlu
 * 150200107
*/

/* * * * * * * * * * * * * * * * * *
 * DO NOT MODIFY UNLESS REQUESTED  *
* * * * * * * * * * * * * * * * * */

#include <iostream>         
#include <vector>
#include <fstream>
#include <sstream>       
#include <stack>
#include <queue>
#include <chrono>       // For time measurement

/* INCLUDE ANY OTHER NECESSARY LIBRARIES HERE */
/* START YOUR CODE HERE */


/* END YOUR CODE HERE */

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */
//Checks if a vector contains given element
bool contains(vector<int>& v, int key){
    for(size_t i=0;i<v.size();i++){
        if(v[i]==key){
            return true;
        }
    }
    return false;
}

//Sort colonies
void sort(vector<pair<int,int>>& v){
    pair<int,int> key;
    int j;
    for (size_t i = 1; i < v.size(); i++) {
        key = v[i];
        j = i - 1;
 
        //Sort biggesst size to smaller size, OR part represent the case of equal size but different type of resources
        while (j >= 0 && (v[j].first < key.first || (v[j].first==key.first && v[j].second>key.second))) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = key;
    }
}

/* END YOUR CODE HERE */

/**
 * Perform Depth-First Search (DFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the DFS traversal.
 * @param col The starting column index for the DFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the DFS traversal.
 */
int dfs(vector<vector<int>>& map, int row, int col, int resource) {

    /* START YOUR CODE HERE */
    int counter=0;
    stack<vector<int>> dfs_stack; //Stack to store nodes. We describe nodes as coordinates.
    dfs_stack.push({row,col});

    while(!dfs_stack.empty()){
        
        int node_row=dfs_stack.top()[0];
        int node_col=dfs_stack.top()[1];
        dfs_stack.pop();

        //Check if coordinates are inbound
        if(node_row<0 ||node_row>=(int)map.size() ||node_col<0 ||node_col>=(int)map[0].size()){
            continue;
        }

        if(map[node_row][node_col]!=resource){
            continue;
        }
        
        counter++;
        map[node_row][node_col]=INT32_MIN;//Mark it as visited

        //Send the neighbors to the stack
        //north
        int north=node_row-1;
        if(north<0){
            north=map.size()-1;
        }
        //south
        int south=node_row+1;
        if(south==(int)map.size()){
            south=0;
        }
        //east
        int east=node_col+1;
        if(east==(int)map[0].size()){
            east=0;
        }
        //west
        int west=node_col-1;
        if(west<0){
            west=map[0].size()-1;
        } 
        dfs_stack.push({north,node_col});
        dfs_stack.push({south,node_col});
        dfs_stack.push({node_row,west});
        dfs_stack.push({node_row,east});
    }
    return counter;

    /* END YOUR CODE HERE */

}


/**
 * Perform Breadth-First Search (BFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the BFS traversal.
 * @param col The starting column index for the BFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the BFS traversal.
 */
int bfs(vector<vector<int>>& map, int row, int col, int resource) {
    
    /* START YOUR CODE HERE */
    int counter=0;
    queue<vector<int>> bfs_queue;//Queue to store nodes. Coordinates can be considered as nodes.
    bfs_queue.push({row,col});

    while(!bfs_queue.empty()){
       int node_row=bfs_queue.front()[0];
       int node_col=bfs_queue.front()[1];
       bfs_queue.pop();

       //Check for inbound
       if(node_row<0 ||node_row>=(int)map.size() ||node_col<0 ||node_col>=(int)map[0].size()){
        continue;
       }
       
       //Check resource type 
       if(map[node_row][node_col]!=resource){
        continue;
       }

       counter++;
       map[node_row][node_col]=INT32_MIN;//Mark it as visited

       //Send the neighbors to the queue
       //north
       int north=node_row-1;
       if(north<0){
        north=map.size()-1;
       }
       //south
       int south=node_row+1;
       if(south==(int)map.size()){
        south=0;
       }
       //east
       int east=node_col+1;
       if(east==(int)map[0].size()){
        east=0;
       }
       //west
       int west=node_col-1;
       if(west<0){
        west=map[0].size()-1;
       }
       bfs_queue.push({north,node_col});
       bfs_queue.push({south,node_col});
       bfs_queue.push({node_row,west});
       bfs_queue.push({node_row,east});

    }
    return counter;
    /* END YOUR CODE HERE */

}

/**
 * Finds the top-k largest colonies in a given map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param useDFS A boolean flag indicating whether to use Depth-First Search (DFS) or Breadth-First Search (BFS) algorithm.
 * @param k The number of top colonies to find.
 * @return A vector of pairs representing the size and resource type of the top-k largest colonies.
 */
vector<pair<int, int>> top_k_largest_colonies(vector<vector<int>>& map, bool useDFS, unsigned int k) {

    auto start = high_resolution_clock::now();      // Start measuring time
    
    /* START YOUR CODE HERE */
    vector<pair<int,int>> results;
    vector<vector<int>> map_to_use=map;//Use another map so that our original map remains same
    vector<int> resource_types;//Use to find the number of resource types
    vector<pair<int,int>> results_to_send;

    if(map.size()==0){

        return results_to_send;
    }

    //Find the number of resource types
    for(size_t i=0;i<map.size();i++){
        for(size_t j=0;j<map[0].size();j++){
            if(!contains(resource_types,map[i][j])){
                resource_types.push_back(map[i][j]);
            }
        }
    }

    if(useDFS){
        for(size_t i=0;i<resource_types.size();i++){
            for(size_t r=0;r<map_to_use.size();r++){
                for(size_t c=0;c<map_to_use[0].size();c++){
                    int size=dfs(map_to_use,r,c,resource_types[i]);
                    if(size!=0){
                        results.push_back(make_pair(size,resource_types[i]));
                    }
                }
            }
        }
    }else{
        for(size_t i=0;i<resource_types.size();i++){
            for(size_t r=0;r<map_to_use.size();r++){
                for(size_t c=0;c<map_to_use[0].size();c++){
                    int size=bfs(map_to_use,r,c,resource_types[i]);
                    if(size!=0){
                        results.push_back(make_pair(size,resource_types[i]));
                    }
                }
            }
        }
    }

    //Sort the results
    sort(results);

    //Take first k elements of results
    for(size_t i=0;i<k;i++){
        if(i==results.size()){
            break;
        }
        results_to_send.push_back(results[i]);
    }

    /* END YOUR CODE HERE */

    auto stop = high_resolution_clock::now();                       // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start);       // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    /* START YOUR CODE HERE */
    return results_to_send;

    /* END YOUR CODE HERE */

}

// Main function
int main(int argc, char* argv[]) {

    // Check the command line arguments
    if (argc != 4) {

        // It should be: <1 or 0> <k> <filename.txt>
        cerr << "Usage: " << argv[0] << " <1 or 0> <k> <filename.txt>" << endl;
        return 1;
    }

    // Get the arguments
    bool useDFS = (atoi(argv[1]) == 1);
    int k = atoi(argv[2]);

    // Read the map from the text file
    string filename = argv[3];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<vector<int>> sector_map;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<int> row;
        int num;
        while (iss >> num) {
            row.push_back(num);
        }
        sector_map.push_back(row);
    }
    file.close();   

    // Find the largest colony
    vector<pair<int, int>> result = top_k_largest_colonies(sector_map, useDFS, k);

    // Print the result
    cout << "Algorithm: " << (useDFS ? "DFS" : "BFS") << endl;
    cout << "Map: " << filename << endl;

    // If no colonies are found
    if (result.empty()) {
        cout << "No colonies found." << endl;
        return 0;
    }

    // Print the top-k colonies
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "Colony " << i + 1 << ": Size = " << result[i].first << ", Resource Type = " << result[i].second << endl;
    }

    return 0;
}