//
// Copyright 2022 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


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
