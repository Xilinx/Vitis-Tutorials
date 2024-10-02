% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems
% This function plots the Music Null space spectrum 

function  plotMusicResults (data, cfg)
close
 


% Space Spectrum  Plot
f = figure('Name','Music  Simulation Plots','NumberTitle','off', 'visible', ...
           cfg.visiblePlot, 'NumberTitle','off','Position', [150 220 900 600]);

%% Plot Inverse Pm (null plot)
subplot(3, 1, 1);
plot(data.sweepPts (1:end), data.Pm_null(1:end-2),'b.-');
title('Music Nulls');
xlabel('Angle of Arrival (deg)'); ylabel('Inverse MUSIC Spectrum'); grid on;
hold on;

for source = 1 : cfg.nSource  
   xline(cfg.doaDeg(source),'-.r');
end

l = length(data.doaEstDeg);
for source = 1 : l 
   xline(data.doaEstDeg(source),'-.k');
end

yline(data.nullThreshold,'-.k', 'Null Threshold', 'LabelVerticalAlignment','middle');

%% Plot Pm (peak plot)
subplot(3, 1, 2);
plot(data.sweepPts (1:end), data.Pm_peak(1:end-2),'b.-');
title('Music Peaks');
xlabel('Angle of Arrival (deg)'); ylabel('MUSIC Spectrum'); grid on;
hold on;

for source = 1 : cfg.nSource  
   xline(cfg.doaDeg(source),'-.r');
end
l = length(data.doaEstDeg);
for source = 1 : l 
   xline(data.doaEstDeg(source),'-.k');
end

doaDeg_cfg  = sort(cfg.doaDeg.');
doaDeg_est  = sort(data.doaEstDeg.');
 

% --------------------------------------------------
if ( isempty(doaDeg_est) )

    scenario =1;

elseif (length(cfg.doaDeg) <= length(data.doaEstDeg))
    scenario =2;
else
    scenario =3;
end
% --------------------------------------------------

switch scenario
    case 1
        subplot(3, 1, 3);
        strg = ('MUSIC DID NOT DETECT ANY SOURCE');
    text(0.35, 0.6, strg,'Color','red','FontSize',10 )
    case 2
        
        %% Print Estimation Results
        subplot(3, 1, 3);
        err = abs(doaDeg_cfg-doaDeg_est);
        strg = strcat ("Reference / Estimated DOA Angle (deg): ", " ", ...
            num2str(sort(cfg.doaDeg.'), '%.2f'), " / ", num2str(sort(data.doaEstDeg.'), '%.2f'), ',  Error = ', num2str(err, '%.2f'));
        text(0.2, 0.6, strg,'Color','black','FontSize',10 )

    case 3
        %% Print Estimation Results
        subplot(3, 1, 3);
        strg = strcat ("Reference DOA Angle (deg): ", " ", ...
            num2str(sort(doaDeg_cfg), '%.2f'));
        text(0.05, 0.6, strg,'Color','black','FontSize',10 )

        strg = strcat ("Estimated DOA Angle (deg): ", " ", ...
            num2str(sort(doaDeg_est), '%.2f'));
        text(0.6, 0.6, strg,'Color','black','FontSize',10 )

        strg = strcat ("Algo. did NOT detect all sources");
        text(0.6, 0.1, strg,'Color','red','FontSize',10 )
end

 
sourceDir      = strcat('S', num2str(cfg.nSource));
dataDir = 'data';
  
if ~isfolder(dataDir)
       mkdir(dataDir)
end
cd(dataDir)

if ~isfolder(sourceDir)
       mkdir(sourceDir)
end
cd(sourceDir)

str      = strcat('S', num2str(cfg.nSource));
fileName = strcat('TestPLot_', str,'.jpg');
saveas(f,fileName);
cd ../..

hold off
