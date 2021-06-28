import os
import sys

inp = open(sys.argv[1], "r")
out = open(sys.argv[2], "w+")

for line in inp:
   tmp = line.split(",")
   out.write(tmp[0]+","+tmp[1]+","+tmp[2]+","+tmp[3]+"\n")
   out.write(tmp[4]+","+tmp[5]+","+tmp[6]+","+tmp[7]+"\n")

inp.close()
out.close()
