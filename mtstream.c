//
//  Jason R. McNeil
//    07 DEC 2010
//
#import "MT19937-64.h"

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 62500 * 2

double get_time()
{
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

void get_args(int argc, char** argv, char** filename, uint *megabytes, uint *seed) {
    uint i = 1;
    uint val;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            val = argv[i][1];
            switch (val) {
                case 'c':	*megabytes = atoi(argv[++i]);
                    break;
                case 's':	*seed = atoi(argv[++i]);
                    break;
                case 'f':	*filename = argv[++i];
                    break;
            }
        }
    }
}

double start_time;
double end_time;

int main (int argc, const char * argv[]) {
    uint count = 0;
    uint seed = get_time();
    char *filename = NULL;

    get_args(argc, (char**)argv, &filename, &count, &seed);

    if (count < 1){
        printf("Usage : %s -c <number_of_megabytes> [-f <output_file>] [-s rand_seed]\n", argv[0]);
        return -1;
    }

    init_genrand64(seed);

    FILE *fp;
    if (filename) {
        printf("Initial Seed              : %d\n", seed);
        fp = fopen(filename, "wb");
    } else {
        fp = stdout;
    }

    if (fp == NULL) {
        printf("Error creating file");
        return -1;
    }

    uint64_t *val = calloc(BUFFER_SIZE, sizeof(uint64_t));
    uint64_t bytes = 0;
    start_time = get_time();
    uint64_t i = 0;
    uint64_t j = 0;
    for (j = 0; j < count; j++){
        for (i = 0; i < BUFFER_SIZE; i++){
            val[i] = genrand64_int64();
        }
        bytes += fwrite(val, sizeof(uint64_t), BUFFER_SIZE, fp);
    }
    end_time = get_time();

    if (filename) {
        printf("Time (seconds)            : %f\n", end_time - start_time);
        printf("Megabytes (10^6) written  : %f\n", bytes * sizeof(uint64_t) / 1000000.0);
        printf("Megabytes / second        : %f\n", (bytes * sizeof(uint64_t) / 1000000.0) / (end_time - start_time));
        fclose(fp);
    }

    return 0;
}
