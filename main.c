#include <stdio.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>

#include "timer.h"

#define N 20


void print_header (void);
void print_timing (double tvegas, double thome, double tapprox);
void print_steps (double distance, double result);
double f (double x1, double x2, double y1, double y2, double z1, double z2, double d);

//structure for the VEGAS integrator
struct my_params
{
    double r;
};

extern double g (double *t, size_t dim, void *params);

//the integration itself
int main (void)
{
    //printing the header
    print_header ();

    printf ("\nGSL VEGAS Integrator:\nDistance     Energy\n");

    //initialization of variables and arrays
    double res, err;
    double distance[N];
    double result[N];
    size_t dim = 6;
    double xl[] = { 0., 0., 0., 0., 0., 0. };
    double xu[] = { 1., 1., 1., 1., 1., 1. };
    double tvegas;

    //initialize random number generator
    gsl_rng *r = gsl_rng_alloc (gsl_rng_taus2);
    unsigned long seed = 1UL;
    double vegas_error[N];
    
    gsl_rng_set (r, seed);

    size_t calls = 1000000;
    double dd = (4 - 1.001) / N;

    timer_start ();

    //running it "n" times
    for (int q = 0; q < N; q++)
    {
        struct my_params params = { 1.001 + dd * (double) q };

	//running the VEGAS integrator
	gsl_monte_function G = { &g, dim, &params };

	//storing distances in distance array
	distance[q] = 1.001 + dd * (double) q;
	gsl_monte_vegas_state *sv = gsl_monte_vegas_alloc (dim);

	gsl_monte_vegas_init (sv);
	gsl_monte_vegas_integrate (&G, xl, xu, dim, calls / 5, r, sv, &res, &err);

	do
        {
	    gsl_monte_vegas_integrate (&G, xl, xu, dim, calls, r, sv, &res, &err);
	    fflush (stdout);
	}
	while (fabs (gsl_monte_vegas_chisq (sv) - 1.0) > 0.2);

	//storing VEGAS results in resulting array
	result[q] = res;
	vegas_error[q] = err;
	print_steps (distance[q], result[q]);
	gsl_monte_vegas_free (sv);
    }
    tvegas = timer_stop ();

    //free random number generator space
    gsl_rng_free (r);

    printf ("\nCustom Integrator:\nDistance     Energy\n");
    //beginning custom integrator

    //initialization of variables
    double x1, x2, y1, y2, z1, z2;
    int num = 1000000;
    double tcustom;
    
    //random number generator
    gsl_rng *rando = gsl_rng_alloc (gsl_rng_taus2);

    gsl_rng_set (rando, seed);

    //initializing the results array for the custom integrator
    double varray[N];

    timer_start ();
    //looped over distances

    for (int kt = 0; kt < N; kt++)
    {
	double dz = 1.001 + kt * dd;
	double value = 0.;

	//assigning random coordinates
	for (int qi = 0; qi < num; qi++)
	{
	    x1 = gsl_rng_uniform (rando);
	    x2 = gsl_rng_uniform (rando);
	    y1 = gsl_rng_uniform (rando);
	    y2 = gsl_rng_uniform (rando);
	    z1 = gsl_rng_uniform (rando);
	    z2 = gsl_rng_uniform (rando);

	    //calculating the value of the integrand
	    value += f (x1, x2, y1, y2, z1, z2, dz);
	}
	//results stored in custom array

	varray[kt] = value / num;
	print_steps (distance[kt], varray[kt]);

    }
    tcustom = timer_stop ();

    double karray[N];
    double tapprox;

    timer_start ();
    printf ("\nDipole Approximation:\nDistance     Energy\n");

    //Dipole Approximation
    for (int v = 0; v < N; v++)
    {
	karray[v] = -2 / (distance[v] * distance[v] * distance[v]);
	print_steps (distance[v], karray[v]);
    }
    tapprox = timer_stop ();

    
    //distances with results of VEGAS and custom integrators
    printf
      ("\nResults (with absolute value of energy):\nDistance     GSL VEGAS     Custom     Dipole Approximation\n");
    for (int j = 0; j < N; j++)
    {
	printf ("%.6f    %.6f    %.6f    %.6f\n", distance[j],
	    fabs (result[j]), fabs (varray[j]), fabs (karray[j]));
    }

    double vegasarray[N], customarray[N], approxarray[N];

    for (int l = 0; l < N; l++)
    {
	vegasarray[l] = fabs (result[l]);
	customarray[l] = fabs (varray[l]);
	approxarray[l] = fabs (karray[l]);
    }
    
    //timing and time ratios of each method calculation
    printf ("\nTiming for Each Method:\n");
    print_timing (tvegas, tcustom, tapprox);
    printf ("\nRatio of time VEGAS / custom = %.6f\n", tvegas / tcustom);
    printf ("Ratio of time VEGAS / dipole approximation = %.6f\n", tvegas / tapprox);
    printf ("Ratio of time custom/ dipole approximation = %.6f\n", tcustom/ tvegas);
    //calculating error
    printf ("distance     VEGAS Error     Custom Error\n");
    for (int h = 0; h < N; h++)
    {
	printf ("%.6f     %.6f     %.6f\n", distance[h], vegas_error[h], fabs (customarray[h] - vegasarray[h]));
    }

    //calculating average error of VEGAS and custom integrators
    double custom_error_total = 0., GSL_error_total = 0.;;
        for(int ph = 0; ph < N; ph++)
        {
	    custom_error_total += fabs(customarray[ph] - vegasarray[ph]);
	    GSL_error_total += vegas_error[ph];
        }

	double custom_error_avg = custom_error_total/N;
	double GSL_error_avg = GSL_error_total/N;
	printf("\nAverage error for VEGAS integrator: %.6f\nAverage error for custom integrator: %.6f\n", GSL_error_avg, custom_error_avg);
	
	//using the ratio and 1/sqrt(n) error dependence
	//the time for custom integrator to achieve the same level of accuracy as VEGAS estimation
	double error_ratio = custom_error_avg/GSL_error_avg;
	printf("\nEstimated time needed for custom integrator to achieve the same error as VEGAS integrator: %.6f\n", tcustom*error_ratio*error_ratio);

	printf("\nThe custom integrator will take %.6f more seconds to achieve the same level of precision as the VEGAS integrator\n", (tcustom*error_ratio*error_ratio)-tvegas);

	return 0;
}
