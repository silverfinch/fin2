#include <math.h>

//Custm integrand for project
double f (double x1, double x2, double y1, double y2, double z1, double z2, double d);
double f (double x1, double x2, double y1, double y2, double z1, double z2, double d)
{
    double dist2;
    double rho1, rho2;

    dist2 = (x1 - x2 + d) * (x1 - x2 + d) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);

    double norm = 512. / (9. * (M_PI - 2.));

    rho1 = norm * atan (2. * (x1 - .5)) * pow (sin (M_PI * y1), 4.) * pow (sin (M_PI * z1), 4.);
    rho2 = norm * atan (2. * (x2 - .5)) * pow (sin (M_PI * y2), 4.) * pow (sin (M_PI * z2), 4.);

    return rho1 * rho2 / sqrt (dist2);
}
