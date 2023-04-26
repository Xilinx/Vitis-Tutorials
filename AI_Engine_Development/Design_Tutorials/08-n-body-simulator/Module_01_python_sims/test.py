#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT


import unittest
import os
import sys 
sys.path.insert(1,'./pylib')
import nbody
import numpy as np 
from particles import *          
import time

class PySimUnitTest(unittest.TestCase):
    
    #command-line settings
    CREATE_ANIMATION_DATA = "OFF"

    """
    A class that conducts unit tests on AIESIMULATOR results. 
    """

    def setUp(self):
        self.softening_factor_2 = np.float32(1000.0)
        self.timestep = np.float32(1.0)
        self.numTimeSteps = 1 

    def tearDown(self):
        #simulate and get output particles
        print("Simulating %s particles for 1 timestep" % self.NUM_I_PARTICLES)
        start_time = time.time()
        self.output_particles = nbody.compute(self.particles_j,self.particles_i,self.softening_factor_2,self.timestep) 
        stop_time = time.time()
        print("Elapsed time for NBody Simulator executed in x86 machines using python is %s seconds ..." % (stop_time - start_time))
        
        if self.CREATE_ANIMATION_DATA=="ON":
            filename = f'build/animation_data_{self.particles_i.getNumParticles()}_particles.txt'
            print(f"creating animation data file: {filename}") 

            with open(filename, 'w') as ofh:
                for i in range(300):
                    for p in range(self.particles_i.getNumParticles()):
                        x =self.particles_i.x[p] 
                        y =self.particles_i.y[p] 
                        z =self.particles_i.z[p] 
                        m =self.particles_i.m[p] 

                        ofh.write(str(x) + " " + str(y) + " " + str(z)+ " "+ str(m)+ "\n") 
                    self.output_particles = nbody.compute(self.particles_j,self.particles_i,self.softening_factor_2,self.timestep) 
                    self.particles_i = self.output_particles
                    self.particles_j = self.output_particles

    def test_random_x100(self):
        """ Sets all x,y,z,vx,vy,vz,m to be random values. 
        """          

        #Initialize Particles
        self.NUM_I_PARTICLES = 12800
        self.NUM_J_PARTICLES = self.NUM_I_PARTICLES

        self.particles_i = Particles(self.NUM_I_PARTICLES)
        #self.particles_i.setInitialConditions()
        self.particles_i.setSphereInitialConditions()
        self.particles_j = Particles(self.NUM_J_PARTICLES)

        self.particles_j.x = self.particles_i.x[0:self.NUM_J_PARTICLES]  
        self.particles_j.y = self.particles_i.y[0:self.NUM_J_PARTICLES]  
        self.particles_j.z = self.particles_i.z[0:self.NUM_J_PARTICLES]  
        self.particles_j.vx = self.particles_i.vx[0:self.NUM_J_PARTICLES]  
        self.particles_j.vy = self.particles_i.vy[0:self.NUM_J_PARTICLES]  
        self.particles_j.vz = self.particles_i.vz[0:self.NUM_J_PARTICLES]  
        self.particles_j.m = self.particles_i.m[0:self.NUM_J_PARTICLES]  


    def test_random_x10(self):
        """ Sets all x,y,z,vx,vy,vz,m to be random values. 
        """          

        #Initialize Particles
        self.NUM_I_PARTICLES = 1280
        self.NUM_J_PARTICLES = self.NUM_I_PARTICLES

        self.particles_i = Particles(self.NUM_I_PARTICLES)
        #        self.particles_i.setInitialConditions()
        self.particles_i.setSphereInitialConditions()
        self.particles_j = Particles(self.NUM_J_PARTICLES)

        self.particles_j.x = self.particles_i.x[0:self.NUM_J_PARTICLES]  
        self.particles_j.y = self.particles_i.y[0:self.NUM_J_PARTICLES]  
        self.particles_j.z = self.particles_i.z[0:self.NUM_J_PARTICLES]  
        self.particles_j.vx = self.particles_i.vx[0:self.NUM_J_PARTICLES]  
        self.particles_j.vy = self.particles_i.vy[0:self.NUM_J_PARTICLES]  
        self.particles_j.vz = self.particles_i.vz[0:self.NUM_J_PARTICLES]  
        self.particles_j.m = self.particles_i.m[0:self.NUM_J_PARTICLES]  

    def test_random_x1(self):
        """ Sets all x,y,z,vx,vy,vz,m to be random values. 
        """          

        #Initialize Particles
        self.NUM_I_PARTICLES = 128
        self.NUM_J_PARTICLES = self.NUM_I_PARTICLES

        self.particles_i = Particles(self.NUM_I_PARTICLES)
       # self.particles_i.setInitialConditions()
        self.particles_i.setSphereInitialConditions()

        self.particles_j = Particles(self.NUM_J_PARTICLES)

        self.particles_j.x = self.particles_i.x[0:self.NUM_J_PARTICLES]  
        self.particles_j.y = self.particles_i.y[0:self.NUM_J_PARTICLES]  
        self.particles_j.z = self.particles_i.z[0:self.NUM_J_PARTICLES]  
        self.particles_j.vx = self.particles_i.vx[0:self.NUM_J_PARTICLES]  
        self.particles_j.vy = self.particles_i.vy[0:self.NUM_J_PARTICLES]  
        self.particles_j.vz = self.particles_i.vz[0:self.NUM_J_PARTICLES]  
        self.particles_j.m = self.particles_i.m[0:self.NUM_J_PARTICLES]  

    def test_3_particles(self):
        """ Sets all x,y,z,vx,vy,vz,m to be random values. 
        """          

        #Initialize Particles
        self.NUM_I_PARTICLES = 3
        self.NUM_J_PARTICLES = self.NUM_I_PARTICLES

        self.particles_i = Particles(self.NUM_I_PARTICLES)
        self.particles_i.setInitialConditions()
        self.particles_j = Particles(self.NUM_J_PARTICLES)

        self.particles_j.x = self.particles_i.x[0:self.NUM_J_PARTICLES]  
        self.particles_j.y = self.particles_i.y[0:self.NUM_J_PARTICLES]  
        self.particles_j.z = self.particles_i.z[0:self.NUM_J_PARTICLES]  
        self.particles_j.vx = self.particles_i.vx[0:self.NUM_J_PARTICLES]  
        self.particles_j.vy = self.particles_i.vy[0:self.NUM_J_PARTICLES]  
        self.particles_j.vz = self.particles_i.vz[0:self.NUM_J_PARTICLES]  
        self.particles_j.m = self.particles_i.m[0:self.NUM_J_PARTICLES]  
        
        
if __name__ == '__main__':
    PySimUnitTest.CREATE_ANIMATION_DATA = os.environ.get("CREATE_ANIMATION_DATA",PySimUnitTest.CREATE_ANIMATION_DATA)
    unittest.main()
    
