	.org 0x00
add_loop:
	mov a,&in_y
	mov a,[a]
	sub a,1
	jb end
	mov b,&in_y
	mov [b],a
	mov a,&in_x
	mov a,[a]
	mov b,&out_lo
	mov b,[b]
	add b,a
	mov a,&out_lo
	mov [a],b
	jnc add_loop
	mov a,&out_hi
	mov a,[a]
	add a,1
	mov b,&out_hi
	mov [b],a
	jmp add_loop
end:
	jmp end

	.org 0xF0
	in_x .db ?
	in_y .db ?
	out_lo .db ?
	out_hi .db ?