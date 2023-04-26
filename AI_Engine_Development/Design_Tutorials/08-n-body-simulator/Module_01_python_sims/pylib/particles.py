#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

import numpy as np 
import random

class Particles:
    def __init__(self, numParticles):
        """Initialization Function
        
        Creates arrays that hold the positions (x,y,z), 
        velocities (vx,vy,vz), and mass (m) of some 
        number of particles. 

        """
        self.numParticles = numParticles

        self.x = np.float32(np.ones(self.numParticles))
        self.y = np.float32(np.ones(self.numParticles))
        self.z = np.float32(np.ones(self.numParticles))

        self.vz = np.float32(np.ones(self.numParticles))
        self.vx = np.float32(np.ones(self.numParticles))
        self.vy = np.float32(np.ones(self.numParticles))

        self.m = np.float32(np.ones(self.numParticles))

        #Bounds for initial conditions

        self.minPos = -1000
        self.maxPos = 1000
        self.maxMass = 1000
        self.minV = -2.0
        self.maxV = 2.0

    def getMinPos(self):
        return self.minPos
    
    def getMaxPos(self):
        return self.maxPos

    def getMaxMass(self):
        return self.maxMass

    def getMinV(self):
        return self.minV

    def getMaxV(self):
        return self.maxV

    def getNumParticles(self):
        return self.numParticles
        
    def setInitialConditions(self):
        """ Sets Randomized Inital Conditions
        
        Sets all the m,x,y,z,vx,vy,vz values to a random distribution. 

        """
        self.m = np.float32(np.random.uniform(0.0, self.getMaxMass(), self.getNumParticles()))
        self.x = np.float32(np.random.uniform(self.getMinPos(), self.getMaxPos(), self.getNumParticles()))
        self.vx = np.float32(np.random.uniform(self.getMinV(), self.getMaxV(), self.getNumParticles()))
        self.y = np.float32(np.random.uniform(self.getMinPos(), self.getMaxPos(), self.getNumParticles()))
        self.vy = np.float32(np.random.uniform(self.getMinV(), self.getMaxV(), self.getNumParticles()))
        self.z = np.float32(np.random.uniform(self.getMinPos(), self.getMaxPos(), self.getNumParticles()))
        self.vz = np.float32(np.random.uniform(self.getMinV(), self.getMaxV(), self.getNumParticles()))
        
    def setSphereInitialConditions(self):
        radius, h_max, v_max = 1000, 1280, 720
        theta = np.linspace(0, 2*np.pi, self.getNumParticles())
        a, b = 1 * np.cos(theta), 1 * np.sin(theta)
        r = np.random.rand((self.getNumParticles()))
        x, y = radius*r * np.cos(theta), radius*r * np.sin(theta)
        self.x = [int(i+640) for i in x]
        self.y = [int(i+360) for i in y]
        self.m = [random.randint(10, 110) for i in range(0, self.getNumParticles())]  

        self.vx = np.float32(np.random.uniform(self.getMinV(), self.getMaxV(), self.getNumParticles()))
        self.vy = np.float32(np.random.uniform(self.getMinV(), self.getMaxV(), self.getNumParticles())) 
        self.z = np.float32(np.random.uniform(self.getMinPos(), self.getMaxPos(), self.getNumParticles()))
        self.vz = np.float32(np.random.uniform(self.getMinV(), self.getMaxV(), self.getNumParticles()))

    
    def compareArrays(self,a,b,name,rel_tol):        
        result = True
        if(np.allclose(a,b,rtol=rel_tol)==False):
            print(f"Comparison Failed! {name} array doesn't match")
            result = False
            
            allResults = np.isclose(a,b,rtol=rel_tol)
            
            #print values not equal to each other
            for d1, d2,res in zip(a,b,allResults):
                print(d1,d2,res)
        
        if (result == True):
            print(f"Comparision successful! {name} matches golden data")
        return result

    def compare(self, p2):
        rel_tol = float("1e-4")
        result = True 
        result &= self.compareArrays(self.x,p2.x,"x",rel_tol)
        result &= self.compareArrays(self.y,p2.y,"y",rel_tol)
        result &= self.compareArrays(self.z,p2.z,"z",rel_tol)
        result &= self.compareArrays(self.vx,p2.vx,"vx",rel_tol)
        result &= self.compareArrays(self.vy,p2.vy,"vy",rel_tol)
        result &= self.compareArrays(self.vz,p2.vz,"vz",rel_tol)
        result &= self.compareArrays(self.m,p2.m,"m",rel_tol)
        return result
