/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>


using namespace std;

int main(){


    int numVertices; 
    int numEdages; 
    string city;
    char line[1024] = {0};

    string dir = "../coo_data/col_row.txt";
    std::ifstream colrowifstream(dir.c_str(), std::ios::in);
    if (!colrowifstream) {
        std::cout << "Error : " << dir << " file doesn't exist !" << std::endl;
        exit(1);
    }

    colrowifstream.getline(line, sizeof(line));
    std::stringstream numOdata(line);
    numOdata >> numVertices;
    numOdata >> numEdages;
    numOdata >> city;
    


    
    int * coo_row = new int [numEdages];
    int * coo_col = new int [numEdages];

    
    int * csr_col = new int [numEdages];
    int * csr_row = new int [numVertices+1];

    int index = 0;

    while (colrowifstream.getline(line, sizeof(line))) {
        std::stringstream data(line);
        data >> coo_row[index];
        data >> coo_col[index];
        index++;
    }

    for (int i=0; i < numEdages; ++i){
        csr_col[i] = coo_col[i];
        csr_row[coo_row[i]+1]++;
    }
    for (int i=0;i<numVertices;++i){
        csr_row[i+1] += csr_row[i];
    }
    
    csr_row[numVertices] = numEdages;

    string dir_1 = "../csr_data/offset.mtx";
    ofstream offsetofstream(dir_1.c_str());
    streambuf* p0ld = cout.rdbuf(offsetofstream.rdbuf());

    cout << numVertices << " " << city << endl;
   
    for (int i=0;i<numVertices+1;i++){
      
            cout << csr_row[i] << endl;
        
    }


   
    delete [] coo_row;
    delete [] coo_col;

   
    delete [] csr_col;
    delete [] csr_row;
    
  
}
