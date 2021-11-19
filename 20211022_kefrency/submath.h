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