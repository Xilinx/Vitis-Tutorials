#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "include.h"
#include "kernels.h"


using namespace adf;

class clipped : public graph {  

  private:
    kernel interpolator;
    kernel classify;
   
  public:
 
adf::input_plio in0,clip_out;
	adf::output_plio  out0,clip_in;
    clipped() {
	in0 = adf::input_plio::create("DataIn1", adf::plio_32_bits,"data/input.txt");
    out0 = adf::output_plio::create("DataOut1",adf::plio_32_bits, "data/output.txt");
	clip_out =  adf::input_plio::create("Data_clip_out0",adf::plio_32_bits, "data/input1.txt");
	clip_in  =  adf::output_plio::create("Data_clip_in0", adf::plio_32_bits,"data/output1.txt");
    interpolator = kernel::create(fir_27t_sym_hb_2i);
    
    classify     = kernel::create(classifier);

      

      connect< window<INTERPOLATOR27_INPUT_BLOCK_SIZE, INTERPOLATOR27_INPUT_MARGIN> >(in0.out[0], interpolator.in[0]);

      connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], clip_in.in[0]);
   
	  connect< stream >(clip_out.out[0], classify.in[0]);
      connect< window<CLASSIFIER_OUTPUT_BLOCK_SIZE> >(classify.out[0], out0.in[0]);
       
	

      source(interpolator) = "hb27_2i.cc";
      
      source(classify)    = "classify.cc";

      runtime<ratio>(interpolator) = 0.9;
      runtime<ratio>(classify) = 0.8;

	 
    };
};

#endif /* __GRAPH_H__ */
