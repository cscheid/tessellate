#include "glu.h"
#include "tess.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/

typedef struct Vertex {
    double pt[3];
    int index;
    struct Vertex *prev;
} Vertex;

Vertex *latest_v = NULL;
Vertex *new_vertex(double x, double y)
{
    Vertex *result = (Vertex *)malloc(sizeof(Vertex));
    result->prev = latest_v;
    result->pt[0] = x;
    result->pt[1] = y;
    result->pt[2] = 0;

    if (latest_v == NULL) {
        result->index = 0;
    } else {
        result->index = latest_v->index+1;
    }
    return latest_v = result;
}

typedef struct Triangle {
    int v[3];
    struct Triangle *prev;
} Triangle;

Triangle *latest_t = NULL;
int n_tris = 0;
Triangle *new_triangle(int v1, int v2, int v3)
{
    Triangle *result = (Triangle *)malloc(sizeof(Triangle));
    result->prev = latest_t;
    result->v[0] = v1;
    result->v[1] = v2;
    result->v[2] = v3;
    n_tris++;
    return latest_t = result;
}

/******************************************************************************/

Vertex *v_prev = NULL;
Vertex *v_prevprev = NULL;
GLenum current_mode;

void (*vertex_cb)(Vertex *);

void skip_vertex(Vertex *v) {};

void fan_vertex(Vertex *v) {
    if (v_prevprev == NULL) {
        v_prevprev = v;
        return;
    }
    if (v_prev == NULL) {
        v_prev = v;
        return;
    }
    new_triangle(v_prevprev->index, v_prev->index, v->index);
    v_prev = v;
}

void strip_vertex(Vertex *v)
{ 
    if (v_prevprev == NULL) {
        v_prevprev = v;
        return;
    }
    if (v_prev == NULL) {
        v_prev = v;
        return;
    }
    new_triangle(v_prevprev->index, v_prev->index, v->index);
 
    v_prev = v_prevprev;
    v_prevprev = v;
}

void triangle_vertex(Vertex *v) {
    if (v_prevprev == NULL) {
        v_prevprev = v;
        return;
    }
    if (v_prev == NULL) {
        v_prev = v;
        return;
    }
    new_triangle(v_prevprev->index, v_prev->index, v->index);
    v_prev = v_prevprev = NULL;
}

void vertex(void *vertex_data)
{
    Vertex *ptr = (Vertex *)vertex_data;
    vertex_cb(ptr);
}

void begin(GLenum which)
{
    v_prev = v_prevprev = NULL;
    switch (which) {
    case GL_TRIANGLES: vertex_cb = &triangle_vertex; break;
    case GL_TRIANGLE_STRIP: vertex_cb = &strip_vertex; break;
    case GL_TRIANGLE_FAN: vertex_cb = &fan_vertex; break;
    default:
        fprintf(stderr, "ERROR, can't handle %d\n", (int)which);
        vertex_cb = &skip_vertex;
    }
}

void end()
{
}

void combine(const GLdouble newVertex[3],
             const void *neighborVertex[4],
             const GLfloat neighborWeight[4], void **outData)
{
    Vertex *result = new_vertex(newVertex[0], newVertex[1]);
    *outData = result;
}

void write_output(double **coordinates_out, int **tris_out, int *vc, int *tc)
{
    int n_verts = 1 + latest_v->index;
    *vc = n_verts;
    int n_tris_copy = n_tris;
    *tc = n_tris;
    *coordinates_out = malloc(n_verts * sizeof(double) * 2);
    *tris_out = (n_tris ? malloc(n_tris * sizeof(int) * 3) : NULL);

    while (latest_v) {
        (*coordinates_out)[2*latest_v->index]   = latest_v->pt[0];
        (*coordinates_out)[2*latest_v->index+1] = latest_v->pt[1];
        Vertex *prev = latest_v->prev;
        free(latest_v);
        latest_v = prev;
    }

    while (latest_t) {
        (*tris_out)[3*(n_tris_copy-1)]   = latest_t->v[0];
        (*tris_out)[3*(n_tris_copy-1)+1] = latest_t->v[1];
        (*tris_out)[3*(n_tris_copy-1)+2] = latest_t->v[2];
        Triangle *prev = latest_t->prev;
        free(latest_t);
        latest_t = prev;
        n_tris_copy--;
    }
}

void tessellate
    (double **verts,
     int *nverts,
     int **tris,
     int *ntris,
     const double **contoursbegin, 
     const double **contoursend)
{
    const double *contourbegin, *contourend;
    Vertex *current_vertex;
    GLUtesselator *tess;

    latest_v = NULL;
    latest_t = NULL;
    v_prev = NULL;
    v_prevprev = NULL;
    n_tris = 0;
    tess = gluNewTess();


    gluTessCallback(tess, GLU_TESS_VERTEX,  (GLvoid (*) ()) &vertex);
    gluTessCallback(tess, GLU_TESS_BEGIN,   (GLvoid (*) ()) &begin);
    gluTessCallback(tess, GLU_TESS_END,     (GLvoid (*) ()) &end);
    gluTessCallback(tess, GLU_TESS_COMBINE, (GLvoid (*) ()) &combine);

    gluTessBeginPolygon(tess, 0);
    do {
        contourbegin = *contoursbegin++;
        if (contoursbegin == contoursend) {
            break;
        }
        contourend = *contoursbegin;
        gluTessBeginContour(tess);
        while (contourbegin != contourend) {
            current_vertex = new_vertex(contourbegin[0], contourbegin[1]);
            contourbegin += 2;
            gluTessVertex(tess, current_vertex->pt, current_vertex);
        }
        gluTessEndContour(tess);
    } while (1);
    gluTessEndPolygon(tess);

    write_output(verts, tris, nverts, ntris);

    gluDeleteTess(tess);
}
