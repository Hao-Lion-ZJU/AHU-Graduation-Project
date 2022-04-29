%*****************2022.2.14*****************%
%****安徽大学-2018级机械设计制造及其自动化****%
%*********Z41814024-郝亮亮-毕业设计**********%
%****************QQ:1526885155**************%
%此代码主要实现Roth于1989年提出的利用空域-频域傅里叶变换计算电流密度的算法
clear
clc
%%
%参数初始化
load C:\Users\xcbns\Desktop\bendwire.mat 
B_x=S;
%B_x=peaks(960,800);     %采样的x方向的磁场强度，单位uT
X=60;               %x方向采样距离,单位mm
Y=60;               %y方向采样距离,单位mm
Z=5;               %采样平面所在高度，单位mm
kw=1.8;               %汉宁滤波器参数 单位mm^-1
u0=400*pi;           %自由空间磁导率 单位 uT mm/A
d=0.5;              %载流片厚度 单位mm
N=size(B_x,1);      %返回采样矩阵的行数
M=size(B_x,2);      %返回采样矩阵的列数
dx=X/M;         %空域采样周期
dy=Y/N;
dkx=2*pi/X;     %频域采样周期,单位为mm^-1
Kx=dkx*M;
dky=2*pi/Y;
Ky=dky*N;
sampleIndex_x=1:M; %索引
sampleIndex_y=1:N;
x=sampleIndex_x.*dx;    %空域坐标
y=sampleIndex_y.*dy;
[A,B]=meshgrid(x,y);
kx=sampleIndex_x.*dkx;  %频域坐标
ky=sampleIndex_y.*dky;
[KA,KB]=meshgrid(kx,ky);
%%
%绘制空域-频域图
b_x=fft2(B_x);      %对原矩阵快速傅里叶变换
figure(1);
%subplot(2,1,1);
%surf(A,B,B_x)       %绘制原场强图
imagesc(x,y,B_x)       %绘制原场强图
title('Spatial Domain the magnetic field intensity'); 
xlabel('x/mm'); 
ylabel('y/mm');
zlabel('B/uT');
%subplot(2,1,2);
%surf(KA,KB,abs(fftshift(b_x))./(M*N))       %绘制频域图
% title('Frequency Domain of the magnetic field intensity'); 
% xlabel('kx/mm^-1'); 
% ylabel('ky/mm^-1');
% zlabel('|B_x(kx,ky)|');
% xlim([-1,Kx+1]);
% ylim([-1,Ky+1]);
%%
%反演磁场
g_x=zeros(N,M);
g_y=zeros(N,M);
filter=g_x;
gx=@(kx,ky)-(kx./ky)*u0*d/2*exp(sqrt(kx.^2+ky.^2)*Z);  %等效的空间高通滤波函数
gy=@(kx,ky) 2/u0/d*exp(sqrt(kx.^2+ky.^2)*Z);  %等效的空间高通滤波函数
for m=1:M
    for n=1:N
        g_x(n,m)=gx(m*dkx,n*dky);
        g_y(n,m)=gy(m*dkx,n*dky);
    end
end
j_x=-g_x.*b_x;
j_y=-g_y.*b_x;
figure(2);
subplot(2,1,1);
surf(KA,KB,abs(fftshift(j_x))./(M*N))       %绘制电流密度的频域图
title('Frequency Domain of the current density'); 
xlabel('kx/mm^-1'); 
ylabel('ky/mm^-1');
zlabel('|J_x(kx,ky)|');
xlim([1,Kx+1]);
ylim([1,Ky+1]);
subplot(2,1,2);
J_x=ifft2(j_x);
J_y=ifft2(j_y);
J=sqrt(J_x.^2+J_y.^2);
%surf(A,B,abs(J_x))       %反演电流密度空域图
imagesc(x,y,abs(J))
title('Spatial Domain the current density'); 
xlabel('x/mm'); 
ylabel('y/mm');
zlabel('J_x/mm·A^-1');
xlim([1,X+1]);
ylim([1,Y+1]);
%%
% 增加汉宁窗滤波后反演磁场
f=@(x,y)(0.5*(cos(pi*sqrt(x.^2+y.^2)/kw)+1)).*(sqrt(x.^2+y.^2)<=kw)+(0).*(sqrt(x.^2+y.^2)>kw); %汉宁滤波器
for m=1:M
    for n=1:N
        filter(n,m)=f(m*dkx,n*dky);
    end
end
f_j_x=j_x.*filter;
f_j_y=j_x.*filter;
figure(3);
%subplot(2,1,1);
%surf(KA,KB,abs(fftshift(f_j_x))./(M*N))       %绘制滤波后的电流密度的频域图
title('Frequency Domain of the current density with filter'); 
xlabel('kx/mm^-1'); 
ylabel('ky/mm^-1');
zlabel('|F_J_x(kx,ky)|');
xlim([-1,Kx+1]);
ylim([-1,Ky+1]);
%subplot(2,1,2);
F_J_x=ifft2(f_j_x);
F_J_y=ifft2(f_j_y);
F_J=sqrt(F_J_x.^2+F_J_y.^2);
%surf(A,B,abs(F_J))       %反演电流密度空域图
imagesc(x,y,abs(F_J))       %反演电流密度空域图
title('Spatial Domain the current density with filter'); 
xlabel('x/mm'); 
ylabel('y/mm');
zlabel('F_J_x/mm·A^-1');
xlim([1,X+1]);
ylim([1,Y+1]);
save('C:/Users/xcbns/Desktop/inv','F_J_x');