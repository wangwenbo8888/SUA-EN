# Translation Log

## Overview
Translating HIFU project UI strings from Chinese to English using ENGLISH_TRANSLATION_TABLE.md as the authoritative mapping.

## Scope
- Source: D:\9706_draintank_EN
- Target directory: MainFrame\HIFUMainFrameWithOperatorNavigation
- File types: *.cpp, *.h, *.qml, *.ui (excluding generated files: moc_*, qrc_*, ui_*, build dirs)

## Progress
- 2026-07-03 03:21: Applied 468 translation entries across 45 of 134 source files.
- 2026-07-03 03:24: Applied v2 translations (564 entries) across 35 of 134 source files.
- 2026-07-03 03:26: Applied v3 translations (564 entries) across 35 of 134 source files.

---

## 2026-07-16 操作记录

### 1. 界面4 医师注册 — Physician ID → User ID
- **文件**: `usermanager.ui`, `ui_usermanager.h`, `usermanager.cpp`
- **修改**: `Physician ID` → `User ID`（标签 + 表头 setHeaderData + 注释行）

### 2. 界面7 治疗界面 — 18条批注全部修改完成
**修改文件**: `newmainwindow.ui`, `ui_newmainwindow.h`, `printdialog.ui`, `ui_printdialog.h`, `printdialog.cpp`, `ReportInfo.qml`

| 原文 | 新文 |
|------|------|
| HIFU System | HIFU Treatment System Software |
| Used Time | Elapsed Time |
| Left Time | Remaining Time |
| US Driver | Ultrasound Driver |
| Motion Sys | Motion System |
| US | B-mode US |
| Den/Mid/Spa | Dense/Medium/Sparse |
| Modified para | Modify Parameters |
| T/B/L/R/F/B | Up/Down/Left/Right/Front/Back |
| Acoustic Wind/Show Wind/Hide Wind | Acoustic Window/Show Window/Hide Window |
| Pulse | Pulse Count |
| Fill | Fill Pump Speed |
| Drain | Drain Pump Speed |
| Temp | Temperature |

同步修改了 `printdialog.ui`、`printdialog.cpp`、`ui_printdialog.h`、`ReportInfo.qml` 中的 Pulse 和 Phone。

### 3. 治疗界面控件尺寸调整
**修改文件**: `newmainwindow.ui`, `ui_newmainwindow.h`

**状态栏 (frame_2)**:
- `label_hifu_driver_text`: w 81→171
- `label_position_text`: w 101→131
- `label_grabber_text`: w 70→101
- `label_water_text`: w 91→101
- 所有图标、分隔符、后续标签重新定位，收紧间距

**Support Tab 标签**:
- `label_speed`: w 81→105, font 12pt→10pt
- `label_speed_2`: w 101→120, font 12pt→10pt
- `label_pulse_number`: w 71→90, font 12pt→10pt

**治疗按钮**:
- `toolButton_beam_path`: w 95→135
- `toolButton_showpath`: w 90→115
- `toolButton_hidepath`: w 90→115, x 610→645
- `pushButton_modify_para`: w 121→150

**Group Box 扩宽**:
- `groupBox_plan_toolbox`: w 110→150
- `groupBox` (Treatment): w 110→200

### 4. 机械臂方向按钮改为箭头图标
**修改文件**: `newmainwindow.ui`, `ui_newmainwindow.h`, `resource.qrc`

| 按钮 | 原文字 | 图标文件 |
|------|--------|----------|
| `pushButton_headL` | L | `:/arrows/left.png` |
| `pushButton_headR` | R | `:/arrows/right.png` |
| `pushButton_headF` | F | `:/arrows/front.png` |
| `pushButton_headB` | B | `:/arrows/back.png` |
| `pushButton_headD` | B | `:/arrows/down.png` |
| `pushButton_headU` | T | `:/arrows/up.png` |

- 箭头图片从 `D:\9706_draintank_EN\机械臂箭头\` 复制到 `img/arrows/`
- `resource.qrc` 添加 `/arrows` 资源前缀
- 6个按钮的 `setText()` 替换为 `setIcon()` + `setIconSize(18,18)`
