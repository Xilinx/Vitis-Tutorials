function Y = convergent(X)
if isreal(X)
    Z = rem(X,2);
    Z(abs(Z)~=0.5) = 0;
else
    R = rem(real(X),2);
    J = rem(imag(X),2);
    R(abs(R)~=0.5) = 0;
    J(abs(J)~=0.5) = 0;
    Z = complex(R,J);
end
Y = round(X-Z);