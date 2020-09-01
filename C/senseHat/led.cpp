#include <bitset>
#include "led.h"
#include "iothub/iothub_op.h"
uint8_t ASCII_MAP [][8] = 
{
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 0x20  
    {0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10}, // 0x21 !
    {0x00, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00}, // 0x22 "
    {0x00, 0x28, 0x28, 0x7c, 0x28, 0x7c, 0x28, 0x28}, // 0x23 #
    {0x00, 0x10, 0x3c, 0x50, 0x38, 0x14, 0x78, 0x10}, // 0x24 $
    {0x00, 0x60, 0x64, 0x08, 0x10, 0x20, 0x4c, 0x0c}, // 0x25 %
    {0x00, 0x30, 0x48, 0x50, 0x20, 0x54, 0x48, 0x34}, // 0x26 &
    {0x00, 0x30, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00}, // 0x27 '
    {0x00, 0x08, 0x10, 0x20, 0x20, 0x20, 0x10, 0x08}, // 0x28 (
    {0x00, 0x20, 0x10, 0x08, 0x08, 0x08, 0x10, 0x20}, // 0x29 )
    {0x00, 0x00, 0x10, 0x54, 0x38, 0x54, 0x10, 0x00}, // 0x2a *
    {0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00}, // 0x2b +
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x20}, // 0x2c ,
    {0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00}, // 0x2d -
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30}, // 0x2e .
    {0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00}, // 0x2f /
    {0x00, 0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38}, // 0x30 0
    {0x00, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38}, // 0x31 1
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c}, // 0x32 2
    {0x00, 0x7c, 0x08, 0x10, 0x08, 0x04, 0x44, 0x38}, // 0x33 3
    {0x00, 0x08, 0x18, 0x28, 0x48, 0x7c, 0x08, 0x08}, // 0x34 4
    {0x00, 0x7c, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38}, // 0x35 5
    {0x00, 0x18, 0x20, 0x40, 0x78, 0x44, 0x44, 0x38}, // 0x36 6
    {0x00, 0x7c, 0x04, 0x08, 0x10, 0x10, 0x10, 0x10}, // 0x37 7
    {0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38}, // 0x38 8
    {0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x08, 0x30}, // 0x39 9
    {0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00}, // 0x3a :
    {0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x10, 0x20}, // 0x3b ;
    {0x00, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08}, // 0x3c <
    {0x00, 0x00, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x00}, // 0x3d =
    {0x00, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20}, // 0x3e >
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x00, 0x10}, // 0x3f ?
    {0x00, 0x38, 0x44, 0x04, 0x34, 0x54, 0x54, 0x38}, // 0x40 @
    {0x00, 0x38, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44}, // 0x41 A
    {0x00, 0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x78}, // 0x42 B
    {0x00, 0x38, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38}, // 0x43 C
    {0x00, 0x70, 0x48, 0x44, 0x44, 0x44, 0x48, 0x70}, // 0x44 D
    {0x00, 0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7c}, // 0x45 E
    {0x00, 0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40}, // 0x46 F
    {0x00, 0x38, 0x44, 0x40, 0x5c, 0x44, 0x44, 0x3c}, // 0x47 G
    {0x00, 0x44, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44}, // 0x48 H
    {0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38}, // 0x49 I
    {0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x48, 0x30}, // 0x4a J
    {0x00, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44}, // 0x4b K
    {0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c}, // 0x4c L
    {0x00, 0x44, 0x6c, 0x54, 0x54, 0x44, 0x44, 0x44}, // 0x4d M
    {0x00, 0x44, 0x44, 0x64, 0x54, 0x4c, 0x44, 0x44}, // 0x4e N
    {0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38}, // 0x4f O
    {0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40}, // 0x50 P
    {0x00, 0x38, 0x44, 0x44, 0x44, 0x54, 0x48, 0x34}, // 0x51 Q
    {0x00, 0x78, 0x44, 0x44, 0x78, 0x50, 0x48, 0x44}, // 0x52 R
    {0x00, 0x3c, 0x40, 0x40, 0x38, 0x04, 0x04, 0x78}, // 0x53 S
    {0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, // 0x54 T
    {0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38}, // 0x55 U
    {0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10}, // 0x56 V
    {0x00, 0x44, 0x44, 0x44, 0x54, 0x54, 0x6c, 0x44}, // 0x57 W
    {0x00, 0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44}, // 0x58 X
    {0x00, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10}, // 0x59 Y
    {0x00, 0x7c, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7c}, // 0x5a Z
    {0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38}, // 0x5b [
    {0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00}, // 0x5c back slash
    {0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38}, // 0x5d ]
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x00, 0x10}, // 0x5e ^
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c}, // 0x5f _
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x00, 0x10}, // 0x60 `
    {0x00, 0x00, 0x00, 0x38, 0x04, 0x3c, 0x44, 0x3c}, // 0x61 a
    {0x00, 0x40, 0x40, 0x58, 0x64, 0x44, 0x44, 0x38}, // 0x62 b
    {0x00, 0x00, 0x00, 0x38, 0x40, 0x40, 0x44, 0x38}, // 0x63 c
    {0x00, 0x04, 0x04, 0x34, 0x4c, 0x44, 0x44, 0x3c}, // 0x64 d
    {0x00, 0x00, 0x00, 0x38, 0x44, 0x7c, 0x40, 0x38}, // 0x65 e
    {0x00, 0x08, 0x14, 0x10, 0x38, 0x10, 0x10, 0x10}, // 0x66 f
    {0x00, 0x00, 0x00, 0x3c, 0x44, 0x3c, 0x04, 0x38}, // 0x67 g
    {0x00, 0x40, 0x40, 0x58, 0x64, 0x44, 0x44, 0x44}, // 0x68 h
    {0x00, 0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x38}, // 0x69 i
    {0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x48, 0x30}, // 0x6a j
    {0x00, 0x20, 0x20, 0x24, 0x28, 0x30, 0x28, 0x24}, // 0x6b k
    {0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38}, // 0x6c l
    {0x00, 0x00, 0x00, 0x68, 0x54, 0x54, 0x54, 0x54}, // 0x6d m
    {0x00, 0x00, 0x00, 0x58, 0x64, 0x44, 0x44, 0x44}, // 0x6e n
    {0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38}, // 0x6f o
    {0x00, 0x00, 0x00, 0x78, 0x44, 0x78, 0x40, 0x40}, // 0x70 p
    {0x00, 0x00, 0x00, 0x3c, 0x44, 0x3c, 0x04, 0x04}, // 0x71 q
    {0x00, 0x00, 0x00, 0x2c, 0x30, 0x20, 0x20, 0x20}, // 0x72 r
    {0x00, 0x00, 0x00, 0x3c, 0x40, 0x38, 0x04, 0x78}, // 0x73 s
    {0x00, 0x00, 0x10, 0x38, 0x10, 0x10, 0x14, 0x08}, // 0x74 t
    {0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x4c, 0x34}, // 0x75 u
    {0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x10}, // 0x76 v
    {0x00, 0x00, 0x00, 0x44, 0x44, 0x54, 0x54, 0x28}, // 0x77 w
    {0x00, 0x00, 0x00, 0x64, 0x18, 0x10, 0x30, 0x4c}, // 0x78 x
    {0x00, 0x00, 0x00, 0x44, 0x24, 0x18, 0x10, 0x60}, // 0x79 y
    {0x00, 0x00, 0x00, 0x7c, 0x08, 0x10, 0x20, 0x7c}, // 0x7a z
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x00, 0x10}, // 0x7b {
    {0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, // 0x7c |
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x00, 0x10}, // 0x7d }
    {0x00, 0x00, 0x00, 0x34, 0x58, 0x00, 0x00, 0x00}, // 0x7e ~
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x00, 0x10}, // 0x7f 
};

#define ASCII_MAP_SIZE = sizeof(ASCII_MAP)/sizeof(*ASCII_MAP);

LED_CONFIG* led_initialize(void)
{
    int ret = 0;
    LED_CONFIG* config;

    printf("LED init start\r\n");

    config = (LED_CONFIG*)malloc(sizeof(LED_CONFIG));

    if (config == NULL)
    {
        printf("ERR : Failed to allocate memory for LED_CONFIG");
        return config;
    }

    (void)memset(config, 0, sizeof(LED_CONFIG));

    config->isInitialized = false;

    if ((config->fb_dev = scan_fb_devices()) == NULL)
    {
        printf("ERR : Failed to find Frame Buffer device\r\n");
        ret = -1;
    }

    if (ret >= 0)
    {
        config->fd = config->fb_dev->fd;
        config->isInitialized = true;
        led_clear(config);
        printf("Initialized LED\r\n");
    }

    printf("LED init finish : ID %s : Sts %d\r\n", config->fb_dev->name, ret);
   
    return config;
}

void led_uninitialize(LED_CONFIG* config)
{
    if (config != NULL)
    {
        if (config->fb_dev != NULL)
        {
            if (config->fb_dev->fd > 0)
            {
                close(config->fb_dev->fd);
            }
        }
        free(config);
    }
}

static uint16_t rgbTorgb565(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t red = (r >> 3) & 0x1F;
	uint16_t green = (g >> 2) & 0x3F;
	uint16_t blue = (b >> 3) & 0x1F;
	return (red << 11) + (green << 5) + blue;
}

void stringTo8x8(const char *strText, uint8_t strLen, uint8_t* asciiData)
{
    int i, j, k;

    // loop through each character
    for (i = 0; i < strLen ; i++)
    {
        if (strText[i] >= 0x20 && strText[i] <= 0x7E)
        {
            int index = strText[i] - 0x20;
            memcpy(&asciiData[(i * 8)], &ASCII_MAP[index], 8);
        }
    }    
}

int led_clear(LED_CONFIG* config)
{
    char buf[128];
    memset(buf, 0, 128);
	if(pwrite(config->fd, buf, 128, 0) != 128) {
		printf("ERR : pwrite error\n");
		return errno;
	}
	return 0;
}

int led_set(LED_CONFIG* config, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{    
	uint16_t pixel = rgbTorgb565(r, g, b);

	if(pwrite(config->fd, &pixel, sizeof(pixel), (x + y * 8) * 2) != sizeof(pixel))
    {
        printf("ERR : pwrite failed %d\r\n", errno);
		return errno;
	}
	return 0;
}

int led_putText(LED_CONFIG* config, const char *strText)
{
    int strLen = strlen(strText);
    uint8_t pixelBuffer[8 * 32]; // max 32 characters
    int i, j, k;

    stringTo8x8(strText, strLen, pixelBuffer);

    for (i = 0; i < strLen; i++)
    {
        led_clear(config);
        for (j = 0; j < 8; j++)
        {
            std::bitset<8> pixelMap = std::bitset<8>(pixelBuffer[i * 8 + j]);
            for (k = 0; k < pixelMap.size(); k++)
            {
                if (pixelMap[k] == 1)
                {
                    led_set(config, j, k, 255, 255, 255);   
                }
            }
        }
        ThreadAPI_Sleep(500);
    }


    return 0;
}

int led_fill(LED_CONFIG* config, uint8_t r, uint8_t g, uint8_t b)
{    
	uint16_t pixel = rgbTorgb565(r, g, b);
    int row;
    int col;

    for (row = 0 ; row < 8; row++)
    {
        for (col = 0; col < 8; col++)
        {
            if(pwrite(config->fd, &pixel, sizeof(pixel), (col + row * 8) * 2) != sizeof(pixel))
            {
                printf("ERR : pwrite failed %d\r\n", errno);
                return errno;
            }
        }
    }
	return 0;
}