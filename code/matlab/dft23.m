function [h3] = dft23(h)
% Ypologismos tou metasximatismou Fourier tou h3 mesw tis idiotitas:
%H3(w1,w2)=H(w1)H(w2)H*(w1+w2)
[N1, N2] = size(h);
M1 = 2*N1-1;
M2 = 2*N2-1;

% Zero padding
h = padarray(h, [N1-1 N2-1], 'post');

% 1st step: Calculate H(w)
H = fft2(h);
% 2nd step: Calculate H3(w1,w2)
% H3 = zeros(M1, M2, M1, M2);
% for m11 = 1:M1
%   for m12 = 1:M2
%     for m21 = 1:M1
%       for m22 = 1:M2
%         if m11 + m21 - 1 <= M1 && m12 + m22 - 1 <= M2
%           m31 = m11 + m21 - 1;
%           m32 = m12 + m22 - 1;
%         elseif m11 + m21 - 1 > M1 && m12 + m22 - 1 <= M2
%           m31 = m11 + m21 - 1 - M1;
%           m32 = m12 + m22 - 1;
%         elseif m11 + m21 - 1 <= M1 && m12 + m22 - 1 > M2
%           m31 = m11 + m21 - 1;
%           m32 = m12 + m22 - 1 - M2;
%         else
%           m31 = m11 + m21 - 1 - M1;
%           m32 = m12 + m22 - 1 - M2;
%         end
%         H3(m11, m12, m21, m22) = H(m11, m12) * H(m21, m22) * conj(H(m31, m32));
%       end
%     end
%   end
% end

%% Use mexa64 for speed
H3 = reshape((dft23_core(M1,M2,H(:)'))',[M1 M2 M1 M2]);
h3 = fftshift(ifftn(H3));
