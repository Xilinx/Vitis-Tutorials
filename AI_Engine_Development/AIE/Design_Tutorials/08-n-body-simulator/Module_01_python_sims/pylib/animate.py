#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys 
import pandas as pd


def read_data(filename,nbr_iterations,nbr_elements): 
    df = pd.read_csv(filename, sep=" ", header=None)
    df.columns = ["x", "y", "z","m"] 
    pid = np.arange(3)

    iterationarray = np.arange(nbr_iterations) 
    itercol = np.repeat(iterationarray,nbr_elements)

    idcol = list()  

    for i in range(nbr_iterations):
        idcol.extend(pid.tolist()) 
    
    idcol = np.array(idcol)
    df = df.assign(particle_id=pd.Series(idcol))
    df = df.assign(iteration=pd.Series(itercol))
    
    return df
 

def update(iteration):
    datax = generate_points(iteration,"x")
    datay = generate_points(iteration,"y")
    dataz = generate_points(iteration,"z")
    datam = generate_points(iteration,"m")

    data = np.array(list(zip(datax,datay)))    
    points.set_offsets(data)
    
    m_sizes = (datam/smax)*mass_mult
    points.set_sizes(m_sizes)

    c_colors = (dataz/cmax)*100
    points.set_array(c_colors)


    return points,
    
def generate_points(iteration,col):
    xdf = raw_data.loc[raw_data['iteration'] == iteration, col]
    xnp = xdf.to_numpy()
    return xnp

if __name__ == "__main__":
    ts = 0
    nump = 0
    filename = ""
    outputFilename = "" 

    for i, arg in enumerate(sys.argv):
        if(i==1):
            print(f"INPUT DATAFILE: {arg}")
            filename = arg

        if(i==2):
            print(f"NUM_PARTICLES: {arg}")
            nump = int(arg)

        if(i==3):
            print(f"OUTPUT FILENAME: {arg}")
            outputFilename = arg

    
    data = np.loadtxt(filename)
    ts = int(len(data)/nump)
    print(f"TIMESTEPS: {ts}")
    
    raw_data = read_data(filename,ts,nump)

    xmin = int(raw_data['x'].min() + 10)
    xmax = int(raw_data['x'].max() + 10)
    ymin = int(raw_data['y'].min() + 10)
    ymax = int(raw_data['y'].max() + 10)

    print(f"xmin={xmin}")
    print(f"xmax={xmax}")
    print(f"ymin={ymin}")
    print(f"ymax={ymax}")

    fig, ax = plt.subplots() 
    fig.set_facecolor('black')
    ax.set_facecolor('black') 
    ax.set_title('N-Body Animation')
    plt.style.use("dark_background")
    ax.set_title("'dark_background' style sheet")
    ax.axis('off')
    ax.grid(False) 
    
    c = raw_data['z']
    c = c.to_numpy()
    cmax = c.max()
    c = (c/cmax)*100

    s = raw_data['m']
    s = s.to_numpy()
    
    smax = s.max()
    mass_mult = 1
    s = s/s.max()
    s = s*mass_mult

    points= ax.scatter(np.arange(len(c)),np.arange(len(c)),s=s,c=c,cmap=plt.cm.bwr, marker='.')

    ax.set_ylim(ymin, ymax)
    ax.set_xlim(xmin, xmax)
    
    ani = animation.FuncAnimation(fig, update, ts,interval=50,blit=True, repeat=True)
    ani.save(outputFilename, writer='imagemagick', fps=30);
    #plt.show()
