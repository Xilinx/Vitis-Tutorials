#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include "fft_top.h"
#include <stdio.h>

// For reading from stimulus input files
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#define BUF_SIZE 64
int main()
{
    const int SIM_FRAMES = 20;
    const int SAMPLES = (1 << FFT_NFFT_MAX);

    int error_num = 0;
    bool ovflo_all = false;
    char res_filename[BUF_SIZE]={0};
    char dat_filename[BUF_SIZE]={0};
    static cmpxDataIn xn_input[SAMPLES];
    static cmpxDataOut xk_output[SAMPLES];

    for (int frame = 0; frame < SIM_FRAMES; ++frame)
    {
        int NFFT = 0;
        int CP_LEN = 0; // length of the cyclic prefix to be inserted for each frame
        int FWD_INV = 0;
        int sc_sch = 0;
        int line_no = 1;
        FILE *stimfile;

        // Open stimulus .dat file for reading
        sprintf(dat_filename, "stimulus_%02d", frame);
        strcat(dat_filename,".dat");
        stimfile = fopen(dat_filename, "r");
        
        int tmp_re, tmp_im;
        float dummy_re, dummy_im;
        const int max = 1 << FFT_INPUT_WIDTH; // might not work for > 32 bits!
        const int max_half_minus_one = (max/2)-1;
        // Scaling factor to get integer into -1 <= x < +1 range 
        const double sc = ldexp(1.0, FFT_INPUT_WIDTH-1); // might not work for > 32 bits!

        if (stimfile == NULL)
        {
            printf("ERROR: Can't open %s\n",dat_filename);
            exit(999);
        }
        else
        {
            printf("INFO: Reading %s\n",dat_filename);
            while (fgetc(stimfile) != EOF && line_no < SAMPLES+5)
            {
                switch (line_no)
                {
                case 1:
                  // Point size
                  fscanf(stimfile,"%X",&NFFT);
                  printf("NFFT %d\n",NFFT);
                  break;
                case 2:
                  // CP length
                  fscanf(stimfile,"%X",&CP_LEN);
                  printf("CP_LEN %d\n",CP_LEN);
                  break;
                case 3:
                  // fwd-inv
                  fscanf(stimfile,"%X",&FWD_INV);
                  printf("FWD_INV %d\n",FWD_INV);
                  break;
                case 4:
                  // Scaling schedule sc_sch
                  fscanf(stimfile,"%X",&sc_sch);
                  printf("sc_sch %X\n",sc_sch);
                  break;
                default:
                    // hex data (first 2 columns)
                    fscanf(stimfile,"%x %x %f %f",&tmp_re,&tmp_im,&dummy_re,&dummy_im);
                    //printf("%x %x\n",tmp_re,tmp_im);

                    double input_data_re, input_data_im;
                    if (tmp_re > max_half_minus_one) {
                      input_data_re = ((tmp_re-65536)/sc);
                    } else {
                      input_data_re = (tmp_re/sc);
                    }
                    //xn_input[line_no-5].re = input_data_re;
                    //xn_re_hw[line_no-5] = dummy_re;

                    if (tmp_im > max_half_minus_one) {
                      input_data_im = ((tmp_im-65536)/sc);
                    } else {
                      input_data_im = (tmp_im/sc);
                    }
                    //xn_input[line_no-5].im = input_data_im;
                    //xn_im_hw[line_no-5] = dummy_im;

                    xn_input[line_no-5] = cmpxDataIn(input_data_re, input_data_im);

                }
                line_no++;
            }
        }
        fclose(stimfile);

        // Simulate the FFT
#if 0
        config_t cfg = sc_sch;
        cfg <<= 1;
        cfg |= (config_t)FWD_INV;
#else
        bool ovflo;

#endif
        fft_top(FWD_INV, xn_input, xk_output, &ovflo);

        ovflo_all |= ovflo;

	    // Open golden results .res file for reading 
        FILE* resfile;
        sprintf(res_filename, "stimulus_%02d", frame);
        strcat(res_filename,".res");
        if ((resfile = fopen(res_filename, "r")) == 0)
        {
            printf("ERROR: Can't open %s\n", res_filename);
            exit(888);
        }

        int tmp;
        fscanf(resfile, "%X", &tmp);
        fscanf(resfile, "%X", &tmp);
        for (int i = 0; i < (1<<NFFT); i++)
        {
            fscanf(resfile,"%x %x %f %f", &tmp_re, &tmp_im, &dummy_re, &dummy_im);
            data_out_t golden = dummy_re;
            //if (golden != xk_output[i].re)
            if (golden != xk_output[i].real())
            {
                error_num++;
                cout << "Frame:" << frame << " index: " << i 
                     << "  Golden: " <<  golden.to_float() << " vs. RE Output: " << setprecision(14) << xk_output[i].real().to_float() << endl;
            }
            golden = dummy_im;
            //if (golden != xk_output[i].im)
            if (golden != xk_output[i].imag())
            {
                error_num++;
                cout << "Frame:" << frame << " index: " << i 
                     << "  Golden: " << golden.to_float() << " vs. IM Output: " << setprecision(14) << xk_output[i].imag().to_float() << endl;
            }
        }
        fclose(resfile);
    }

    cout << " ERRORS: " << error_num << endl;
    if (error_num > 0)
        cout << " (FAILED!!!)" << endl;
    else if (ovflo_all)   
        cout << " (OVERFLOW!!!)" << endl;
    else
        cout << " (PASSED!!!)" << endl;

    return error_num > 0;
}
