% clear all; close all; clc;

% Read and parse data
data = dlmread('Model_data.csv', ',', 2, 0);
t = data(:,1); 
y = data(:,2);
temp_offset = y(1);
yr = 64.1 - temp_offset;
y = y - y(1); % normalize data (start from 0)

% Plot step response
plot(t, y, 'r');
title("Normalized model data, PMW duty 100%");
xlabel("Time (s)");
ylabel("Temperature (C)");
xlim([0 max(t)]);

% Estimated transfer function
k = 37.75;
T = 246.6;
T0 = 14.83;
est_model = tf([k], [T 1], 'InputDelay', T0);
hold on;
step(est_model);
legend("Real step response", "Estimated step response", 'Location', 'southeast');

% PID controller in simulink
sim('UAR.slx')