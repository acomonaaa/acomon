# AGENT_ONBOARDING

> **权威文件是 `REVIEW_PROTOCOL.md`。** 本文件是摘要与交接模板；与协议冲突时**一律以协议为准**。

新 Agent 接手本仓库：先读根目录 `REVIEW_PROTOCOL.md`。可直接粘贴下面的提示词。

## 交接提示词（Author · 推荐）

> 你要在一个受协作协议约束的仓库里工作。动手前先读仓库根目录的 `REVIEW_PROTOCOL.md`，严格按它执行。
>
> 三个硬性要求：**不要直推 main**；所有改动走独立分支 + PR，开 PR 时必须打 `review:pending` 标签并填好 PR 模板里的「验证证据」；**开完 PR 就停，等审查，不要自己合并**。
>
> 收到 `review:blocked` 后，按协议 §4 解析 `REVIEW VERDICT` / `ROUND` / `BLOCKER` / `MAJOR`，只改 BLOCKER 与 MAJOR，同分支 `git push`，不要 force push、不要重开 PR。

## 交接提示词（Reviewer）

> 你是审查方。先读 `REVIEW_PROTOCOL.md`（尤其 §4 输出格式与 §7 清单）。
>
> 用 `gh pr view <PR> --json state,labels,reviews` 确认 PR 为 `review:pending`。读 diff 与协议清单后，按 §4 固定格式提交审查，并同步标签：`CHANGES_REQUESTED` → `review:blocked`，`APPROVED` → `review:pass`。
>
> 你只审查、不合并 master、不代替 Author 改业务代码。

## Author 开工速查

1. `gh auth status`（未登录则停下告知用户）
2. 开分支 → 改代码 → **具名** `git add` → push
3. `gh pr create --label review:pending`，填验证证据
4. 等审查；只响应 BLOCKER/MAJOR；最多 3 轮（看正文 `ROUND`）

命令细节见 `REVIEW_PROTOCOL.md` §3、§5。

## 常见坑

| 坑 | 后果 |
|----|------|
| 直推 main | 审查无从谈起 |
| 忘打 `review:pending` | 巡查扫不到 PR |
| 用 `--json comments` 读意见 | 常得到空结果，应用 `--json reviews` |
| 改完 force push | 审查方丢失增量 diff |

## 本仓库快速事实

- 远程：`https://github.com/acomonaaa/acomon.git`
- 构建：`cmake --preset Debug && cmake --build --preset Debug`
- 业务入口：`Core/Src/user_app.c` → `User_App_Init()`
- 默认仿真：`APP_CLOUD_SIM=1`、`APP_SENSOR_SIM=1`（`Core/Inc/app_config.h`）
