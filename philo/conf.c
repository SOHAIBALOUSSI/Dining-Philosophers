#include <unistd.h>

int main()
{
    long num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Number of threads: %ld\n", num_threads);
}