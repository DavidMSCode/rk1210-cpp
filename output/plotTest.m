clear all; close all;
clc

fileID = fopen('../rk1210Out.txt','r');
h1 = fscanf(fileID,'%e',[7 Inf])';

fclose(fileID);

simpleEarth
hold on
plot3(h1(:,2),h1(:,3),h1(:,4))


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