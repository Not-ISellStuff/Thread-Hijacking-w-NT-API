# Thread-Hijacking
Shell Code Injection in C made to find a running process from a specific list and allocates and writes memory to the process then hijacks the thread making it execute the shell code using the native api (nt.dll)

Works for both 32 and 64 bit windows systems

btw idek if this is 100% working I just made sure it compiled and ran

i just made this cuz i havent worked with this type of stuff in a while so i refreshed my memory by writing this

# Compile

gcc -o hijack.exe hijack.c loader.c main.c target.c -lntdll -lws2_32 -lkernel32

# Virus Total Scan
