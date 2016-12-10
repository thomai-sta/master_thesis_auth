function [representation] = imrep_lut(h, quantTheta1, quantTheta2)
% Calculates image representation as a vector

%% Initial parameters
[~, N] = size(h);
h3 = dft23(h);
h_3 = zeros(quantTheta1+1,quantTheta2+1);
display 'correlation'

load(['lut', num2str(N), ',', num2str(quantTheta1), ',', num2str(quantTheta2)...
  ,'.mat'])
[lut_count, ~] = size(lut);
%% Step 2.3
tic;
for i = 1:lut_count
  t1x = lut(i,1);
  t1y = lut(i,2);
  t2x = lut(i,3);
  t2y = lut(i,4);
  theta1 = lut(i,5);
  theta2 = lut(i,6);
  %% Filling h_3 matrix
  h_3(theta1+1,theta2+1) = ...
    h_3(theta1+1,theta2+1) +...
    h3(N-t1y,N+t1x,N-t2y,N+t2x);
end
representation = h_3(:);
