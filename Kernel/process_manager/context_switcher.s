extern schedule;

contextSwitch:
    mov rdi, rsp
    call schedule
    mov rsp, rax