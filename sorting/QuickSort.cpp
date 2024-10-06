/*
Erol Koçoğlu - 150200107
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <chrono>


typedef struct {
    std::string name;
    int population;
} City;

//Declaration of the functions we use
void insertion_sort(std::vector<City>& arr, int start, int end);
void quicksort(std::vector<City>& arr,int k, int start, int end, std::string strategy, bool v);
int partition(std::vector<City>& arr, int start, int end, std::string strategy, bool v);
void read_file(std::string file_name, std::vector<City>& arr);
void write_file(std::string file_name, std::vector<City>& arr);

int main(int argc, char* argv[]){
    srand(time(NULL));
    std::vector<City> arr;
    std::string input_file_name=argv[1];
    std::string strategy=argv[2];
    int k=atoi(argv[3]);
    std::string output_file_name=argv[4];
    bool v=false;
    std::string v1="v";
    if(argv[5]=="v"){//With this, it gives a warning but get compiled
        v=true;
    }

    read_file(input_file_name,arr);
    const auto start{std::chrono::steady_clock::now()};
    quicksort(arr,k,0,arr.size()-1,strategy,v);
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<long, std::nano> elapsed_time{end-start};
    std::cout<<"Time taken by Quicksort with pivot strategy '"<<strategy<<"' and threshold "<<k<<": "<<elapsed_time.count()<<" ns"<<std::endl;

    write_file(output_file_name,arr);

    return EXIT_SUCCESS;
}

//Implementation of the functions
void insertion_sort(std::vector<City>& arr, int start, int end){
    for(int j=start+1; j<=end; j++){
        City key_city=arr[j];
        int i=j-1;
        while(i>=start && arr[i].population>key_city.population){
            arr[i+1]=arr[i];
            i=i-1;
        }
        arr[i+1]=key_city;
    }
}

void quicksort(std::vector<City>& arr,int k, int start, int end,std::string strategy, bool v){
    if(k==1){//Naive quicksort 
        if(start<end){
            int pivot=partition(arr,start,end,strategy,v);
            quicksort(arr,k,start,pivot-1,strategy,v);
            quicksort(arr,k,pivot+1,end,strategy,v);
        }
    }else{//Hybrid quicksort
        if(start<end){
            if((end-start +1)>k){
                int pivot=partition(arr,start,end,strategy,v);
                quicksort(arr,k,start,pivot-1,strategy,v);
                quicksort(arr,k,pivot+1,end,strategy,v); 
            }else{
                insertion_sort(arr,start,end);
            }
        }
    }
}

int partition(std::vector<City>& arr, int start, int end, std::string strategy, bool v){
    int pivot;
    
    if(strategy=="l"){
        pivot=arr[end].population;
    }else if(strategy=="r"){
        int random=(rand()%(end - start + 1)) + start;
        City temp=arr[random];
        arr[random]=arr[end];
        arr[end]=temp;
        pivot=arr[end].population;
    }else{//For median, sometimes it does not work and gives segmentation fault error
        if(end-start==1){
            pivot=arr[end].population;
        }else{
            std::vector<int> randoms{-1,-1,-1};
            int i=0;
            while(true){
                if(i==3){
                    break;
                }
                int random=(rand()%(end - start + 1)) + start;
                for(int j=0;j<3;j++){//We choose 3 different numbers to find median
                    if(randoms[j]==random){
                        break;
                    }
                    if(j==2){
                        randoms[i]=random;
                        i++;
                    }
                }
            }
            //Find median of three numbers
            //Find min
            City min=arr[randoms[0]];
            if(min.population>arr[randoms[1]].population){
                min=arr[randoms[1]];
            }
            if(min.population>arr[randoms[2]].population){
                min=arr[randoms[2]];
            }

            //Find max
            City max=arr[randoms[2]];
            if(max.population<arr[randoms[1]].population){
                max=arr[randoms[1]];
            }
            if(max.population<arr[randoms[0]].population){
                max=arr[randoms[0]];
            }

            int median;
            for(int i=0;i<3;i++){
                if(arr[randoms[i]].name!=min.name && arr[randoms[i]].name!=max.name){
                    median=randoms[i];//If it is neither min nor max then it is median
                    break;
                }
            }
            City temp=arr[median];
            arr[median]=arr[end];
            arr[end]=temp;
            pivot=arr[end].population;
        }

        
    }

    if(v){
        std::ofstream file("log.txt", std::ios::app);
        std::string values="[";
        for(int i=start;i<end;i++){
            values=values +std::to_string(arr[i].population) +", ";
        }
        values=values +std::to_string(arr[end].population)+"]";
        file<<"Pivot: "<<pivot<<" Array: "<<values<<"\n";
        file.close();

    }
    
    int i=start-1;

    for(int j=start;j<end;j++){
        if(arr[j].population<=pivot){
            i++;
            City temp=arr[i];
            arr[i]=arr[j];
            arr[j]=temp;
        }
    }
    City temp=arr[i+1];
    arr[i+1]=arr[end];
    arr[end]=temp;

    return i+1;

}

void read_file(std::string file_name, std::vector<City>& arr){
    std::ifstream file(file_name);

    if(!file.is_open()){
        std::cout<<"COULD NOT OPEN THE FILE!"<<std::endl;
    }

    std::string name;
    int population;

    std::string line;
    std::string delimiter=";";//Used to split the line

    City temp_city;
    while(getline(file,line)){
        name=line.substr(0, line.find(delimiter));
        population=stoi(line.substr(line.find(delimiter)+1));
        
        temp_city.name=name;
        temp_city.population=population;
        arr.push_back(temp_city);//Fulfill the vector
    }

    file.close();
}

void write_file(std::string file_name, std::vector<City>& arr){
    std::ofstream file(file_name);

    if(!file.is_open()){
        std::cout<<"ERROR OCCURRED WRITING FILE!"<<std::endl;
    }

    for(int i=0;i<arr.size();i++){
        file<<arr[i].name<<";"<<arr[i].population<<"\n";
    }

    file.close();
}