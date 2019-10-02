fopen2019
-----

# PROBLEM
fopen occasionally fails on relatively recent Android such as Pixcel3, leaving errno = 13 (Permission Denied).

# ABOUT THIS
This is an example project to reproduce the problem.

This project is based on [hello-jni](https://github.com/android/ndk-samples/tree/master/hello-jni) project.

The core is [fopen2019.c](hello-fopen/app/src/main/cpp/fopen2019.c). It calls fopen many times, allocating memory with malloc.

# REPRODUCED DEVICES
```
Pixel3
- Android 9
- Build PQ1A.181205.006

XPERIA1 SOV40
- Android 9
- Build 55.0.C.0.475
```
