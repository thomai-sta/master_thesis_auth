function [m3] = m23est(x)

N = length(x);
M = 2*N-1;
m3 = zeros(M,M,M,M);
%% Properties:
% m3(m1,m2) = m3(m2,m1) = m3(m1-m2,-m2)
%     = m3(m2-m1,-m1) = m3(-m1,m2-m1) = m3(-m2,m1-m2)
% m3(m1,m2) = 0, gia kathe (m1,m2):|m1-m2|>=N Arkei na upologistoun
%     ta stoixeia tou pinaka gia m1,m2:[-(N-1),...,0]x[-(N-1),...,0]

%%
% Gia kathe diaforetiko m1 to apotelesma einai enas 2D pinakas.
for m11 = 1:N
    for m12 = 1:N
        for m21 = m11:N
            for m22 = m12:N
%                 shift x
                x_shifted_1 = x(m21-m11+1:m21,m22-m12+1:m22);
                x_shifted_2 = x(1:m11,1:m12);
                x_masked = x(N-m11+1:N,N-m12+1:N);
                m3(m11,m12,m21,m22) = sum(sum(x_masked.*x_shifted_1.*x_shifted_2))/N^2;
                m3(m21,m22,m11,m12) = m3(m11,m12,m21,m22);
                m3(M-m11+1,M-m12+1,N+m21-m11,N+m22-m12) = m3(m11,m12,m21,m22);
                m3(N+m11-m21,N+m12-m22,M-m21+1,M-m22+1) = m3(m11,m12,m21,m22);
                m3(N+m21-m11,N+m22-m12,M-m11+1,M-m12+1) = m3(m11,m12,m21,m22);
                m3(M-m21+1,M-m22+1,N+m11-m21,N+m12-m22) = m3(m11,m12,m21,m22);
                clear x_shifted_2;
                clear x_masked;
                clear x_shifted_1;
            end
        end
    end
end
