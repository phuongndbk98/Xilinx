///////////////////////////////////////////////////////////////////////////
// Copyright 2020 Xilinx
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////


#include <systemc>
#include <vector>
#include <iostream>
#include "hls_stream.h"
#include "ap_int.h"
#include "ap_fixed.h"
using namespace std;
using namespace sc_dt;
class AESL_RUNTIME_BC {
  public:
    AESL_RUNTIME_BC(const char* name) {
      file_token.open( name);
      if (!file_token.good()) {
        cout << "Failed to open tv file " << name << endl;
        exit (1);
      }
      file_token >> mName;//[[[runtime]]]
    }
    ~AESL_RUNTIME_BC() {
      file_token.close();
    }
    int read_size () {
      int size = 0;
      file_token >> mName;//[[transaction]]
      file_token >> mName;//transaction number
      file_token >> mName;//pop_size
      size = atoi(mName.c_str());
      file_token >> mName;//[[/transaction]]
      return size;
    }
  public:
    fstream file_token;
    string mName;
};
unsigned int ap_apatb_in_sample_V_cap_bc;
static AESL_RUNTIME_BC __xlx_in_sample_V_size_Reader("../tv/stream_size/stream_size_in_in_sample_V.dat");
unsigned int ap_apatb_out_sample_V_cap_bc;
static AESL_RUNTIME_BC __xlx_out_sample_V_size_Reader("../tv/stream_size/stream_size_out_out_sample_V.dat");
struct __cosim_s8__ { char data[8]; };
extern "C" void polar_clip(__cosim_s8__*, __cosim_s8__*);
extern "C" void apatb_polar_clip_hw(volatile void * __xlx_apatb_param_in_sample, volatile void * __xlx_apatb_param_out_sample) {
  // collect __xlx_in_sample_tmp_vec
  unsigned __xlx_in_sample_V_tmp_Count = 0;
  unsigned __xlx_in_sample_V_read_Size = __xlx_in_sample_V_size_Reader.read_size();
  vector<__cosim_s8__> __xlx_in_sample_tmp_vec;
  while (!((hls::stream<__cosim_s8__>*)__xlx_apatb_param_in_sample)->empty() && __xlx_in_sample_V_tmp_Count < __xlx_in_sample_V_read_Size) {
    __xlx_in_sample_tmp_vec.push_back(((hls::stream<__cosim_s8__>*)__xlx_apatb_param_in_sample)->read());
    __xlx_in_sample_V_tmp_Count++;
  }
  ap_apatb_in_sample_V_cap_bc = __xlx_in_sample_tmp_vec.size();
  // store input buffer
  __cosim_s8__* __xlx_in_sample_input_buffer= new __cosim_s8__[__xlx_in_sample_tmp_vec.size()];
  for (int i = 0; i < __xlx_in_sample_tmp_vec.size(); ++i) {
    __xlx_in_sample_input_buffer[i] = __xlx_in_sample_tmp_vec[i];
  }
  //Create input buffer for out_sample
  ap_apatb_out_sample_V_cap_bc = __xlx_out_sample_V_size_Reader.read_size();
  __cosim_s8__* __xlx_out_sample_input_buffer= new __cosim_s8__[ap_apatb_out_sample_V_cap_bc];
  // DUT call
  polar_clip(__xlx_in_sample_input_buffer, __xlx_out_sample_input_buffer);
  for (unsigned i = 0; i <ap_apatb_out_sample_V_cap_bc; ++i)
    ((hls::stream<__cosim_s8__>*)__xlx_apatb_param_out_sample)->write(__xlx_out_sample_input_buffer[i]);
}