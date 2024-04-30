#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
#!/bin/awk -f
# parse packet switching report json file into C header format

BEGIN {
	print "{"
	startcol = 0;
	isfirst  = 1;

}
{

	if ((NF==2)&&($1=="STARTCOL")) {
		startcol = $2;
	}
	else if ((NF==3)&&($1 !~ /#/)) {

		if(isfirst==1){
			print "  \"NodeConstraints\": {";
			isfirst = 0;
		}else
			print "    },";

		##
		print "    \"" $1 "\": {";
		print "       \"shim\": {";
		print "         \"column\":  " startcol+$2 ",";
		print "         \"channel\": " $3;
		print "       }";

	}

}
END{

	print "    }";
	print "  }";
	print "}";

}
