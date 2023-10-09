/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "include.h"
#include "globals.h"

#define POS_TIMESTEP 0
#define POS_SF2      1
#define POS_I_X      2
#define POS_I_Y      3
#define POS_I_Z      4

void nbody(       
	input_window_float * w_input_i,         // x, y, z, vx, vy, vz, m
	input_window_float * w_input_j,         // x, y, z, m
	output_window_float * w_output_i 	// x, y, z, vx, vy, vz, m
)  {

	v8float acc      = undef_v8float();
	v8float ts_sf2_i = undef_v8float();
	ts_sf2_i         = upd_elem(ts_sf2_i, POS_TIMESTEP, timestep);
	const v8float sf2 = ext_w(fpshuffle16(::xset_w(0, shft_elem(undef_v8float(), softening_factor_2)), 0, 0x00000000, 0x00000000), 0);        

	// Step 1 of 4 - Save i from input window  - once per iter
	{
		v8float * ptr_i  = (v8float *)particles_i;
		for (int n=0; n<(7*NUM_I_VECTORS); n++) 	{
			*ptr_i = window_readincr_v8(w_input_i);
			ptr_i++;
		}
	}
 	

 	// Step 2 of 4 - Compute new i for all particles (independent of acc-i
	// calculate new position using vi and timestep - out of the way
	{
		
		const v8float * restrict xi_v8      = (v8float *)(particles_i)     + (0*NUM_I_VECTORS); 	
		const v8float * restrict yi_v8      = (v8float *)(particles_i)     + (1*NUM_I_VECTORS); 	
		const v8float * restrict zi_v8      = (v8float *)(particles_i)     + (2*NUM_I_VECTORS); 	
		const v8float * restrict vxi_v8     = (v8float *)(particles_i)     + (3*NUM_I_VECTORS); 	
		const v8float * restrict vyi_v8     = (v8float *)(particles_i)     + (4*NUM_I_VECTORS); 	
		const v8float * restrict vzi_v8     = (v8float *)(particles_i)     + (5*NUM_I_VECTORS); 	
		v8float * restrict xi_v8_new  = (v8float *)(particles_i_new) + (0*NUM_I_VECTORS); 	
		v8float * restrict yi_v8_new  = (v8float *)(particles_i_new) + (1*NUM_I_VECTORS); 	
		v8float * restrict zi_v8_new  = (v8float *)(particles_i_new) + (2*NUM_I_VECTORS); 	
		
		for (int i=0; i<NUM_I_VECTORS; i++) {
			#ifdef KERNEL_BYPASS
				*xi_v8_new = *xi_v8; xi_v8_new++; xi_v8++; 
				*yi_v8_new = *yi_v8; yi_v8_new++; yi_v8++;
				*zi_v8_new = *zi_v8; zi_v8_new++; zi_v8++; 
			#else
				*xi_v8_new = fpmac(*xi_v8, ::xset_w(0, ts_sf2_i), POS_TIMESTEP, 0x00000000, *vxi_v8, 0, 0x76543210); xi_v8_new++; xi_v8++; vxi_v8++; 
				*yi_v8_new = fpmac(*yi_v8, ::xset_w(0, ts_sf2_i), POS_TIMESTEP, 0x00000000, *vyi_v8, 0, 0x76543210); yi_v8_new++; yi_v8++; vyi_v8++; 
				*zi_v8_new = fpmac(*zi_v8, ::xset_w(0, ts_sf2_i), POS_TIMESTEP, 0x00000000, *vzi_v8, 0, 0x76543210); zi_v8_new++; zi_v8++; vzi_v8++;
			#endif
		}
	}	

	// Step 3 of 4 - Compute acc-i for all i for a given set-j
	for ( int j=0; j<LOOP_COUNT_J ; j++) {
	        
	        window_acquire(w_input_j); 

		// Step 3.1 - read all j's - window has all this data
		{
			v8float * ptr_j     = (v8float *)particles_j;
			for (int k=0; k<(4*NUM_J_VECTORS);k++) 	{
				*ptr_j = window_readincr_v8(w_input_j);
				ptr_j++;
			}
		}

	        window_release(w_input_j); 
	
		// Step 3.2
		for (int i=0; i<NUM_I; i++) {
			float *xi = (particles_i + (0*NUM_I) + i);
			float *yi = (particles_i + (1*NUM_I) + i);
			float *zi = (particles_i + (2*NUM_I) + i);
			float temp_x[NUM_J];
			float temp_y[NUM_J];
			float temp_z[NUM_J];
			float temp_d[NUM_J];
			ts_sf2_i = upd_elem(ts_sf2_i, POS_I_X, *xi);
			ts_sf2_i = upd_elem(ts_sf2_i, POS_I_Y, *yi);
			ts_sf2_i = upd_elem(ts_sf2_i, POS_I_Z, *zi);

			// Step 3.2.1 - calculate dx, dy, dz
			{
				v8float * restrict dx = (v8float *)temp_x;
				v8float * restrict dy = (v8float *)temp_y;
				v8float * restrict dz = (v8float *)temp_z;
				const v8float * restrict xj_v8  = (v8float *)(particles_j) + (0*NUM_J_VECTORS);
				const v8float * restrict yj_v8  = (v8float *)(particles_j) + (1*NUM_J_VECTORS);
				const v8float * restrict zj_v8  = (v8float *)(particles_j) + (2*NUM_J_VECTORS);

				//for (int jj=0; jj<NUM_J_VECTORS; jj++)  {
				for (int jj=0; jj<NUM_J_VECTORS; jj++) chess_loop_range(4,) chess_prepare_for_pipelining {
					const v8float rx = fpsub(*xj_v8, ::xset_w(0, ts_sf2_i) , POS_I_X , 0x00000000); xj_v8++;
					const v8float ry = fpsub(*yj_v8, ::xset_w(0, ts_sf2_i) , POS_I_Y , 0x00000000); yj_v8++;
					const v8float rz = fpsub(*zj_v8, ::xset_w(0, ts_sf2_i) , POS_I_Z , 0x00000000); zj_v8++;
					*dx = rx; dx++;  
					*dy = ry; dy++; 
					*dz = rz; dz++; 
				}
			}

			// Step 3.2.2 - calculate l2 dist, dist^(3/2)
			{
				const v8float *restrict dx = (v8float *)temp_x;
				const v8float *restrict dy = (v8float *)temp_y;
				const v8float *restrict dz = (v8float *)temp_z;
				v8float *restrict dd = (v8float *)temp_d;
				//for (int jj=0; jj<NUM_J_VECTORS; jj++)  {
				for (int jj=0; jj<NUM_J_VECTORS; jj++) chess_flatten_loop 
               //chess_loop_range(4,) chess_prepare_for_pipelining
            {
					v8float l2dist = fpmac(sf2,    ::xset_w(0, *dx), 0, 0x76543210, *dx, 0, 0x76543210); dx++; 
					l2dist         = fpmac(l2dist, ::xset_w(0, *dy), 0, 0x76543210, *dy, 0, 0x76543210); dy++; 
					l2dist         = fpmac(l2dist, ::xset_w(0, *dz), 0, 0x76543210, *dz, 0, 0x76543210); dz++; 
					    
					// dist   = invsqrt( l2dist**3 )
					const v8float acc2 = fpmul(::xset_w(0, l2dist), 0, 0x76543210, l2dist, 0, 0x76543210);
					acc  = fpmul(::xset_w(0, l2dist), 0, 0x76543210, acc2, 0, 0x76543210);
			
					v8float ret = undef_v8float();
					for (unsigned h = 0; h < 8; ++h) {
						const float e = ::ext_elem(acc, 7 - h);
						const float r = invsqrt(e);
						ret = ::shft_elem(ret, r);
					}
					*dd = ret; dd++;
				}
			}

			// Step 3.2.3 - 
			{
				v8float accx = null_v8float();
				v8float accy = null_v8float();
				v8float accz = null_v8float();
				const v8float * restrict dx = (v8float *)temp_x;
				const v8float * restrict dy = (v8float *)temp_y;
				const v8float * restrict dz = (v8float *)temp_z;
				const v8float * restrict dd = (v8float *)temp_d;
				const v8float * restrict mj_v8  = (v8float *)(particles_j) + (3*NUM_J_VECTORS);
				//for (int jj=0; jj<NUM_J_VECTORS; jj++)  {
				for (int jj=0; jj<NUM_J_VECTORS; jj++) chess_flatten_loop 
               //chess_loop_range(4,) chess_prepare_for_pipelining
            {
					// s = mass * dist
					acc = fpmul(::xset_w(0, *dd), 0, 0x76543210, *mj_v8, 0, 0x76543210); mj_v8++; dd++;

					// accx += dx*s; accy += dy*s; accz += dz*s
					accx = fpmac(accx, ::xset_w(0, acc), 0, 0x76543210, *dx, 0, 0x76543210); dx++;	
					accy = fpmac(accy, ::xset_w(0, acc), 0, 0x76543210, *dy, 0, 0x76543210); dy++;
					accz = fpmac(accz, ::xset_w(0, acc), 0, 0x76543210, *dz, 0, 0x76543210); dz++;
				}
				// dx, dy, dz now have acceleration 
				
				// Accumulate acceleration of i w.r.t all 8 j's - reduction tree
				v8float acceleration = undef_v8float();
				const v8float a_xy = fpadd(concat(ext_v(accx, 0), ext_v(accy, 0)), concat(accx, accy), 0, 0xfedc7654);
	            		const v8float a_z = fpadd(accz, ::xset_w(0, accz), 0, 0x7654);

				acceleration = fpshuffle(  ::concat(a_xy, a_z), 0, 0x951840);
			        acceleration = fpadd(acceleration, ::concat(a_xy, a_z), 0, 0xB73A62);
			        acceleration = fpadd(acceleration, ::xset_w(0, acceleration),   0, 0x543);

				/*accx = fpadd(accx, ::xset_w(0, accx), 0, 0x7654);
				accy = fpadd(accy, ::xset_w(0, accy), 0, 0x7654);
				accz = fpadd(accz, ::xset_w(0, accz), 0, 0x7654);

				accx = fpadd(accx, ::xset_w(0, accx), 0, 0x32);
				accy = fpadd(accy, ::xset_w(0, accy), 0, 0x32);
				accz = fpadd(accz, ::xset_w(0, accz), 0, 0x32);

				accx = fpadd(accx, ::xset_w(0, accx), 0, 0x1);
				accy = fpadd(accy, ::xset_w(0, accy), 0, 0x1);
				accz = fpadd(accz, ::xset_w(0, accz), 0, 0x1);
				acceleration = upd_elem(acceleration, 0, ext_elem(accx, 0));
				acceleration = upd_elem(acceleration, 1, ext_elem(accy, 0));
				acceleration = upd_elem(acceleration, 2, ext_elem(accz, 0));
				*/
				float *vxi = (particles_i + (3*NUM_I) + i);
				float *vyi = (particles_i + (4*NUM_I) + i);
				float *vzi = (particles_i + (5*NUM_I) + i);
				v8float velocity = undef_v8float();
				velocity = upd_elem(velocity, 0, *vxi);
				velocity = upd_elem(velocity, 1, *vyi);
				velocity = upd_elem(velocity, 2, *vzi);
				
				
				acc = fpmac(velocity, ::xset_w(0, ts_sf2_i), POS_TIMESTEP, 0x00000000, acceleration, 0, 0x76543210 );
				#ifndef KERNEL_BYPASS
					*vxi = ext_elem(acc, 0);
					*vyi = ext_elem(acc, 1);
					*vzi = ext_elem(acc, 2);
				#endif
			}
			
		}
	}	
		// Step 4 of 4 - Transmit new i
		// send x, y, z
	{	v8float *newi  = (v8float *)(particles_i_new);
	        for ( int k=0; k<(3*NUM_I_VECTORS) ; k++) {
		       window_writeincr(w_output_i, *newi );
		       newi++;
		}
		
	}
	
	// send vx, vy, vz
	{	v8float *newi  = (v8float *)(particles_i) + (3*NUM_I_VECTORS);
	        for ( int k=0; k<(3*NUM_I_VECTORS) ; k++) {
		       window_writeincr(w_output_i, *newi );
		       newi++;
		}
	}
	
	// send m	
	{	v8float *m  = (v8float *)(particles_i) + (6*NUM_I_VECTORS);
	        for ( int k=0; k<NUM_I_VECTORS ; k++) 	{
	               window_writeincr(w_output_i, *m );
		       m++;
		}
	}
}
