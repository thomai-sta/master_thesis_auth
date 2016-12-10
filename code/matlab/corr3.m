function [h3] = corr3(h)

N = length(h);
h1 = hankel(h);
%% Properties:
% h3(m1,m2) = h3(m2,m1) = h3(m1-m2,-m2)
%     = h3(m2-m1,-m1) = h3(-m1,m2-m1) = h3(-m2,m1-m2)
% h3(m1,m2) = 0, gia kathe (m1,m2):|m1-m2|>=N Arkei na upologistoun
%     ta stoixeia tou pinaka gia (m1,m2):[0,...,N-1]x[0,...,N-1]

%% Calculating h3
M = 2*N-1;
h3 = zeros(M);
for i=N:M    
    for j=i:M
%     calculate h3
        h3(i,j) = sum(h1(1,:).*h1(i-N+1,:).*h1(j-N+1,:));
        h3(j,i) = h3(i,j);
        h3(M-i+1,N+j-i) = h3(i,j);
        h3(N+i-j,M-j+1) = h3(i,j);
        h3(N+j-i,M-i+1) = h3(i,j);
        h3(M-j+1,N+i-j) = h3(i,j);
    end
end
