
set expandtab
set ts=4
set shiftwidth=4
set autoindent
set smartindent
set hlsearch
syntax enable
syntax on


set backspace=indent,eol,start


nmap  <F2>  :cw<cr>
nmap  <F3>  :cn<cr>
nmap  <F4>  :cp<cr>
:map  <F5>  :tabprevious<CR>
:map  <F6>  :tabnext<CR>
nmap  <F7>  :set foldmethod=indent<cr>
:map  <C-n> :tabnew .<CR>
:map  <C-h> :WMToggle<CR>


:imap  <F5> <ESC>:tabprevious<CR>i
:imap  <F6> <ESC>:tabnext<CR>i
:imap  <C-n> <ESC>:tabnew .<CR>i

":map <F8>a :DoxAuthor
":map <F8>f :Dox

:map <F8>a :call Cxxudoxygen()
:map <F8>f :call Cxxudoxygenfunc()

"highlight Normal ctermfg=White ctermbg=Black

"colorscheme murphy
"colorscheme peachpuff


let g:winManagerWindowLayout='FileExplorer|TagList'
"map wm:WMToggle<CR>


":set cscopequickfix=s-,c-,d-,i-,t-,e-
"cscope_maps.vim
nmap <C-_>s :cs find s <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>g :cs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>c :cs find c <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>t :cs find t <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>e :cs find e <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
nmap <C-_>i :cs find i <C-R>=expand("<cfile>")<CR><CR>
nmap <C-_>d :cs find d <C-R>=expand("<cword>")<CR><CR>

let g:DoxygenToolkit_authorName="xu.chunxiao@embedway.com"
let g:DoxygenToolkit_briefTag_funcName="yes"
let g:DoxygenToolkit_briefTag_pre="@Synopsis  " 
let g:DoxygenToolkit_paramTag_pre="@Param " 
let g:DoxygenToolkit_returnTag="@Returns   " 
let g:DoxygenToolkit_interCommentTag=" * "
let g:DoxygenToolkit_endCommentTag=" */"
let g:DoxygenToolkit_endCommentBlock = " */"

function! Cxxudoxygenfunc()
set nosi
set noai
Dox
set si
set ai
endfunction

function! Cxxudoxygen()
set nosi
set noai
DoxAuthor
set si
set ai
endfunction
