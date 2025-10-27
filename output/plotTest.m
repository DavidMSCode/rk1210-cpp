clear all; close all;
clc

fileID = fopen('rk1210Out.txt','r');
h1 = fscanf(fileID,'%e');
numCols = 7; % Change this to match the number of columns in your data
h1 = reshape(h1, numCols, []).';

fclose(fileID);

hold on
plot3(h1(:,2),h1(:,3),h1(:,4))
%Plot a sphere with radius 6378.13 km
[xs, ys, zs] = sphere(50);
surf(6378.13*xs, 6378.13*ys, 6378.13*zs, 'FaceColor', 'b', 'FaceAlpha', 0.3, 'EdgeColor', 'none');
axis equal
xlabel('X (km)');
ylabel('Y (km)');
zlabel('Z (km)');
title('Orbit Trajectory');
grid on
view(45, 30);
hold off


% N = 10:4:150;
% 
% Nst = [];
% Mst = [];
% for i = 1:length(N)
%     
%     s  = sprintf('%i, ',N(i));
%     Nst = [Nst s];
%     s  = sprintf('%i, ',N(i)+1);
%     Mst = [Mst s];
% end
% 
% Nst
% Mst