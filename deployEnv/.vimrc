" 让配置变更立即生效
autocmd BufWritePost $MYVIMRC source $MYVIMRC

" 开启文件类型侦测
filetype on
" 根据侦测到的不同类型加载对应的插件
filetype plugin on
" 定义快捷键到行首和行尾
nmap LB 0
nmap LE $
" 设置快捷键将选中文本块复制至系统剪贴板
vnoremap <Leader>y "+y
" 设置快捷键将系统剪贴板内容粘贴至 vim
nmap <Leader>p "+p
" 定义快捷键关闭当前分割窗口
nmap <Leader>q :q<CR>
" 定义快捷键保存当前窗口内容
nmap <Leader>w :w<CR>
" 定义快捷键保存所有窗口内容并退出 vim
nmap <Leader>WQ :wa<CR>:q<CR>
" 不做任何保存，直接退出 vim
nmap <Leader>Q :qa!<CR>
" 依次遍历子窗口
nnoremap nw <C-W><C-W>
" 跳转至右方的窗口
nnoremap <Leader>lw <C-W>l
" 跳转至左方的窗口
nnoremap <Leader>hw <C-W>h
" 跳转至上方的子窗口
nnoremap <Leader>kw <C-W>k
" 跳转至下方的子窗口
nnoremap <Leader>jw <C-W>j
" 定义快捷键在结对符之间跳转
nmap <Leader>M %
" 开启实时搜索功能
set incsearch
" 搜索时大小写不敏感
set ignorecase
" 关闭兼容模式
set nocompatible
" 关闭自动注释
set fo-=r
" vim 自身命令行模式智能补全
set wildmenu

" vundle 环境设置
filetype off
set rtp+=~/.vim/bundle/Vundle.vim
" vundle 管理的插件列表必须位于 vundle#begin() 和 vundle#end() 之间
call vundle#begin()
Plugin 'VundleVim/Vundle.vim'
Plugin 'Raimondi/delimitMate'
Plugin 'altercation/vim-colors-solarized'
Plugin 'tomasr/molokai'
Plugin 'vim-scripts/phd'
Plugin 'octol/vim-cpp-enhanced-highlight'
Plugin 'derekwyatt/vim-fswitch'
Plugin 'kshenoy/vim-signature'
Plugin 'suan/vim-instant-markdown'
Plugin 'vim-scripts/BOOKMARKS--Mark-and-Highlight-Full-Lines'
Plugin 'Lokaltog/vim-powerline'
Plugin 'majutsushi/tagbar'
Plugin 'vim-scripts/indexer.tar.gz'
Plugin 'vim-scripts/DfrankUtil'
Plugin 'vim-scripts/vimprj'
Plugin 'dyng/ctrlsf.vim'
Plugin 'terryma/vim-multiple-cursors'
Plugin 'scrooloose/nerdcommenter'
Plugin 'vim-scripts/DrawIt'
Plugin 'SirVer/ultisnips'
Plugin 'Valloric/YouCompleteMe'
Plugin 'derekwyatt/vim-protodef'
Plugin 'scrooloose/nerdtree'
Plugin 'fholgado/minibufexpl.vim'
Plugin 'gcmt/wildfire.vim'
Plugin 'sjl/gundo.vim'
Plugin 'Lokaltog/vim-easymotion'
Plugin 'nathanaelkane/vim-indent-guides'
Plugin 'lilydjwg/fcitx.vim'
"插件列结
call vundle#end()
filetype plugin indent on

" 配方案
set background=dark
" colorscheme solarized
colorscheme molokai
"colorscheme phd

"总显示状态栏
set laststatus=2
"显示光标当前位置
set ruler
"开启行号显示
set number
"高亮显示当前行/列
set cursorline
set cursorcolumn
"高亮显示搜索结果
set hlsearch

"开启语法高亮功能
syntax enable
"允许用指定语法高亮配色方案替换默认方案
syntax on

"自适应不同语言的智能缩进
filetype indent on
"将制表符扩展为空格
set expandtab
"设置编辑时制表符占用空格数
set tabstop=4
"设置格式化时制表符占用空格数
set shiftwidth=4
"让 vim 把连续数量的空格视为一个制表符
set softtabstop=4

" 自动换行tab
set ts=4
set expandtab
set autoindent
"粘贴时不自动换行
set pastetoggle

"随 vim 自启动
let g:indent_guides_enable_on_vim_startup=1
"从第二层开始可视化显示缩进
let g:indent_guides_start_level=2
"色块宽度
let g:indent_guides_guide_size=1
"快捷键 i 开/关缩进可视化
:nmap <silent> <Leader>i <Plug>IndentGuidesToggle

" 定义函数AutoSetFileHead，自动插入文件头
function! AutoSetShFileHead()
    "如果文件类型为.sh文件
    if &filetype == 'sh'
        call setline(1, "\#!/bin/bash")
    endif

    normal G
    normal o
    normal o
endfunc

function! AutoSetPyFileHead()
    "如果文件类型为python
    if &filetype == 'python'
        call setline(1, "\#!/usr/bin/env python")
        call append(1, "\# -*- coding: utf-8 -*-")
    endif

    normal G
    normal o
    normal o
endfunc

function! AutoSetCFileHead()
    if &filetype == 'h' || &filetype == "cpp" || &filetype == "c"
        call setline(1,"/*********************************************************")
        call append(line("."),   "*   Copyright (C) ".strftime("%Y")." All rights reserved.")
        call append(line(".")+1, "*   ") 
        call append(line(".")+2, "* File Name:".expand("%:t"))
        call append(line(".")+3, "* Purpose:")
        call append(line(".")+4, "* Creation Date:".strftime("%Y-%m-%d"))
        call append(line(".")+5, "* Created By: bxq2011hust@qq.com")
        call append(line(".")+6, "* Compile :")
        call append(line(".")+7, "*********************************************************/")
        exe "1,6g/File Name:.*/s//File Name: " .expand("%")
        exe "1,6g/Creation Date:.*/s//Creation Date: " .strftime("%Y-%m-%d")

        "如果文件类型为.h文件
        if &filetype == 'h'
            exe "1,9g/^#include.*/s//#pragma once/"
        endif
    endif

    normal G
    normal o
    normal o
endfunc

autocmd BufNewFile *.sh exec ":call AutoSetShFileHead()"
autocmd BufNewFile *.py exec ":call AutoSetPyFileHead()"
autocmd BufNewFile *.c,*.cpp,*.h exec ":call AutoSetCFileHead()"
