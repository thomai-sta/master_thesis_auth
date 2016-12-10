function [m2] = m2est(x)

N = length(x);

%  m2(-m) = m2(m) ara upologizw mono ta m2(m) gia m = 0,1...N-1
m2 = zeros(1,N);

%% 
for i=1:N
%     shift left x 
    x_shifted = x(1+i:N);
    x_masked = x(1:N-i);
%     calculate m2
    m2(i) = sum(x_masked.*x_shifted)/N;
    clear x_shifted;
    clear x_masked;
end
m0 = sum(x.*x)/N;
m2 = [fliplr(m2) m0 m2];
