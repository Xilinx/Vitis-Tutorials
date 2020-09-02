
#include "common.h"
#include "constants.h"
#include "filters.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <tuple>

using std::chrono::duration;
using std::chrono::system_clock;
using std::tie;

int main(int argc, char* argv[]) {
    // Parse command line
    arguments opt = parse_args(argc, argv);
    int input_size = opt.width * opt.height * sizeof(RGBPixel);

    FILE *streamIn, *streamOut;
    tie(streamIn, streamOut) = get_streams(opt);

    float* coefficients = gaussian;
    int coefficient_size = 3;

    printf("Processing %d frames of %s ...\n", opt.nframes, opt.input_file);

    auto start = system_clock::now();
    convolve(streamIn, streamOut, coefficients, coefficient_size, opt);
    float elapsed = duration<float>(system_clock::now() - start).count();

    fflush(streamIn);
    fflush(streamOut);

    double mbps = opt.nframes * input_size / 1024. / 1024. / elapsed;
    printf("\n\nProcessed %2.2f MB in %3.3fs (%3.2f MBps)\n\n",
           input_size / 1024. /1024., elapsed, mbps);

    return EXIT_SUCCESS;
}
