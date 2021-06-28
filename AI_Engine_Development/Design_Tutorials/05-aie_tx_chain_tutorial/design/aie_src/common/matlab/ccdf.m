function [par, logprob] = ccdf(y,dc)

if(~exist('dc','var'))
   dc = 1 ;
end % if

dbfs = 20*log10(std(y)/2^15/dc);
%dbfs = 20*log10(5827/2^15/dc);


power = y.*conj(y) + 1; %add 1 to avoid log of zero
sp = sort(power);
for k = 1:60
    logprob(k) = -k/10; %#ok<AGROW>
    frac = 1 - 10^(-k/10);
    par(k) = 10*log10(sp(floor(frac*length(sp)))/2^30)- dbfs; %#ok<AGROW>
end

if nargout == 0;
    figure; plot(par, logprob); grid;
    axis([0 13 -6 0]);
    title('CCDF');
end
