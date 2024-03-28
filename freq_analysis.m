clear;
close all;
clc;

data = load("firFilter.txt");
dev = load("standardDeviation.txt");

fs = 75;
T = 1/fs;
N=12086;
t = (0:N-1)*T;

data = detrend(data);

figure;
subplot(211);
plot(t, data);
ylim([-2 2]);
subplot(212);
plot(t, dev);

Y = fft(data);

k = 0:N/2;
freq = k*fs/N;

figure;
stem(freq, abs(Y(1:(N/2)+1)));

%% 

figure;

plot(fs/N*(-N/2:N/2-1),abs(fftshift(data)),"LineWidth",3)
title("fft Spectrum in the Positive and Negative Frequencies")
xlabel("f (Hz)")
ylabel("|fft(X)|")


%% 

P2 = abs(Y/N);
P1 = P2(1:N/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f = fs/N*(0:(N/2));
figure;
plot(f,P1,"LineWidth",1) 
title("Single-Sided Amplitude Spectrum of X(t)")
xlabel("f (Hz)")
ylabel("|P1(f)|")