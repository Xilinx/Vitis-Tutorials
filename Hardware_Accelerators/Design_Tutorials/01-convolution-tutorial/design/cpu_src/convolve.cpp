
#include "common.h"
#include "constants.h"
#include "kernels.h"

#include <vector>
#include <cstdio>

using std::vector;

bool operator==(const RGBPixel& lhs, const RGBPixel& rhs) {
    return lhs.r == rhs.r &&
           lhs.g == rhs.g &&
           lhs.b == rhs.b;
}

void
test(vector<RGBPixel>& in, vector<RGBPixel>& out,
     float* coefficients, int coefficient_size,
     int width, int height) {
    vector<RGBPixel> gold(out.size());
    convolve_cpu(in.data(), gold.data(), coefficients, coefficient_size, width, height);
    auto it = mismatch(begin(gold), end(gold), begin(out));
    if(it.first != end(gold)) {
        printf("Incorrect result: \n Expected: (%d %d %d)\nResult:  (%d %d %d)\n ",
               it.first->r, it.first->g, it.first->b, it.second->r, it.second->g, it.second->b);
    }
}

void convolve(FILE* streamIn, FILE* streamOut,
              float* coefficients, int coefficient_size,
              arguments args) {
    size_t frame_bytes = args.width * args.height * sizeof(RGBPixel);
    size_t gray_frame_bytes = args.width * args.height * sizeof(GrayPixel);
    vector<RGBPixel> inFrame(args.width * args.height);
    vector<RGBPixel> outFrame(args.width * args.height);
    vector<GrayPixel> grayFrame(args.width * args.height);

    size_t bytes_read = 0;
    size_t bytes_written = 0;
    for(int frame_count = 0; frame_count < args.nframes; frame_count++) {
        // Read frame
        bytes_read = fread(inFrame.data(), 1, frame_bytes, streamIn);
        if(bytes_read != frame_bytes) {
        	printf("\nError: partial frame.\nExpected %zu\nActual %zu\n", frame_bytes, bytes_read);
        	break;
        }

        convolve_cpu(inFrame.data(), outFrame.data(),
                     coefficients, coefficient_size,
                     args.width, args.height);

        if(args.gray) {
          grayscale_cpu(outFrame.data(), grayFrame.data(), args.width, args.height);
          bytes_written = fwrite(outFrame.data(), 1, gray_frame_bytes, streamOut);
          fflush(streamOut);
          if (bytes_written != gray_frame_bytes) {
            printf("\nError: partial frame.\nExpected %zu\nActual %zu\n",
                   gray_frame_bytes, bytes_written);
            break;
          }
        } else {
          bytes_written = fwrite(outFrame.data(), 1, frame_bytes, streamOut);
          fflush(streamOut);
          if (bytes_written != frame_bytes) {
            printf("\nError: partial frame.\nExpected %zu\nActual %zu\n",
                   frame_bytes, bytes_written);
            break;
          }
          // test(inFrame, outFrame, coefficients, coefficient_size, width, height);
        }

        print_progress(frame_count, args.nframes);
    }
}
