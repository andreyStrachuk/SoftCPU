in
pop ax
in
pop bx
in
pop cx

push 0
push ax

je :lineara

push 0
push cx

je :linearc

call :square


hlt

:lineara

push bx

push -1
push cx
mul

div

push 1

out

out


ret


:linearc


push ax

push -1
push bx
mul

div

push 0

push 2

out
out
out

hlt

ret

:square

push 0

push ax
push cx
push 4
mul
mul

push bx
push bx
mul

sub

pop dx
push dx

jb :noroots

:noroots

push 0

out

hlt

ret
