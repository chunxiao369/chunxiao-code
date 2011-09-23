;;========================================

;; 外观设置

;;========================================

 

;; 禁用工具栏

(tool-bar-mode nil)

;; 禁用菜单栏，F10 开启关闭菜单

;(menu-bar-mode nil)

;; 禁用滚动栏，用鼠标滚轮代替

(scroll-bar-mode nil)

;; 禁用启动画面

(setq inhibit-startup-message t)

;; 尺寸

(setq initial-frame-alist '((width . 90) (height . 35)))

(set-background-color "black")
(set-foreground-color "white")
(set-face-foreground 'secondary-selection "skyblue")
;;========================================

;; 缓冲区

;;========================================

 

;; 设定行距

(setq default-line-spacing 0)

;; 页宽

(setq default-fill-column 90)

;; 缺省模式 text-mode

(setq default-major-mode 'text-mode)

;; 设置删除纪录

;(setq kill-ring-max 200)

;; 以空行结束
(setq require-final-newline t)

;; 语法加亮
(global-font-lock-mode t)

;; 高亮显示区域选择

(transient-mark-mode t)

;; 页面平滑滚动，scroll-margin 5 靠近屏幕边沿3行时开始滚动，可以很好的看到上下文。

(setq scroll-margin 5 scroll-conservatively 10000)

;; 高亮显示成对括号，但不来回弹跳 

(show-paren-mode t)

(setq show-paren-style 'parentheses)

;; 鼠标指针规避光标

;(mouse-avoidance-mode 'animate)

;; 粘贴于光标处，而不是鼠标指针处

(setq mouse-yank-at-point t)

;; 使用 C-k 删除整行（包括回车）

(setq-default kill-whole-line t)

 

;;========================================

;; 其他设置

;;========================================

 

;; 不要生成备份文件

(setq-default make-backup-files nil)

;; 显示行号和列号

(setq column-number-mode t)

(setq line-number-mode t)

(setq-default indent-tabs-mode t)

 

;;====================================

;; 编码设定

;;====================================

 

;; 新建文件的编码方式

(setq default-buffer-file-coding-system 'utf-8)

;; 读取文件时优先用 utf-8 方式解码

(prefer-coding-system 'utf-8)

 

;;====================================

;; 插件拓展

;;====================================

 

;; 显示行号

(require 'linum)

(setq linum-format "%4d")

(add-hook 'find-file-hooks (lambda () (linum-mode 1)))

 

;;===================================

;; 自定义快捷键

;;===================================

 

;; 跳转到指定行

(global-set-key (kbd "C-g") 'goto-line)

;; 显示所有buffers

(global-set-key (kbd "C-u") 'buffer-menu)

;; undo处理

(global-set-key (kbd "C-z") 'undo)

(global-set-key [(control return)] 'eshell)

;; define key for tags

(global-set-key (kbd "M-i") 'find-tag)

(global-set-key (kbd "M-o") 'pop-tag-mark)

 

;;=====================================

;; 代码风格

;;=====================================

 

;; C/C++/Java mode

(defun my-c-mode-hook()

  ;; 将回车代替C-j的功能，换行的同时对齐

  (define-key c-mode-map [return] 'newline-and-indent)

  (interactive)

  ;; 设置C程序的对齐风格

  (c-set-style "K&R")

  ;; 自动模式，在此种模式下当你键入{时，会自动根据你设置的对齐风格对齐

  ;; (c-toggle-auto-state)

  ;; 此模式下，当按Backspace时会删除最多的空格

  (c-toggle-hungry-state)

  ;; TAB键的宽度设置为8

  (setq c-basic-offset 4)

  ;; 在菜单中加入当前Buffer的函数索引

  (imenu-add-menubar-index)

  ;; 在状态条上显示当前光标在哪个函数体内部

  (which-function-mode))

 

(defun my-c++-mode-hook()

  (define-key c++-mode-map [return] 'newline-and-indent)

  (interactive)

  (c-set-style "stroustrup")

  (c-toggle-auto-state)

  (c-toggle-hungry-state)

  (setq c-basic-offset 4)

  (imenu-add-menubar-index)

  (which-function-mode))

 

(defun my-java-mode()

  (define-key java-mode-map [return] 'newline-and-indent)

  (interactive)

  (c-set-style "java")

  (c-toggle-auto-state)

  (c-toggle-hungry-state)

  (setq c-basic-offset 4)

  (imenu-add-menubar-index)

  (which-function-mode))

 

(add-hook 'c-mode-hook 'my-c-mode-hook)

(add-hook 'c++-mode-hook 'my-c++-mode-hook)

(add-hook 'java-mode-hook 'my-java-mode)

(put 'upcase-region 'disabled nil)
