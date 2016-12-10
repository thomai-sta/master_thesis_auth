function [h2] = corr22(h)

[K, L] = size(h);
if K>L
    h = padarray(h,[0 K-L],'post');
    N = K;
elseif L>K
    h = padarray(h,[L-K 0],'post');
    N = L;
else
    N = K;
end
M = 2*N-1;
h2 = zeros(M);
%%
% Isxuei pali oti h3(m) = h3(-m). Tha upologisw to panw miso tou pinaka.
for i = 1:N
    for j = 1:M
        if j<=N
            h_shifted = h(N-i+1:N,N-j+1:N);
            h_masked = h(1:i,1:j);
            h2(i,j) = sum(sum(h_shifted.*h_masked));
            h2(M-i+1,M-j+1) = h2(i,j);
            clear h_shifted;
            clear h_masked;
        else
            h_shifted = h(N-i+1:N,1:2*N-j);
            h_masked = h(1:i,j-N+1:N);
            h2(i,j) = sum(sum(h_shifted.*h_masked));
            h2(M-i+1,M-j+1) = h2(i,j);
            clear h_shifted;
            clear h_masked;
        end
    end
end
if K>L
    h2 = h2(:,2:M-1);
elseif L>K
    h2 = h2(2:M-1,:);
end
