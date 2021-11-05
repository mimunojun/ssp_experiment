#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "wave.h"
#include "fft.h"

int main(int argc, char *argv[]){
  MONO_PCM pcm;
  int n, k, N, kN;
  double *x_real, *x_imag, *X_real, *x_norm, *X_imag, W_real, W_imag, *kx_real, *kx_imag, *kx_norm;
  char path[] = "wave/";
  // char outputpath_f[] = "output/csv/";
  // char outputpath_k[] = "output/csv/";

  strcat(path, argv[1]);
  wave_read_16bit_mono(&pcm, path);

  N=512;
  kN=256;

  x_real = calloc(N, sizeof(double));
  x_imag = calloc(N, sizeof(double));
  // X_real = calloc(N, sizeof(double));
  // X_imag = calloc(N, sizeof(double));
  x_norm = calloc(N, sizeof(double));

  kx_real = calloc(kN, sizeof(double));
  kx_imag = calloc(kN, sizeof(double));
  kx_norm = calloc(kN, sizeof(double));

  for(n=0; n<N; n++){
    x_real[n] = pcm.s[n];
    x_imag[n] = 0.0;
  }



  // for(k=0; k<N; k++){
  //   for(n=0; n<N; n++){
  //     W_real = cos(2.0*M_PI*k*n/N);
  //     W_imag = -sin(2.0*M_PI*k*n/N);
  //     X_real[k] += W_real * x_real[n] - W_imag * x_imag[n];
  //     X_imag[k] += W_real * x_imag[n] + W_imag * x_real[n];
  //   }
  //   X_norm[k] = sqrt(X_real[k]*X_real[k] + X_imag[k]*X_imag[k]);
  // }

  FFT(x_real, x_imag, N);


  for(k=0; k<N; k++){
    x_norm[k] = sqrt(x_real[k]*x_real[k] + x_imag[k]*x_imag[k]);
    // printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  }

  for(n=0; n<kN; n++){
    kx_real[n] = x_norm[n];
    kx_imag[n] = 0.0;
  }

  FFT(kx_real, kx_imag, kN);

  for(k=0; k<N; k++){
    kx_norm[k] = sqrt(kx_real[k]*kx_real[k] + kx_imag[k]*kx_imag[k]);
    // printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  }

  FILE *fp_freq, *fp_kfre;

  // strcat(outputpath_f, argv[1]);
  // strcat(outputpath_f, ".f.csv");
  fp_freq = fopen("f.csv", "w");
  fprintf(fp_freq, "f, X_norm\n");
  for(k=1; k<N/2; k++){
    fprintf(fp_freq, "%d, %f\n ", 8000/(N)*k, x_norm[k]);
  }

  fclose(fp_freq);

  // strcat(outputpath_k, argv[1]);
  // strcat(outputpath_k, ".k.csv");
  fp_kfre = fopen("k.csv", "w");
  fprintf(fp_kfre, "kf, X_kfre\n");
  for(k=2; k<kN/2; k++){
    fprintf(fp_kfre, "%d, %f\n ", k, kx_norm[k]);
  }

  
  fclose(fp_kfre);


  free(pcm.s);
  free(x_real);
  free(x_imag);
  free(x_norm);
  free(kx_imag);
  free(kx_imag);
  free(kx_norm);
  free(X_real);
  free(X_imag);

  return 0;
}
