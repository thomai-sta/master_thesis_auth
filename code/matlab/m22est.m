function [m2] = m22est(x)

N = length(x);
M = 2*N-1;
m2 = zeros(M);
%%
% Isxuei pali oti m3(m) = m3(-m). Tha upologisw to panw miso tou pinaka.
for i = 1:N
    for j = 1:M
        if j<=N
            x_shifted = h(N-i+1:N,N-j+1:N);
            x_masked = h(1:i,1:j);
            m2(i,j) = sum(sum(x_shifted.*x_masked))/N^2;
            m2(M-i+1,M-j+1) = m2(i,j);
            clear x_shifted;
            clear x_masked;
        else
            x_shifted = h(N-i+1:N,1:2*N-j);
            x_masked = h(1:i,j-N+1:N);
            m2(i,j) = sum(sum(x_shifted.*x_masked))/N;
            m2(M-i+1,M-j+1) = m2(i,j);
            clear h_shifted;
            clear h_masked;
        end
    end
end
