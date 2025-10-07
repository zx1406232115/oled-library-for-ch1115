#ifndef __OLED_FONT_H
#define __OLED_FONT_H

#include <stdint.h>



/*�����ַ��ֽڿ��*/
#define OLED_CHN_CHAR_WIDTH			(2)		//UTF-8�����ʽ��3��GB2312�����ʽ��2

/*��ģ������Ԫ*/
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//��������
	uint8_t Data[60];						//��ģ����
} ChineseCell20x20_t;
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//��������
	uint8_t Data[32];						//��ģ����
} ChineseCell16x16_t;
typedef struct 
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];   // ��������
    uint8_t Data[24];                      // ��ģ���ݣ�12*12�ĺ�����Ҫ24�ֽ�
} ChineseCell12x12_t;

typedef struct 
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];   // ��������
    uint8_t Data[8];                      // ��ģ���ݣ�8*8�ĺ�����Ҫ24�ֽ�
} ChineseCell8x8_t;

/*ASCII��ģ��������*/
extern const uint8_t OLED_F10x20[][30];
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F7x12[][14];
extern const uint8_t OLED_F6x8[][6];

/*������ģ��������*/
extern const ChineseCell20x20_t OLED_CF20x20[];
extern const ChineseCell16x16_t OLED_CF16x16[];
extern const ChineseCell12x12_t OLED_CF12x12[];  // ����12x12��ģ����
extern const ChineseCell8x8_t OLED_CF8x8[];  // ����12x12��ģ����
/*ͼ����������*/
extern const uint8_t Arrow[];
extern const uint8_t UnKnown[];
extern const uint8_t Image_setings[];
extern const uint8_t Image_window[];
extern const uint8_t Image_wechat[];
extern const uint8_t Image_qq[];
extern const uint8_t Image_alipay[];
extern const uint8_t Image_more[];
extern const uint8_t Image_calc[];
extern const uint8_t Image_night[];
extern const uint8_t Image_sleep[];


extern const uint8_t Image_settings_64[];
extern const uint8_t Image_calc_64[];
extern const uint8_t Image_wechat_64[];
extern const uint8_t Image_alipay_64[];
extern const uint8_t Image_night_64[];
extern const uint8_t Image_more_64[];
extern const uint8_t OLED_UI_LOGO[];
extern const uint8_t OLED_UI_LOGOTEXT[];
extern const uint8_t OLED_UI_LOGOTEXT64[];
extern const uint8_t OLED_UI_LOGOGithub[];
extern const uint8_t Image_alipay_QR_Code[];
extern const uint8_t OLED_UI_SettingsLogo[];
/*��������ĸ�ʽ�������λ�ü����µ�ͼ����������*/
//...

#endif

