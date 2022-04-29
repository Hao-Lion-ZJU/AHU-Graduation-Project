%*****************2022.2.14*****************%
%****安徽大学-2018级机械设计制造及其自动化****%
%*********Z41814024-郝亮亮-毕业设计**********%
%****************QQ:1526885155**************%
%此代码主要模拟传感器采样的载流铜片的磁场强度，返回出采样点矩阵
%电势分布方程为  v=V*x/lenth;
%电流密度J与电势v关系 J=-σ▽v
clear
clc
%%
%参数初始化
M=240;      %x方向采样点数
N=60;       %y方向采样点数
X=120;       %采样距离
Y=30;    
Z=2;
sigama=59.6e4;   %材料的电导率
u0=400*pi;           %自由空间磁导率 单位 uT mm/A
V=2;        %两端电势差 单位：V
lenth=60;   %载流铜片长度 单位：mm
width=6;    %载流铜片宽度 单位：mm
d=0.1;      %铜片厚度 单位：mm
B_y=zeros(N,M);
J_x=zeros(N,M);
dx=X/M;
dy=Y/N;
x=(0:M-1)*dx;
y=(0:N-1)*dy;
[A,B]=meshgrid(x,y);
%%
d_len=lenth/dx;
d_wid=width/dy;
J_x((N/2-d_wid/2):(N/2+d_wid/2),(M/2-d_len/2):(M/2+d_len/2))=sigama*V/lenth;
%J_x=abs(peaks(50));
figure(1);
subplot(2,1,1)
imagesc(x,y,J_x)
%surf(A,B,J_x)
title('Spatial Domain the current density'); 
xlabel('x/mm'); 
ylabel('y/mm');
%%
for m=0:M-1
    for n=0:N-1
        sum=0;
        for a=0:M-1
            for b=0:N-1
                d1=(a-m)*dx;
                d2=(b-n)*dy;
                d3=Z;
                r=(d1^2+d2^2+d3^2)^1.5;
                sum=sum+(J_x(b+1,a+1)./r);
            end
        end
        B_y(n+1,m+1)=sum*u0*d/4/pi;
    end
end
figure(2);
surf(A,B,B_y)
title('Spatial Domain the magnetic field intensity'); 
xlabel('x/mm'); 
ylabel('y/mm');
save('C:/Users/xcbns/Desktop/samples','B_y');