	GLOBAL	_wb
	GLOBAL	_r
	GLOBAL	_c
; void wb()
	SECTION	code_compiler
._wb	equ	$
;       r->buffer[r->head] = c;
	ld      hl,(_r)	; get the r pointer
	ld      e,(hl)	; get buffer
	inc     hl
	ld      d,(hl)
	push    de	; and save
	inc     hl	; get head
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	pop	hl	
	add     hl,de	; compute buffer offset
	push    hl	; and save
	ld      a,(_c)	; get c
	pop     de
	ld      (de),a	; store in buffer
;       if (r->head >= r->size)
	ld      hl,(_r)	; get head
	inc     hl
	inc     hl
	ld      e,(hl)
	inc     hl
	ld      d,(hl)
	push    de	; and save
	ld      hl,(_r)	; get size
	ld      bc,6
	add     hl,bc
	ld	e,(hl)
	inc	hl
	ld	d,(hl)	; de has size, stack has head
	pop	hl
	ex	de,hl
; compaire de>= hl  (lifted from c run time code)
   	ld 	a,h
   	add 	a,$80
   	ld 	b,a
   	ld 	a,d
   	add 	a,$80
   	cp 	b
   	ccf
   	jp 	nz,l_cr 
   	ld 	a,e
   	cp 	l
   	ccf
l_cr	equ	$
   	ld 	hl,0
   	jp	nc,l_cr1
   	inc 	hl    ; do not disturb z flag
l_cr1	equ	$
	jp      nc,i_2
;               r->head = 0;
	ld      hl,(_r)
	inc     hl
	inc     hl
	ld	(hl),0
	inc	hl
	ld	(hl),0	; head = 0 now.
;       else
	jp      i_3
i_2	equ	$
;               r->head++;
	ld      hl,(_r)
	inc     hl
	inc     hl
	inc     (hl)	; head ++
; NOTE: there is a bug here.  At this time, ring can only be 255 bytes.
;
;	ld      a,(hl)
;	inc     hl
;	jr      nz,ASMPC+3
;	inc     (hl)
;	ld      h,(hl)
;	ld      l,a
;	dec     hl
i_3	equ	$
;       r->count++;
	ld      hl,(_r)
	ld      bc,8
	add     hl,bc
	inc     (hl)
; NOTE:  there is a bug here.  At this time, count can't exceed 255
;
;	ld      a,(hl)
;	inc     hl
;	jr      nz,ASMPC+3
;	inc     (hl)
;	ld      h,(hl)
;	ld      l,a
;	dec     hl
;}
;	pop	hl	; extra stuff on stack ?
	ret
	SECTION	bss_compiler
._r     equ 	$
	defs    2
._c     equ	$
	defs    1

