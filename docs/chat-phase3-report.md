# 与 AI 的聊天记录整理（三）—— 工程化与报告收尾

> 整理自 2026-10-13 与 Trae 智能体（GLM 模型）的真实对话，按阶段忠实摘录，保留双方原话。

---

## 阶段 8：Makefile 与 README

**AI**：两个概念：Makefile = 菜谱，把"怎么把 shell.c 做成 osh"写下来，以后敲 make 就行。README = 项目的门牌，课程要求写三件事：设计说明、怎么编译使用、分工情况。一个经典大坑先打预防针：Makefile 里命令行的开头必须是一个 Tab 键，敲空格会报 missing separator。

**本人**：贴上 make 输出——make: 'osh' is up to date.，后面 ./osh 冒烟测试 date/history/exit 正常，make clean 后 ls 只剩 Makefile、README.md、shell.c。

**AI**：测试基本全过，但有个值得讲透的细节：make 的输出是 'osh' is up to date——它根本没编译！make 比对两个文件的修改时间，发现 osh 比刚才手动 gcc 编译出来的新，认为"菜已做好、食材没动，不重做"。你 make clean 把 osh 删了正好，请再敲一次 make，这次应该能看到 gcc 那行真正跑起来。

**本人**：readme我把菜谱改成了脚本。再贴 make 输出——gcc -Wall -Wextra -O2 -std=gnu11 -o osh shell.c 完整执行，零警告。

**AI**：截图验证通过，Makefile 正式验货成功。README 你把"菜谱"改成"脚本"——同意，"菜谱"是我教学时的比喻，正式文档里"脚本"更规范，这个改动保留你的版本。提交说明草稿：

> add: Makefile和完整版readme
>
> make一下就能编译，-Wall -Wextra -O2，clean负责删osh。
> 发现make是按文件修改时间偷懒的：手动gcc编出来的osh比
> shell.c新，它就说up to date不干活，make clean之后再make
> 才看到gcc那行真跑了，零警告。
> readme补齐设计说明、编译使用方法，AI分工按课程要求写明
> 各自做了什么，证据指向docs/和git log。

**本人**：贴上定稿——

> add: Makefile和完整版readme
>
> 新增Makefile后输入make就能编译，-Wall -Wextra -O2，clean负责删osh。又发现make是按文件修改时间偷懒的：手动gcc编出来的osh比
> shell.c新，但它说up to date不干活，make clean之后再make
> ，看到gcc那行真跑了，零警告。
> readme补齐设计说明、编译使用方法，AI分工按课程要求写明
> 各自做了什么，证据指向docs/和git log。

**AI**：你的定稿收到（"新增Makefile后输入make"、"又发现"补充语境）。已推送。全部代码工作到此完成——仓库从空仓库一路走到：模板 → 前台 → 后台 → 僵尸修复 → history → !!/!N → 编号修复 → 边界 → 工程化，每一步你都亲手测过。

---

## 阶段 9：报告与材料整理

**AI**：最后是文档收尾四件事：A 补报告用截图（你操作）、B 聊天记录整理（我起草、你逐字审）、C 实验报告改写、D git log 导出与最终打包。

**本人**：报告底稿：确认用 `Citel实验\Lab01\实验报告_黄译辉_24281162.docx` 改写。导出 PDF 的工具：有wps，你写成docx，我自己微调并转换格式。

**AI**：两个确认收到。聊天记录分 3 个文件，忠实整理今天的真实对话。

**本人**：贴上 7 张截图——02_前台执行、03_history与recall、04_后台运行、07_边界与错误、history修前、history修后、验收。

**AI**：7 张全部查验通过：02 里 ps 的 PPID=1127=osh 的 PID 是 fork 的铁证；修前/修后这对截图是报告"问题与解决"一节的王牌；验收图里 make 的 up-to-date 现象 + clean 后重编正好配套问题分析。
