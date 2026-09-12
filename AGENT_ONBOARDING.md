# AGENT_ONBOARDING

新 Agent 接手本仓库时，先读 `REVIEW_PROTOCOL.md` 并遵守下文。可直接把「交接提示词」整段粘贴给新 Agent。

## 交接提示词（版本一 · 推荐先用）

> 你要在一个受协作协议约束的仓库里工作。动手前先读仓库根目录的 `REVIEW_PROTOCOL.md`，严格按它执行。
>
> 三个硬性要求：**不要直推 main**；所有改动走独立分支 + PR，开 PR 时必须打 `review:pending` 标签并填好 PR 模板里的「验证证据」；**开完 PR 就停，等审查，不要自己合并**。
>
> 收到 `review:blocked` 后，用 `gh pr view <PR> --comments` 读审查意见，只改 `BLOCKER` 和 `MAJOR`，`MINOR`/`NIT` 不用管。改完在同一个分支上 `git push`，不要重开 PR、不要 force push。

## 交接提示词（版本二 · 工具不熟时用）

> 本次任务要遵守仓库的协作协议，请先完成接入：
>
> **第 0 步（每次开工前都要做）**
> 1. 读仓库根目录的 `REVIEW_PROTOCOL.md`。
> 2. 确认本机 `gh` 可用且已登录：`gh auth status`。**没登录就停下来告诉我，不要继续。**
> 3. 记住一条铁律：**禁止直接推送到 `main`**，一切改动走独立分支 + PR。
>
> **提交流程**
> ```bash
> git checkout -b feat/<简短说明>
> # ... 改代码 ...
> git add <具体文件>
> git commit -m "feat: <说明>"
> git push -u origin feat/<简短说明>
>
> gh pr create --title "feat: <说明>" \
>              --body-file .github/pull_request_template.md \
>              --label review:pending
> ```
> PR 模板里的「验证证据」必须填（编译输出 / 实测日志）。**没证据会被直接判 BLOCKER**。
> 开完 PR 就停手，等审查结果，**不要自己合并**。
>
> **收到审查后**
> ```bash
> gh pr view <PR> --comments
> gh pr view <PR> --json reviews --jq '.reviews[].body'
> gh api repos/acomonaaa/acomon/pulls/<PR>/comments
> ```
> - 只处理 `BLOCKER` 和 `MAJOR`。`MINOR` / `NIT` 不阻塞合并。
> - 改完在同一分支 `git push`，不要重开 PR、不要 force push。
> - 轮次上限 3 轮。
>
> **绝对不要做的事**：直接推 main、自己合并 PR、修改审查方的意见、跳过 PR 直接改代码。

## 常见坑

| 坑 | 后果 |
|----|------|
| 直推 main | 审查无从谈起 |
| 忘打 `review:pending` | 巡查扫不到 PR |
| 用 `--json comments` 读意见 | 常得到空结果，应使用 `--json reviews` |
| 改完 force push | 审查方丢失增量 diff |

## 本仓库快速事实

- 路径：`D:\keil5\keil_project\STM32F407_LCD_Test`（本地）
- 远程：`https://github.com/acomonaaa/acomon.git`
- 构建：`cmake --preset Debug && cmake --build --preset Debug`
- 业务入口：`Core/Src/user_app.c` → `User_App_Init()`
- 默认仿真：`APP_CLOUD_SIM=1`、`APP_SENSOR_SIM=1`（`Core/Inc/app_config.h`）
