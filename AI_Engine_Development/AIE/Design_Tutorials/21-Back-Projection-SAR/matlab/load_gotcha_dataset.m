%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [gotcha] = load_gotcha_dataset(gotcha_path,pass_u,pol_u,azm_u)
  pass = [1:8];
  pol  = { 'HH','HV','VH','VV'};
  azm = [1:360];
  fprintf(1,sprintf('Loading GOTCHA data (pass: %d, pol: %s, azimuth: %d deg)\n',...
                    pass(pass_u),pol{pol_u},azm(azm_u)));
  fname = sprintf('%s/GOTCHA-CP/DATA/pass%d/%s/data_3dsar_pass%d_az%03d_%s.mat',...
                  gotcha_path,pass(pass_u),pol{pol_u},pass(pass_u),azm(azm_u),pol{pol_u});
  tmp = load(fname);
  gotcha = tmp.data;
end
