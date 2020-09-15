	.org 0x00
add_loop:
	nop
	nop
	nop
	mov a,&in_y
	nop
	mov a,[a]
	sub a,1
	jb end
	nop
	mov b,&in_y
	mov [b],a
	mov a,&in_x
	nop
	nop
	nop
	mov a,[a]
	mov b,&out_lo
	nop
	mov b,[b]
	add b,a
	mov a,&out_lo
	mov [a],b
	jnc add_loop
	mov a,&out_hi
	mov a,[a]
	nop
	add a,1
	nop
	nop
	mov b,&out_hi
	mov [b],a
	nop
	nop
	jmp add_loop
	nop
end:
	nop
	nop
	jmp end

	.org 0xF0
	in_x .db ?
	in_y .db ?
	out_lo .db ?
	out_hi .db ?