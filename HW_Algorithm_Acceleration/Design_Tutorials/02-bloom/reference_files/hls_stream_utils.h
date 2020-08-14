#pragma once

#include<ap_int.h>
#include<hls_stream.h>

namespace hls_stream 
{
  template<typename T>
  void buffer(hls::stream<T> &stream_o, T* axi_i, unsigned int nvals)
  {
    axiToStreamLoop: for(int i=0; i<nvals; i++) {
      stream_o.write( axi_i[i] );
    }  
  }

  template<typename T>
  void buffer(T* axi_o, hls::stream<T> &stream_i, unsigned int nvals)
  {
    streamToAxiLoop: for(int i=0; i<nvals; i++) {
      axi_o[i] = stream_i.read();
    }  
  }

  template<int Wo, int Wi>
  void resize(hls::stream<ap_uint<Wo> > &stream_o, hls::stream<ap_uint<Wi> > &stream_i, unsigned int nvals)
  {
    if (Wo<Wi) { // Wide to Narrow
      ap_uint<Wi> tmp;
      int nwrites = Wi/Wo;
      int nreads  = nvals;
      resizeStreamW2NLoop: for(int i=0; i<nreads; i++) {
        for (int j=0; j<nwrites; j++) {
          if (j==0) tmp = stream_i.read();
          stream_o.write( tmp(Wo-1+Wo*j, Wo*j) );
        }
      }
    }

    if (Wo>Wi) { // Narrow to Wide
      ap_uint<Wo> tmp;
      int nwrites = nvals;
      int nreads  = Wo/Wi;

      resizeStreamN2WLoop: for(int i=0; i<nwrites; i++) {
        for (int j=0; j<nreads; j++) {
          tmp(Wi-1+Wi*j, Wi*j) = stream_i.read();
          if (j==(Wo/Wi-1)) stream_o.write(tmp);
        }
      }
    }

    if (Wo==Wi) { // Equal sizes
      resizeStreamEqLoop: for(int i=0; i<nvals; i++) {
        stream_o.write( stream_i.read() );      
      }
    }
  }
}
