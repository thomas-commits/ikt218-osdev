; From Per's lectures: https://github.com/perara-lectures/ikt218-osdev/blob/master/group_per-arne/src/kernel/src/cpu/isr_asm.asm
; because of modifications on his side.

; This macro creates a stub for an ISR which does not pass an error code.
; (we need to push a dummy error code to keep the stack in the right order).
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli
        push byte 0                 ; Push a dummy error code.
        push  %1                    ; Push the interrupt number.
        jmp isr_common_stub         ; Go to the common handler code.
%endmacro

; This macro creates a stub for an ISR which passes its own error code.
%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli
        push %1                     ; Push the interrupt number.
        jmp isr_common_stub         ; Go to the common handler code.
%endmacro

; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
    global irq%1
    irq%1:
        push byte 0                 ; Push a dummy error code.
        push byte %2                ; Push the ISR number.
        jmp irq_common_stub         ; Go to the common handler code.
%endmacro

; Define the ISR and IRQ handlers for each interrupt.
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE   21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 128

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

; In isr.c
[EXTERN isr_handler]

; This is the common ISR stub. It saves the processor state, sets up for kernel mode segments, 
;calls the C-level fault handler, and finally restores the stack frame.
isr_common_stub:
        pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax to the stack

        mov ax, ds               ; Lower 16-bits of ax = ds.
        push eax                 ; save the data segment descriptor

        mov ax, 0x10             ; load the kernel data segment descriptor
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        call isr_handler

        pop ebx                  ; reload the original data segment descriptor
        mov ds, bx
        mov es, bx
        mov fs, bx
        mov gs, bx

        popa                     ; Pops edi,esi,ebp, ebx, edx, ecx, eax
        add esp, 8               ; Cleans up the pushed error code and pushed ISR number
        sti 
        iret                     ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; In isr.c
[EXTERN irq_handler]

; This is the common IRQ stub. It saves the processor state, sets up for kernel mode segments, 
;calls the C-level fault handler, and finally restores the stack frame.
irq_common_stub:
        pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax to the stack

        mov ax, ds               ; Lower 16-bits of ax = ds.
        push eax                 ; save the data segment descriptor

        mov ax, 0x10             ; load the kernel data segment descriptor
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        call irq_handler

        pop ebx                  ; reload the original data segment descriptor
        mov ds, bx
        mov es, bx
        mov fs, bx
        mov gs, bx

        popa                     ; Pops edi,esi,ebp, ebx, edx, ecx, eax
        add esp, 8               ; Cleans up the pushed error code and pushed ISR number
        sti
        iret                     ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
