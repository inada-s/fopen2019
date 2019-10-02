//
// Created by inada-s on 2019-09-30.
//

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdarg.h>
#include <android/log.h>
#include <jni.h>
#include <malloc.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static const int kDummyFileSize = 10 * 1000 * 1000;
static const int kThreadCount = 10;

void xprintf(const char *format, ...) {
    va_list va;
    va_start(va, format);
    __android_log_vprint(ANDROID_LOG_ERROR, "fopen2019", format, va);
    va_end(va);
}

/*
 * Trys fopen - fread - fclose.
 * The problem is that fopen in this function occasionally fails on Pixel3.
 */
void do_fopen_fread_fclose(void* data) {
    const char* path = (const char*) data;

    FILE *fp;
    fp = fopen(path, "r"); // <- here
    if (fp == NULL) {
        xprintf("[HERE] fopen failed errno = %d", errno);
        exit(1); // suicide
        return;
    }

    const int size = kDummyFileSize;
    unsigned char* buf = (unsigned char*) malloc(size);
    int sz = 0;
    if (sz = fread(buf, sizeof(unsigned char), size, fp), sz != size) {
        xprintf("fread failed, fread = %d, errno = %d", sz, errno);
    }

    if (fclose(fp) != 0) {
        xprintf("fclose failed, errno = %d", errno);
    }

    free(buf);
}

/*
 * creates a dummy file to specified path.
 */
void create_dummy_file(const char* path) {
    xprintf("create_dummy_file %s", path);

    const int size = kDummyFileSize;
    unsigned char* buf = (unsigned char*) malloc(size);

    FILE *fp;
    fp = fopen("/dev/urandom", "rb");
    if (fp == NULL) {
        xprintf("/dev/urandom open failed");
        exit(1);
    }

    int sz = 0;
    if (sz = fread(buf, 1, size, fp), sz != size) {
        xprintf("/dev/urandom fread failed");
        exit(1);
    }

    fclose(fp);
    fp = NULL;


    fp = fopen(path, "wb");
    if (fp == NULL) {
        xprintf("write open failed");
        exit(1);
    }
    if (sz = fwrite(buf, 1, size, fp), sz != size) {
        xprintf("dummy fwrite failed");
        exit(1);
    }

    fclose(fp);
    fp = NULL;

    free(buf);
}

/*
 * main function of the test.
 */
void fopen2019_main(void* data) {
    xprintf("Hello! fopen2019main");

    const char* dummy_file_path = (const char*) data;
    create_dummy_file(dummy_file_path);

    pthread_t threads[kThreadCount] = {};

    for (int t = 0; ;t++) {
        for (int i = 0; i < kThreadCount; ++i) {
            pthread_create(&threads[i], NULL, do_fopen_fread_fclose, data);
        }

        for (int i = 0; i < kThreadCount; ++i) {
            pthread_join(threads[i], NULL);
        }

        xprintf("ok %d", t);
        usleep(1000);


        // I've found that the fopen error tend to occur easily
        // under high memory pressure.
        if (t < 30) {
            const size_t _100MB = 100 * 1024 * 1024;
            void *blackhole = malloc(_100MB);
            if (blackhole == NULL) {
                xprintf("malloc failed");
            } else {
                memset(blackhole, 0, _100MB);
                xprintf("malloc done");
            }
        }
    }
}

static char g_dummy_file_path[1024];
static pthread_t g_fopen2019_th = NULL;

/*
 * JNI entrypoint.
 */
JNIEXPORT void JNICALL
Java_com_example_hellojni_HelloJni_fopen2019main(JNIEnv* env, jobject thiz, jstring jpath) {

    if (g_fopen2019_th != NULL) {
        return;
    }

    const char* npath = (*env)->GetStringUTFChars(env, jpath, 0);

    strcpy(g_dummy_file_path, npath);
    strcat(g_dummy_file_path, "/dummy");
    xprintf("%s", g_dummy_file_path);

    pthread_create(&g_fopen2019_th, NULL, fopen2019_main, (void *)g_dummy_file_path);

    (*env)->ReleaseStringUTFChars(env, jpath, npath);
}

