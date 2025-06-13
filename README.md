# Thread-Hijacking
Shell Code Injection in C made to find a running process from a specific list and allocates and writes memory to the process then hijacks the thread making it execute the shell code

Works for both 32 and 64 bit windows systems

btw idek if this is 100% working I just made sure it compiled and ran

# Compile

gcc -o injector.exe hijack.c loader.c main.c target.c -lntdll -lws2_32 -lkernel32
