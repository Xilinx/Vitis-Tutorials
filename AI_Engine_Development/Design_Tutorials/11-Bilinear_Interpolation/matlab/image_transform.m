%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

% This function is intended to generate pixel coordinates used for testing
% bilinear interpolation. It accepts an input image and performs a series
% of transformations such as translation, shearing, scaling, and rotation
% according to a specified sequence. A user may edit this file to modify
% the transformations performed.

% Inputs to the function are:
%        img_file: A file containing a grayscale image. If no filename is
%                  specified, the xsj.jpg image is used.
%        out_sz:   An array [xdim, ydim] specifying the dimension within
%                  which the input image pixel coordinates are transformed.
%                  If no dimensions are specified, the defalt is 
%                  [1024, 1024].
%        num_frm:  A positive integer specifying the number of frames to
%                  generate, each corresponding to a different coordinate
%                  transformation. The default is 50 if no values are 
%                  specified.

% The function output is a file named bli_coords.mat containing the
% following data:
%        I:        The original image, where pixel coordinates are assumed
%                  to be defined on an array of positive integers.
%        Xq:       An (xdim x ydim x num_frm) array of X values, which are
%                  paired with corresponding Y values to be used as query
%                  points for interpolation. Each (xdim x ydim) sub-array
%                  corresponds to a single transformed image.
%        Yq:       An (xdim x ydim x num_frm) array of Y values, which are
%                  paired with corresponding X values to be used as query
%                  points for interpolation. Each (xdim x ydim) sub-array
%                  corresponds to a single transformed image.

function image_transform(img_file, out_sz, num_frm)

% assign default arguments if necessary
if nargin < 3
    num_frames = 50;
else
    num_frames = num_frm;
end
if nargin < 2
    output_dim = [1024, 1024];
else
    output_dim = out_sz;
end
if nargin == 0
    fname = '../images/epyc.jpg';
else
    fname = img_file;
end

% get reference image and parameters
I = imread(fname);
[resy, resx] = size(I);
xshft = (resx-1)/2;
yshft = (resy-1)/2;
[Xref, Yref] = meshgrid((0:resx-1)' - xshft, (0:resy-1)' - yshft); 

% set parameters for transformations
trnx_max = 50.0;  % translation
trny_max = 80.0;
trnx = trnx_max * sin(linspace(0,pi,num_frames));
trny = trny_max * sin(linspace(0,pi,num_frames));
shrx_max = 0.5;  % shear
shry_max = 0.2;
shrx = shrx_max * sin(linspace(0,pi,num_frames));
shry = shry_max * sin(linspace(0,pi,num_frames));
sclx_max = 0.9;  % scale
scly_max = 1.6;
sclx = 1.0 + (sclx_max-1) * sin(linspace(0,pi,num_frames));
scly = 1.0 + (scly_max-1) * sin(linspace(0,pi,num_frames));
rot_range = 2 * pi;  % rotate
theta = linspace(0,rot_range,num_frames);

% get starting output grid and allocate space for transformed grids
[Xo, Yo] = meshgrid((0:output_dim(1)-1)' - (output_dim(1)-1)/2, (0:output_dim(2)-1)' - (output_dim(2)-1)/2); 

Xq = zeros(output_dim(2),output_dim(1),num_frames);
Yq = zeros(output_dim(2),output_dim(1),num_frames);
coords = [Xo(:) Yo(:)]';
Xq(:,:,1) = Xo + xshft;
Yq(:,:,1) = Yo + yshft;

for idx = 2:num_frames
    %translate
    vec_xfrm = coords + [trnx(idx); trny(idx)];

    % shear
    Mshr = [1 shrx(idx); shry(idx) 1];
    vec_xfrm = Mshr * vec_xfrm;

    % scale
    Mscl = [sclx(idx) 0; 0 scly(idx)];
    vec_xfrm = Mscl * vec_xfrm;

    % rotate
    Mrot = [cos(theta(idx)) -sin(theta(idx)); sin(theta(idx)) cos(theta(idx))];
    vec_xfrm = Mrot * vec_xfrm;

    % add to output array
    Xtf = reshape(vec_xfrm(1,:)',output_dim(2),output_dim(1));
    Ytf = reshape(vec_xfrm(2,:)',output_dim(2),output_dim(1));

    imshow(interp2(Xref,Yref,double(I),Xtf,Ytf,'linear',0) / 255)
    drawnow

    Xq(:,:,idx) = Xtf + xshft;
    Yq(:,:,idx) = Ytf + yshft;
end

save('bli_coords.mat', 'I', 'Xq', 'Yq', '-v7.3');
end