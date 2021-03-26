# simplex-algorithm
Simplex Algorithm and Naive Method For Basic Feasible Solution implemented in C++

## 1. 组成

### 1.1. Naive
暴力枚举C(n, m)个基本可行解，计算代价求最大值。

### 1.2. Simplex
单纯形法，模拟单纯形表格计算。

## 2. 调用

```bash
make        # 编译
make clean  # 清空
make all    # 编译 + 运行 + 清空
```

## 3. 运行结果

用了作业题4(b)和9(a)(b)作为测试样例，与手画表格进行对照，完全一致。
```bash
coefficient: 
-2      1       -3      -1      1       0       0

A and b: 
1       -1      1       1       -1      1       0                       7
-2      -3      -5      0       0       0       0                       8
1       0       -2      2       -2      0       -1                      1

Not Feasible

Not Feasible

==============================
coefficient: 
6       14      13      0       0

A and b: 
1       4       2       1       0                       48
1       2       4       0       1                       60

Optimal
x_1 = 36
x_3 = 6
z = 294

Optimal
x_1 = 36
x_3 = 6
z = 294

==============================
coefficient: 
-3      2       4       0       0

A and b: 
4       5       -2      1       0                       22
1       -2      1       0       1                       30

Optimal
x_2 = 82
x_3 = 194
z = 940

Optimal
x_2 = 82
x_3 = 194
z = 940

==============================
```
