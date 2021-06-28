function Y = rnd_sym_zero(X)
if isreal(X)
    frac_val = abs(rem(X,1));
    [a,b]=find(frac_val == 0.5);
    Y=X-rem(X,1);
    Y(a,b)=ceil(X(a,b)).*(1+sign(X(a,b)))/2 + floor(X(a,b)).*(1-sign(X(a,b)))/2;
else
    frac_val_r = abs(rem(real(X),1));
    frac_val_i = abs(rem(imag(X),1));
    [ar,br]=find(frac_val_r == 0.5);
    [ai,bi]=find(frac_val_i == 0.5);
    Yr=round(real(X));
    Yi=round(imag(X));
    for i = 1:length(ar)
        Yr(ar(i),br(i)) = real(X(ar(i),br(i)))-rem(real(X(ar(i),br(i))),1);
    end
    for i = 1:length(ai)
        Yi(ai(i),bi(i)) = imag(X(ai(i),bi(i)))-rem(imag(X(ai(i),bi(i))),1);
    end
    Y = Yr+1i*Yi;
end
