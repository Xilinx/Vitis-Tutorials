
ifeq ($(banks), 0)
in_M0 = 0
in_M1 = 1
out_M2 = 2
_banks = 1
endif
ifeq ($(banks), 0_1)
in_M0 = 0:1
in_M1 = 2:3
out_M2 = 4:5
_banks = 2
endif
ifeq ($(banks), 0_2)
in_M0 = 0:2
in_M1 = 3:5
out_M2 = 6:8
_banks = 3
endif
ifeq ($(banks), 0_3)
in_M0 = 0:3
in_M1 = 4:7
out_M2 = 8:11
_banks = 4
endif
ifeq ($(banks), overlap)
in_M0 = 0:1
in_M1 = 1:2
out_M2 = 3:4
_banks = overlap
endif
ifeq ($(banks), 0_31)
in_M0 = 0:31
in_M1 = 0:31
out_M2 = 0:31
_banks = all
endif
ifeq ($(banks), custom)
in_M0 = <user input>
in_M1 = 0<user input>
out_M2 = 0<user input>
_banks = custom
endif
