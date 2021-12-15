#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "wave.h"
#include "fft.h"
#include "submath.h"

int main(int argc, char *argv[]){
  MONO_PCM pcm;
  int n, k, N, kN, iN;
  double *x_real, *x_imag, *X_real, *x_norm, *X_imag, W_real, W_imag, *kx_real, *kx_imag, *kx_norm, *x_norm_db, *x_invnorm, *x_invreal, *x_invimag;
  double *h_window, *h_window2, *h_window4;
  
  int data_len;
  char path[] = "wave/";
  // FILE *fp_wave, *fp_freq, *fp_fdb, *fp_kfre, *fp_invk;
  FILE *fp;
  // char outputpath_f[] = "output/csv/";
  // char outputpath_k[] = "output/csv/";
 
  strcat(path, argv[1]);
  wave_read_16bit_mono(&pcm, path);
  data_len = pcm.length;
  


  N=512;
  kN=256;
  iN=128;

  h_window = calloc(N, sizeof(double));
  h_window2 = calloc(kN, sizeof(double));
  h_window4 = calloc(kN/2, sizeof(double));
  
  for(n=0; n<N; n++){
    h_window[n] = 0.5-0.5*cos(2*M_PI*(float)n/N);
  }

  for(n=0; n<kN; n++){
    h_window2[n] = 0.5-0.5*cos(2*M_PI*(float)n/kN);
  }
  for(n=0; n<kN/2; n++){
    h_window4[n] = 0.5-0.5*cos(2*M_PI*(float)n/(kN/2));
  }

  
  printf("data_len = %d\n",data_len);


  // for(int i=0;i<data_len ;i++){
  //   printf("%d,%f\n",i,pcm.s[i]);
  // }

  int c = 0;
  MONO_PCM fdata; //最終的に書き出すデータ
  fdata.fs = 8000;
  fdata.bits = 16;
  fdata.length = data_len;
  fdata.s = calloc(fdata.length, sizeof(double));

  for(c=0; c*N/2 + N < data_len; c++){
    
    X_DATA xdata;
    // printf("hi %d,%d,%d,%d,\n",c,N,c*N/2 + N, data_len);
    data_prep(&xdata, N);


    for(n=0; n<N; n++){
      xdata.x_real[n] = pcm.s[c*N/2+n];
      xdata.x_imag[n] = 0.0;
      xdata.x_real[n] = xdata.x_real[n] * h_window[n];
    }

    FFT(xdata.x_real, xdata.x_imag, N);

    for(n=N-1; n>N/2+32; n--){
      xdata.x_real[n] = xdata.x_real[n-32];
      xdata.x_imag[n] = xdata.x_imag[n-32];
    }
    for(n=0; n<N/2-32; n++){
      xdata.x_real[n] = xdata.x_real[n+32];
      xdata.x_imag[n] = xdata.x_imag[n+32];
    }
    for(n=7*N/16; n<9*N/16; n++){
      xdata.x_real[n] = 0.0;
      xdata.x_imag[n] = 0.0;
    }

    // for(n=N/2; n>32; n--){ //shift+500hz
    //   xdata.x_real[n] = xdata.x_real[n-32];
    //   xdata.x_imag[n] = xdata.x_imag[n-32];
    // }
    // for(n=N/2; n<N-32; n++){
    //   xdata.x_real[n] = xdata.x_real[n+32];
    //   xdata.x_imag[n] = xdata.x_imag[n+32];
    // }
    // for(n=0; n<1*N/16; n++){
    //   xdata.x_real[n] = 0.0;
    //   xdata.x_imag[n] = 0.0;
    // }
    // for(n=15*N/16; n<N/16; n++){
    //   xdata.x_real[n] = 0.0;
    //   xdata.x_imag[n] = 0.0;
    // }

    // for(n=N/4; n<N*3/4; n++){ //lowpass
    //   xdata.x_real[n] = 0.0;
    //   xdata.x_imag[n] = 0.0;
    // }


    fp = fopen("testing4.csv", "w");
    fprintf(fp, "t, X_wave\n");
    for(k=0; k<N; k++){
      fprintf(fp, "%f, %f\n ", (float)8000/(N)*k, xdata.x_real[k]);
    }
    fclose(fp);

    IFFT(xdata.x_real, xdata.x_imag, N);

    for(n=0; n<N; n++){
      fdata.s[c*N/2 + n] += xdata.x_real[n];
    }

    data_free(xdata);
  }
  
  free(h_window);
  free(h_window2);
  free(h_window4);
  free(pcm.s);
  
  wave_write_16bit_mono(&fdata, "testing4.wav");
  free(fdata.s);
  printf("exit\n");

  // //波形x窓関数
  // for(n=0; n<N; n++){
  //   x_real[n] = x_real[n] * h_window[n];
  // }

  // //波形描画
  // fp = fopen("w.csv", "w");
  // fprintf(fp, "t, X_wave\n");
  // for(k=1; k<N; k++){
  //   fprintf(fp, "%f, %f\n ", (float)1/8000*k, x_real[k]);
  // }
  // fclose(fp);

  // //波形toスペクトラム
  // FFT(x_real, x_imag, N);

  // for(k=0; k<N; k++){
  //   x_norm[k] = sqrt(x_real[k]*x_real[k] + x_imag[k]*x_imag[k]);
  //   // printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  // }

  // fp = fopen("f.csv", "w");
  // fprintf(fp, "f, X_norm\n");
  // for(k=1; k<N/2; k++){
  //   fprintf(fp, "%d, %f\n ", 8000/(N)*k, x_norm[k]);
  // }
  // fclose(fp);

  // //スペクトラムのdb化
  // for(n=0; n<kN; n++){
  //   x_norm_db[n] = 20*log10(x_norm[n]/100);
  // }

  // //printf("%f", (sm_min(x_norm_db, kN) + sm_max(x_norm_db, kN))/2);

  // float kx_average = (sm_min(x_norm_db, kN) + sm_max(x_norm_db, kN))/2;

  // //スペクトラムtoケプストラム準備
  // for(n=0; n<kN; n++){
  //   kx_real[n] = x_norm_db[n] - sm_min(x_norm_db, kN);
  //   kx_imag[n] = 0.0;
  // }

  // //スペクトラムx窓関数
  // for(n=0; n<kN; n++){
  //   kx_real[n] = kx_real[n] ;
  // }

  // fp = fopen("fdb.csv", "w");
  // fprintf(fp, "f, X_db\n");
  // for(k=1; k<N/2; k++){
  //   fprintf(fp, "%d, %f\n ", 8000/(N)*k, kx_real[k]);
  // }

  // fclose(fp);

  // //スペクトラムtoケプストラム
  // FFT(kx_real, kx_imag, kN);

  // for(k=0; k<N; k++){
  //   kx_norm[k] = sqrt(kx_real[k]*kx_real[k] + kx_imag[k]*kx_imag[k]);
  //   // printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  // }

  // fp = fopen("k.csv", "w");
  // fprintf(fp, "kf, X_kfre\n");
  // for(k=0; k<kN/2; k++){
  //   fprintf(fp, "%d, %f\n ", k, kx_norm[k]);
  // }
  // fclose(fp);

  // for(n=0; n<kN/2; n++){
  //   x_invreal[n] = 0;
  //   x_invimag[n] = 0;
  // }

  // for(n=0; n<kN/2; n++){
  //   x_invreal[n] = kx_real[n];
  //   x_invimag[n] = kx_imag[n];
  // }

  // for(n=12; n<kN/2; n++){
  //   x_invreal[n] = 0;
  //   x_invimag[n] = 0;
  // }
  

  // IFFT(x_invreal, x_invimag, N/4);

  // for(k=0; k<N; k++){
  //   x_invnorm[k] = sqrt(x_invreal[k]*x_invreal[k] + x_invimag[k]*x_invimag[k]);
  //   // printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  // }

  // fp = fopen("invk.csv", "w");
  // fprintf(fp, "kf, X_kfre\n");
  // for(k=0; k<128; k++){
  //   fprintf(fp, "%d, %f\n ", 8000/(N)*k, x_invnorm[k]);
  // }
  // fclose(fp);

  // for(k=0; k<N; k++){
  //   x_norm[k] = sqrt(x_real[k]*x_real[k] + x_imag[k]*x_imag[k]);
  //   // printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  // }

  // fp = fopen("spectowave2.csv", "w");
  // fprintf(fp, "f, X_norm\n");
  // for(k=0; k<N; k++){
  //   fprintf(fp, "%f, %f\n ", (float)1/8000*k, x_norm[k]);
  // }
  // fclose(fp);



  // for(k=N/10; k<9*N/10; k++){
  //   x_real[k] = 0;
  //   x_imag[k] = 0;
  // }
  
  // IFFT(x_real, x_imag, N/2);

  // for(k=0; k<N; k++){
  //   x_norm[k] = sqrt(x_real[k]*x_real[k] + x_imag[k]*x_imag[k]);
  //   // printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  // }

  // fp = fopen("spectowave.csv", "w");
  // fprintf(fp, "f, X_norm\n");
  // for(k=0; k<N/2; k++){
  //   fprintf(fp, "%f, %f\n ", (float)1/8000*k, x_real[k]);
  // }
  // fclose(fp);


  // // strcat(outputpath_f, argv[1]);
  // // strcat(outputpath_f, ".f.csv");




  // // strcat(outputpath_k, argv[1]);
  // // strcat(outputpath_k, ".k.csv");


  
  // free(x_real);
  // free(x_imag);
  // free(x_norm);
  // free(kx_imag);
  // free(kx_imag);
  // free(kx_norm);
  // free(X_real);
  // free(X_imag);
  // free(x_norm_db);

  return 0;
}
