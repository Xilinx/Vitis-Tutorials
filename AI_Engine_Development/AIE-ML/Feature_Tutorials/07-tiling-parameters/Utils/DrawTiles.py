#!/usr/bin/env python3
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

import os
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import PillowWriter
from PIL import Image



class accessScheme():
    
    def __init__(self):
        self.filename = "accessScheme.txt"
        self.NDims = 0
        self.Traversal = []
        self.BufferSize = []
        self.TileSize = []
        self.Offset = []
        self.Bound = []
        self.Error = 0
            
    def setBufferSize(self, Size):
        self.BufferSize = Size
        self.NDims = len(Size)
        
    def setTileSize(self, Size):
        if (self.NDims != len(Size)):
            print ("Error: Tile size dimension mismatch",file=sys.stderr)
            self.Error = 1         
        self.TileSize = Size
        
    def setOffset(self, Offset):
        if (self.NDims != len(Offset)):
            print ("Error: Offset size dimension mismatch",file=sys.stderr)            
            self.Error = 1         
        self.Offset = Offset
        
    def setBoundary(self, Bound):
        if (self.NDims != len(Bound)):
            print ("Error: Bound size dimension mismatch",file=sys.stderr)            
            self.Error = 1         
        self.Bound = Bound
        
    def addTraversal(self, dim,stride, wrap):
        if(dim >=0 and dim < self.NDims):
            if (len(self.Traversal)==0):
                self.Traversal = [[dim,stride,wrap]]
            else:
                self.Traversal.append([dim,stride,wrap])
        else:
            print (f"Error: Invalid dimension (Traversal [dim={dim}])",file=sys.stderr)
            self.Error = 1         

    def dispParameters(self):
        print ("NDims: ", self.NDims)
        print ("Buffer Size: ", self.BufferSize)
        print ("Offset: ", self.Offset)
        print ("Tile Size: ", self.TileSize)
        print ("Traversal: ", self.Traversal)
        print ("Boundary : ", self.Bound)
        if(self.Error == 1):
            print("ERROR: Invalid Access Scheme",file=sys.stderr)
        
    def getTraversalForDim(self,dim):
        t = [-1, -1, -1] 
        for i in range(len(self.Traversal)):
            T = self.Traversal[i]
            if(T[0]==dim):
                t = T
        return(t)
            
    def drawRect(self,ax,X,Y,color,width,style):
        C = .5
        Xc = [X[0]-C , X[1]-C]
        Yc = [Y[0]-C , Y[1]-C]
        Rx = [Xc[0], Xc[1], Xc[1], Xc[0], Xc[0]]
        Ry = [Yc[0], Yc[0], Yc[1], Yc[1], Yc[0]]
        ax.plot(Rx,Ry,c=color, lw=width, linestyle=style)

    def drawPatch(self,ax,X,Y,color):
        C = .5
        Xc = [X[0]-C+.1 , X[1]-C-.1]
        Yc = [Y[0]-C+.1 , Y[1]-C-.1]
        rectangle = patches.Rectangle((Xc[0], Yc[0]), Xc[1]-Xc[0], Yc[1]-Yc[0], linewidth=1, edgecolor=color, facecolor=color)
        ax.add_patch(rectangle)

    def drawPatch3(self,ax,X,Y,facecolor,width,edgecolor):
        C = .5
        Xc = [X[0]-C+.1 , X[1]-C-.1]
        Yc = [Y[0]-C+.1 , Y[1]-C-.1]
        rectangle = patches.Rectangle((Xc[0], Yc[0]), Xc[1]-Xc[0], Yc[1]-Yc[0], linewidth=width, edgecolor=edgecolor, facecolor=facecolor)
        ax.add_patch(rectangle)

    def incTile(self,tile,dim,wrap):
        # print(f"Increment")
        OK = 0
        # print(f"in tile={tile}")


        for i in range(len(dim)):
            # print(f"i = {i} dim[i] = {dim[i]}")
            # print(f"dim[i] = {dim[i]} OK = {OK}  tile = {tile}")
            if(OK==0):
                tile[dim[i]] = tile[dim[i]]+1
                if (tile[dim[i]] < wrap[dim[i]]):
                    OK = 1
                else:
                    tile[dim[i]] = 0
        # print(f"out tile={tile}")
        return OK,tile
        
    def Ticks(self,x0,x1):
        xtick = range(x0,x1)
        if(x1-x0>16):
            step = int((x1-x0+1)/16)
            xtick = [i for i in range(x0,x0+4*step,step)]
            xtick.extend([i for i in range(x1-3*step-1,x1,step)])
        return xtick
        
        
    def saveTraversal1(self,filename,AspectRatio,FigSize):
        # AspectRatio = 16
        # Initialize Animated Gif
        metadata = dict(title='Movie',artist='TilingParameters')
        writer = PillowWriter(fps=1,metadata=metadata)
        
        # Canvas Total Size    
        x0 = self.Offset[0]
        x1 = x0 + self.TileSize[0]
        # Get Traversal Parameter for Dim 0 if it exists
        T = self.getTraversalForDim(0)
        if(T[0]!=-1):
            x1 = x1 + T[1]*(T[2]-1)
        x0 = np.min([x0,0])
        x1 = np.max([x1,self.BufferSize[0]])
        xtick = self.Ticks(x0,x1)

        y0 = 0
        y1 = 1
        ytick = [0]
        
        Xtot = [x0,x1]
        Ytot = [y0,y1]
        plt.ion()
        fig, ax = plt.subplots()
        fig.set_size_inches(FigSize[0],FigSize[1])
        ax.set_aspect(AspectRatio)
        plt.grid(False)
        plt.axis('off')
        self.drawRect(ax,Xtot,Ytot,"white", 1, "-")
        plt.plot(block=False)

        # Buffer dimensions
        Xbuf = [0, (self.BufferSize[0])]
        Ybuf = [0, 1]        
        
        tile = np.zeros(len(self.Traversal),dtype=int)
        dim = np.zeros(len(self.Traversal),dtype=int)
        stride = np.zeros(len(self.Traversal),dtype=int)
        wrap = np.zeros(len(self.Traversal),dtype=int)
        
        for i in range(self.NDims):
            dim[i] = self.Traversal[i][0]
            stride[i] = self.Traversal[i][1]
            wrap[i] = self.Traversal[i][2]
            
        # print(f"dim = {dim}")
        # print(f"wrap = {wrap}")
        
        with writer.saving(fig, filename, 100):
            OK = 1
            idx = -1
            while (OK==1):
                # if(idx>=0):
                #     print(tile)
                ax.set_aspect(AspectRatio)
                plt.grid(True)
                plt.axis('on')
                plt.gca().invert_yaxis()

                # Draw invisible frame for the total canvas size
                self.drawRect(ax,Xtot,Ytot,"white", 1, "-")
                ax.set_xticks(xtick)
                ax.set_yticks(ytick)
                # Draw a red frame for the Buffer limits
                self.drawRect(ax,Xbuf,Ybuf,"red", 5, "-")
                Xbound = Xbuf
                Ybound = Ybuf
                if (len(self.Bound)>0):
                    Xbound = [ 0, self.Bound[0] ]
                    Ybound = [ 0 , 1]
                self.drawPatch(ax,Xbound,Ybound,"lightblue")

                if(idx>=0):
                # Current Tile Coordinates
                    x0 = self.Offset[0]+tile[0]*stride[0]
                    Xt = [ x0, x0+self.TileSize[0]]
                    y0 = 0
                    Yt = [ y0, y0+1]
                    # print(f"Total : {Xtot} {Ytot}")
                    # print(f"Buffer: {Xbuf} {Ybuf}")
                    # print(f"Tile  : {Xt} {Yt}")
                    # Draw a blue frame for the Tile limits
                    self.drawRect(ax,Xt,Yt,"blue",3,"-")
                plt.plot()
                plt.pause(2)
                writer.grab_frame()
                plt.cla()
                if(idx>=0):
                    OK,tile = self.incTile(tile,dim,wrap)
                else:
                    idx = 1
            # print("")
        
    def saveTraversal2(self,filename,AspectRatio,FigSize):
        # Initialize Animated Gif
        metadata = dict(title='Movie',artist='TilingParameters')
        writer = PillowWriter(fps=1,metadata=metadata)
        
        # Canvas Total Size    
        x0 = self.Offset[0]
        x1 = x0 + self.TileSize[0]
        # Get Traversal Parameter for Dim 0 if it exists
        T = self.getTraversalForDim(0)
        if(T[0]!=-1):
            x1 = x1 + T[1]*(T[2]-1)
        x0 = np.min([x0,0])
        x1 = np.max([x1,self.BufferSize[0]])
        xtick = self.Ticks(x0,x1)
            
        y0 = self.Offset[1]
        y1 = y0 + self.TileSize[1]
        # Get Traversal Parameter for Dim 1 if it exists
        T = self.getTraversalForDim(1)
        if(T[0]!=-1):
            y1 = y1 + T[1]*(T[2]-1)
        y0 = np.min([y0,0])
        y1 = np.max([y1,self.BufferSize[1]])
        ytick = self.Ticks(y0,y1)
            

        Xtot = [x0,x1]
        Ytot = [y0,y1]
        plt.ion()
        fig, ax = plt.subplots()
        fig.set_size_inches(FigSize[0],FigSize[1])
        ax.set_aspect(AspectRatio)
        plt.grid(False)
        plt.axis('off')
        self.drawRect(ax,Xtot,Ytot,"white", 1, "-")
        plt.plot(block=False)

        # Buffer dimensions
        Xbuf = [0, (self.BufferSize[0])]
        Ybuf = [0, (self.BufferSize[1])]        
        
        dim = np.zeros(len(self.Traversal),dtype=int)

        tile = np.zeros(self.NDims,dtype=int)
        stride = np.zeros(self.NDims,dtype=int)
        wrap = np.ones(self.NDims,dtype=int)
        
        # print(f"Traversal len {len(self.Traversal)}")

        for i in range(len(self.Traversal)):
            dim[i] = self.Traversal[i][0]
            stride[self.Traversal[i][0]] = self.Traversal[i][1]
            wrap[self.Traversal[i][0]] = self.Traversal[i][2]
            
        # print(f"dim = {dim}")
        # print(f"wrap = {wrap}")
        # print(f"stride = {stride}")

        with writer.saving(fig, filename, 100):
            OK = 1
            idx = -1
            while (OK==1):
                # if(idx>=0):
                #     print(tile)
                ax.set_aspect(AspectRatio)
                plt.grid(True)
                plt.axis('on')
                # Draw invisible frame for the total canvas size
                self.drawRect(ax,Xtot,Ytot,"white", 1, "-")
                ax.set_xticks(xtick)
                ax.set_yticks(ytick)

                # Draw a red frame for the Buffer limits
                self.drawRect(ax,Xbuf,Ybuf,"red", 5, "-")

                Xbound = Xbuf
                Ybound = Ybuf
                if (len(self.Bound)>0):
                    Xbound = [ 0, self.Bound[0] ]
                    Ybound = [ 0 , self.Bound[1]]
                self.drawPatch(ax,Xbound,Ybound,"lightblue")

                if(idx>=0):
                # Current Tile Coordinates
                    x0 = self.Offset[0]+tile[0]*stride[0]
                    Xt = [ x0, x0+self.TileSize[0]]
                    y0 = self.Offset[1]+tile[1]*stride[1]
                    Yt = [ y0, y0+self.TileSize[1]]
                    # print(f"Total : {Xtot} {Ytot}")
                    # print(f"Buffer: {Xbuf} {Ybuf}")
                    # print(f"Tile  : {Xt} {Yt}")
                    # Draw a blue frame for the Tile limits
                    self.drawRect(ax,Xt,Yt,"blue",3,"-")
                plt.plot()
                plt.gca().invert_yaxis()
                plt.pause(2)
                writer.grab_frame()
                plt.cla()
                if(idx>=0):
                    OK,tile = self.incTile(tile,dim,wrap)
                else:
                    idx = 1
                # print(f"OK = {OK}")


    def saveTraversal3(self,filename,AspectRatio,FigSize):
        # Initialize Animated Gif
        metadata = dict(title='Movie',artist='TilingParameters')
        writer = PillowWriter(fps=1,metadata=metadata)
        
        # Canvas Total Size    
        x0 = self.Offset[0]
        x1 = x0 + self.TileSize[0]
        # Get Traversal Parameter for Dim 0 if it exists
        T = self.getTraversalForDim(0)
        if(T[0]!=-1):
            x1 = x1 + T[1]*(T[2]-1)
        x0 = np.min([x0,0])
        x1 = np.max([x1,self.BufferSize[0]])
        xtick = self.Ticks(x0,x1)
            
        y0 = self.Offset[1]
        y1 = y0 + self.TileSize[1]
        # Get Traversal Parameter for Dim 1 if it exists
        T = self.getTraversalForDim(1)
        if(T[0]!=-1):
            y1 = y1 + T[1]*(T[2]-1)
        y0 = np.min([y0,0])
        y1 = np.max([y1,self.BufferSize[1]])
        ytick = self.Ticks(y0,y1)
            
        z0 = self.Offset[2]
        z1 = z0 + self.TileSize[2]
        # Get Traversal Parameter for Dim 2 if it exists
        T = self.getTraversalForDim(2)
        if(T[0]!=-1):
            z1 = z1 + T[1]*(T[2]-1)
        z0 = np.min([z0,0])
        z1 = np.max([z1,self.BufferSize[2]])
                
        zRange = np.arange(z1-1, z0 - 1, -1) # Higher indexes will be displayed behind

        zxstep = (x1-x0)/8.0
        zystep = -(y1-y0)/4.0
        
        [xmin, xmax] = [min([z0*zxstep,z1*zxstep]),max([z0*zxstep,z1*zxstep])]
        [ymin, ymax] = [min([z0*zystep,z1*zystep]),max([z0*zystep,z1*zystep])]

        Xtot = [x0+xmin,x1+xmax]
        Ytot = [y0+ymin,y1+ymax]
        
        plt.ion()
        fig, ax = plt.subplots()
        fig.set_size_inches(FigSize[0],FigSize[1])
        ax.set_aspect(AspectRatio)
        plt.grid(False)
        plt.axis('off')
        self.drawPatch(ax,Xtot,Ytot,"none")
        plt.plot(block=False)

        # Buffer dimensions
        Xbuf = [0, (self.BufferSize[0])]
        Ybuf = [0, (self.BufferSize[1])]        
        Zbuf = [0, (self.BufferSize[2])]
        
        dim = np.zeros(len(self.Traversal),dtype=int)

        tile = np.zeros(self.NDims,dtype=int)
        stride = np.zeros(self.NDims,dtype=int)
        wrap = np.ones(self.NDims,dtype=int)
        
        # print(f"Traversal len {len(self.Traversal)}")

        for i in range(len(self.Traversal)):
            dim[i] = self.Traversal[i][0]
            stride[self.Traversal[i][0]] = self.Traversal[i][1]
            wrap[self.Traversal[i][0]] = self.Traversal[i][2]
            
        # print(f"dim = {dim}")
        # print(f"wrap = {wrap}")
        # print(f"stride = {stride}")

        with writer.saving(fig, filename, 100):
            OK = 1
            idx = -1
            while (OK==1):
                # if(idx>=0):
                #     print(tile)
                ax.set_aspect(AspectRatio)
                plt.grid(True)
                plt.axis('on')
                # Draw invisible frame for the total canvas size
                self.drawRect(ax,Xtot,Ytot,"none", 1, "-")
                ax.set_xticks(xtick)
                ax.set_yticks(ytick)

                # I have to draw from backward to frontward
                for z in zRange:
                    Xbound = Xbuf
                    Ybound = Ybuf
                    Zbound = Zbuf
                    if (len(self.Bound)>0):
                        Xbound = [ 0, self.Bound[0] ]
                        Ybound = [ 0 , self.Bound[1]]
                        Zbound = [ 0 , self.Bound[3]]
                        
                    DrawBuf = 0
                    XZbuf = [ Xbuf[0]+z*zxstep, Xbuf[1]+z*zxstep]
                    YZbuf = [ Ybuf[0]+z*zystep, Ybuf[1]+z*zystep]
                    
                    DrawBound = 0
                    XZbound = [ Xbound[0]+z*zxstep, Xbound[1]+z*zxstep]
                    YZbound = [ Ybound[0]+z*zystep, Ybound[1]+z*zystep]
                    
                    DrawTile = 0
                    x0 = self.Offset[0]+tile[0]*stride[0]
                    Xt = [ x0, x0+self.TileSize[0]]
                    y0 = self.Offset[1]+tile[1]*stride[1]
                    Yt = [ y0, y0+self.TileSize[1]]
                    z0 = self.Offset[2]+tile[2]*stride[2]
                    Zt = [ z0, z0+self.TileSize[2]]
                    
                    # print(f"Tile={tile} Offset={self.Offset} Stride={stride} Wrap={wrap}")
                    
                    XZt = [ Xt[0]+z*zxstep, Xt[1]+z*zxstep]
                    YZt = [ Yt[0]+z*zystep, Yt[1]+z*zystep]
                    # print(f"z={z}  Xt={Xt}  Yt={Yt}  Zt={Zt}")
                    
                    # Draw patches to remove background
                    if(z>=Zbuf[0] and z<Zbuf[1]):
                        self.drawPatch(ax,XZbuf,YZbuf,"w")
                        # print("buf")
                        DrawBuf = 1

                    if(z>=Zbound[0] and z<Zbound[1]):
                        self.drawPatch(ax,XZbound,YZbound,"w")
                        # print("bound")
                        DrawBound = 1

                    if(idx>=0 and z>=Zt[0] and z<Zt[1]):
                        self.drawPatch(ax,XZt,YZt,"w")                    
                        # print("tile")
                        DrawTile = 1

                    # Draw a Light Blue Patch for the Boundary limits
                    if(DrawBound ==1):
                        self.drawPatch(ax,XZbound,YZbound,"lightblue")

                    # Draw a red frame for the Buffer limits
                    if(DrawBuf == 1):
                        self.drawPatch3(ax,XZbuf,YZbuf,"none", 7, "r")

                    # Draw a blue frame for the Tile limits
                    if(DrawTile==1):
                        self.drawPatch3(ax,XZt,YZt,"none",4,"blue")



                plt.plot()
                plt.gca().invert_yaxis()
                plt.pause(2)
                writer.grab_frame()
                plt.cla()
                if(idx>=0):
                    OK,tile = self.incTile(tile,dim,wrap)
                else:
                    idx = 1
                # print(f"OK = {OK}")


    def saveTraversal(self,filename,AspectRatio,FigSize):
        print(f"Generating: {filename}")
        if(self.NDims==1):
            self.saveTraversal1(filename,AspectRatio,FigSize)
        elif(self.NDims==2):
            self.saveTraversal2(filename,AspectRatio,FigSize)
        elif(self.NDims==3):
            self.saveTraversal3(filename,AspectRatio,FigSize)
        else:
            print("saveTraversal not implemented for that number of dimensions")


def Test():
    FigSize = [8,  2]

    f = 'Tiling1D.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([12])
        a.setTileSize([3])
        a.setOffset([-2])
        a.addTraversal(0,3,3)
        a.dispParameters()
        a.saveTraversal(f,2,FigSize)
    
    f = 'Tiling2D.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()            
        a.setBufferSize([12,12])
        a.setTileSize([8,8])
        a.setOffset([-4,-2])
        a.addTraversal(1,4,3)
        a.addTraversal(0,4,4)
        a.dispParameters()
        a.saveTraversal(f,2,FigSize)
    

    f = 'Tiling3D.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([32, 4, 2])
        a.setTileSize([16,4,2])
        a.setOffset([0,0,-1])
        a.addTraversal(0,16,2)
        a.addTraversal(2,2,2)
        a.dispParameters()
        a.saveTraversal(f,2,FigSize)

    FigSize = [5,  3]

    f = 'Kernel1Access.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([10, 6])
        a.setTileSize([3,2])
        a.setOffset([0,0])
        a.addTraversal(0,3,2)
        a.addTraversal(1,2,3)
        a.dispParameters()
        a.saveTraversal(f,1,FigSize)

    f = 'Kernel2Access.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([10, 6])
        a.setTileSize([2,3])
        a.setOffset([6,0])
        a.addTraversal(0,2,2)
        a.addTraversal(1,3,2)
        a.dispParameters()
        a.saveTraversal(f,1,FigSize)

    f = 'Kernel3Access.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([10, 6])
        a.setTileSize([2,6])
        a.setOffset([0,0])
        a.addTraversal(0,2,2)
        a.dispParameters()
        a.saveTraversal(f,1,FigSize)

    f = 'Kernel4Access.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([10, 6])
        a.setTileSize([3,6])
        a.setOffset([4,0])
        a.addTraversal(0,3,2)
        a.dispParameters()
        a.saveTraversal(f,1,FigSize)


def Examples1D():
    FigSize = [8,  2]

    f = 'Tiling1D_Copy.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([256])
        a.setTileSize([256])
        a.setOffset([0])
        a.addTraversal(0,1,1)
        a.dispParameters()
        a.saveTraversal(f,16,FigSize)

    
    f = 'Tiling1D_Prepend.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([256])
        a.setTileSize([256])
        a.setOffset([-32])
        a.addTraversal(0,1,1)
        a.dispParameters()
        a.saveTraversal(f,16,FigSize)
    
    f = 'Tiling1D_PrePostPadding.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([256])
        a.setTileSize([288])
        a.setOffset([-16])
        a.addTraversal(0,1,1)
        a.dispParameters()
        a.saveTraversal(f,16,FigSize)
    
    f = 'Tiling1D_PrePostBound.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([256])
        a.setTileSize([128])
        a.setOffset([-16])
        a.addTraversal(0,144,2)
        a.setBoundary([96])
        a.dispParameters()
        a.saveTraversal(f,16,FigSize)


def Examples2D():
    FigSize = [8,2]

    f = 'Tiling2D_Copy.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([128, 2])
        a.setTileSize([128, 1])
        a.setOffset([0,0])
        a.addTraversal(1,1,2)
        a.dispParameters()
        a.saveTraversal(f,10,FigSize)

    f = 'Tiling2D_PrePost0.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([128, 2])
        a.setTileSize([160, 1])
        a.setOffset([-16,0])
        a.addTraversal(1,1,2)
        a.dispParameters()
        a.saveTraversal(f,10,FigSize)

    FigSize = [6,2]
    f = 'Tiling2D_Rect4x2.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([16, 4])
        a.setTileSize([4, 2])
        a.setOffset([0,0])
        a.addTraversal(0,4,4)
        a.addTraversal(1,2,2)
        a.dispParameters()
        a.saveTraversal(f,'equal',FigSize)

def Examples3D():
    FigSize = [8,3]

    f = 'Tiling3D_Copy.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([32, 4, 2])
        a.setTileSize([32,4,2])
        a.setOffset([0,0,0])
        a.addTraversal(1,1,1)
        a.dispParameters()
        a.saveTraversal(f,2,FigSize)

    f = 'Tiling3D_ZeroPadding.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([32, 4, 2])
        a.setTileSize([34,6,2])
        a.setOffset([-1,-1,0])
        a.addTraversal(1,1,1)
        a.dispParameters()
        a.saveTraversal(f,2,FigSize)

def MemTiles():
    FigSize = [8,8]

    f = 'MemTile_3D.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([8, 8, 8])
        a.setTileSize([6,6,1])
        a.setOffset([-1,-1,0])
        a.addTraversal(0,4,2)
        a.addTraversal(1,4,2)
        a.addTraversal(2,1,8)
        a.dispParameters()
        a.saveTraversal(f,1,FigSize)

def MemModules():
    FigSize = [5,5]

    f = 'MemModule_Read.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([8,8])
        a.setTileSize([4,2])
        a.setOffset([0,0])
        a.addTraversal(1,2,4)
        a.addTraversal(0,4,2)
        a.dispParameters()
        a.saveTraversal(f,1,FigSize)

    f = 'MemModule_Write.gif'
    if ( not os.path.isfile(f)):
        a = accessScheme()
        a.setBufferSize([8,8])
        a.setTileSize([1,8])
        a.setOffset([0,0])
        a.addTraversal(0,1,8)
        a.dispParameters()
        a.saveTraversal(f,1,FigSize)




if __name__ == '__main__':

    OK = 1
    
    # if (len(sys.argv) < 4 or len(sys.argv) > 6 ):
    #     OK = 0

    if (OK == 0):
        print (f"Usage: DrawTiles ", file = sys.stderr)
        exit(1)

    # filename = sys.argv[1]
    # NCols = int(sys.argv[2])
    # NRows = int(sys.argv[3])
    
    Test()
    Examples1D()
    Examples2D()
    Examples3D()
    MemTiles()
    MemModules()

