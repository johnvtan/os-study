.global outb
outb:
    mov 8(%esp), %al
    mov 4(%esp), %dx
    out %al, %dx
    ret

.global inb
inb:
    mov 4(%esp), %dx
    in %dx, %al
    ret
