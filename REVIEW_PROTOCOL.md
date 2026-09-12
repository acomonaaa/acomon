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

## 4. 审查响应

收到 `review:blocked` 后：

```bash
gh pr view <PR> --comments
gh pr view <PR> --json reviews --jq '.reviews[].body'
gh api repos/acomonaaa/acomon/pulls/<PR>/comments
```

- **只处理 `BLOCKER` 与 `MAJOR`。** `MINOR` / `NIT` 不阻塞，不要为它们反复返工。
- 改完在**同一分支** `git push`。
- 轮次上限 **3**；第 3 轮仍有阻塞项则停下找人。

## 5. 标签

| 标签 | 含义 |
|------|------|
| `review:pending` | 已提交，等待审查 |
| `review:blocked` | 审查不通过，必须修改 |
| `review:pass` | 审查通过，可合并 |

标签不存在时 `gh pr create --label ...` 为**硬失败**。初始化：

```powershell
gh label create "review:pending" --repo acomonaaa/acomon --color FBCA04 --description "已提交，等待审查" --force
gh label create "review:blocked" --repo acomonaaa/acomon --color D73A4A --description "审查不通过，必须修改" --force
gh label create "review:pass"    --repo acomonaaa/acomon --color 0E8A16 --description "审查通过，可合并" --force
```

## 6. 审查清单（本仓库）

审查方按下列项打 `BLOCKER` / `MAJOR` / `MINOR` / `NIT`：

1. **可构建**：`cmake --preset Debug && cmake --build --preset Debug` 0 error（证据写入 PR）。
2. **业务隔离**：业务逻辑不得塞进 CubeMX 生成面；任务/OS 对象应在 `User_App_Init` / `*_app.*`。
3. **并发安全**：共享 `g_SysData` 必须经 `g_DataMutex`；禁止长临界区。
4. **AT/云路径**：状态迁移须用显式 `next_on_ok`；OK/ERROR 仅在 `WAIT_OK` 处理；退避仅 ONLINE 成功后重置。
5. **真机安全**：GPIO/单总线/UART 路径须有超时，禁止无界 `while` 等待。
6. **配置宏**：默认 `APP_CLOUD_SIM=1` / `APP_SENSOR_SIM=1` 不得被无意破坏；真机开关需在 PR 说明。
7. **范围**：不得夹带 `.idea`、`build/`、无关中文 md 未跟踪文件。

## 7. 绝对不要做的事

- 直接推 main/master
- 自己合并 PR
- 修改审查方意见
- 跳过 PR 直接改代码
- force push 已开审分支
