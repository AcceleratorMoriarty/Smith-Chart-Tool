%%*************************************************************************
	% @File Name: Smith.m
	% @Author: Accelerator(Xu HuiYao)
   	% @Version: 2.0
	% @Mail: 2364412203@qq.com
%% 清空
clear; clc; close;

%% 输入参数
Zl_str = input('请输入负载阻抗:');
Re = regexp(Zl_str, '+', 'split');
if(Re==Zl_str)
    Re = regexp(Zl_str, '-', 'split');
    Im = regexp(char(Re(2)), 'j', 'split');
    Zl = str2num(char(Re(1)))-j*str2num(char(Im(1)));
else
    Im = regexp(char(Re(2)), 'j', 'split');
    Zl = str2num(char(Re(1)))+j*str2num(char(Im(1)));
end

Z0 = input('请输入特征阻抗:');

L0 = input('请输入传输线长度λ:');

% 测试数据
% Zl = 50-100j;
% Z0 = 50;
% L0 = 0.4;

%% 计算归一化阻抗

Z_normalization = Zl/Z0;

%% 计算归一化电阻r、归一化电抗x

r = real(Z_normalization);
x = imag(Z_normalization);

%% 计算反射系数

Gamma = (Zl - Z0) / (Zl + Z0);

%% 计算驻波比

VSWR = (1+abs(Gamma))/(1-abs(Gamma));

%% 创建Smith图

figure;
set(gcf, 'Name', 'Smith Chart'); % 修改 figure 窗口的标题
smithplot;
hold on;

%% 绘制归一化电阻圆

theta = 0: pi/100: 2*pi;
C_r = r / (1 + r); % 中心坐标(C_r,0)
R_r = 1 / (1 + r); % 电阻圆半径
% 转换到极坐标
x_r = C_r + R_r * cos(theta);
y_r = R_r * sin(theta);
plot(x_r, y_r, 'LineWidth', 1.5);
hold on;

%% 绘制归一化电抗圆
theta = 0: pi/100: 2*pi;
C_x = 1 / x;
R_x = 1 / x;
x_x = 1 + R_x * cos(theta);
y_x = C_x + R_x * sin(theta);
plot(x_x, y_x,'LineWidth', 1.5);
hold on;

%% 绘制反射系圆

plot(Gamma, 'ro', 'MarkerSize', 5, 'MarkerFaceColor', 'r');
% 缩放反射系数，使其模值为1
normalized_Gamma = Gamma / abs(Gamma);
line([0, real(normalized_Gamma)], [0, imag(normalized_Gamma)], 'Color', [0, 0.5, 0], 'LineWidth', 1.5);

Gamma_abs = abs(Gamma); % 反射系数的模值
theta = linspace(0, 2*pi, 1000); % 反射系数圆角度为360°
x_circle = Gamma_abs * cos(theta);
y_circle = Gamma_abs * sin(theta);
plot(x_circle, y_circle, 'LineWidth', 2);

%% 计算输入阻抗

Zi_normalization = (Zl+j*Z0*tan(L0*2*pi))/(Z0+j*Zl*tan(L0*2*pi));
Zi = Zi_normalization*Z0;
Gamma_i = (Zi - Z0) / (Zi + Z0);
normalized_Gamma_i = Gamma_i / abs(Gamma_i);
line([0, real(normalized_Gamma_i)], [0, imag(normalized_Gamma_i)], 'Color', [0, 0.7, 0], 'LineWidth', 1.5);

%% 显示数据
disp("----------计算结果如下----------");
disp('反射系数：');
disp(Gamma);
disp('驻波系数：');
disp(VSWR);
disp('输入阻抗：');
disp(Zi);
hold off;
