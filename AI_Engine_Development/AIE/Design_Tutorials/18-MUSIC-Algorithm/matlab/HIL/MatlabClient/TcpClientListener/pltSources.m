% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT%

% Author B. Berkane, Fidus Systems 
% Snapshot generation 
                      
function pltSources (pmNullAie, doaAieDeg, doaMatDeg, posRef, musiCfg, sysCfg, hilCfg, sweepTime, n)

%% Parameters
sweepSpan = musiCfg.musicSweepSpan;  % Sweep span in degrees
nPts      = musiCfg.musicSweepNPts;  % Sweep Number of points
sweepPts  = linspace(-sweepSpan/2, sweepSpan/2, nPts);
noDetect  = (numel(doaAieDeg) ~= numel(sysCfg.doaDeg));

if ~(sweepTime == 0)
    fprintf('\n          >> Sweep Time (ns): %f \n', sweepTime);
else
    fprintf('\n          >> Sweep Time (ns): -- \n');
end
%err = doaAieDeg- doaMatDeg.';

%% Source Co-ordinate Tracking Plot
positionPlot=subplot(2,1,1);

% loop through angles
if (~noDetect) % detected all sources  
    % Draw antenna --------------------------------------------------------
    x1 = 0.0; x2 =-0.15;
    y1 = 0.02;
    x = [x2, x1];
    y = [y1, y1];

    plot(x, y, 'b-', 'LineWidth', 1);
    for i =1:8 
    text(-0.007-(i-1)*0.02, 0, '\forall')
    end
    text(-0.10, -0.07, ' ULA')
    % ---------------------------------------------------------------------

    % title, x-y labels and legend ----------------------------------------
    title('DOA Detection of Moving Sources');
    xlabel('X'); ylabel('Y'); grid on;
    text(0.6, 0.1, '\fontname{Courier} \Delta: True Positions', 'FontSize',10')
    % text(0.6, 0.85, '\fontname{Courier} \rightarrow: Detected DOAs', 'FontSize',10')
    hold on
    % ---------------------------------------------------------------------


    % For reference only draw unit 1/2 circle -----------------------------
    r = 1;
    xCenter = 0; yCenter = 0;
    angles  = linspace(0, pi, 256);
    xCircle = xCenter+r*cos(angles);
    yCircle = yCenter+r*sin(angles);
    plot(xCircle, yCircle, 'k--','LineWidth', 0.1)
    hold on
    % ---------------------------------------------------------------------

    % Draw arrow for boresight --------------------------------------------
    p1    =          [0.00     0.00];
    p2    =          [0.00     0.65];
    delta = p2-p1;  %[delta(1) delta(2)]
    quiver(p1(1),p1(2),delta(1),delta(2),"filled",'b:', 'LineWidth', 1, 'MaxHeadSize', 3);
    text(-0.055, 0.62, '\fontname{Courier} Boresight')
    % --------------------------------------------------------------------

else % Stop plots when 1 or more soures are not detected
    plot (1:1:10,1:1:10, '-w'); % << .. draw invisile plot!
    strg = strcat('Snapshot #',num2str(n));
    text(2,9,strg,'Color','k','FontSize',12);
    text(2,8,'\fontname{Courier} -- MuSiC Did Not Detect 1 or more Sources','Color','k','FontSize',12);
    text(2,7,'\fontname{Courier} -- Plotting Resumes When MuSiC Detects all Sources','Color','k','FontSize',12);

    % err = abs(doaDeg_cfg-doaDeg_est);
    %     strg = strcat ("Reference / Estimated DOA Angle (deg): ", " ", ...
    %         num2str(sort(cfg.doaDeg.'), '%.2f'), " / ", num2str(sort(data.doaEstDeg.'), '%.2f'), ',  Error = ', num2str(err, '%.2f'));
    %     text(0.2, 0.6, strg,'Color','black','FontSize',10 )
end

switch noDetect

    case false
        
        if (~isempty(doaAieDeg))
        for k =1:length(doaAieDeg)
            % Draw a line for doa slop
            bStr = dec2bin(k,3);
            colorCode = bStr -'0';
            [xSource, ySource] = boxCordinates(doaAieDeg(k));
            %    fprintf(' (%.2f, %.2f), (%.2f, %.2f), doa %.2f \n', xSource, xSource, pos(k,1), pos(k,2), doa(k));
            line([xCenter, xSource], [yCenter, ySource], 'Color', colorCode, 'LineWidth', 1);
            plot (posRef(k,1), posRef(k,2),'^', 'MarkerSize',15);

            hold on
            text(xSource, ySource, strcat('\fontname{Courier} \leftarrow Detected DOA', num2str(k)'))

            % Arrows representing EM waves
            quiver (xSource, ySource,  0.95*(xCenter-xSource), 0.95*(yCenter-ySource), 0 ,'Color', colorCode, 'MaxHeadSize', 0.12)
            quiver (xSource, ySource,  0.75*(xCenter-xSource), 0.75*(yCenter-ySource), 0 ,'Color', colorCode, 'MaxHeadSize', 0.08)

            % print Snapshot nbr on plot
            str = strcat('\fontname{Courier} Snapshot   #',num2str(n));
            text(-0.9,0.17,str,'Color','k','FontSize',12)

            % Print Sweep Time on Plot 
            
            if (sweepTime ~= 0)
                strg = strcat('\fontname{Courier} Sweep-Time ', " ", num2str(sweepTime), 'ns');
            else
                strg = strcat('\fontname{Courier} Sweep-Time  --');
            end

           
            text(-0.9,0.1,strg,'Color','k','FontSize',12)

            % error 
            % strg = strcat ("Reference / Estimated DOA Angle (deg): ", " ", ...
            %     num2str(sort(doaAieDeg.'), '%.2f'), " / ", num2str(sort(doaAieDeg.'), '%.2f'), ',  Error = ', num2str(err, '%.2f'));
            % text(0.2, 0.6, strg,'Color','black','FontSize',10 )
        end

        else
            % print Snapshot nbr on plot
            str = strcat('\fontname{Courier} Snapshot   #',num2str(n));
            text(-0.9,0.17,str,'Color','k','FontSize',12)

            % Print Sweep Time on Plot  
            if (sweepTime ~= 0)
             
                strg = strcat('\fontname{Courier} Sweep-Time ', " ", num2str(sweepTime), 'ns');
            else
                strg = strcat('\fontname{Courier} Sweep-Time  --');
            end
            text(-0.9,0.1,strg,'Color','k','FontSize',12)
        end
        hold off;

    case true

        % do nothing .. OPTIONAL pause to allow time to look into spectrum
        % pause(0.5)

end



%% Pseudo Spectrum plot
spectrumPlot=subplot(2,1,2);
pmPeak = 1./pmNullAie;
pmPeakNorm = pmPeak./max(pmPeak);

switch noDetect

    case false
        plot(sweepPts (1:end), pmPeakNorm, 'k.-')
        hold on
        title('Music Peaks');
        xlabel('Angle of Arrival (deg) w.r.t. Boresight'); ylabel('MUSIC Pseudo-Spectrum'); grid on;

        % Estimated DOAs
        doaAieDeg = sort(doaAieDeg);

        for source = 1 : sysCfg.nSource
            bStr = dec2bin(source,3);
            colorCode = bStr -'0';
            xline(doaAieDeg(source),'-.', 'Color', colorCode, 'LineWidth', 2);
        end
        hold off

    case true
        % Do Nothing  .. Note that condition set by default to always false
        % to execute only the false branch

end

% Add AMD / Fidus Logo
H =gca; % handle to the current axis in the currentfigure
haPos = get(H,'position');
haPos(1:2) = [0.92, 0.92]; % chnage position of logo here
ha2=axes('position',[haPos(1:2), .06,.08,]);
[x, ~]=imread('AmdFidusLogo.png'); % fidus Built commun Logo
image(x)
set(ha2,'handlevisibility','off','visible','off') % off axis of logo
 
%% Record the actions!


if (hilCfg.capVideo == true) 
    if ~isfolder('Videos')
        mkdir ('Videos')
    end
    filename = 'Videos/musicInAction_tmp.gif';
    pause (1.0)
    drawnow 
    frame = getframe(1);
    im = frame2im(frame);
    [imind,cmap] = rgb2ind(im,256);

    if n == 1
        imwrite(imind,cmap,filename,'gif', 'Loopcount',inf);
    elseif n==sysCfg.nSnapshots
         imwrite(imind,cmap,filename,'gif','WriteMode','append');
         copyfile    'Videos/musicInAction_tmp.gif'  'Videos/musicInAction.gif';
    else
        imwrite(imind,cmap,filename,'gif','WriteMode','append');
    end
end
hold off;

end


%% Angle w.r.t. Boresight to x,y in [-1, 1] x [0, 1] 
%  [-1, 1] x [0, 1] box represents the field view
function [x, y] = boxCordinates(angle)

angle    = 90-angle;
angleRad = deg2rad(angle);
if (angle < 45)
    x = 1;
    y = tan(angleRad);
elseif (angle >= 45) && (angle < 135)
    y  = 1;
    x  = 1/tan(angleRad);
else
    x  = -1;
    y  = -tan(angleRad);
end

end