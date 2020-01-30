

typedef struct Node_s {
    GLuint idx;
    GLboolean avg;
    struct Node_s* next;
} Node_t;


 float getMax(float aa, float bb) 
{
    if (bb > aa)
        return bb;
    return aa;
}

 float getAbs(float ff)
{
    if (ff < 0)
        return -ff;
    return ff;
}


 float getDot(float* u, float* v)
{
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}


 void getCross(float* u, float* v, float* n)
{
    
    n[0] = u[1]*v[2] - u[2]*v[1];
    n[1] = u[2]*v[0] - u[0]*v[2];
    n[2] = u[0]*v[1] - u[1]*v[0];
}


 void getNormalize(float* v)
{
    float l;
    
    l = (float)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= l;
    v[1] /= l;
    v[2] /= l;
}
