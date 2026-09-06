[org 0x7C00]
[bits 16]

start:
    cli

    ; Save BIOS boot drive
    mov [BOOT_DRIVE], dl

    ; Set up real-mode segments
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Set up stack
    mov ax, 0x8000
    mov ss, ax
    mov sp, 0xFFFF

    sti
    cld

; --------------------------------------------------
; Load kernel from sector 2 into physical 0x10000
; --------------------------------------------------

load_kernel:
    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov ah, 0x02        ; BIOS disk read
    mov al, 32           ; Read 32 sectors
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Sector 2
    mov dh, 0           ; Head 0
    mov dl, [BOOT_DRIVE]

    int 0x13
    jc disk_error

; --------------------------------------------------
; Enter protected mode
; --------------------------------------------------

    cli

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

; --------------------------------------------------
; 32-bit protected mode
; --------------------------------------------------

[bits 32]

protected_mode:

    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0x90000
    mov esp, 0x90000

    ; Kernel was loaded at physical 0x10000
    jmp 0x10000

; --------------------------------------------------
; Disk error
; --------------------------------------------------

[bits 16]

disk_error:
    mov si, disk_error_msg

.print:
    lodsb
    or al, al
    jz .halt

    mov ah, 0x0E
    mov bh, 0
    int 0x10

    jmp .print

.halt:
    cli
    hlt
    jmp .halt

disk_error_msg db "Disk error!", 0

; --------------------------------------------------
; GDT
; --------------------------------------------------

gdt_start:

    ; Null descriptor
    dq 0

    ; Code segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    ; Data segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BOOT_DRIVE db 0

; --------------------------------------------------
; Boot signature
; --------------------------------------------------

times 510 - ($ - $$) db 0
dw 0xAA55