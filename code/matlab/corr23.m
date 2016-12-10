function [h3] = corr23(h)
%% An to sima den einai tetragwniko
% tote prostithentai midenika sti mikroteri diastasi wste na ginei
% tetragwniko

% [K, L] = size(h);
% if K>L
%     h = padarray(h,[0 K-L],'post');
%     N = K;
% elseif L>K
%     h = padarray(h,[L-K 0],'post');
%     N = L;
% else
%     N = K;
% end
[N1, N2] = size(h);
M1 = 2*N1-1;
M2 = 2*N2-1;
h3 = zeros(M1,M2,M1,M2);
%% Properties:
% h3(m1,m2) = 
% h3(m2,m1) = 
% h3(m1-m2,-m2) = 
% h3(m2-m1,-m1) = 
% h3(-m1,m2-m1) = 
% h3(-m2,m1-m2)

for m11 = 1:M1
    y1 = N1-m11;
    for m12 = 1:M2
        x1 = m12-N2;
        temp1 = zeros(N1,N2);
        if m11<=N1 && m12<=N2
            temp1(N1-m11+1:N1,N2-m12+1:N2) = h(1:m11,1:m12);
        elseif m11<=N1 && m12>N2
            temp1(N1-m11+1:N1,1:2*N2-m12) = h(1:m11,m12-N2+1:N2);
        elseif m11>N1 && m12<=N2
            temp1(1:2*N1-m11,N2-m12+1:N2) = h(m11-N1+1:N1,1:m12);
        elseif m11>N1 && m12>N2
            temp1(1:2*N1-m11,1:2*N2-m12) = h(m11-N1+1:N1,m12-N2+1:N2);
        end
        absm1 = x1^2+y1^2;
%         absm1
        for m21 = 1:M1
            y2 = N1-m21;
            for m22 = 1:M2
                x2 = m22-N2;
                if (x1*x2+y1*y2>=0) && (x1*x2+y1*y2<=absm1) && ...
                        (x1-x2>=-N2+1) && (x1-x2<=N2-1) && (y1-y2>=-N1+1) &&...
                        (y1-y2<=N1-1) && (x1*y2-y1*x2>=0)
% %                     Me to if elegxoume an to m2 anikei sto S''(m1)
% m11
% m12
% m21
% m22
                    temp2 = zeros(N1,N2);                    
                    if m21<=N1 && m22<=N2
                        temp2(N1-m21+1:N1,N2-m22+1:N2) = h(1:m21,1:m22);
                    elseif m21<=N1 && m22>N2
                        temp2(N1-m21+1:N1,1:2*N2-m22) = h(1:m21,m22-N2+1:N2);
                    elseif m21>N1 && m22<=N2
                        temp2(1:2*N1-m21,N2-m22+1:N2) = h(m21-N1+1:N1,1:m22);
                    elseif m21>N1 && m22>N2
                        temp2(1:2*N1-m21,1:2*N2-m22) = h(m21-N1+1:N1,m22-N2+1:N2);
                    end
%                     temp2
                    h3(m11,m12,m21,m22) = sum(sum(h.*temp1.*temp2));
                    h3(m21,m22,m11,m12) = h3(m11,m12,m21,m22);
                    if (N1+m21-m11>0)&&(N2+m22-m12>0)&&(N1+m21-m11<=M1)&&(N2+m22-m12<=M2)
                        h3(M1-m11+1,M2-m12+1,N1+m21-m11,N2+m22-m12) = h3(m11,m12,m21,m22);
                        h3(N1+m21-m11,N2+m22-m12,M1-m11+1,M2-m12+1) = h3(m11,m12,m21,m22);
                    end
                    if (N1+m11-m21>0)&&(N2+m12-m22>0)&&(N1+m11-m21<=M1)&&(N2+m12-m22<=M2)
                        h3(N1+m11-m21,N2+m12-m22,M1-m21+1,M2-m22+1) = h3(m11,m12,m21,m22);
                        h3(M1-m21+1,M2-m22+1,N1+m11-m21,N2+m12-m22) = h3(m11,m12,m21,m22);
                    end
                    clear temp2;
                end
            end
        end
        clear temp1;
    end    
%     disp(num2str(100*m11/M))
end
% if K>L
%     h3 = h3(:,:,2:M-1,2:M-1);
% elseif L>K
%     h3 = h3(2:M-1,2:M-1,:,:);
% end
