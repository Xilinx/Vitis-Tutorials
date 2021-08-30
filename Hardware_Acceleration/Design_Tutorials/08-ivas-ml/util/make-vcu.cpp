/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: make-vcu <input_file.nv12>" << std::abort return EXIT_SUCCESS;
    }

    std::cout << "Converting " << argv[1] << std::endl;

    std::ifstream fin(argv[1], std::ifstream::binary);
    std::ofstream fout("out.nv12", std::ofstream::binary);

    if (!fin || !fout) {
        std::cerr << "WOOPS!" << std::endl;
        return EXIT_FAILURE;
    }
    fin.seekg(0, fin.end);
    size_t size    = fin.tellg();
    char *memblock = new char[size];
    fin.seekg(0, fin.beg);

    if (!memblock) {
        fin.close();
        fout.close();
        return EXIT_FAILURE;
    }

    fin.read(memblock, size);

    for (int y = 0; y < 1088; y++) {
        for (int x = 0; x < 2048; x++) {
            if (x < 1920 && y < 1080) {
                fout << memblock[1920 * y + x];
            }
            else {
                fout << 0;
            }
        }
    }

    for (int y = 0; y < (1088 / 2); y++) {
        for (int x = 0; x < (2048); x++) {
            if (x < (1920) && y < (1080 / 2)) {
                fout << memblock[1920 * 1080 + (1920) * y + x];
            }
            else {
                fout << 0;
            }
        }
    }

    fin.close();
    fout.close();

    return EXIT_SUCCESS;
}
