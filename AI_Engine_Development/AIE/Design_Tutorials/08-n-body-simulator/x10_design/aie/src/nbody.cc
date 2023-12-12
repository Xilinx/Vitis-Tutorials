#include "include.h"
#include "globals.h"
using namespace adf;
using namespace aie;
#define POS_TIMESTEP 0
#define POS_SF2      1
#define POS_I_X      2
#define POS_I_Y      3
#define POS_I_Z      4

alignas(aie::vector_decl_align) float inbuffer[224] ;
void nbody(
	input_buffer<float> & __restrict w_input_i,  		// x, y, z, vx, vy, vz, m = NUM_I*7 samples input_buffer<int>  & __restrict i
	input_async_buffer<float> & __restrict w_input_j,
	output_buffer<float> & __restrict w_output_i  		// new x, y, z, vx, vy, vz, m = NUM_I*7 samples
){
	aie::vector<float,8> acc;
	aie::vector<float,8> ts_sf2_i;
	float timestep= 1.0;
	float softening_factor_2 = 1000.0;
	int pos_timestep =0;
	float particles_i[(NUM_I*7)] = {0.0};
  	float particles_i_new[(NUM_I*3)] = {0.0};
  	float particles_j[(NUM_I*4)] = {0.0};
  	auto addr=0;
 	
	/// create ts   ///
	ts_sf2_i.set(timestep,pos_timestep);	

	aie::vector<float,8> sf2 = aie::broadcast<float,8>( softening_factor_2 );   
	auto inIter=aie::begin_vector<8>(w_input_i);
	
	//////// step 1/4 read input i  /////////
  	{
  		vector<float,8>* ptr_i  = (vector<float,8> *)particles_i;
  		for (int n=0; n<(7*NUM_I_VECTORS); n++) {
  			*ptr_i = *inIter++;
			ptr_i++;
      		}
  	}


 	// Step 2 of 4 - Compute new i for all particles (independent of acc-i
	// calculate new position using vi and timestep - out of the way
	{
			
		const vector<float,8> * restrict xi_v8      = (vector<float,8> *)(particles_i)     + (0*NUM_I_VECTORS); 	
		const vector<float,8> * restrict yi_v8      = (vector<float,8> *)(particles_i)     + (1*NUM_I_VECTORS); 	
		const vector<float,8> * restrict zi_v8      = (vector<float,8> *)(particles_i)     + (2*NUM_I_VECTORS); 	
		const vector<float,8> * restrict vxi_v8     = (vector<float,8> *)(particles_i)     + (3*NUM_I_VECTORS); 	
		const vector<float,8> * restrict vyi_v8     = (vector<float,8> *)(particles_i)     + (4*NUM_I_VECTORS); 	
		const vector<float,8> * restrict vzi_v8     = (vector<float,8> *)(particles_i)     + (5*NUM_I_VECTORS); 	
		vector<float,8> * restrict xi_v8_new  = (vector<float,8> *)(particles_i_new) + (0*NUM_I_VECTORS); 	
		vector<float,8> * restrict yi_v8_new  = (vector<float,8> *)(particles_i_new) + (1*NUM_I_VECTORS); 	
		vector<float,8> * restrict zi_v8_new  = (vector<float,8> *)(particles_i_new) + (2*NUM_I_VECTORS); 	
		
		for (int i=0; i<NUM_I_VECTORS; i++) {
			#ifdef KERNEL_BYPASS
				*xi_v8_new = *xi_v8; xi_v8_new++; xi_v8++; 
				*yi_v8_new = *yi_v8; yi_v8_new++; yi_v8++;
				*zi_v8_new = *zi_v8; zi_v8_new++; zi_v8++; 
			#else

				*xi_v8_new = aie::mac( aie::accum<accfloat,8>(*xi_v8 ), ts_sf2_i[POS_TIMESTEP], *vxi_v8 );//fpmac(*xi_v8, ::xset_w(0, ts_sf2_i), POS_TIMESTEP, 0x00000000, *vxi_v8, 0, 0x76543210); 
				*yi_v8_new = aie::mac( aie::accum<accfloat,8>( *yi_v8 ), ts_sf2_i[POS_TIMESTEP], *vyi_v8 );  //fpmac(*yi_v8, ::xset_w(0, ts_sf2_i), POS_TIMESTEP, 0x00000000, *vyi_v8, 0, 0x76543210);
				*zi_v8_new = aie::mac( aie::accum<accfloat,8>( *zi_v8 ), ts_sf2_i[POS_TIMESTEP], *vzi_v8 );//fpmac(*zi_v8, ::xset_w(0, ts_sf2_i), POS_TIMESTEP, 0x00000000, *vzi_v8, 0, 0x76543210); 				
				
				 xi_v8_new++; xi_v8++; vxi_v8++;yi_v8_new++; yi_v8++; vyi_v8++;zi_v8_new++; zi_v8++; vzi_v8++;
			#endif

			
		}
	} 
	// Step 3 of 4 - Compute acc-i for all i for a given set-j
	for ( int j=0; j<LOOP_COUNT_J ; j++) {
		w_input_j.acquire();
		auto jnIter=aie::begin_vector<8>(w_input_j);
		// Step 3.1 - read all j's - window has all this data
		{
			vector<float,8> * ptr_j     = (vector<float,8> *)particles_j;
			//float* d = (float *)ptr_j;
			for (int k=0; k<(4*NUM_J_VECTORS);k++) 	{
				*ptr_j = *jnIter++;
				ptr_j++;
	
			}
	 	 }
		w_input_j.release();
	
		// Step 3.2
		////printf("\n---------------------------step 3.2------------------------------\n");
		for (int i=0; i<NUM_I; i++) {
			float *xi = (particles_i + (0*NUM_I) + i);
			float *yi = (particles_i + (1*NUM_I) + i);
			float *zi = (particles_i + (2*NUM_I) + i);
			float temp_x[NUM_J];
			float temp_y[NUM_J];
			float temp_z[NUM_J];
			float temp_d[NUM_J];
			ts_sf2_i.set(*xi,POS_I_X );
			ts_sf2_i.set(*yi,POS_I_Y );
			ts_sf2_i.set(*zi,POS_I_Z );
			
			// Step 3.2.1 - calculate dx, dy, dz
			{
				vector<float,8> * restrict dx = (vector<float,8> *)temp_x;
				vector<float,8> * restrict dy = (vector<float,8> *)temp_y;
				vector<float,8> * restrict dz = (vector<float,8> *)temp_z;
				const vector<float,8> * restrict xj_v8  = (vector<float,8> *)(particles_j) + (0*NUM_J_VECTORS);
				const vector<float,8> * restrict yj_v8  = (vector<float,8> *)(particles_j) + (1*NUM_J_VECTORS);
				const vector<float,8> * restrict zj_v8  = (vector<float,8> *)(particles_j) + (2*NUM_J_VECTORS);
				//for (int jj=0; jj<NUM_J_VECTORS; jj++)  {
				for (int jj=0; jj<NUM_J_VECTORS; jj++) chess_loop_range(4,) chess_prepare_for_pipelining {
					const vector<float,8> rx = sub(*xj_v8,ts_sf2_i[POS_I_X]); xj_v8++;
					const vector<float,8> ry = sub(*yj_v8, ts_sf2_i[POS_I_Y]);yj_v8++;
					const vector<float,8> rz = sub(*zj_v8, ts_sf2_i[POS_I_Z]);zj_v8++;
					
					*dx = rx;  
					*dy = ry; 
					*dz = rz;					
					dx++;
	                                dy++;
				        dz++; 
				}
			} 
	
			// Step 3.2.2 - calculate l2 dist, dist^(3/2)
			{
				const vector<float,8> *restrict dx = (vector<float,8> *)temp_x;
				const vector<float,8> *restrict dy = (vector<float,8> *)temp_y;
				const vector<float,8> *restrict dz = (vector<float,8> *)temp_z;
				vector<float,8> *restrict dd = (vector<float,8> *)temp_d;
				//for (int jj=0; jj<NUM_J_VECTORS; jj++)  {
				for (int jj=0; jj<NUM_J_VECTORS; jj++) chess_flatten_loop 
	              	//chess_loop_range(4,) chess_prepare_for_pipelining
			        {
					vector<float,8> l2dist = mac(aie::accum<accfloat,8>(sf2), *dx, *dx); dx++;  
					l2dist         = mac(aie::accum<accfloat,8>(l2dist), *dy, *dy); dy++;  
					l2dist         = mac(aie::accum<accfloat,8>(l2dist), *dz, *dz); dz++; 
					    
					const vector<float,8> acc2 = mul(l2dist,l2dist);
					acc  = mul(l2dist,acc2);			
					vector<float,8> ret;
					for (unsigned h = 0; h < 8; ++h) {
						const float e = acc.get(7-h);
						const float r = invsqrt(e);
						ret.push(r);
					}
					*dd = ret; 
					dd++;
	
				}
			}
	
			//Step 3.2.3 - 
			{
				vector<float,8> accx =aie::zeros<float,8>();
				vector<float,8> accy =aie::zeros<float,8>();
				vector<float,8> accz =aie::zeros<float,8>();
				const vector<float,8> * restrict dx = (vector<float,8> *)temp_x;
				const vector<float,8> * restrict dy = (vector<float,8> *)temp_y;
				const vector<float,8> * restrict dz = (vector<float,8> *)temp_z;
				const vector<float,8> * restrict dd = (vector<float,8> *)temp_d;
				const vector<float,8> * restrict mj_v8  = (vector<float,8> *)(particles_j) + (3*NUM_J_VECTORS);
				//for (int jj=0; jj<NUM_J_VECTORS; jj++)  {:
				for (int jj=0; jj<NUM_J_VECTORS; jj++) chess_flatten_loop 
	               		//chess_loop_range(4,) chess_prepare_for_pipelining
	            		{
					// s = mass * dist
					acc = mul(*dd, *mj_v8);
					mj_v8++; dd++;
	
					// accx += dx*s; accy += dy*s; accz += dz*s
					accx = aie::mac(aie::accum<accfloat,8>(accx), acc, *dx);
					accy = aie::mac(aie::accum<accfloat,8>(accy), acc, *dy);
					accz = aie::mac(aie::accum<accfloat,8>(accz), acc, *dz);
					dx++;
					dy++;					
					dz++;									}
				// dx, dy, dz now have acceleration 
				
				// Accumulate acceleration of i w.r.t all 8 j's - reduction tree
				vector<float,8> acceleration ;
				const vector<float,8> a_xy = aie::add(aie::concat(accx.extract<4>(0),accy.extract<4>(0)),aie::concat(accx.extract<4>(1),accy.extract<4>(1)));
				const vector<float,8> a_z = aie::add(accz,aie::shuffle_down_fill(accz,aie::broadcast<float,8>(accz[0]),4));

				vector<float,16> acc_1=aie::concat(a_xy, a_z);
				auto tmp = aie::transpose( acc_1, 4, 4 ); 
   				acceleration = aie::select( aie::shuffle_down( tmp, 1 ), tmp, aie::mask<16>( 7u )).template extract<8>( 0 );  
				acceleration = aie::select( aie::broadcast<float,8>( tmp[0] ), acceleration, aie::mask<8>( 63u ));

				auto tmp1 =aie::shuffle_down_rotate(tmp,8);
				auto acc_2 = aie::select( aie::shuffle_down( tmp1, 1 ), tmp1, aie::mask<16>( 7u )).template extract<8>( 0 );
				acc_2= aie::select( aie::broadcast<float,8>( a_xy[0] ), acc_2, aie::mask<8>( 63u ));
				acceleration = aie::add(acceleration,acc_2);
	
				auto acc_3 = aie::select(aie::broadcast<float,8>( acceleration[0] ),aie::shuffle_down( acceleration, 3 ), aie::mask<8>( 7u ));
				acceleration = aie::add(acceleration,acc_3);
				
				float *vxi = (particles_i + (3*NUM_I) + i);
				float *vyi = (particles_i + (4*NUM_I) + i);
				float *vzi = (particles_i + (5*NUM_I) + i);
				vector<float,8> velocity;
				velocity.set(*vxi,0);			
				velocity.set(*vyi,1);	
				velocity.set(*vzi,2);				
				
				acc = mac(aie::accum<accfloat,8>(velocity), ts_sf2_i[POS_TIMESTEP], acceleration);
				#ifndef KERNEL_BYPASS
					*vxi = acc.get(0);
					*vyi = acc.get(1);
					*vzi = acc.get(2);
				#endif
			}
		}
	}

	// Step 4 of 4 - Transmit new i
	// send x, y, z
	auto out_ptr=aie::begin_vector<8>(w_output_i);
	{
		vector<float,8> *newdi  = (vector<float,8> *)(particles_i_new);
	        for ( int k=0; k<(3*NUM_I_VECTORS) ; k++) {
		       *out_ptr = *newdi ;
		       newdi++;out_ptr++;
		}
	}
	
	// send vx, vy, vz
	{
		vector<float,8> *newvi  = (vector<float,8> *)(particles_i) + (3*NUM_I_VECTORS);
	        for ( int k=0; k<(3*NUM_I_VECTORS) ; k++) {
		       *out_ptr = *newvi ;
		       newvi++;out_ptr++;
		}
	}
	
	// send m	
	{
		vector<float,8> *m  = (vector<float,8> *)(particles_i) + (6*NUM_I_VECTORS);
	        for ( int k=0; k<NUM_I_VECTORS ; k++) 	{
	               *out_ptr= *m ;
		       m++;out_ptr++;
		}
	}
	

}

