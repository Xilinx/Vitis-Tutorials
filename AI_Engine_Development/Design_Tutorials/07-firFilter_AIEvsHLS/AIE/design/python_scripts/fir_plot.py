#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


import matplotlib.pyplot as plt
#from matplotlib.font_manager import FontProperties...

#240 Taps FIR: Power vs. Number of Filters...
x1 = [1,10]
y1 = [0.774,1.726]
# plotting the line 1 points
#plt.plot(x1, y1, label = "240 Taps AIE FIR")
plt.figure(1)
plt.plot(x1, y1, color='blue', linewidth = 3,
         marker='o', markerfacecolor='blue', markersize=8,label = "240 Taps AIE FIR")
#plt.set_xlabel('time [s]', fontsize='large', fontweight='bold')
# line 2 points
x2 = [1,10]
y2 = [0.474,4.328]
# plotting the line 2 points
#plt.plot(x2, y2, label = "240 Taps HLS FIR")
plt.plot(x2, y2, color='orange', linewidth = 3,
         marker='o', markerfacecolor='orange', markersize=8, label = "240 Taps HLS FIR") 
# naming the x axis
plt.xlabel('Number of Filters')
# naming the y axis
plt.ylabel('Power (W)')
# giving a title to my graph
plt.title('240 Taps FIR: Power vs. Number of Filters',fontsize= 16, fontweight='bold')
 
# show a legend on the plot
plt.legend()
plt.grid() 
plt.margins()
# function to show the plot
#plt.show()

#240 Taps FIR: Computational Efficiency vs. Number of Filters...
x1 = [1,10]
y1 = [173.74677,77.78099]
# plotting the line 1 points
#plt.plot(x1, y1, label = "240 Taps AIE FIR")
plt.figure(2)
plt.plot(x1, y1, color='blue', linewidth = 3,
         marker='o', markerfacecolor='blue', markersize=8,label = "240 Taps AIE FIR")
#plt.set_xlabel('time [s]', fontsize='large', fontweight='bold')
# line 2 points
x2 = [1,10]
y2 = [263.3838,28.5305]
# plotting the line 2 points
#plt.plot(x2, y2, label = "240 Taps HLS FIR")
plt.plot(x2, y2, color='orange', linewidth = 3,
         marker='o', markerfacecolor='orange', markersize=8, label = "240 Taps HLS FIR") 
# naming the x axis
plt.xlabel('Number of Filters')
# naming the y axis
plt.ylabel('Computational Efficiency (MSPS/W)')
# giving a title to my graph
plt.title('240 Taps FIR: Computational Efficiency vs. Number of Filters',fontsize= 16, fontweight='bold')
 
# show a legend on the plot
plt.legend()
plt.grid() 
plt.margins()

#129 Taps FIR: Casc Length Metrics...
##129 Taps FIR: Throughput vs. Cascade Length...
x3 = [1,2,4]
y3 = [215.578,309.599,387.054]

plt.figure(3)
plt.plot(x3, y3, color='blue', linewidth = 3,
         marker='o', markerfacecolor='blue', markersize=8, label = "129 Taps AIE FIR") 
# naming the x axis
plt.xlabel('Cascade Length')
# naming the y axis
plt.ylabel('Throughput (MSPS)')
# giving a title to my graph
plt.title('129 Taps FIR: Throughput vs. Cascade Length',fontsize= 16, fontweight='bold')
 
# show a legend on the plot
plt.legend()
plt.grid() 
plt.margins()

##129 Taps FIR: Power vs. Cascade Length...
x4 = [1,2,4]
y4 = [0.769,0.85,1.02]

plt.figure(4)
plt.plot(x4, y4, color='blue', linewidth = 3,
         marker='o', markerfacecolor='blue', markersize=8, label = "129 Taps AIE FIR") 
# naming the x axis
plt.xlabel('Cascade Length')
# naming the y axis
plt.ylabel('Power (W)')
# giving a title to my graph
plt.title('129 Taps FIR: Power vs. Cascade Length',fontsize= 16, fontweight='bold')
 
# show a legend on the plot
plt.legend()
plt.grid() 
plt.margins()

##129 Taps FIR: Computational Efficiency vs. Cascade Length...
x5 = [1,2,4]
y5 = [280.3355,364.2341,379.4647]

plt.figure(5)
plt.plot(x5, y5, color='blue', linewidth = 3,
         marker='o', markerfacecolor='blue', markersize=8, label = "129 Taps AIE FIR") 
# naming the x axis
plt.xlabel('Cascade Length')
# naming the y axis
plt.ylabel('Computational Efficiency (MSPS/W)')
# giving a title to my graph
plt.title('129 Taps FIR: Computational Efficiency vs. Cascade Length',fontsize= 16, fontweight='bold')
 
# show a legend on the plot
plt.legend()
plt.grid() 
plt.margins()

# function to show the plot
plt.show()

######################################################################################
#Subplots
#129 Taps FIR: Casc Length Metrics...
##129 Taps FIR: Throughput vs. Cascade Length...
#x1 = [1,2,4]
#y1 = [168.19,38.14,1]
#fig, casc_sbplts = plt.subplots(1, 2)
## plotting the line 1 points
##plt.plot(x1, y1, label = "240 Taps AIE FIR")
##casc_sbplts[0].plot(x1, y1, color='blue', linewidth = 3,
##         marker='o', markerfacecolor='blue', markersize=8,label = "129 Taps AIE FIR")
#casc_sbplts[0].plot(x1, y1)
## naming the x and y axes
#casc_sbplts[0].set(xlabel='Cascade Length', ylabel='Throughput (MSPS)')
#casc_sbplts[0].set_title('129 Taps FIR: Throughput vs. Cascade Length')
#
## show a legend on the plot
##casc_sbplts[0].legend()
#casc_sbplts[0].grid() 
#casc_sbplts[0].margins()
#
###129 Taps FIR: Power vs. Cascade Length...
#x2 = [1,2,4]
#y2 = [277.97,30.05,1]
## plotting the line 1 points
##plt.plot(x2, y2, label = "240 Taps AIE FIR")
##casc_sbplts[1].plot(x2, y2, color='blue', linewidth = 3,
##         marker='o', markerfacecolor='blue', markersize=8,label = "129 Taps AIE FIR")
#casc_sbplts[1].plot(x1, y1)
## naming the x and y axes
#casc_sbplts[1].set(xlabel='Cascade Length', ylabel='Throughput (MSPS)')
#casc_sbplts[1].set_title('129 Taps FIR: Throughput vs. Cascade Length')
#
## show a legend on the plot
##casc_sbplts[1].legend()
#casc_sbplts[1].grid() 
#casc_sbplts[1].margins()
#
## function to show the plot
#fig.show()
