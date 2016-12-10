function [lut] = get_lut(N, quantTheta1, quantTheta2)

%% Initial parameters
t0 = [1 0];

tic;
lutcnt = 0;
counter = 1;
%% Step 2.3
for t1x = -N+1:N-1
  for t1y = -N+1:N-1
    t1 = [t1x t1y];
    t1Length = norm(t1);
    for t2x = -N+1:N-1
      for t2y = -N+1:N-1
        t2 = [t2x t2y];
        t2Rot = [-t2y t2x];  % 90 degrees clockwise rotation of t2
        %                 Testing for S''(�1)
        if(dot(t1,t2)>=0) && (dot(t1,t2)<=t1Length^2) &&...
            (dot(t1,t2Rot)>=0)...
            && (t1x-t2x>=-N+1) && (t1x-t2x<=N-1)...
            && (t1y-t2y>=-N+1) && (t1y-t2y<=N-1)
          %                     S''(�1) guarantees that �1 & �2 are less than 90 degrees.
          % �2 E S''(�1)
          t2Length = norm(t2);
          t2Minust1 = t2-t1;
          t2Minust1Length = norm(t2Minust1);
          if (t1Length*t2Length*t2Minust1Length~=0)
            %                         A triangle is formed
            %% Calculate �1 & �2
            t(counter, 1) = t1x;
            t(counter, 2) = t1y;
            t(counter, 3) = t2x;
            t(counter, 4) = t2y;
            counter = counter+1;
            theta1 = acosd(dot(t1,t2) / (t1Length*t2Length));
            theta2 = acosd(dot(t2Minust1,-t1) /...
              (t2Minust1Length*t1Length));
            %                         Quantization of �1 & �2
            theta1 = round((theta1/90)*quantTheta1);
            theta2 = round((theta2/90)*quantTheta2);
            
            lutcnt = lutcnt + 1;
            %% Filling lut
            lut(lutcnt,1) = t1x;
            lut(lutcnt,2) = t1y;
            lut(lutcnt,3) = t2x;
            lut(lutcnt,4) = t2y;
            lut(lutcnt,5) = theta1;
            lut(lutcnt,6) = theta2;
          end % End of 'if' testing for vector lengths
        end % End of 'if' testing for S''
      end     % End of 'for t2y' loop
    end         % End of 'for t2x' loop
  end             % End of 'for t1y' loop
  %     disp(num2str(100*(N+t1x)/(2*N-1)))
end                 % End of 'for t1x' loop
save(['lut', num2str(N), ',', num2str(quantTheta1), ',', num2str(quantTheta2)...
  ,'.mat'], 'lut')
