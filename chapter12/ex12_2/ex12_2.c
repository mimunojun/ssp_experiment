#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "wave.h"

#define NUMBER_OF_BUFFER 8 /* �o�̓o�b�t�@�̐� */
#define BUFFER_SIZE 160 /* �o�̓o�b�t�@�̃T�C�Y */

int main(void)
{
  MONO_PCM pcm;
  int n, out0, out1, offset, frame, number_of_frame;
  
  short out_buffer[NUMBER_OF_BUFFER][BUFFER_SIZE]; /* �o�̓o�b�t�@ */
  WAVEHDR out_hdr[NUMBER_OF_BUFFER]; /* �o�̓o�b�t�@�̃w�b�_ */
  HWAVEOUT out_hdl = 0; /* �T�E���h�f�o�C�X�̃n���h�� */
  
  WAVEFORMATEX wave_format_ex = {WAVE_FORMAT_PCM, /* PCM */
                                 1, /* ���m���� */
                                 8000, /* �W�{�����g���i8000Hz�j */
                                 16000, /*1�b������̉��f�[�^�̃T�C�Y�i16000byte�j */
                                 2, /* ���f�[�^�̍ŏ��P�ʁi2byte�j */
                                 16, /* �ʎq�����x�i16bit�j */
                                 0 /* �I�v�V�������̃T�C�Y�i0byte�j */
                                };
  
  wave_read_16bit_mono(&pcm, "speech.wav"); /* ���f�[�^�̓��� */
  
  number_of_frame = pcm.length / BUFFER_SIZE;
  
  /* �T�E���h�f�o�C�X�̃I�[�v�� */
  waveOutOpen(&out_hdl, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);
  
  /* �T�E���h�f�o�C�X�̈ꎞ��~ */
  waveOutPause(out_hdl);
  
  /* 0���������ނ��Ƃ�out_hdr���N���A���� */
  for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++)
  {
    memset(&out_hdr[out0], 0, sizeof(WAVEHDR));
  }
  
  out0 = 0;
  out1 = 0;
  offset = 0;
  frame = 0;
  
  while (frame < number_of_frame)
  {
    if (out0 < NUMBER_OF_BUFFER)
    {
      /* �o�̓o�b�t�@�ɉ��f�[�^���������� */
      for (n = 0; n < BUFFER_SIZE; n++)
      {
        out_buffer[out0][n] = (short)(32768.0 * pcm.s[offset + n]);
      }
      offset += BUFFER_SIZE;
      frame++;
      
      /* �o�̓o�b�t�@�̐ݒ� */
      out_hdr[out0].lpData = (char *)out_buffer[out0]; /* �o�̓o�b�t�@�̎w�� */
      out_hdr[out0].dwBufferLength = BUFFER_SIZE * 2; /* �o�̓o�b�t�@�̃T�C�Y */
      out_hdr[out0].dwFlags = 0; /* �t���O�̃N���A */
      
      /* �o�̓o�b�t�@�̃��b�N */
      waveOutPrepareHeader(out_hdl, &out_hdr[out0], sizeof(WAVEHDR));
      
      /* �o�̓o�b�t�@���o�͑҂��L���[�ɒǉ����� */
      waveOutWrite(out_hdl, &out_hdr[out0], sizeof(WAVEHDR));
      
      out0++;
      if (out0 == NUMBER_OF_BUFFER)
      /* ���ׂĂ̏o�̓o�b�t�@���o�͑҂��L���[�Ɋi�[���ꂽ�Ƃ� */
      {
        /* �Đ����J�n���� */
        waveOutRestart(out_hdl);
      }
    }
    else
    {
      if ((out_hdr[out1].dwFlags & WHDR_DONE) != 0)
      /* �o�̓o�b�t�@�̂����܂ŉ��f�[�^���Đ����ꂽ�Ƃ� */
      {
        /* �o�̓o�b�t�@�ɉ��f�[�^���������� */
        for (n = 0; n < BUFFER_SIZE; n++)
        {
          out_buffer[out1][n] = (short)(32768.0 * pcm.s[offset + n]);
        }
        offset += BUFFER_SIZE;
        frame++;
        
        /* �o�̓o�b�t�@�̃A�����b�N */
        waveOutUnprepareHeader(out_hdl, &out_hdr[out1], sizeof(WAVEHDR));
        
        /* �o�̓o�b�t�@�̐ݒ� */
        out_hdr[out1].lpData = (char *)out_buffer[out1]; /* �o�̓o�b�t�@�̎w�� */
        out_hdr[out1].dwBufferLength = BUFFER_SIZE * 2; /* �o�̓o�b�t�@�̃T�C�Y */
        out_hdr[out1].dwFlags = 0; /* �t���O�̃N���A */
        
        /* �o�̓o�b�t�@�̃��b�N */
        waveOutPrepareHeader(out_hdl, &out_hdr[out1], sizeof(WAVEHDR));
        
        /* �o�̓o�b�t�@���o�͑҂��L���[�ɒǉ����� */
        waveOutWrite(out_hdl, &out_hdr[out1], sizeof(WAVEHDR));
        
        out1++;
        if (out1 == NUMBER_OF_BUFFER)
        {
          out1 = 0; /* out1��0�ɖ߂��C�o�̓o�b�t�@���ė��p���� */
        }
      }
    }
    
    Sleep(1); /* 1ms�̃X���[�v���͂��ނ��ƂŁC�v���O�����̎��s���x�𒲐߂��� */
  }
  
  for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++)
  {
    while ((out_hdr[out0].dwFlags & WHDR_DONE) == 0)
    /* �Đ�����������̂�҂� */
    {
      Sleep(1); /* 1ms�̃X���[�v���͂��ނ��ƂŃv���O�����̎��s���x�𒲐߂��� */
    }
  }
  
  waveOutPause(out_hdl); /* ���f�[�^�̏o�͂��~���� */
  for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++)
  {
    /* �o�̓o�b�t�@�̃A�����b�N */
    if ((out_hdr[out0].dwFlags & WHDR_PREPARED) != 0)
    {
      waveOutUnprepareHeader(out_hdl, &out_hdr[out0], sizeof(WAVEHDR));
    }
  }
  waveOutClose(out_hdl); /* �T�E���h�f�o�C�X�̃N���[�Y */
  
  free(pcm.s);
  
  return 0;
}
