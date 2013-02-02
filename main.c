#include "tessellate.h"
#include <stdio.h>
#include <stdlib.h>

void example_1()
{
    double *coordinates_out;
    int *tris_out;
    int nverts, ntris, i;

    double p[10] = { 0, 0, 1, 5, 2, 0, -1, 3, 3, 3 };
    const double *contours[] = {p, p+10};

    tessellate(&coordinates_out, &nverts,
               &tris_out, &ntris,
               contours, contours + 2);

    for (i=0; i<2 * nverts; ++i) {
        fprintf(stdout, "%g ", coordinates_out[i]);
    }
    fprintf(stdout, "\n");
    for (i=0; i<3 * ntris; ++i) {
        fprintf(stdout, "%d ", tris_out[i]);
    }
    fprintf(stdout, "\n");
    free(coordinates_out);
    if (tris_out)
        free(tris_out);
}

void example_2()
{
    double *coordinates_out;
    int *tris_out;
    int nverts, ntris, i;

    double p[16] = { 0, 0, 3, 0, 3, 3, 0, 3, 1, 1, 2, 1, 2, 2, 1, 2 };
    const double *contours[] = {p, p + 8, p+16};

    tessellate(&coordinates_out, &nverts,
               &tris_out, &ntris,
               contours, contours + 3);

    for (i=0; i<2 * nverts; ++i) {
        fprintf(stdout, "%g ", coordinates_out[i]);
    }
    fprintf(stdout, "\n");
    for (i=0; i<3 * ntris; ++i) {
        fprintf(stdout, "%d ", tris_out[i]);
    }
    fprintf(stdout, "\n");
    free(coordinates_out);
    if (tris_out)
        free(tris_out);
}

int main()
{
    example_1();
    example_2();
    return 0;
}
