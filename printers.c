#include <stdio.h>

void print_header (void);
void print_header (void)
{
    printf("Patience, the program takes some time to complete.\n Upon completion, all results will be printed in a table and graphed.\n");
}

void print_timing (double tvegas, double thome, double tapprox);
void print_timing (double tvegas, double thome, double tapprox)
{
    printf("VEGAS Time: %.6f  Custom Time: %.6f  Dipole Approximation: %.6f", tvegas, thome, tapprox);
}

void print_steps (double distance, double result);
void print_steps (double distance, double result)
{
    printf("%.6f   %.6f\n", distance, result);
}
