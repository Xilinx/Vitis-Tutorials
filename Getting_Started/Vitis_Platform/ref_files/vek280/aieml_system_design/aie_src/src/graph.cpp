/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "MultGraph.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace adf;

TestMatMult GMatMult;


#if defined(__AIESIM__) || defined(__X86SIM__)

int main(int argc, char ** argv) {

    GMatMult.init();

    cout << "entering the runtime" << endl;
    cout << "Number of iterations : " << NITERATIONS << endl;

    GMatMult.run(NITERATIONS);

    cout << "Finished iterations" << endl;
    GMatMult.end();

    return 0;
}

#endif
