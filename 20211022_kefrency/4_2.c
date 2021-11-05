#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "fft.h"

int main(){
    
  MONO_PCM pcm;
  int n, k, N, kN;
  double *x_real, *x_imag, *X_real, *x_norm, *X_imag, W_real, W_imag, *kx_real, *kx_imag, *kx_norm;

  wave_read_16bit_mono(&pcm, "clap.wav");

  N=64;

  x_real = calloc(N, sizeof(double));
  x_imag = calloc(N, sizeof(double));
  // X_real = calloc(N, sizeof(double));
  // X_imag = calloc(N, sizeof(double));
  x_norm = calloc(N, sizeof(double));

  printf("hi");

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
    printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  }

  FILE *fp_freq, *fp_kfre;

  fp_freq = fopen("freq.csv", "w");
  fprintf(fp_freq, "f, X_norm\n");
  for(k=0; k<N/2; k++){
    fprintf(fp_freq, "%d, %f\n ", 1000/8*k, x_norm[k]);
  }

  fclose(fp_freq);


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
