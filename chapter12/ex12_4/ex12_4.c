#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

#define NUMBER_OF_BUFFER 8 /* �o�b�t�@�̐� */
#define BUFFER_SIZE 160 /* �o�b�t�@�̃T�C�Y */

void data_pos(char *str, short s);

int main(void)
{
  char key;
  int n, in0, in1, out0, out1, flag, offset, fs;
  double am, fm;
  short s[BUFFER_SIZE]; /* ���f�[�^ */

  short in_buffer[NUMBER_OF_BUFFER][BUFFER_SIZE]; /* ���̓o�b�t�@ */
  WAVEHDR in_hdr[NUMBER_OF_BUFFER]; /* ���̓o�b�t�@�̃w�b�_ */
  HWAVEIN in_hdl = 0; /* �T�E���h�f�o�C�X�̃n���h�� */

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

  /* �T�E���h�f�o�C�X�̃I�[�v�� */
   waveInOpen(&in_hdl, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);

  /* 0���������ނ��Ƃ�in_hdr���N���A���� */
  for (in0 = 0; in0 < NUMBER_OF_BUFFER; in0++)
  {
    memset(&in_hdr[in0], 0, sizeof(WAVEHDR));
  }

  /* �T�E���h�f�o�C�X�̃I�[�v�� */
  waveOutOpen(&out_hdl, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);

  /* �T�E���h�f�o�C�X�̈ꎞ��~ */
  waveOutPause(out_hdl);

  /* 0���������ނ��Ƃ�out_hdr���N���A���� */
  for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++)
  {
    memset(&out_hdr[out0], 0, sizeof(WAVEHDR));
  }

  in0 = 0;
  in1 = 0;
  out0 = 0;
  out1 = 0;
  flag = 0;
  offset = 0;

  FILE *outputfile;         // �o�̓X�g���[��

  outputfile = fopen("d.txt", "w");  // �t�@�C�����������ݗp�ɃI�[�v��(�J��)
  if (outputfile == NULL) {          // �I�[�v���Ɏ��s�����ꍇ
    printf("cannot open\n");         // �G���[���b�Z�[�W���o����
    exit(1);                         // �ُ�I��
  }

  char str[200] = "";

  /* �����O�ϒ��̃p�����[�^ */
  fs = 8000; /* �W�{�����g�� */
  am = 1.0; /* �U�� */
  fm = 300.0; /* ���g�� */

  while (1) /* �������[�v */
  {
    if (flag == 1) /* flag��1�̂Ƃ� */
    {
      /* ���f�[�^�̏o�� */
      if (out0 < NUMBER_OF_BUFFER)
      {
        /* �o�̓o�b�t�@�ɉ��f�[�^���������� */
        for (n = 0; n < BUFFER_SIZE; n++)
        {
          /* �{�C�X�`�F���W�� */
          out_buffer[out0][n] = am * sin(2.0 * M_PI * fm * (offset + n) / fs) * s[n];

          data_pos(&str[0], s[n]);
          fprintf(outputfile, "%s", str);
          fprintf(outputfile, "%hd\n", s[n]);
          printf("%s", str);
        }
        offset += BUFFER_SIZE;

        flag = 0; /* flag��0�ɂ��� */

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
          /* ���f�[�^�̏o�͂��J�n���� */
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
            /* �{�C�X�`�F���W�� */
            out_buffer[out1][n] = am * sin(2.0 * M_PI * fm * (offset + n) / fs) * s[n];
          }
          offset += BUFFER_SIZE;

          flag = 0; /* flag��0�ɂ��� */

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
    }

    /* ���f�[�^�̓��� */
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
      /* ���ׂĂ̓��̓o�b�t�@�����͑҂��L���[�ɒǉ����ꂽ�Ƃ� */
      {
        /* ���f�[�^�̓��͂��J�n���� */
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
          s[n] = in_buffer[in1][n];
        }

        flag = 1; /* flag��1�ɂ��� */

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

    if (kbhit()) /* �L�[�{�[�h������͂�����Ƃ� */
    {
      key = getch(); /* �L�[�̃`�F�b�N */
      if (key == 13) /* Enter�L�[�������ꂽ�Ƃ� */
      {
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

        fclose(outputfile);          // �t�@�C�����N���[�Y(����)


        return 0;
      }
    }

    Sleep(1); /* 1ms�̃X���[�v���͂��ނ��ƂŁC�v���O�����̎��s���x�𒲐߂��� */
  }
}

void data_pos(char *str, short s){
  int i,j,k;
  i = (int)(s/300 + 50);
  if(i<0) i=1;
  if(i>200) i=190;
  for(j=0; j<i; j++){
    str[j] = ' ';
  }
  str[j] = '.';
  str[j+1] = '\n';
}
