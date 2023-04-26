/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "nbody_x4_x100.h"

using namespace adf;
#define COL_OFFSET 0


//input j PLIO
PLIO *in_j  = new PLIO("in_j",  adf::plio_32_bits, "../../data/input_j.txt", 400);
        
//input i PLIOs
PLIO *in_i0  = new PLIO("in_i0",  adf::plio_32_bits, "../../data/input_i0.txt", 400);
PLIO *in_i1  = new PLIO("in_i1",  adf::plio_32_bits, "../../data/input_i1.txt", 400);
PLIO *in_i2  = new PLIO("in_i2",  adf::plio_32_bits, "../../data/input_i2.txt", 400);
PLIO *in_i3  = new PLIO("in_i3",  adf::plio_32_bits, "../../data/input_i3.txt", 400);
PLIO *in_i4  = new PLIO("in_i4",  adf::plio_32_bits, "../../data/input_i4.txt", 400);
PLIO *in_i5  = new PLIO("in_i5",  adf::plio_32_bits, "../../data/input_i5.txt", 400);
PLIO *in_i6  = new PLIO("in_i6",  adf::plio_32_bits, "../../data/input_i6.txt", 400);
PLIO *in_i7  = new PLIO("in_i7",  adf::plio_32_bits, "../../data/input_i7.txt", 400);
PLIO *in_i8  = new PLIO("in_i8",  adf::plio_32_bits, "../../data/input_i8.txt", 400);
PLIO *in_i9  = new PLIO("in_i9",  adf::plio_32_bits, "../../data/input_i9.txt", 400);
PLIO *in_i10  = new PLIO("in_i10",  adf::plio_32_bits, "../../data/input_i10.txt", 400);
PLIO *in_i11  = new PLIO("in_i11",  adf::plio_32_bits, "../../data/input_i11.txt", 400);
PLIO *in_i12  = new PLIO("in_i12",  adf::plio_32_bits, "../../data/input_i12.txt", 400);
PLIO *in_i13  = new PLIO("in_i13",  adf::plio_32_bits, "../../data/input_i13.txt", 400);
PLIO *in_i14  = new PLIO("in_i14",  adf::plio_32_bits, "../../data/input_i14.txt", 400);
PLIO *in_i15  = new PLIO("in_i15",  adf::plio_32_bits, "../../data/input_i15.txt", 400);
PLIO *in_i16  = new PLIO("in_i16",  adf::plio_32_bits, "../../data/input_i16.txt", 400);
PLIO *in_i17  = new PLIO("in_i17",  adf::plio_32_bits, "../../data/input_i17.txt", 400);
PLIO *in_i18  = new PLIO("in_i18",  adf::plio_32_bits, "../../data/input_i18.txt", 400);
PLIO *in_i19  = new PLIO("in_i19",  adf::plio_32_bits, "../../data/input_i19.txt", 400);
PLIO *in_i20  = new PLIO("in_i20",  adf::plio_32_bits, "../../data/input_i20.txt", 400);
PLIO *in_i21  = new PLIO("in_i21",  adf::plio_32_bits, "../../data/input_i21.txt", 400);
PLIO *in_i22  = new PLIO("in_i22",  adf::plio_32_bits, "../../data/input_i22.txt", 400);
PLIO *in_i23  = new PLIO("in_i23",  adf::plio_32_bits, "../../data/input_i23.txt", 400);
PLIO *in_i24  = new PLIO("in_i24",  adf::plio_32_bits, "../../data/input_i24.txt", 400);
PLIO *in_i25  = new PLIO("in_i25",  adf::plio_32_bits, "../../data/input_i25.txt", 400);
PLIO *in_i26  = new PLIO("in_i26",  adf::plio_32_bits, "../../data/input_i26.txt", 400);
PLIO *in_i27  = new PLIO("in_i27",  adf::plio_32_bits, "../../data/input_i27.txt", 400);
PLIO *in_i28  = new PLIO("in_i28",  adf::plio_32_bits, "../../data/input_i28.txt", 400);
PLIO *in_i29  = new PLIO("in_i29",  adf::plio_32_bits, "../../data/input_i29.txt", 400);
PLIO *in_i30  = new PLIO("in_i30",  adf::plio_32_bits, "../../data/input_i30.txt", 400);
PLIO *in_i31  = new PLIO("in_i31",  adf::plio_32_bits, "../../data/input_i31.txt", 400);
PLIO *in_i32  = new PLIO("in_i32",  adf::plio_32_bits, "../../data/input_i32.txt", 400);
PLIO *in_i33  = new PLIO("in_i33",  adf::plio_32_bits, "../../data/input_i33.txt", 400);
PLIO *in_i34  = new PLIO("in_i34",  adf::plio_32_bits, "../../data/input_i34.txt", 400);
PLIO *in_i35  = new PLIO("in_i35",  adf::plio_32_bits, "../../data/input_i35.txt", 400);
PLIO *in_i36  = new PLIO("in_i36",  adf::plio_32_bits, "../../data/input_i36.txt", 400);
PLIO *in_i37  = new PLIO("in_i37",  adf::plio_32_bits, "../../data/input_i37.txt", 400);
PLIO *in_i38  = new PLIO("in_i38",  adf::plio_32_bits, "../../data/input_i38.txt", 400);
PLIO *in_i39  = new PLIO("in_i39",  adf::plio_32_bits, "../../data/input_i39.txt", 400);
PLIO *in_i40  = new PLIO("in_i40",  adf::plio_32_bits, "../../data/input_i40.txt", 400);
PLIO *in_i41  = new PLIO("in_i41",  adf::plio_32_bits, "../../data/input_i41.txt", 400);
PLIO *in_i42  = new PLIO("in_i42",  adf::plio_32_bits, "../../data/input_i42.txt", 400);
PLIO *in_i43  = new PLIO("in_i43",  adf::plio_32_bits, "../../data/input_i43.txt", 400);
PLIO *in_i44  = new PLIO("in_i44",  adf::plio_32_bits, "../../data/input_i44.txt", 400);
PLIO *in_i45  = new PLIO("in_i45",  adf::plio_32_bits, "../../data/input_i45.txt", 400);
PLIO *in_i46  = new PLIO("in_i46",  adf::plio_32_bits, "../../data/input_i46.txt", 400);
PLIO *in_i47  = new PLIO("in_i47",  adf::plio_32_bits, "../../data/input_i47.txt", 400);
PLIO *in_i48  = new PLIO("in_i48",  adf::plio_32_bits, "../../data/input_i48.txt", 400);
PLIO *in_i49  = new PLIO("in_i49",  adf::plio_32_bits, "../../data/input_i49.txt", 400);
PLIO *in_i50  = new PLIO("in_i50",  adf::plio_32_bits, "../../data/input_i50.txt", 400);
PLIO *in_i51  = new PLIO("in_i51",  adf::plio_32_bits, "../../data/input_i51.txt", 400);
PLIO *in_i52  = new PLIO("in_i52",  adf::plio_32_bits, "../../data/input_i52.txt", 400);
PLIO *in_i53  = new PLIO("in_i53",  adf::plio_32_bits, "../../data/input_i53.txt", 400);
PLIO *in_i54  = new PLIO("in_i54",  adf::plio_32_bits, "../../data/input_i54.txt", 400);
PLIO *in_i55  = new PLIO("in_i55",  adf::plio_32_bits, "../../data/input_i55.txt", 400);
PLIO *in_i56  = new PLIO("in_i56",  adf::plio_32_bits, "../../data/input_i56.txt", 400);
PLIO *in_i57  = new PLIO("in_i57",  adf::plio_32_bits, "../../data/input_i57.txt", 400);
PLIO *in_i58  = new PLIO("in_i58",  adf::plio_32_bits, "../../data/input_i58.txt", 400);
PLIO *in_i59  = new PLIO("in_i59",  adf::plio_32_bits, "../../data/input_i59.txt", 400);
PLIO *in_i60  = new PLIO("in_i60",  adf::plio_32_bits, "../../data/input_i60.txt", 400);
PLIO *in_i61  = new PLIO("in_i61",  adf::plio_32_bits, "../../data/input_i61.txt", 400);
PLIO *in_i62  = new PLIO("in_i62",  adf::plio_32_bits, "../../data/input_i62.txt", 400);
PLIO *in_i63  = new PLIO("in_i63",  adf::plio_32_bits, "../../data/input_i63.txt", 400);
PLIO *in_i64  = new PLIO("in_i64",  adf::plio_32_bits, "../../data/input_i64.txt", 400);
PLIO *in_i65  = new PLIO("in_i65",  adf::plio_32_bits, "../../data/input_i65.txt", 400);
PLIO *in_i66  = new PLIO("in_i66",  adf::plio_32_bits, "../../data/input_i66.txt", 400);
PLIO *in_i67  = new PLIO("in_i67",  adf::plio_32_bits, "../../data/input_i67.txt", 400);
PLIO *in_i68  = new PLIO("in_i68",  adf::plio_32_bits, "../../data/input_i68.txt", 400);
PLIO *in_i69  = new PLIO("in_i69",  adf::plio_32_bits, "../../data/input_i69.txt", 400);
PLIO *in_i70  = new PLIO("in_i70",  adf::plio_32_bits, "../../data/input_i70.txt", 400);
PLIO *in_i71  = new PLIO("in_i71",  adf::plio_32_bits, "../../data/input_i71.txt", 400);
PLIO *in_i72  = new PLIO("in_i72",  adf::plio_32_bits, "../../data/input_i72.txt", 400);
PLIO *in_i73  = new PLIO("in_i73",  adf::plio_32_bits, "../../data/input_i73.txt", 400);
PLIO *in_i74  = new PLIO("in_i74",  adf::plio_32_bits, "../../data/input_i74.txt", 400);
PLIO *in_i75  = new PLIO("in_i75",  adf::plio_32_bits, "../../data/input_i75.txt", 400);
PLIO *in_i76  = new PLIO("in_i76",  adf::plio_32_bits, "../../data/input_i76.txt", 400);
PLIO *in_i77  = new PLIO("in_i77",  adf::plio_32_bits, "../../data/input_i77.txt", 400);
PLIO *in_i78  = new PLIO("in_i78",  adf::plio_32_bits, "../../data/input_i78.txt", 400);
PLIO *in_i79  = new PLIO("in_i79",  adf::plio_32_bits, "../../data/input_i79.txt", 400);
PLIO *in_i80  = new PLIO("in_i80",  adf::plio_32_bits, "../../data/input_i80.txt", 400);
PLIO *in_i81  = new PLIO("in_i81",  adf::plio_32_bits, "../../data/input_i81.txt", 400);
PLIO *in_i82  = new PLIO("in_i82",  adf::plio_32_bits, "../../data/input_i82.txt", 400);
PLIO *in_i83  = new PLIO("in_i83",  adf::plio_32_bits, "../../data/input_i83.txt", 400);
PLIO *in_i84  = new PLIO("in_i84",  adf::plio_32_bits, "../../data/input_i84.txt", 400);
PLIO *in_i85  = new PLIO("in_i85",  adf::plio_32_bits, "../../data/input_i85.txt", 400);
PLIO *in_i86  = new PLIO("in_i86",  adf::plio_32_bits, "../../data/input_i86.txt", 400);
PLIO *in_i87  = new PLIO("in_i87",  adf::plio_32_bits, "../../data/input_i87.txt", 400);
PLIO *in_i88  = new PLIO("in_i88",  adf::plio_32_bits, "../../data/input_i88.txt", 400);
PLIO *in_i89  = new PLIO("in_i89",  adf::plio_32_bits, "../../data/input_i89.txt", 400);
PLIO *in_i90  = new PLIO("in_i90",  adf::plio_32_bits, "../../data/input_i90.txt", 400);
PLIO *in_i91  = new PLIO("in_i91",  adf::plio_32_bits, "../../data/input_i91.txt", 400);
PLIO *in_i92  = new PLIO("in_i92",  adf::plio_32_bits, "../../data/input_i92.txt", 400);
PLIO *in_i93  = new PLIO("in_i93",  adf::plio_32_bits, "../../data/input_i93.txt", 400);
PLIO *in_i94  = new PLIO("in_i94",  adf::plio_32_bits, "../../data/input_i94.txt", 400);
PLIO *in_i95  = new PLIO("in_i95",  adf::plio_32_bits, "../../data/input_i95.txt", 400);
PLIO *in_i96  = new PLIO("in_i96",  adf::plio_32_bits, "../../data/input_i96.txt", 400);
PLIO *in_i97  = new PLIO("in_i97",  adf::plio_32_bits, "../../data/input_i97.txt", 400);
PLIO *in_i98  = new PLIO("in_i98",  adf::plio_32_bits, "../../data/input_i98.txt", 400);
PLIO *in_i99  = new PLIO("in_i99",  adf::plio_32_bits, "../../data/input_i99.txt", 400);

//output PLIOs
PLIO *out_i0 = new PLIO("out_i0", adf::plio_32_bits, "./data/output_i0.txt", 400);
PLIO *out_i1 = new PLIO("out_i1", adf::plio_32_bits, "./data/output_i1.txt", 400);
PLIO *out_i2 = new PLIO("out_i2", adf::plio_32_bits, "./data/output_i2.txt", 400);
PLIO *out_i3 = new PLIO("out_i3", adf::plio_32_bits, "./data/output_i3.txt", 400);
PLIO *out_i4 = new PLIO("out_i4", adf::plio_32_bits, "./data/output_i4.txt", 400);
PLIO *out_i5 = new PLIO("out_i5", adf::plio_32_bits, "./data/output_i5.txt", 400);
PLIO *out_i6 = new PLIO("out_i6", adf::plio_32_bits, "./data/output_i6.txt", 400);
PLIO *out_i7 = new PLIO("out_i7", adf::plio_32_bits, "./data/output_i7.txt", 400);
PLIO *out_i8 = new PLIO("out_i8", adf::plio_32_bits, "./data/output_i8.txt", 400);
PLIO *out_i9 = new PLIO("out_i9", adf::plio_32_bits, "./data/output_i9.txt", 400);
PLIO *out_i10 = new PLIO("out_i10", adf::plio_32_bits, "./data/output_i10.txt", 400);
PLIO *out_i11 = new PLIO("out_i11", adf::plio_32_bits, "./data/output_i11.txt", 400);
PLIO *out_i12 = new PLIO("out_i12", adf::plio_32_bits, "./data/output_i12.txt", 400);
PLIO *out_i13 = new PLIO("out_i13", adf::plio_32_bits, "./data/output_i13.txt", 400);
PLIO *out_i14 = new PLIO("out_i14", adf::plio_32_bits, "./data/output_i14.txt", 400);
PLIO *out_i15 = new PLIO("out_i15", adf::plio_32_bits, "./data/output_i15.txt", 400);
PLIO *out_i16 = new PLIO("out_i16", adf::plio_32_bits, "./data/output_i16.txt", 400);
PLIO *out_i17 = new PLIO("out_i17", adf::plio_32_bits, "./data/output_i17.txt", 400);
PLIO *out_i18 = new PLIO("out_i18", adf::plio_32_bits, "./data/output_i18.txt", 400);
PLIO *out_i19 = new PLIO("out_i19", adf::plio_32_bits, "./data/output_i19.txt", 400);
PLIO *out_i20 = new PLIO("out_i20", adf::plio_32_bits, "./data/output_i20.txt", 400);
PLIO *out_i21 = new PLIO("out_i21", adf::plio_32_bits, "./data/output_i21.txt", 400);
PLIO *out_i22 = new PLIO("out_i22", adf::plio_32_bits, "./data/output_i22.txt", 400);
PLIO *out_i23 = new PLIO("out_i23", adf::plio_32_bits, "./data/output_i23.txt", 400);
PLIO *out_i24 = new PLIO("out_i24", adf::plio_32_bits, "./data/output_i24.txt", 400);
PLIO *out_i25 = new PLIO("out_i25", adf::plio_32_bits, "./data/output_i25.txt", 400);
PLIO *out_i26 = new PLIO("out_i26", adf::plio_32_bits, "./data/output_i26.txt", 400);
PLIO *out_i27 = new PLIO("out_i27", adf::plio_32_bits, "./data/output_i27.txt", 400);
PLIO *out_i28 = new PLIO("out_i28", adf::plio_32_bits, "./data/output_i28.txt", 400);
PLIO *out_i29 = new PLIO("out_i29", adf::plio_32_bits, "./data/output_i29.txt", 400);
PLIO *out_i30 = new PLIO("out_i30", adf::plio_32_bits, "./data/output_i30.txt", 400);
PLIO *out_i31 = new PLIO("out_i31", adf::plio_32_bits, "./data/output_i31.txt", 400);
PLIO *out_i32 = new PLIO("out_i32", adf::plio_32_bits, "./data/output_i32.txt", 400);
PLIO *out_i33 = new PLIO("out_i33", adf::plio_32_bits, "./data/output_i33.txt", 400);
PLIO *out_i34 = new PLIO("out_i34", adf::plio_32_bits, "./data/output_i34.txt", 400);
PLIO *out_i35 = new PLIO("out_i35", adf::plio_32_bits, "./data/output_i35.txt", 400);
PLIO *out_i36 = new PLIO("out_i36", adf::plio_32_bits, "./data/output_i36.txt", 400);
PLIO *out_i37 = new PLIO("out_i37", adf::plio_32_bits, "./data/output_i37.txt", 400);
PLIO *out_i38 = new PLIO("out_i38", adf::plio_32_bits, "./data/output_i38.txt", 400);
PLIO *out_i39 = new PLIO("out_i39", adf::plio_32_bits, "./data/output_i39.txt", 400);
PLIO *out_i40 = new PLIO("out_i40", adf::plio_32_bits, "./data/output_i40.txt", 400);
PLIO *out_i41 = new PLIO("out_i41", adf::plio_32_bits, "./data/output_i41.txt", 400);
PLIO *out_i42 = new PLIO("out_i42", adf::plio_32_bits, "./data/output_i42.txt", 400);
PLIO *out_i43 = new PLIO("out_i43", adf::plio_32_bits, "./data/output_i43.txt", 400);
PLIO *out_i44 = new PLIO("out_i44", adf::plio_32_bits, "./data/output_i44.txt", 400);
PLIO *out_i45 = new PLIO("out_i45", adf::plio_32_bits, "./data/output_i45.txt", 400);
PLIO *out_i46 = new PLIO("out_i46", adf::plio_32_bits, "./data/output_i46.txt", 400);
PLIO *out_i47 = new PLIO("out_i47", adf::plio_32_bits, "./data/output_i47.txt", 400);
PLIO *out_i48 = new PLIO("out_i48", adf::plio_32_bits, "./data/output_i48.txt", 400);
PLIO *out_i49 = new PLIO("out_i49", adf::plio_32_bits, "./data/output_i49.txt", 400);
PLIO *out_i50 = new PLIO("out_i50", adf::plio_32_bits, "./data/output_i50.txt", 400);
PLIO *out_i51 = new PLIO("out_i51", adf::plio_32_bits, "./data/output_i51.txt", 400);
PLIO *out_i52 = new PLIO("out_i52", adf::plio_32_bits, "./data/output_i52.txt", 400);
PLIO *out_i53 = new PLIO("out_i53", adf::plio_32_bits, "./data/output_i53.txt", 400);
PLIO *out_i54 = new PLIO("out_i54", adf::plio_32_bits, "./data/output_i54.txt", 400);
PLIO *out_i55 = new PLIO("out_i55", adf::plio_32_bits, "./data/output_i55.txt", 400);
PLIO *out_i56 = new PLIO("out_i56", adf::plio_32_bits, "./data/output_i56.txt", 400);
PLIO *out_i57 = new PLIO("out_i57", adf::plio_32_bits, "./data/output_i57.txt", 400);
PLIO *out_i58 = new PLIO("out_i58", adf::plio_32_bits, "./data/output_i58.txt", 400);
PLIO *out_i59 = new PLIO("out_i59", adf::plio_32_bits, "./data/output_i59.txt", 400);
PLIO *out_i60 = new PLIO("out_i60", adf::plio_32_bits, "./data/output_i60.txt", 400);
PLIO *out_i61 = new PLIO("out_i61", adf::plio_32_bits, "./data/output_i61.txt", 400);
PLIO *out_i62 = new PLIO("out_i62", adf::plio_32_bits, "./data/output_i62.txt", 400);
PLIO *out_i63 = new PLIO("out_i63", adf::plio_32_bits, "./data/output_i63.txt", 400);
PLIO *out_i64 = new PLIO("out_i64", adf::plio_32_bits, "./data/output_i64.txt", 400);
PLIO *out_i65 = new PLIO("out_i65", adf::plio_32_bits, "./data/output_i65.txt", 400);
PLIO *out_i66 = new PLIO("out_i66", adf::plio_32_bits, "./data/output_i66.txt", 400);
PLIO *out_i67 = new PLIO("out_i67", adf::plio_32_bits, "./data/output_i67.txt", 400);
PLIO *out_i68 = new PLIO("out_i68", adf::plio_32_bits, "./data/output_i68.txt", 400);
PLIO *out_i69 = new PLIO("out_i69", adf::plio_32_bits, "./data/output_i69.txt", 400);
PLIO *out_i70 = new PLIO("out_i70", adf::plio_32_bits, "./data/output_i70.txt", 400);
PLIO *out_i71 = new PLIO("out_i71", adf::plio_32_bits, "./data/output_i71.txt", 400);
PLIO *out_i72 = new PLIO("out_i72", adf::plio_32_bits, "./data/output_i72.txt", 400);
PLIO *out_i73 = new PLIO("out_i73", adf::plio_32_bits, "./data/output_i73.txt", 400);
PLIO *out_i74 = new PLIO("out_i74", adf::plio_32_bits, "./data/output_i74.txt", 400);
PLIO *out_i75 = new PLIO("out_i75", adf::plio_32_bits, "./data/output_i75.txt", 400);
PLIO *out_i76 = new PLIO("out_i76", adf::plio_32_bits, "./data/output_i76.txt", 400);
PLIO *out_i77 = new PLIO("out_i77", adf::plio_32_bits, "./data/output_i77.txt", 400);
PLIO *out_i78 = new PLIO("out_i78", adf::plio_32_bits, "./data/output_i78.txt", 400);
PLIO *out_i79 = new PLIO("out_i79", adf::plio_32_bits, "./data/output_i79.txt", 400);
PLIO *out_i80 = new PLIO("out_i80", adf::plio_32_bits, "./data/output_i80.txt", 400);
PLIO *out_i81 = new PLIO("out_i81", adf::plio_32_bits, "./data/output_i81.txt", 400);
PLIO *out_i82 = new PLIO("out_i82", adf::plio_32_bits, "./data/output_i82.txt", 400);
PLIO *out_i83 = new PLIO("out_i83", adf::plio_32_bits, "./data/output_i83.txt", 400);
PLIO *out_i84 = new PLIO("out_i84", adf::plio_32_bits, "./data/output_i84.txt", 400);
PLIO *out_i85 = new PLIO("out_i85", adf::plio_32_bits, "./data/output_i85.txt", 400);
PLIO *out_i86 = new PLIO("out_i86", adf::plio_32_bits, "./data/output_i86.txt", 400);
PLIO *out_i87 = new PLIO("out_i87", adf::plio_32_bits, "./data/output_i87.txt", 400);
PLIO *out_i88 = new PLIO("out_i88", adf::plio_32_bits, "./data/output_i88.txt", 400);
PLIO *out_i89 = new PLIO("out_i89", adf::plio_32_bits, "./data/output_i89.txt", 400);
PLIO *out_i90 = new PLIO("out_i90", adf::plio_32_bits, "./data/output_i90.txt", 400);
PLIO *out_i91 = new PLIO("out_i91", adf::plio_32_bits, "./data/output_i91.txt", 400);
PLIO *out_i92 = new PLIO("out_i92", adf::plio_32_bits, "./data/output_i92.txt", 400);
PLIO *out_i93 = new PLIO("out_i93", adf::plio_32_bits, "./data/output_i93.txt", 400);
PLIO *out_i94 = new PLIO("out_i94", adf::plio_32_bits, "./data/output_i94.txt", 400);
PLIO *out_i95 = new PLIO("out_i95", adf::plio_32_bits, "./data/output_i95.txt", 400);
PLIO *out_i96 = new PLIO("out_i96", adf::plio_32_bits, "./data/output_i96.txt", 400);
PLIO *out_i97 = new PLIO("out_i97", adf::plio_32_bits, "./data/output_i97.txt", 400);
PLIO *out_i98 = new PLIO("out_i98", adf::plio_32_bits, "./data/output_i98.txt", 400);
PLIO *out_i99 = new PLIO("out_i99", adf::plio_32_bits, "./data/output_i99.txt", 400);

nbodySystem<COL_OFFSET> myGraph;

simulation::platform<101,100> platform(in_j,
                                   in_i0,
                                   in_i1,
                                   in_i2,
                                   in_i3,
                                   in_i4,
                                   in_i5,
                                   in_i6,
                                   in_i7,
                                   in_i8,
                                   in_i9,
                                   in_i10,
                                   in_i11,
                                   in_i12,
                                   in_i13,
                                   in_i14,
                                   in_i15,
                                   in_i16,
                                   in_i17,
                                   in_i18,
                                   in_i19,
                                   in_i20,
                                   in_i21,
                                   in_i22,
                                   in_i23,
                                   in_i24,
                                   in_i25,
                                   in_i26,
                                   in_i27,
                                   in_i28,
                                   in_i29,
                                   in_i30,
                                   in_i31,
                                   in_i32,
                                   in_i33,
                                   in_i34,
                                   in_i35,
                                   in_i36,
                                   in_i37,
                                   in_i38,
                                   in_i39,
                                   in_i40,
                                   in_i41,
                                   in_i42,
                                   in_i43,
                                   in_i44,
                                   in_i45,
                                   in_i46,
                                   in_i47,
                                   in_i48,
                                   in_i49,
                                   in_i50,
                                   in_i51,
                                   in_i52,
                                   in_i53,
                                   in_i54,
                                   in_i55,
                                   in_i56,
                                   in_i57,
                                   in_i58,
                                   in_i59,
                                   in_i60,
                                   in_i61,
                                   in_i62,
                                   in_i63,
                                   in_i64,
                                   in_i65,
                                   in_i66,
                                   in_i67,
                                   in_i68,
                                   in_i69,
                                   in_i70,
                                   in_i71,
                                   in_i72,
                                   in_i73,
                                   in_i74,
                                   in_i75,
                                   in_i76,
                                   in_i77,
                                   in_i78,
                                   in_i79,
                                   in_i80,
                                   in_i81,
                                   in_i82,
                                   in_i83,
                                   in_i84,
                                   in_i85,
                                   in_i86,
                                   in_i87,
                                   in_i88,
                                   in_i89,
                                   in_i90,
                                   in_i91,
                                   in_i92,
                                   in_i93,
                                   in_i94,
                                   in_i95,
                                   in_i96,
                                   in_i97,
                                   in_i98,
                                   in_i99,
                                   out_i0,
                                   out_i1,
                                   out_i2,
                                   out_i3,
                                   out_i4,
                                   out_i5,
                                   out_i6,
                                   out_i7,
                                   out_i8,
                                   out_i9,
                                   out_i10,
                                   out_i11,
                                   out_i12,
                                   out_i13,
                                   out_i14,
                                   out_i15,
                                   out_i16,
                                   out_i17,
                                   out_i18,
                                   out_i19,
                                   out_i20,
                                   out_i21,
                                   out_i22,
                                   out_i23,
                                   out_i24,
                                   out_i25,
                                   out_i26,
                                   out_i27,
                                   out_i28,
                                   out_i29,
                                   out_i30,
                                   out_i31,
                                   out_i32,
                                   out_i33,
                                   out_i34,
                                   out_i35,
                                   out_i36,
                                   out_i37,
                                   out_i38,
                                   out_i39,
                                   out_i40,
                                   out_i41,
                                   out_i42,
                                   out_i43,
                                   out_i44,
                                   out_i45,
                                   out_i46,
                                   out_i47,
                                   out_i48,
                                   out_i49,
                                   out_i50,
                                   out_i51,
                                   out_i52,
                                   out_i53,
                                   out_i54,
                                   out_i55,
                                   out_i56,
                                   out_i57,
                                   out_i58,
                                   out_i59,
                                   out_i60,
                                   out_i61,
                                   out_i62,
                                   out_i63,
                                   out_i64,
                                   out_i65,
                                   out_i66,
                                   out_i67,
                                   out_i68,
                                   out_i69,
                                   out_i70,
                                   out_i71,
                                   out_i72,
                                   out_i73,
                                   out_i74,
                                   out_i75,
                                   out_i76,
                                   out_i77,
                                   out_i78,
                                   out_i79,
                                   out_i80,
                                   out_i81,
                                   out_i82,
                                   out_i83,
                                   out_i84,
                                   out_i85,
                                   out_i86,
                                   out_i87,
                                   out_i88,
                                   out_i89,
                                   out_i90,
                                   out_i91,
                                   out_i92,
                                   out_i93,
                                   out_i94,
                                   out_i95,
                                   out_i96,
                                   out_i97,
                                   out_i98,
                                   out_i99);

//Connect platform to graph's j input
connect<> netij(platform.src[0], myGraph.in_j);

//Connect platform to graph's i inputs
connect<> netii0(platform.src[1], myGraph.in_i[0]);
connect<> netii1(platform.src[2], myGraph.in_i[1]);
connect<> netii2(platform.src[3], myGraph.in_i[2]);
connect<> netii3(platform.src[4], myGraph.in_i[3]);
connect<> netii4(platform.src[5], myGraph.in_i[4]);
connect<> netii5(platform.src[6], myGraph.in_i[5]);
connect<> netii6(platform.src[7], myGraph.in_i[6]);
connect<> netii7(platform.src[8], myGraph.in_i[7]);
connect<> netii8(platform.src[9], myGraph.in_i[8]);
connect<> netii9(platform.src[10], myGraph.in_i[9]);
connect<> netii10(platform.src[11], myGraph.in_i[10]);
connect<> netii11(platform.src[12], myGraph.in_i[11]);
connect<> netii12(platform.src[13], myGraph.in_i[12]);
connect<> netii13(platform.src[14], myGraph.in_i[13]);
connect<> netii14(platform.src[15], myGraph.in_i[14]);
connect<> netii15(platform.src[16], myGraph.in_i[15]);
connect<> netii16(platform.src[17], myGraph.in_i[16]);
connect<> netii17(platform.src[18], myGraph.in_i[17]);
connect<> netii18(platform.src[19], myGraph.in_i[18]);
connect<> netii19(platform.src[20], myGraph.in_i[19]);
connect<> netii20(platform.src[21], myGraph.in_i[20]);
connect<> netii21(platform.src[22], myGraph.in_i[21]);
connect<> netii22(platform.src[23], myGraph.in_i[22]);
connect<> netii23(platform.src[24], myGraph.in_i[23]);
connect<> netii24(platform.src[25], myGraph.in_i[24]);
connect<> netii25(platform.src[26], myGraph.in_i[25]);
connect<> netii26(platform.src[27], myGraph.in_i[26]);
connect<> netii27(platform.src[28], myGraph.in_i[27]);
connect<> netii28(platform.src[29], myGraph.in_i[28]);
connect<> netii29(platform.src[30], myGraph.in_i[29]);
connect<> netii30(platform.src[31], myGraph.in_i[30]);
connect<> netii31(platform.src[32], myGraph.in_i[31]);
connect<> netii32(platform.src[33], myGraph.in_i[32]);
connect<> netii33(platform.src[34], myGraph.in_i[33]);
connect<> netii34(platform.src[35], myGraph.in_i[34]);
connect<> netii35(platform.src[36], myGraph.in_i[35]);
connect<> netii36(platform.src[37], myGraph.in_i[36]);
connect<> netii37(platform.src[38], myGraph.in_i[37]);
connect<> netii38(platform.src[39], myGraph.in_i[38]);
connect<> netii39(platform.src[40], myGraph.in_i[39]);
connect<> netii40(platform.src[41], myGraph.in_i[40]);
connect<> netii41(platform.src[42], myGraph.in_i[41]);
connect<> netii42(platform.src[43], myGraph.in_i[42]);
connect<> netii43(platform.src[44], myGraph.in_i[43]);
connect<> netii44(platform.src[45], myGraph.in_i[44]);
connect<> netii45(platform.src[46], myGraph.in_i[45]);
connect<> netii46(platform.src[47], myGraph.in_i[46]);
connect<> netii47(platform.src[48], myGraph.in_i[47]);
connect<> netii48(platform.src[49], myGraph.in_i[48]);
connect<> netii49(platform.src[50], myGraph.in_i[49]);
connect<> netii50(platform.src[51], myGraph.in_i[50]);
connect<> netii51(platform.src[52], myGraph.in_i[51]);
connect<> netii52(platform.src[53], myGraph.in_i[52]);
connect<> netii53(platform.src[54], myGraph.in_i[53]);
connect<> netii54(platform.src[55], myGraph.in_i[54]);
connect<> netii55(platform.src[56], myGraph.in_i[55]);
connect<> netii56(platform.src[57], myGraph.in_i[56]);
connect<> netii57(platform.src[58], myGraph.in_i[57]);
connect<> netii58(platform.src[59], myGraph.in_i[58]);
connect<> netii59(platform.src[60], myGraph.in_i[59]);
connect<> netii60(platform.src[61], myGraph.in_i[60]);
connect<> netii61(platform.src[62], myGraph.in_i[61]);
connect<> netii62(platform.src[63], myGraph.in_i[62]);
connect<> netii63(platform.src[64], myGraph.in_i[63]);
connect<> netii64(platform.src[65], myGraph.in_i[64]);
connect<> netii65(platform.src[66], myGraph.in_i[65]);
connect<> netii66(platform.src[67], myGraph.in_i[66]);
connect<> netii67(platform.src[68], myGraph.in_i[67]);
connect<> netii68(platform.src[69], myGraph.in_i[68]);
connect<> netii69(platform.src[70], myGraph.in_i[69]);
connect<> netii70(platform.src[71], myGraph.in_i[70]);
connect<> netii71(platform.src[72], myGraph.in_i[71]);
connect<> netii72(platform.src[73], myGraph.in_i[72]);
connect<> netii73(platform.src[74], myGraph.in_i[73]);
connect<> netii74(platform.src[75], myGraph.in_i[74]);
connect<> netii75(platform.src[76], myGraph.in_i[75]);
connect<> netii76(platform.src[77], myGraph.in_i[76]);
connect<> netii77(platform.src[78], myGraph.in_i[77]);
connect<> netii78(platform.src[79], myGraph.in_i[78]);
connect<> netii79(platform.src[80], myGraph.in_i[79]);
connect<> netii80(platform.src[81], myGraph.in_i[80]);
connect<> netii81(platform.src[82], myGraph.in_i[81]);
connect<> netii82(platform.src[83], myGraph.in_i[82]);
connect<> netii83(platform.src[84], myGraph.in_i[83]);
connect<> netii84(platform.src[85], myGraph.in_i[84]);
connect<> netii85(platform.src[86], myGraph.in_i[85]);
connect<> netii86(platform.src[87], myGraph.in_i[86]);
connect<> netii87(platform.src[88], myGraph.in_i[87]);
connect<> netii88(platform.src[89], myGraph.in_i[88]);
connect<> netii89(platform.src[90], myGraph.in_i[89]);
connect<> netii90(platform.src[91], myGraph.in_i[90]);
connect<> netii91(platform.src[92], myGraph.in_i[91]);
connect<> netii92(platform.src[93], myGraph.in_i[92]);
connect<> netii93(platform.src[94], myGraph.in_i[93]);
connect<> netii94(platform.src[95], myGraph.in_i[94]);
connect<> netii95(platform.src[96], myGraph.in_i[95]);
connect<> netii96(platform.src[97], myGraph.in_i[96]);
connect<> netii97(platform.src[98], myGraph.in_i[97]);
connect<> netii98(platform.src[99], myGraph.in_i[98]);
connect<> netii99(platform.src[100], myGraph.in_i[99]);

//Connect platform to graph's outputs 
connect<> neto0(myGraph.out[0],  platform.sink[0]);
connect<> neto1(myGraph.out[1],  platform.sink[1]);
connect<> neto2(myGraph.out[2],  platform.sink[2]);
connect<> neto3(myGraph.out[3],  platform.sink[3]);
connect<> neto4(myGraph.out[4],  platform.sink[4]);
connect<> neto5(myGraph.out[5],  platform.sink[5]);
connect<> neto6(myGraph.out[6],  platform.sink[6]);
connect<> neto7(myGraph.out[7],  platform.sink[7]);
connect<> neto8(myGraph.out[8],  platform.sink[8]);
connect<> neto9(myGraph.out[9],  platform.sink[9]);
connect<> neto10(myGraph.out[10],  platform.sink[10]);
connect<> neto11(myGraph.out[11],  platform.sink[11]);
connect<> neto12(myGraph.out[12],  platform.sink[12]);
connect<> neto13(myGraph.out[13],  platform.sink[13]);
connect<> neto14(myGraph.out[14],  platform.sink[14]);
connect<> neto15(myGraph.out[15],  platform.sink[15]);
connect<> neto16(myGraph.out[16],  platform.sink[16]);
connect<> neto17(myGraph.out[17],  platform.sink[17]);
connect<> neto18(myGraph.out[18],  platform.sink[18]);
connect<> neto19(myGraph.out[19],  platform.sink[19]);
connect<> neto20(myGraph.out[20],  platform.sink[20]);
connect<> neto21(myGraph.out[21],  platform.sink[21]);
connect<> neto22(myGraph.out[22],  platform.sink[22]);
connect<> neto23(myGraph.out[23],  platform.sink[23]);
connect<> neto24(myGraph.out[24],  platform.sink[24]);
connect<> neto25(myGraph.out[25],  platform.sink[25]);
connect<> neto26(myGraph.out[26],  platform.sink[26]);
connect<> neto27(myGraph.out[27],  platform.sink[27]);
connect<> neto28(myGraph.out[28],  platform.sink[28]);
connect<> neto29(myGraph.out[29],  platform.sink[29]);
connect<> neto30(myGraph.out[30],  platform.sink[30]);
connect<> neto31(myGraph.out[31],  platform.sink[31]);
connect<> neto32(myGraph.out[32],  platform.sink[32]);
connect<> neto33(myGraph.out[33],  platform.sink[33]);
connect<> neto34(myGraph.out[34],  platform.sink[34]);
connect<> neto35(myGraph.out[35],  platform.sink[35]);
connect<> neto36(myGraph.out[36],  platform.sink[36]);
connect<> neto37(myGraph.out[37],  platform.sink[37]);
connect<> neto38(myGraph.out[38],  platform.sink[38]);
connect<> neto39(myGraph.out[39],  platform.sink[39]);
connect<> neto40(myGraph.out[40],  platform.sink[40]);
connect<> neto41(myGraph.out[41],  platform.sink[41]);
connect<> neto42(myGraph.out[42],  platform.sink[42]);
connect<> neto43(myGraph.out[43],  platform.sink[43]);
connect<> neto44(myGraph.out[44],  platform.sink[44]);
connect<> neto45(myGraph.out[45],  platform.sink[45]);
connect<> neto46(myGraph.out[46],  platform.sink[46]);
connect<> neto47(myGraph.out[47],  platform.sink[47]);
connect<> neto48(myGraph.out[48],  platform.sink[48]);
connect<> neto49(myGraph.out[49],  platform.sink[49]);
connect<> neto50(myGraph.out[50],  platform.sink[50]);
connect<> neto51(myGraph.out[51],  platform.sink[51]);
connect<> neto52(myGraph.out[52],  platform.sink[52]);
connect<> neto53(myGraph.out[53],  platform.sink[53]);
connect<> neto54(myGraph.out[54],  platform.sink[54]);
connect<> neto55(myGraph.out[55],  platform.sink[55]);
connect<> neto56(myGraph.out[56],  platform.sink[56]);
connect<> neto57(myGraph.out[57],  platform.sink[57]);
connect<> neto58(myGraph.out[58],  platform.sink[58]);
connect<> neto59(myGraph.out[59],  platform.sink[59]);
connect<> neto60(myGraph.out[60],  platform.sink[60]);
connect<> neto61(myGraph.out[61],  platform.sink[61]);
connect<> neto62(myGraph.out[62],  platform.sink[62]);
connect<> neto63(myGraph.out[63],  platform.sink[63]);
connect<> neto64(myGraph.out[64],  platform.sink[64]);
connect<> neto65(myGraph.out[65],  platform.sink[65]);
connect<> neto66(myGraph.out[66],  platform.sink[66]);
connect<> neto67(myGraph.out[67],  platform.sink[67]);
connect<> neto68(myGraph.out[68],  platform.sink[68]);
connect<> neto69(myGraph.out[69],  platform.sink[69]);
connect<> neto70(myGraph.out[70],  platform.sink[70]);
connect<> neto71(myGraph.out[71],  platform.sink[71]);
connect<> neto72(myGraph.out[72],  platform.sink[72]);
connect<> neto73(myGraph.out[73],  platform.sink[73]);
connect<> neto74(myGraph.out[74],  platform.sink[74]);
connect<> neto75(myGraph.out[75],  platform.sink[75]);
connect<> neto76(myGraph.out[76],  platform.sink[76]);
connect<> neto77(myGraph.out[77],  platform.sink[77]);
connect<> neto78(myGraph.out[78],  platform.sink[78]);
connect<> neto79(myGraph.out[79],  platform.sink[79]);
connect<> neto80(myGraph.out[80],  platform.sink[80]);
connect<> neto81(myGraph.out[81],  platform.sink[81]);
connect<> neto82(myGraph.out[82],  platform.sink[82]);
connect<> neto83(myGraph.out[83],  platform.sink[83]);
connect<> neto84(myGraph.out[84],  platform.sink[84]);
connect<> neto85(myGraph.out[85],  platform.sink[85]);
connect<> neto86(myGraph.out[86],  platform.sink[86]);
connect<> neto87(myGraph.out[87],  platform.sink[87]);
connect<> neto88(myGraph.out[88],  platform.sink[88]);
connect<> neto89(myGraph.out[89],  platform.sink[89]);
connect<> neto90(myGraph.out[90],  platform.sink[90]);
connect<> neto91(myGraph.out[91],  platform.sink[91]);
connect<> neto92(myGraph.out[92],  platform.sink[92]);
connect<> neto93(myGraph.out[93],  platform.sink[93]);
connect<> neto94(myGraph.out[94],  platform.sink[94]);
connect<> neto95(myGraph.out[95],  platform.sink[95]);
connect<> neto96(myGraph.out[96],  platform.sink[96]);
connect<> neto97(myGraph.out[97],  platform.sink[97]);
connect<> neto98(myGraph.out[98],  platform.sink[98]);
connect<> neto99(myGraph.out[99],  platform.sink[99]);
#ifdef __AIESIM__

int main(void) {
  myGraph.init();
  myGraph.run(1);
  myGraph.end();
  return 0;
}

#endif
