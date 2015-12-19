#include <math.h>
#include <stdlib.h>

//Integrand of VEGAS integrator

double g (double *t, size_t dim, void *params);
double g (double *t, size_t dim, void *params)
{
    double dist2, delta;
    double rho1, rho2;
    int sdim = ((int) dim) / 2;
    double r = *(double *) params;
	
    dist2 = 0.;
    for (int i = 0; i < sdim; i++)
    {
        delta = t[i] - t[i + sdim];
        if (i == 0)
        {
            delta += r;
        }
        dist2 += delta * delta;
    }
    double norm = 512. / (9. * (M_PI - 2.));

    rho1 =
        norm * atan (2 * (t[0] - .5)) * pow (sin (M_PI * t[1]),
        4.) * pow (sin (M_PI * t[2]), 4.);
    rho2 =
        norm * atan (2 * (t[3] - .5)) * pow (sin (M_PI * t[4]),
        4.) * pow (sin (M_PI * t[5]), 4.);
    r += 0.5;
    return rho1 * rho2 / sqrt (dist2);
}
