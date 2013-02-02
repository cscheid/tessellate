#include "tessellate.h"
#include <stdio.h>
#include <stdlib.h>

int main()
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
    return 0;
}
