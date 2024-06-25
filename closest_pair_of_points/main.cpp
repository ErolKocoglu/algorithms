/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Erol Koçoğlu
 * 150200107
*/

#include <iostream>   // For input/output
#include <cmath>      // For distance calculations
#include <vector>     // For vector operations
#include <algorithm>  // For sorting operations
#include <limits>     // For min(), max() functions
#include <fstream>    // To read/write files
#include <chrono>     // For time measurement

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point {
    double x, y;
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2) {

    //YOUR CODE HERE
    double x_distance= p1.x - p2.x;
    double y_distance= p1.y - p2.y;

    return sqrt(x_distance*x_distance + y_distance*y_distance);

}

// Helper function to compare points based on x-coordinate, can use this for sorting.
bool compareX(Point p1, Point p2) {

    //YOUR CODE HERE
    return p1.x<p2.x;

}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2) {

    //YOUR CODE HERE
    return p1.y<p2.y;

}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point>& points, int start, int end) {

    //YOUR CODE HERE
    double min=MAXFLOAT;
    pair<Point,Point> closest;
    //Check all combinations of points
    for(int i=start; i<end;i++){
        for(int j=i+1;j<=end;j++){
            if(distance(points[i],points[j])<min){
                min=distance(points[i],points[j]);
                closest.first=points[i];
                closest.second=points[j];
            }
            
        }
    }

    return closest;

}

// The main objective of the assignment. This is the function to find the closest pair of points recursively using divide and conquer.
pair<Point, Point> closestPair(vector<Point>& points, int start, int end) {

    //YOUR CODE HERE
    if((end-start)<=3){
        return bruteForceClosestPair(points,start,end);
    }

    int mid=(start+end)/2;
    pair<Point,Point> left=closestPair(points,start,mid);//minimum of left area
    pair<Point,Point> right=closestPair(points,mid+1,end);//minimum of right area
    pair<Point,Point> closest;//minimum of all pairs

    double d_left=distance(left.first,left.second);
    double d_right=distance(right.first,right.second);
    double d=min(d_left,d_right);
    if(d==d_left){
        closest=left;
    }else{
        closest=right;//determining minimum
    }

    vector<Point> mid_points;
    //Points near the middle line
    for(size_t i=0;i<points.size();i++){
        if(abs(points[i].x - points[mid].x) < d){
            mid_points.push_back(points[i]);
        }
    }

    sort(begin(mid_points), std::end(mid_points),compareY);
    //Checking if any pair is closer than current min distance
    for(size_t i=0;i<mid_points.size();i++){
        for(size_t j=i+1;j<i+15 && j<mid_points.size();j++){
            if(distance(mid_points[i],mid_points[j])<d){
                d=distance(mid_points[i],mid_points[j]);
                closest={mid_points[i],mid_points[j]};
            }
        }
    }

    return closest;

}

// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point>& point_vector, pair<Point,Point> point_pair) {

    //YOUR CODE HERE
    Point first=point_pair.first;
    Point second=point_pair.second;
    //Find and remove points from vector O(n) time
    for(size_t i=0;i<point_vector.size();i++){
        if((first.x==point_vector[i].x && first.y==point_vector[i].y)){
            point_vector.erase(point_vector.begin()+i);
            break;
        }
    }

    for(size_t i=0;i<point_vector.size();i++){
        if((second.x==point_vector[i].x && second.y==point_vector[i].y)){
            point_vector.erase(point_vector.begin()+i);
            break;
        }
    }

    return point_vector;

}

// This function should wrap up the entire algorithm. It should:
//    1- Find the closest pair
//    2- Add the pair to the "pairs" vector
//    3- Remove the paired result from the map
// at each iteration.
// Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points) {
    vector<pair<Point,Point>> pairs; //add your pairs to this vector
    Point unconnected = {-1,-1}; //set this as the unconnected point if it exists, leave as is otherwise.

    //YOUR CODE HERE
    sort(begin(points),end(points),compareX);
    auto start = std::chrono::high_resolution_clock::now();
    while(points.size()>1){
        pair<Point,Point> p=closestPair(points,0,points.size()-1);
        //pair<Point,Point> p=bruteForceClosestPair(points,0,points.size()-1);
        if(p.first.y>p.second.y){
            //Swap first and second points if first point's y > second point's y
            Point temp=p.first;
            p.first=p.second;
            p.second=temp;
        }else if(p.first.y==p.second.y){
            if(p.first.x>p.second.x){
                Point temp=p.first;
                p.first=p.second;
                p.second=temp;
            }
        }

        pairs.push_back(p);
        points=removePairFromVector(points,p);
    }

    if(points.size()==1){
        //If there is one point left, then this unconnected
        unconnected=points[0];
    }

    auto stop = std::chrono::high_resolution_clock::now();                       // Stop measuring time
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);       // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    //before printing, please make sure that within each pair, the city with the smaller y coordinate is printed first...
    //...if the y coordinates are the same, print the city with the smaller x coordinate first.

    //This part prints the output, don't modify.    
    for(size_t i = 0; i < pairs.size(); i++){
        cout << "Pair " << i+1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1){
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y;
    }


}

//Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
vector<Point> readCoordinatesFromFile(const string& filename) {
    vector<Point> points;

    //YOUR CODE HERE
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
    }
    
    string line;
    string delimiter=" ";//Used to split the string
    while(getline(file,line)){
        double x=stod(line.substr(0,line.find(delimiter)));
        double y=stod(line.substr(line.find(delimiter)+1));
        Point p{x,y};
        points.push_back(p);//Fill the array
    }
    file.close();

    return points;
}

// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    vector<Point> points = readCoordinatesFromFile(argv[1]);
    findClosestPairOrder(points);
    return 0;
}
