function [h2] = corr2(h)

%  h2(-m) = h2(m) ara upologizw mono ta h2(m) gia m = 0,1...N-1
h1 = hankel(h);
A = h1*h1';
h2 = [fliplr(A(1,2:length(A))) A(1,:)];

%% Old code
% N = length(h);
% %% 
% for i=1:N-1
% %     shift left h 
%     h_shifted = h(1+i:N);
%     h_masked = h(1:N-i);
% %     calculate h2
%     h2(i) = h_masked*h_shifted';
%     clear h_shifted;
%     clear h_masked;
% end
% h0 = h*h';
% h2 = [fliplr(h2) h0 h2];
