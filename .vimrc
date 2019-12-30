set colorcolumn=110
highlight ColorColumn ctermbg=darkgray
augroup project
  autocmd!
  autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END
nnoremap <F4> :!mkdir -p build <bar> cmake -B build <bar> make -C build<cr>
nnoremap <F4> :!build/PiHelper/pihelper localhost<cr>
nnoremap <F9> :!rm -rf build<cr>
let &path.="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include,"
