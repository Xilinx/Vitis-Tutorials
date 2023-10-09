#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT


import numpy as np
from particles import *

def compute(particles_j, particles_i, softeningFactor2, timestep):
    
    accumulated_accx = np.zeros(particles_i.numParticles)
    accumulated_accy = np.zeros(particles_i.numParticles)
    accumulated_accz = np.zeros(particles_i.numParticles)
    
    for i in range(particles_i.numParticles):
        xi = np.ones(particles_j.numParticles) * particles_i.x[i]
        yi = np.ones(particles_j.numParticles) * particles_i.y[i]
        zi = np.ones(particles_j.numParticles) * particles_i.z[i]
        
        dx = np.subtract(particles_j.x, xi,dtype="float32")
        dy = np.subtract(particles_j.y, yi,dtype="float32")
        dz = np.subtract(particles_j.z, zi,dtype="float32")            
        
        # l2dist = (xj - xi)^2 + (yj - yi)^2  + (zj - zi)^2 + sf2
        l2dist = np.add(np.multiply(dx, dx, dtype="float32"), np.multiply(dy, dy, dtype="float32"), dtype="float32")
        l2dist = np.add(l2dist, np.multiply(dz, dz, dtype="float32"), dtype="float32")
        l2dist = np.add(l2dist, softeningFactor2, dtype="float32")
        
        # r = invsqrt( l2dist**3 )
        r = np.power(l2dist, 3, dtype="float32")
        r = np.float32(1/np.sqrt(r, dtype="float32"))
        
        # s = r * m
        s = np.multiply(r, particles_j.m, dtype="float32")
        
        # acceleration
        accx = np.multiply(dx, s, dtype="float32")
        accy = np.multiply(dy, s, dtype="float32")
        accz = np.multiply(dz, s, dtype="float32")
        
        # accumulate acceleration of ith particle due to all other particles
        accumulated_accx[i] = np.sum(accx, dtype="float32") 
        accumulated_accy[i] = np.sum(accy, dtype="float32") 
        accumulated_accz[i] = np.sum(accz, dtype="float32")   
        
    xNew = np.multiply(particles_i.vx, timestep, dtype="float32")
    yNew = np.multiply(particles_i.vy, timestep, dtype="float32")
    zNew = np.multiply(particles_i.vz, timestep, dtype="float32")
    
    particlesNew = Particles(particles_i.getNumParticles()) 
    particlesNew.x = np.add(particles_i.x, xNew, dtype="float32")
    particlesNew.y = np.add(particles_i.y, yNew, dtype="float32")
    particlesNew.z = np.add(particles_i.z, zNew, dtype="float32")
    particlesNew.m = particles_i.m
    
    #calculate new velocities
    for i in range(particles_i.getNumParticles()): 
        newVX = np.multiply(accumulated_accx[i], timestep, dtype="float32")
        newVX = np.add(particles_i.vx[i], newVX, dtype="float32")
        newVY = np.multiply(accumulated_accy[i], timestep, dtype="float32")
        newVY = np.add(particles_i.vy[i], newVY, dtype="float32")
        newVZ = np.multiply(accumulated_accz[i], timestep, dtype="float32")
        newVZ = np.add(particles_i.vz[i], newVZ, dtype="float32")
        
        particlesNew.vx[i] = newVX
        particlesNew.vy[i] = newVY
        particlesNew.vz[i] = newVZ   
        
    
    particles_i = particlesNew
    return particles_i
