#include <iostream>
#include "adf/adf_api/AIEControlConfig.h"


/************************** Graph Configurations  *****************************/

  adf::GraphConfig GraphConfigurations[] = {
  //{id, name, graphLoadElfFunc, graphInitFunc, graphDebugHalt, coreColumns, coreRows, iterMemColumns, iterMemRows, iterMemAddrs, triggered, plKernelInstanceNames, plAxiLiteModes, plDriverStartFuncs, plDriverCheckIPDoneFuncs}
    {0, "dut", nullptr, nullptr, nullptr, {6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21}, {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, {6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21}, {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, {0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4, 0x17e4}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {}, {}, {}, {},  }, 
  };
  const int NUM_GRAPH = 1;

/************************** PLIO Configurations  *****************************/

  adf::PLIOConfig PLIOConfigurations[] = {
  //{id, name, loginal_name, shim_column, slaveOrMaster, streamId}
    {0, "attr_i_dlbfid0", "dlbfid0", 10, 0, 5},
    {1, "attr_i_dlbfid1", "dlbfid1", 10, 0, 6},
    {2, "attr_i_dlbfid2", "dlbfid2", 8, 0, 4},
    {3, "attr_i_dlbfid3", "dlbfid3", 13, 0, 4},
    {4, "attr_i_dlbfic00", "dlbfic00", 6, 0, 0},
    {5, "attr_i_dlbfic01", "dlbfic01", 7, 0, 0},
    {6, "attr_i_dlbfic02", "dlbfic02", 8, 0, 2},
    {7, "attr_i_dlbfic03", "dlbfic03", 9, 0, 0},
    {8, "attr_i_dlbfic10", "dlbfic10", 6, 0, 6},
    {9, "attr_i_dlbfic11", "dlbfic11", 7, 0, 6},
    {10, "attr_i_dlbfic12", "dlbfic12", 8, 0, 0},
    {11, "attr_i_dlbfic13", "dlbfic13", 9, 0, 6},
    {12, "attr_i_dlbfic20", "dlbfic20", 6, 0, 1},
    {13, "attr_i_dlbfic21", "dlbfic21", 7, 0, 1},
    {14, "attr_i_dlbfic22", "dlbfic22", 8, 0, 6},
    {15, "attr_i_dlbfic23", "dlbfic23", 9, 0, 1},
    {16, "attr_i_dlbfic30", "dlbfic30", 6, 0, 4},
    {17, "attr_i_dlbfic31", "dlbfic31", 7, 0, 4},
    {18, "attr_i_dlbfic32", "dlbfic32", 8, 0, 1},
    {19, "attr_i_dlbfic33", "dlbfic33", 9, 0, 4},
    {20, "attr_i_dlbfic40", "dlbfic40", 10, 0, 0},
    {21, "attr_i_dlbfic41", "dlbfic41", 11, 0, 0},
    {22, "attr_i_dlbfic42", "dlbfic42", 12, 0, 0},
    {23, "attr_i_dlbfic43", "dlbfic43", 13, 0, 2},
    {24, "attr_i_dlbfic50", "dlbfic50", 10, 0, 1},
    {25, "attr_i_dlbfic51", "dlbfic51", 11, 0, 6},
    {26, "attr_i_dlbfic52", "dlbfic52", 12, 0, 6},
    {27, "attr_i_dlbfic53", "dlbfic53", 13, 0, 0},
    {28, "attr_i_dlbfic60", "dlbfic60", 10, 0, 2},
    {29, "attr_i_dlbfic61", "dlbfic61", 11, 0, 1},
    {30, "attr_i_dlbfic62", "dlbfic62", 12, 0, 1},
    {31, "attr_i_dlbfic63", "dlbfic63", 13, 0, 6},
    {32, "attr_i_dlbfic70", "dlbfic70", 10, 0, 4},
    {33, "attr_i_dlbfic71", "dlbfic71", 11, 0, 4},
    {34, "attr_i_dlbfic72", "dlbfic72", 12, 0, 4},
    {35, "attr_i_dlbfic73", "dlbfic73", 13, 0, 1},
    {36, "attr_o_dlbfo0", "dlbfo0", 9, 1, 1},
    {37, "attr_o_dlbfo1", "dlbfo1", 6, 1, 1},
    {38, "attr_o_dlbfo2", "dlbfo2", 9, 1, 3},
    {39, "attr_o_dlbfo3", "dlbfo3", 6, 1, 3},
    {40, "attr_o_dlbfo4", "dlbfo4", 13, 1, 1},
    {41, "attr_o_dlbfo5", "dlbfo5", 10, 1, 1},
    {42, "attr_o_dlbfo6", "dlbfo6", 13, 1, 3},
    {43, "attr_o_dlbfo7", "dlbfo7", 10, 1, 3},
    {44, "attr_i_ulbfid0", "ulbfid0", 14, 0, 4},
    {45, "attr_i_ulbfid1", "ulbfid1", 15, 0, 4},
    {46, "attr_i_ulbfid2", "ulbfid2", 16, 0, 4},
    {47, "attr_i_ulbfid3", "ulbfid3", 17, 0, 4},
    {48, "attr_i_ulbfid4", "ulbfid4", 18, 0, 4},
    {49, "attr_i_ulbfid5", "ulbfid5", 19, 0, 4},
    {50, "attr_i_ulbfid6", "ulbfid6", 20, 0, 4},
    {51, "attr_i_ulbfid7", "ulbfid7", 21, 0, 4},
    {52, "attr_i_ulbfic00", "ulbfic00", 14, 0, 2},
    {53, "attr_i_ulbfic01", "ulbfic01", 15, 0, 2},
    {54, "attr_i_ulbfic02", "ulbfic02", 16, 0, 2},
    {55, "attr_i_ulbfic03", "ulbfic03", 17, 0, 2},
    {56, "attr_i_ulbfic04", "ulbfic04", 18, 0, 2},
    {57, "attr_i_ulbfic05", "ulbfic05", 19, 0, 2},
    {58, "attr_i_ulbfic06", "ulbfic06", 20, 0, 2},
    {59, "attr_i_ulbfic07", "ulbfic07", 21, 0, 2},
    {60, "attr_i_ulbfic10", "ulbfic10", 14, 0, 0},
    {61, "attr_i_ulbfic11", "ulbfic11", 15, 0, 0},
    {62, "attr_i_ulbfic12", "ulbfic12", 16, 0, 0},
    {63, "attr_i_ulbfic13", "ulbfic13", 17, 0, 0},
    {64, "attr_i_ulbfic14", "ulbfic14", 18, 0, 0},
    {65, "attr_i_ulbfic15", "ulbfic15", 19, 0, 0},
    {66, "attr_i_ulbfic16", "ulbfic16", 20, 0, 0},
    {67, "attr_i_ulbfic17", "ulbfic17", 21, 0, 0},
    {68, "attr_i_ulbfic20", "ulbfic20", 14, 0, 6},
    {69, "attr_i_ulbfic21", "ulbfic21", 15, 0, 6},
    {70, "attr_i_ulbfic22", "ulbfic22", 16, 0, 6},
    {71, "attr_i_ulbfic23", "ulbfic23", 17, 0, 6},
    {72, "attr_i_ulbfic24", "ulbfic24", 18, 0, 6},
    {73, "attr_i_ulbfic25", "ulbfic25", 19, 0, 6},
    {74, "attr_i_ulbfic26", "ulbfic26", 20, 0, 6},
    {75, "attr_i_ulbfic27", "ulbfic27", 21, 0, 6},
    {76, "attr_i_ulbfic30", "ulbfic30", 14, 0, 1},
    {77, "attr_i_ulbfic31", "ulbfic31", 15, 0, 1},
    {78, "attr_i_ulbfic32", "ulbfic32", 16, 0, 1},
    {79, "attr_i_ulbfic33", "ulbfic33", 17, 0, 1},
    {80, "attr_i_ulbfic34", "ulbfic34", 18, 0, 1},
    {81, "attr_i_ulbfic35", "ulbfic35", 19, 0, 1},
    {82, "attr_i_ulbfic36", "ulbfic36", 20, 0, 1},
    {83, "attr_i_ulbfic37", "ulbfic37", 21, 0, 1},
    {84, "attr_o_ulbfo0", "ulbfo0", 21, 1, 1},
    {85, "attr_o_ulbfo1", "ulbfo1", 14, 1, 1},
    {86, "attr_o_ulbfo2", "ulbfo2", 21, 1, 3},
    {87, "attr_o_ulbfo3", "ulbfo3", 14, 1, 3},
  };
  const int NUM_PLIO = 88;


/************************** ADF API initializer *****************************/

  class InitializeAIEControlXRT
  {
  public:
    InitializeAIEControlXRT()
    {
      std::cout<<"Initializing ADF API..."<<std::endl;
#ifdef __EXCLUDE_PL_CONTROL__
      bool exclude_pl_control = true;
#else
      bool exclude_pl_control = false;
#endif
      adf::initializeConfigurations(nullptr, 0, 0, 0,
                                    GraphConfigurations, NUM_GRAPH,
                                    nullptr, 0,
                                    nullptr, 0,
                                    nullptr, 0,
                                    nullptr, 0,
                                    nullptr, 0,
                                    nullptr, 0,
                                    PLIOConfigurations, NUM_PLIO,
                                    nullptr, 0, 0, nullptr,
                                    false, exclude_pl_control, false, nullptr,
                                    true, 2);

    }
  } initAIEControlXRT;



#if !defined(__CDO__)

// Kernel Stub Definition
  void bf8x8_fst_api(input_window<cint16> *,input_window<cint16> *,output_stream<cacc48_t> *) { /* Stub */ } 
  void bf8x8_lst_api(input_window<cint16> *,input_window<cint16> *,input_stream<cacc48_t> *,output_window<cint16> *) { /* Stub */ } 
  void bf8x8_mid_api(input_window<cint16> *,input_window<cint16> *,input_stream<cacc48_t> *,output_stream<cacc48_t> *) { /* Stub */ } 
#endif
