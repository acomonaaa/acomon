## Summary

<!-- 1-3 句：改了什么、为什么 -->

## Type

- [ ] feat
- [ ] fix
- [ ] chore
- [ ] docs

## 验证证据（必填，无证据 = BLOCKER）

<!-- 贴编译输出摘要 / 实测日志片段 -->

```
# 示例
cmake --build --preset Debug
# 0 error; FLASH xx% RAM xx%
```

- [ ] 已本地跑通 Debug 构建（或说明为何无法构建）

## 审查关注点

<!-- 可选：希望审查方重点看的文件/行为 -->

## 检查单（作者自勾）

- [ ] 未直接推送 main/master
- [ ] 未 `git add -A` 夹带无关文件
- [ ] 标签为 `review:pending`
- [ ] 业务层与 CubeMX 生成面隔离未破坏
