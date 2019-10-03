fopen2019
-----

# PROBLEM
fopen occasionally fails on relatively recent Android device such as Pixel3, leaving errno = 13 (Permission Denied).

# ABOUT THIS
This is an example project to reproduce the problem.

This project is based on [hello-jni](https://github.com/android/ndk-samples/tree/master/hello-jni) project.

The core is [fopen2019.c](hello-fopen/app/src/main/cpp/fopen2019.c). It calls fopen many times, allocating memory with malloc.

# HOW TO CHECK

See [hello-jni/README.md](hello-fopen#getting-started). The problem doesn't occur on Android Emulator, so you must prepare on real device.

After clicking Run/Run 'app', open logcat window and find the message `fopen failed errno = 13`.

Example logcat of my Pixel3:
```
2019-10-03 14:52:21.636 21314-21368/com.example.hellojni E/fopen2019: malloc done
2019-10-03 14:52:21.650 21314-21368/com.example.hellojni E/fopen2019: ok 1
2019-10-03 14:52:21.678 21314-21368/com.example.hellojni E/fopen2019: malloc done
2019-10-03 14:52:21.699 21314-21368/com.example.hellojni E/fopen2019: ok 2
2019-10-03 14:52:21.724 21314-21368/com.example.hellojni E/fopen2019: malloc done
2019-10-03 14:52:21.739 21314-21368/com.example.hellojni E/fopen2019: ok 3
...
2019-10-03 14:52:23.104 21314-21368/com.example.hellojni E/fopen2019: malloc done
2019-10-03 14:52:23.128 21314-21368/com.example.hellojni E/fopen2019: ok 23
2019-10-03 14:52:23.230 21314-21368/com.example.hellojni E/fopen2019: malloc done
2019-10-03 14:52:23.310 21314-21636/com.example.hellojni E/fopen2019: [HERE] fopen failed errno = 13
2019-10-03 14:52:23.310 21314-21632/com.example.hellojni E/fopen2019: [HERE] fopen failed errno = 13
2019-10-03 14:52:23.310 21314-21640/com.example.hellojni E/fopen2019: [HERE] fopen failed errno = 13
2019-10-03 14:52:23.310 21314-21638/com.example.hellojni E/fopen2019: [HERE] fopen failed errno = 13
2019-10-03 14:52:23.310 21314-21637/com.example.hellojni E/fopen2019: [HERE] fopen failed errno = 13
2019-10-03 14:52:23.310 21314-21635/com.example.hellojni E/fopen2019: [HERE] fopen failed errno = 13
2019-10-03 14:52:23.310 21314-21634/com.example.hellojni E/fopen2019: [HERE] fopen failed errno = 13
2019-10-03 14:52:23.380 21314-21636/com.example.hellojni A/libc: Fatal signal 6 (SIGABRT), code -6 (SI_TKILL) in tid 21636 (xample.hellojni), pid 21314 (xample.hellojni)
2019-10-03 14:52:23.707 21314-21632/com.example.hellojni A/libc: Invalid address 0x6f390496a0 passed to free: value not allocated
2019-10-03 14:52:23.730 21314-21640/com.example.hellojni A/libc: Invalid address 0x6f39049680 passed to free: value not allocated
```


# REPRODUCED DEVICES
```
Pixel3
- Android 9
- Build PQ1A.181205.006

XPERIA1 SOV40
- Android 9
- Build 55.0.C.0.475
```
