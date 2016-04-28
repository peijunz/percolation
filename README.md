# 逾渗模型
## 低维键逾渗的键分类
### 剪枝 Prune Branches
剪枝就是把所有度为1的点循环剪除，直到所有点度都非1

这一步可以放在缩环后吗？

### 缩环 Shrink Circuits
+ 成环时，把中间有分岔的节点的父节点都回溯到这个环的最接近时间零点的根节点
+ 无需剪除。

标记所有的键的类型

### 计数

+ 预先产生一个包含访问状态的bond矩阵！不进行任何剪除，只是修改状态分量，比较快
+ 进行BFS遍历的时候统计好所有类型逾渗的最大团的大小

## 低维键逾渗的拓扑结构

### 剪枝 Prune Branches
### 切环 Cut Circuits
#### 步骤：
+ BFS的时候一旦访问到成环的时候，那么判断是否是Wrapping
+ 只要不是Wrapping就切开这个环，并剪除由此可能导致的两个树枝
    + 剪第一个树枝的时候记录下最终的终点F1
    + 剪第二个的时候一旦发现所在点等于终点就停止修剪或者剪完到真正的终点为止得到F2
        + F2!=F1就说明都是non-bridge
        + F2==F1则说明可能有junction，修剪F2，记入Junction

#### 结果
+ 剪除了所有的Junction
+ 顺便剪除了游离的Non-bridge
+ 得到了某些Wrapping集团的核心点组，可以给后面几步作为遍历的种子

### 约化 Ruduction
通过进一步约化得到集团的拓扑分类？
