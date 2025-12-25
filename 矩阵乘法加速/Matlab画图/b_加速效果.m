% 数据
dimensions = [256, 512, 1024, 2048];
normalMultiplyTime = [0.336942, 2.90102, 24.1838, 202.328];
strassenTime = [0.223853,	1.59627,	12.3036	,81.487];
openMpTime = [0.0855515,	0.374352	,2.35037	,15.9881];
mpiTime = [0.10194,	0.66683,	4.0017,	19.9011];
speedUpStrassen = [1.51, 1.82, 2.14, 2.48];
speedUpOpenMp = [2.62,	4.26,	4.81,	5.09];
speedUpMpi = [2.19	,2.39,3.07,	4.09];

% 绘制折线图
figure;
plot(dimensions, speedUpStrassen, '-o', dimensions, speedUpOpenMp, '-*', dimensions, speedUpMpi, '-x');
xlabel('维数');
ylabel('加速比');
title('不同维数下的算法加速比');
legend('Strassen串行', 'OpenMp并行', 'MPI并行');

% 绘制折线图
figure;
plot(dimensions, normalMultiplyTime, '-o', dimensions, strassenTime, '-*', dimensions, openMpTime, '-x', dimensions, mpiTime, '-s');
xlabel('维数');
ylabel('运行时间（均值）');
title('不同维数下的算法运行时间');
legend('普通矩阵乘法', 'Strassen串行', 'OpenMp并行', 'MPI并行');

% 绘制直方图
figure;
bar(dimensions, [speedUpStrassen', speedUpOpenMp', speedUpMpi']);
xlabel('维数');
ylabel('加速比');
title('不同维数下的算法加速比');
legend('Strassen串行', 'OpenMp并行', 'MPI并行');

