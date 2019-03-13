#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

#define RGB24_BYTES_IN_PIXEL  3
#define RGB24_BITS_IN_PIXEL  24

static void
getBmpFileHeader(byte* const pbBmpFileHeader, // bmp�ļ�ͷ14���ֽ�
                 const   int iWidth, 
                 const   int iHeight, 
                 const   int iBytesInPixel)
{
    // 2bytes - ˵���ļ������ͣ���ֵ������ 0x42 0x4D ��Ҳ�����ַ�'BM'�������ʾ��������BMP
    pbBmpFileHeader[0] = 'B';
    pbBmpFileHeader[1] = 'M';

    // 4bytes - ˵����λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ(14�ֽ��ļ�ͷ��40�ֽ�λͼ��Ϣͷ��3�ֽ�RGB)
    const int iFileSize = 14 + 40 + (iWidth * iHeight * iBytesInPixel);
    for (int idx = 0;
             idx < 4;
             idx++)
    {
        pbBmpFileHeader[2 + idx] = (byte)(iFileSize >> (idx * 8));
    }

    // 2bytes - ��������������Ϊ0
    pbBmpFileHeader[6] = 0;
    pbBmpFileHeader[7] = 0;

    // 2bytes - ��������������Ϊ0
    pbBmpFileHeader[8] = 0;
    pbBmpFileHeader[9] = 0;

    // 4bytes - ˵�����ļ�ͷ��ʼ��ʵ�ʵ�ͼ������֮����ֽڵ�ƫ����
    pbBmpFileHeader[10] = 54;
    pbBmpFileHeader[11] = 0;
    pbBmpFileHeader[12] = 0;
    pbBmpFileHeader[13] = 0;
}

static void 
getBmpInfoHeader(byte* const pbBmpInfoHeader, // λͼ��Ϣͷ40���ֽ�
                 const   int width, 
                 const   int height,
                 const   int iBitsInPixel)
{
    // 4bytes - ˵�� BitmapInfoHeader �ṹ����Ҫ���ֽ���
    pbBmpInfoHeader[0] = 40;
    pbBmpInfoHeader[1] = 0;
    pbBmpInfoHeader[2] = 0;
    pbBmpInfoHeader[3] = 0;

    // 4bytes - ˵��ͼ��Ŀ�ȣ�������Ϊ��λ
    for (int idx = 0;
             idx < 4;
             idx++)
    {
        pbBmpInfoHeader[4 + idx] = (byte)(width >> (idx * 8));
    }

    // 4bytes - ˵��ͼ��ĸ߶ȣ�������Ϊ��λ
    // ע�����ֵ������������ͼ��ĸ߶�֮�⣬��������һ���ô�������ָ����ͼ���ǵ����λͼ�����������λͼ��
    // �����ֵ��һ��������˵��ͼ���ǵ���ģ��������ݵĵ�һ����ʵ��ͼ������һ�С�
    // �����ֵ��һ����������˵��ͼ��������ġ�
    // �������BMP�ļ����ǵ����λͼ��Ҳ����ʱ���߶�ֵ��һ��������
    for (int idx = 0;
             idx < 4;
             idx++)
    {
        pbBmpInfoHeader[8 + idx] = (byte)(height >> (idx * 8));
    }

    // 2bytes - ��ʾbmpͼƬ��ƽ��������Ȼ��ʾ��ֻ��һ��ƽ�棬���Ժ����1
    pbBmpInfoHeader[12] = 1;
    pbBmpInfoHeader[13] = 0;

    // 2bytes - ˵��������/���أ���ֵΪ1��4��8��16��24��32
    pbBmpInfoHeader[14] = iBitsInPixel;
    pbBmpInfoHeader[15] = 0;

    // 4bytes - ˵��ͼ������ѹ�������ͣ����У�
    // 0 - BI_RGB       - û��ѹ��
    // 1 - BI_RLE8      - ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��
    // 2 - BI_RLE4      - ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
    // 3 - BI_BITFIELDS - ÿ�����صı�����ָ�����������
    // 4 - BI_JPEG      - JPEG��ʽ
    // 5 - BI_PNG       - PNG��ʽ
    pbBmpInfoHeader[16] = 0;
    pbBmpInfoHeader[17] = 0;
    pbBmpInfoHeader[18] = 0;
    pbBmpInfoHeader[19] = 0;

    // 4bytes - ˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0��
    pbBmpInfoHeader[20] = 0;
    pbBmpInfoHeader[21] = 0;
    pbBmpInfoHeader[22] = 0;
    pbBmpInfoHeader[23] = 0;

    // 4bytes - ˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ��
    pbBmpInfoHeader[24] = 0;
    pbBmpInfoHeader[25] = 0;
    pbBmpInfoHeader[26] = 0;
    pbBmpInfoHeader[27] = 0;

    // 4bytes - ˵����ֱ�ֱ��ʣ�������/�ױ�ʾ��
    pbBmpInfoHeader[28] = 0;
    pbBmpInfoHeader[29] = 0;
    pbBmpInfoHeader[30] = 0;
    pbBmpInfoHeader[31] = 0;

    // 4bytes - ˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ�����
    pbBmpInfoHeader[32] = 0;
    pbBmpInfoHeader[33] = 0;
    pbBmpInfoHeader[34] = 0;
    pbBmpInfoHeader[35] = 0;

    // 4bytes - ˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��
    pbBmpInfoHeader[36] = 0;
    pbBmpInfoHeader[37] = 0;
    pbBmpInfoHeader[38] = 0;
    pbBmpInfoHeader[39] = 0;
}

#define BMP200X200ByteSize 200 * 200 * 3
#define BMP640X480ByteSize 640 * 480 * 3

#define RAW_BMP_WIDTH  640
#define RAW_BMP_HEIGHT 480

#define PATCH_BMP_WIDTH  200
#define PATCH_BMP_HEIGHT 200

int
main(int   argc, 
     char* argv[])
{
    FILE* fpRawBmpFile     = NULL;
    FILE* fp200X200BmpFile = NULL;

    byte* pb200X200Bmp = NULL;
    byte* pb640X480Bmp = NULL;

    int iResult = EXIT_SUCCESS;
    do
    {
        fpRawBmpFile = fopen(argv[1], "rb");

        if (NULL == fpRawBmpFile) {
            printf("%s can't open!\n", argv[1]);
            iResult = EXIT_FAILURE;
            break;
        }

        pb200X200Bmp = (byte*)malloc(BMP200X200ByteSize);
        memset(pb200X200Bmp, 0, BMP200X200ByteSize);

        pb640X480Bmp = (byte*)malloc(BMP640X480ByteSize);
        memset(pb640X480Bmp, 0, BMP640X480ByteSize);

        byte caBmpFileHeader[14];
        memset(caBmpFileHeader, 0, sizeof(caBmpFileHeader));

        byte caBmpInfoHeader[40];
        memset(caBmpInfoHeader, 0, sizeof(caBmpInfoHeader));

        fread(caBmpFileHeader, sizeof(caBmpFileHeader), 1, fpRawBmpFile);
        fread(caBmpInfoHeader, sizeof(caBmpInfoHeader), 1, fpRawBmpFile);
        fread(pb640X480Bmp, BMP640X480ByteSize, 1, fpRawBmpFile);
        fclose(fpRawBmpFile);
        fpRawBmpFile = NULL;

        const int rowStart = (RAW_BMP_HEIGHT - PATCH_BMP_HEIGHT) / 2;
        const int colStart = (RAW_BMP_WIDTH  - PATCH_BMP_WIDTH)  / 2;

        int iCurPos = 0;

        for (int idxRow = 0;
                 idxRow < PATCH_BMP_HEIGHT;
                 idxRow++)
        {
            int cutPixelPos = ((rowStart + idxRow) * RAW_BMP_WIDTH) + colStart;

            memcpy(pb200X200Bmp + iCurPos, 
                   pb640X480Bmp + (cutPixelPos * RGB24_BYTES_IN_PIXEL),
                   PATCH_BMP_WIDTH * RGB24_BYTES_IN_PIXEL);

            iCurPos += PATCH_BMP_WIDTH * RGB24_BYTES_IN_PIXEL;
        }

        fp200X200BmpFile = fopen(argv[2], "wb");

        if (NULL == fp200X200BmpFile) {
            printf("%s can't open!\n", argv[2]);
            iResult = EXIT_FAILURE;
            break;
        }

        memset(caBmpFileHeader, 0, sizeof(caBmpFileHeader));
        getBmpFileHeader(caBmpFileHeader, PATCH_BMP_WIDTH, PATCH_BMP_HEIGHT, RGB24_BYTES_IN_PIXEL);
        fwrite(caBmpFileHeader, sizeof(caBmpFileHeader), 1, fp200X200BmpFile);

        memset(caBmpInfoHeader, 0, sizeof(caBmpInfoHeader));
        getBmpInfoHeader(caBmpInfoHeader, PATCH_BMP_WIDTH, -PATCH_BMP_HEIGHT, RGB24_BITS_IN_PIXEL);
        fwrite(caBmpInfoHeader, sizeof(caBmpInfoHeader), 1, fp200X200BmpFile);

        fwrite(pb200X200Bmp, BMP200X200ByteSize, 1, fp200X200BmpFile);

        fclose(fp200X200BmpFile);
        fp200X200BmpFile = NULL;

    }while(0);

    if (NULL != fpRawBmpFile) {
        fclose(fpRawBmpFile);
        fpRawBmpFile = NULL;
    }

    if (NULL != fp200X200BmpFile) {
        fclose(fp200X200BmpFile);
        fp200X200BmpFile = NULL;
    }

    if (NULL != pb200X200Bmp) {
        free(pb200X200Bmp);
        pb200X200Bmp = NULL;
    }

    if (NULL != pb640X480Bmp) {
        free(pb640X480Bmp);
        pb640X480Bmp = NULL;
    }

    return iResult;
}