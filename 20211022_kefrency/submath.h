typedef struct{
    double *x_real, *x_imag, *x_norm;
} X_DATA;

float sm_max(double *a, int N);
float sm_min(double *a, int N);

void data_prep(X_DATA *d, int N);
void data_free(X_DATA d);

float sm_max(double *a, int N){
    float max = -99999;
    for(int i=0; i<N; i++){
        if(max < a[i]) max = a[i];
    }
    return max;
}

float sm_min(double *a, int N){
    float min = 99999;
    for(int i=0; i<N; i++){
        if(min > a[i]) min = a[i];
    }
    return min;
}





void data_prep(X_DATA *d, int N){
    d->x_real = calloc(N, sizeof(double));
    d->x_imag = calloc(N, sizeof(double));
    d->x_norm = calloc(N, sizeof(double));
}

void data_free(X_DATA d){
    free(d.x_real);
    free(d.x_imag);
    free(d.x_norm);
}