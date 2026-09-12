# REVIEW_PROTOCOL

机器可读的 Agent 协作契约。目标仓库：`acomonaaa/acomon`（STM32F407 端云协同智慧农业）。

## 1. 铁律

1. **禁止直接推送到 `main` / `master`。** 一切改动走独立分支 + Pull Request。
2. 开 PR 时必须打标签 **`review:pending`**，并填好 PR 模板中的「验证证据」。
3. **开完 PR 就停，等待审查，不要自己合并。**
4. 不要 force push 已开审的分支；不要重开新 PR 来规避审查。
5. 不要修改/删除审查方的意见。

## 2. 分支命名

- `feat/<简短说明>`
- `fix/<简短说明>`
- `chore/<简短说明>`
- `docs/<简短说明>`

## 3. 提交流程

```bash
git checkout -b feat/<简短说明>
# ... 改代码 ...
git add <具体文件>          # 禁止 git add -A
git commit -m "feat: <说明>"
git push -u origin feat/<简短说明>

gh pr create --title "feat: <说明>" \
             --body-file .github/pull_request_template.md \
             --label review:pending
```

`gh` 未登录时：停下来告知用户，**不要**绕过 PR 直推。

## 4. 审查方输出格式（必须遵守）

审查意见正文**必须**使用下列固定结构；**标签与 verdict 必须一致**（`CHANGES_REQUESTED` ↔ `review:blocked`，`APPROVED` ↔ `review:pass`）。标签供机器轮询，verdict 行供人读与归档。`ROUND` 即审查轮次，Author 据此计数（上限 3）。

```text
## REVIEW VERDICT: CHANGES_REQUESTED | APPROVED
## ROUND: <n>/3

### BLOCKER
- [ ] `path/file.c:123` — 问题
      REASON: ...
      FIX: ...

### MAJOR
- [ ] `path/file.c:45` — 问题
      REASON: ...
      FIX: ...

### MINOR
- ...

### NIT
- ...

### ACCEPTANCE
1. 必须满足的验收点列表
```

- 无阻塞项时写 `## REVIEW VERDICT: APPROVED`，并打 `review:pass`。
- 有 BLOCKER 或 MAJOR 时写 `CHANGES_REQUESTED`，并打 `review:blocked`。
- 空正文、无 verdict 行的审查视为无效，Author 可请求重审。

## 5. 审查响应

收到 `review:blocked` 后：

```bash
gh pr view <PR> --comments
gh pr view <PR> --json reviews --jq '.reviews[].body'
gh api repos/acomonaaa/acomon/pulls/<PR>/comments
```

- 从正文解析 `ROUND` 与 `### BLOCKER` / `### MAJOR` 列表。
- **只处理 `BLOCKER` 与 `MAJOR`。** `MINOR` / `NIT` 不阻塞，不要为它们反复返工。
- 改完在**同一分支** `git push`。
- 轮次上限 **3**（以审查正文 `ROUND` 字段为准）；第 3 轮仍有阻塞项则停下找人。

## 6. 标签

| 标签 | 含义 |
|------|------|
| `review:pending` | 已提交，等待审查 |
| `review:blocked` | 审查不通过，必须修改 |
| `review:pass` | 审查通过，可合并 |

标签不存在时 `gh pr create --label ...` 为**硬失败**。初始化：

```bash
gh label create "review:pending" --repo acomonaaa/acomon --color FBCA04 --description "已提交，等待审查" --force
gh label create "review:blocked" --repo acomonaaa/acomon --color D73A4A --description "审查不通过，必须修改" --force
gh label create "review:pass"    --repo acomonaaa/acomon --color 0E8A16 --description "审查通过，可合并" --force
```

## 7. 审查清单（本仓库）

**适用范围：**

- 涉及 `Core/`、`ui/`、`CMakeLists.txt`、`cmake/`、`Drivers/` 的改动：**必须**附构建证据。
- 仅 `docs/`、`chore/`、协议文档类改动：**免**全量构建证据，但 PR 描述须写明「无代码行为变更」。

审查方按下列项打 `BLOCKER` / `MAJOR` / `MINOR` / `NIT`：

1. **可构建**（见适用范围）：`cmake --preset Debug && cmake --build --preset Debug` 0 error，证据写入 PR。
2. **业务隔离**：业务逻辑不得塞进 CubeMX 生成面；任务/OS 对象应在 `User_App_Init` / `*_app.*`。
3. **并发安全**：优先使用 `SystemData_Snapshot` / `SystemData_SetMode` / `SystemData_SetActuator` / `SystemData_SetThresholds` 等封装；共享 `g_SysData` 必须经 `g_DataMutex`；禁止长临界区。
4. **AT/云路径**：状态迁移须用显式 `next_on_ok`；OK/ERROR 仅在 `AT_ST_WAIT_OK` 处理；退避仅 ONLINE 成功后重置。
5. **真机安全**：GPIO/单总线/UART 路径须有超时，禁止无界 `while` 等待。
6. **配置宏**：默认 `APP_CLOUD_SIM=1` / `APP_SENSOR_SIM=1` 不得被无意破坏；真机开关需在 PR 说明。
7. **范围**：不得夹带 `.idea/`、`build/`、`cmake-build-*/`；不得在仓库根目录新增未跟踪的 md 文件。

## 8. 绝对不要做的事

- 直接推 main/master
- 自己合并 PR
- 修改审查方意见
- 跳过 PR 直接改代码
- force push 已开审分支
