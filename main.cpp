#include <stdio.h>
#include <assert.h>
#include <random>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "src/mt19937.h" /* my implement*/

// maybe random...
uint32_t generate_seed() {
    uint32_t seed;

    int fd = open("/dev/random", O_RDONLY);
    if (fd == -1) {
        perror("Error when open /dev/random");
        exit(EXIT_FAILURE);
    }

    ssize_t n = read(fd, &seed, sizeof(seed));
    if (n != sizeof(seed)) {
        perror("Erro when read /dev/random");
        exit(EXIT_FAILURE);
    }

    close(fd);
    return seed;
}

int main() {
    ///////////////////////////////////// Stage 1 /////////////////////////////////////

    printf("[*] Stage 1: Testing my `MT1997` implement vs `std::mt19937` built-in function.\n");
    uint32_t seed=generate_seed();
    
    std::mt19937 mt19937_cpp(seed);
    MT19937 mt19937_myself = mt19937_seed(seed);

    for (int i=0; i<1337*N; ++i)
        assert(mt19937_cpp() == mt19937_rand(&mt19937_myself));
    printf("[+] Stage 1 passed!\n");



    ///////////////////////////////////// Stage 2 /////////////////////////////////////
    printf("[*] Stage 2: Recovers the state from an MT19937 instance using 624(=N) outputs.\n");
    // NOTE: No twist should have been performed during the outputs.

    uint32_t outputs[N];
    for (int i=0; i<N; ++i)
        outputs[i] = mt19937_cpp();

    MT19937 cracked = mt19937_cracker(outputs);
    printf("[+] Next 10 outputs:\n");
    for (int i=0; i<10; ++i) {
        u_int32_t r1=mt19937_cpp(), r2=mt19937_rand(&cracked);

        if (r1 == r2)
            printf("    Correct: %10lu | Cracked: %10lu\n", 
                (unsigned long)r1, (unsigned long)r2
            );
        else {
            printf("[-] Stage 2 failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("[+] Stage 2 passed!\n");

    exit(EXIT_SUCCESS);
}