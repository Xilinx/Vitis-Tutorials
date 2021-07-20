function [y, x] = centered_psd(data, fs, no_scale, rbw)
if ~exist('rbw', 'var')
    rbw = .02;
end
if ~exist('no_scale', 'var')
    no_scale = 0;
end

[pxx, w] = pwelch(data, [],[],[], fs, 'twosided');

if(~no_scale)
   p_dB = 10*log10(pxx/max(pxx));
else
   p_dB = 10*log10(pxx);
end % if

x = w - fs/2;
y = [p_dB(end/2 + 1:end); p_dB(1:end/2)];
if nargout == 0
    plot(x, y);
    title('power spectral density');
    xlabel('f MHz');
    ylabel('dBr');
    axis([-fs/2 fs/2 -100 0]);
    grid;
end

