ifeq ($(rama),1)
   ifeq ($(banks), 0)
   in_M0 = HBM[0].0.RAMA
   in_M1 = HBM[1].1.RAMA
   out_M2 = HBM[2].2.RAMA
   _banks = 1
   endif
   ifeq ($(banks), 0_1)
   in_M0 = HBM[0:1].1.RAMA
   in_M1 = HBM[2:3].3.RAMA
   out_M2 = HBM[4:5].5.RAMA
   _banks = 2
   endif
   ifeq ($(banks), 0_2)
   in_M0 = HBM[0:2].1.RAMA
   in_M1 = HBM[3:5].4.RAMA
   out_M2 = HBM[6:8].7.RAMA
   _banks = 3
   endif
   ifeq ($(banks), 0_3)
   in_M0 = HBM[0:3].1.RAMA
   in_M1 = HBM[4:7].4.RAMA
   out_M2 = HBM[8:11].8.RAMA
   _banks = 4
   endif
   ifeq ($(banks), overlap)
   in_M0 = HBM[0:1].1.RAMA
   in_M1 = HBM[1:2].2.RAMA
   out_M2 = HBM[3:4].3.RAMA
   _banks = overlap
   endif
   ifeq ($(banks), 0_31)
   in_M0 = HBM[0:31].1.RAMA
   in_M1 = HBM[0:31].4.RAMA
   out_M2 = HBM[0:31].8.RAMA
   _banks = all
   endif
else
   ifeq ($(banks), 0)
   in_M0 = HBM[0]
   in_M1 = HBM[1]
   out_M2 = HBM[2]
   _banks = 1
   endif
   ifeq ($(banks), 0_1)
   in_M0 = HBM[0:1]
   in_M1 = HBM[2:3]
   out_M2 = HBM[4:5]
   _banks = 2
   endif
   ifeq ($(banks), 0_2)
   in_M0 = HBM[0:2]
   in_M1 = HBM[3:5]
   out_M2 = HBM[6:8]
   _banks = 3
   endif
   ifeq ($(banks), 0_3)
   in_M0 = HBM[0:3]
   in_M1 = HBM[4:7]
   out_M2 = HBM[8:11]
   _banks = 4
   endif
   ifeq ($(banks), overlap)
   in_M0 = HBM[0:1]
   in_M1 = HBM[1:2]
   out_M2 = HBM[3:4]
   _banks = overlap
   endif
   ifeq ($(banks), 0_31)
   in_M0 = HBM[0:31]
   in_M1 = HBM[0:31]
   out_M2 = HBM[0:31]
   _banks = all
   endif
endif
