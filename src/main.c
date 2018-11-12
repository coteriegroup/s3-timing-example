#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <zconf.h>
#include <float.h>

typedef enum _measure_type
{
    MT_CLOCK = 1,
    MT_TIMES,
    MT_RUSAGE,
    MT_POSIX
} measure_type;

unsigned long fibonacci(unsigned char n)
{
    if (n == 1 || n == 0) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

double measure(measure_type mt, int n, int count)
{
    double time = 0;
    int i;
    switch (mt)
    {
        case MT_CLOCK:
            for (i = 0; i < count; i++)
            {
                clock_t begin = clock();
                fibonacci(n);
                clock_t end = clock();
                time += 1.0 * (end - begin) / CLOCKS_PER_SEC;
            }
            break;

        case MT_TIMES:
            for (i = 0; i < count; i++)
            {
                double ticks = (double) sysconf(_SC_CLK_TCK);
                struct tms begin, end;
                times(&begin);
                fibonacci(n);
                times(&end);
                time += 1.0 * (end.tms_utime - begin.tms_utime) / ticks;
            }
            break;
        case MT_RUSAGE:
            for (i = 0; i < count; i++)
            {
                struct rusage begin, end;
                getrusage(RUSAGE_SELF, &begin);
                fibonacci(n);
                getrusage(RUSAGE_SELF, &end);
                double begin_secs = begin.ru_utime.tv_sec + begin.ru_utime.tv_usec / 1000000.0;
                double end_secs = end.ru_utime.tv_sec + end.ru_utime.tv_usec / 1000000.0;
                time += (end_secs - begin_secs) / 1000.0;
            }
            break;

        case MT_POSIX:
            for (i = 0; i < count; i++)
            {
                clockid_t id = CLOCK_PROCESS_CPUTIME_ID;
                struct timespec begin, end;
                clock_gettime(id, &begin);
                fibonacci(n);
                clock_gettime(id, &end);
                double end_secs = end.tv_sec + end.tv_nsec / 1000000000.0;
                double begin_secs = begin.tv_sec + begin.tv_nsec / 1000000000.0;
                time += (end_secs - begin_secs) / 1000.0;
            }
            break;

    }
    return time / count;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "not enough args\n");
        exit(-1);
    }
    measure_type type = atoi(argv[1]);
    int n = atoi(argv[2]);
    int count = atoi(argv[3]);
    printf("%0.*f msecs\n", DBL_DIG, measure(type, n, count));
    return 0;
}
