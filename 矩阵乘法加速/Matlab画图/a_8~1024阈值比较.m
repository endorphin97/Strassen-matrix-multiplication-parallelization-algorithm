% 数据点
thresholds = [1024, 512, 256, 128, 64, 32, 16, 8];
execution_times = [16.6977, 14.4653, 13.0375, 12.1609, 11.6755, 13.1596, 19.6416, 40.6962];

% 额外点
extra_threshold = [1, 1024];
extra_execution_time = [17.6281, 17.6281];

% 绘制 Strassen 算法执行时间曲线
plot(thresholds, execution_times, 'Color', [0.5, 0.5, 1], 'LineStyle', '-', ...
    'Marker', 'o', 'MarkerFaceColor', [0.5, 0.5, 1], 'MarkerSize', 8, 'LineWidth', 1.5); % 淡蓝色
hold on;

% 绘制传统矩阵乘法的执行时间
plot(extra_threshold, extra_execution_time, 'Color', [1, 0.5, 0.5], 'LineStyle', '-', ...
    'Marker', 's', 'MarkerFaceColor', [1, 0.5, 0.5], 'MarkerSize', 8, 'LineWidth', 1.5); % 淡红色

% 添加标签和标题
xlabel('阈值', 'FontName', 'SimHei', 'FontSize', 10);
ylabel('运行时间（秒）', 'FontName', 'SimHei', 'FontSize', 10);
title('n=1024时传统矩阵乘法和Strassen算法时间对比', 'FontName', 'SimHei', 'FontSize', 10);

% 显示对应点的横纵坐标（选择第四个点）
text(thresholds(5), execution_times(5), ...
    ['(', num2str(thresholds(5)), ', ', num2str(execution_times(5), '%.4f'), ')'], ...
    'FontSize', 10);

% 显示额外点的纵坐标
text(extra_threshold(2), extra_execution_time(2), ...
    num2str(extra_execution_time(2), '%.4f'), ...
    'FontSize', 10, 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');

% 显示图例
legend('Strassen算法执行时间', '传统矩阵乘法', 'FontName', 'SimHei', 'FontSize', 10);

% 显示图像
hold off;
