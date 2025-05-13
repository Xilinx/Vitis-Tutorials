%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function plot_sar(imX,range,label)
  imshow(imX,range,'XData',[-50,50],'Ydata',[-50,50]); 
  title(label);
  ax = gca;
  set(ax,'XTick',[-50:25:50]);
  set(ax,'XTickLabels',[-50:25:50]);
  set(ax,'YTick',[-50:25:50]);
  set(ax,'YTickLabels',[-50:25:50]);
  xlabel('X-Range (m)');
  ylabel('Y-Range (m)');
  axis on;
  colorbar;
  colormap jet;
end
