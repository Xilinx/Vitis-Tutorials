#include "fft.h"

#define BUF_SIZE 64



int main()
{

	const int input_rows_log2 = 6;
	const int input_cols_log2 = 5;

	const int input_rows = 1 << input_rows_log2;
	const int input_cols = 1 << input_cols_log2;



	//============================================================================
	// Read in test files
	//============================================================================
	char filename[BUF_SIZE] = { 0 };
	FILE *file;

	std::complex<float> input   [input_rows][input_cols];
//	std::complex<float> output  [input_rows][input_cols];
	std::complex<float> expected[input_rows][input_cols];

	// Input
	sprintf(filename, "../../../../fft_input.bin");

	file = fopen(filename, "r");
    if (!file) {
        perror("../../../../fft_input.bin");
        return 1;
    }

	fread(&input, sizeof(input[0][0]), input_rows * input_cols, file);

	fclose(file);

	// Expected output
	sprintf(filename, "../../../../fft_output.bin");

	file = fopen(filename, "r");
    if (!file) {
        perror("../../../../fft_output.bin");
        return 1;
    }

	fread(&expected, sizeof(expected[0][0]), input_rows * input_cols, file);

	fclose(file);



	//============================================================================
	// Drive input and capture output
	//============================================================================
	hls::stream<complex_axis_t> stream_in;
	hls::stream<complex_axis_t> stream_out;

	complex_axis_t output[MAX_ROWS][MAX_COLS];



	for (int i = 0; i < input_cols; i++) {
		for (int j = 0; j < input_rows; j++) {
			complex_axis_t temp;
			temp.data = input[j][i];
			temp.last = 0;
			stream_in << temp;
		}
	}

	myfft(stream_in, input_rows_log2, input_cols_log2, stream_out);

	for (int i = 0; i < input_cols; i++) {
		for (int j = 0; j < input_rows; j++) {
			output[j][i] = stream_out.read();
		}
	}


	//============================================================================
	// Verify output
	//============================================================================
	int err_cnt = 0;

	for (chip_col_t i = 0; i < input_cols; i++) {
		for (chip_row_t j = 0; j < input_rows; j++) {
			if (abs(output[j][i].data - expected[j][i]) > 0.01) {
				err_cnt += 1;
				printf("Error at column %d, row %d: Expected: %f + %fi, Actual: %f + %fi\n",
						int(i),
						int(j),
						float(real(expected[j][i])),
						float(imag(expected[j][i])),
						float(real(output[j][i].data)),
						float(imag(output[j][i].data)));
			}
		}
	}

	if (err_cnt) {
		printf("!!! %d ERRORS ENCOUNTERED - Test Fails !!!\n", err_cnt);
		return 1;
	} else {
		printf("*** Test Passes ***\n");
		return 0;
	}



}
