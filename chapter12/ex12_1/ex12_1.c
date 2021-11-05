#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include <mmeapi.h>
#include "wave.h"

#define NUMBER_OF_BUFFER 8 /* ���̓o�b�t�@�̐� */
#define BUFFER_SIZE 160 /* ���̓o�b�t�@�̃T�C�Y */

int main(void)
{
  MONO_PCM pcm;
  int n, in0, in1, offset, frame, number_of_frame;
  
  short in_buffer[NUMBER_OF_BUFFER][BUFFER_SIZE]; /* ���̓o�b�t�@ */
  WAVEHDR in_hdr[NUMBER_OF_BUFFER]; /* ���̓o�b�t�@�̃w�b�_ */
  HWAVEIN in_hdl = 0; /* �T�E���h�f�o�C�X�̃n���h�� */
  
  WAVEFORMATEX wave_format_ex = {WAVE_FORMAT_PCM, /* PCM */
                                 1, /* ���m���� */
                                 8000, /* �W�{�����g���i8000Hz�j */
                                 16000, /*1�b������̉��f�[�^�̃T�C�Y�i16000byte�j */
                                 2, /* ���f�[�^�̍ŏ��P�ʁi2byte�j */
                                 16, /* �ʎq�����x�i16bit�j */
                                 0 /* �I�v�V�������̃T�C�Y�i0byte�j */
                                };
  
  pcm.fs = 8000; /* �W�{�����g�� */
  pcm.bits = 16; /* �ʎq�����x */
  pcm.length = pcm.fs * 1; /* ���f�[�^�̒��� */
  pcm.s = calloc(pcm.length, sizeof(double)); /* ���f�[�^ */
  
  number_of_frame = pcm.length / BUFFER_SIZE;
  
  /* �T�E���h�f�o�C�X�̃I�[�v�� */
   waveInOpen(&in_hdl, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);
  
  /* 0���������ނ��Ƃ�in_hdr���N���A���� */
  for (in0 = 0; in0 < NUMBER_OF_BUFFER; in0++)
  {
    memset(&in_hdr[in0], 0, sizeof(WAVEHDR));
  }
  
  in0 = 0;
  in1 = 0;
  offset = 0;
  frame = 0;
  
  while (frame < number_of_frame)
  {
    if (in0 < NUMBER_OF_BUFFER)
    {
      /* ���̓o�b�t�@�̐ݒ� */
      in_hdr[in0].lpData = (char *)in_buffer[in0]; /* ���̓o�b�t�@�̎w�� */
      in_hdr[in0].dwBufferLength = BUFFER_SIZE * 2; /* ���̓o�b�t�@�̃T�C�Y */
      in_hdr[in0].dwFlags = 0; /* �t���O�̃N���A */
      
      /* ���̓o�b�t�@�̃��b�N */
      waveInPrepareHeader(in_hdl, &in_hdr[in0], sizeof(WAVEHDR));
      
      /* ���̓o�b�t�@����͑҂��L���[�ɒǉ����� */
      waveInAddBuffer(in_hdl, &in_hdr[in0], sizeof(WAVEHDR));
      
      in0++;
      if (in0 == NUMBER_OF_BUFFER)
      /* ���ׂĂ̓��̓o�b�t�@�����͑҂��L���[�Ɋi�[���ꂽ�Ƃ� */
      {
        /* �^�����J�n���� */
        waveInStart(in_hdl);
      }
    }
    else
    {
      if ((in_hdr[in1].dwFlags & WHDR_DONE) != 0)
      /* ���̓o�b�t�@�̂����܂ŉ��f�[�^���^�����ꂽ�Ƃ� */
      {
        /* ���̓o�b�t�@�̃A�����b�N */
        waveInUnprepareHeader(in_hdl, &in_hdr[in1], sizeof(WAVEHDR));
        
        /* ���̓o�b�t�@���特�f�[�^��ǂݎ�� */
        for (n = 0; n < BUFFER_SIZE; n++)
        {
          pcm.s[offset + n] = in_buffer[in1][n] / 32768.0;
        }
        offset += BUFFER_SIZE;
        frame++;
        
        /* ���̓o�b�t�@�̐ݒ� */
        in_hdr[in1].lpData = (char *)in_buffer[in1]; /* ���̓o�b�t�@�̎w�� */
        in_hdr[in1].dwBufferLength = BUFFER_SIZE * 2; /* ���̓o�b�t�@�̃T�C�Y */
        in_hdr[in1].dwFlags = 0; /* �t���O�̃N���A */
        
        /* ���̓o�b�t�@�̃��b�N */
        waveInPrepareHeader(in_hdl, &in_hdr[in1], sizeof(WAVEHDR));
        
        /* ���̓o�b�t�@����͑҂��L���[�ɒǉ����� */
        waveInAddBuffer(in_hdl, &in_hdr[in1], sizeof(WAVEHDR));
        
        in1++;
        if (in1 == NUMBER_OF_BUFFER)
        {
          in1 = 0; /* in1��0�ɖ߂��C���̓o�b�t�@���ė��p���� */
        }
      }
    }
    
    Sleep(1); /* 1ms�̃X���[�v���͂��ނ��ƂŁC�v���O�����̎��s���x�𒲐߂��� */
  }
  
  waveInStop(in_hdl); /* ���f�[�^�̓��͂��~���� */
  for (in0 = 0; in0 < NUMBER_OF_BUFFER; in0++)
  {
    /* ���̓o�b�t�@�̃A�����b�N */
    if ((in_hdr[in0].dwFlags & WHDR_PREPARED) != 0)
    {
      waveInUnprepareHeader(in_hdl, &in_hdr[in0], sizeof(WAVEHDR ));
    }
  }
  waveInClose(in_hdl); /* �T�E���h�f�o�C�X�̃N���[�Y */
  
  wave_write_16bit_mono(&pcm, "ex12_1.wav"); /* ���f�[�^�̏o�� */
  
  free(pcm.s);
  
  return 0;
}
