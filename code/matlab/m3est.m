function [m3] = m3est(x)


N = length(x);

%% Properties:
% m3(m1,m2) = m3(m2,m1) = m3(m1-m2,-m2)
%     = m3(m2-m1,-m1) = m3(-m1,m2-m1) = m3(-m2,m1-m2)
% m3(m1,m2) = 0, gia kathe (m1,m2):|m1-m2|>=N Arkei na upologistoun
%     ta stoixeia tou pinaka gia (m1,m2):[-(N-1),...,0]x[-(N-1),...,0]

%% Calculating m3
M = 2*N-1;
m3 = zeros(M);
% Afou eimaste se arnitika m, tote to sima tha upostei shift pros ta deksia
for i=1:N    
    for j=i:N
%        shift x
        x_shifted_1 = x(j-i+1:j);
        x_shifted_2 = x(1:i);
        x_masked = x(N-i+1:N);
%        calculate m3
        m3(i,j) = sum(x_masked.*x_shifted_1.*x_shifted_2)/N;
        m3(j,i) = m3(i,j);
        m3(M-i+1,N+j-i) = m3(i,j);
        m3(N+i-j,M-j+1) = m3(i,j);
        m3(N+j-i,M-i+1) = m3(i,j);
        m3(M-j+1,N+i-j) = m3(i,j);
        clear x_shifted_2;
        clear x_masked;
        clear x_shifted_1;
    end
end
