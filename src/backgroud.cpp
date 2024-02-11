#include "background.h"

void matriceRgb(CRGB background[WIDTH][HEIGHT], int mode)
{
    const CRGB rgb[] = { CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Cyan, CRGB::Blue, CRGB::Magenta };
    static short int hue = 0;
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            mode
                ? background[x][y] = rgb[(hue + x + y) / 16 % 6]
                : background[x][y].setHSV(map(x + y + hue, 0, 4 * (WIDTH + HEIGHT), 0, 255), 255, 255);
        }
    }
    hue += 1;
}

void matrix(CRGB background[WIDTH][HEIGHT])
{
    int alea;
    for (int y = HEIGHT - 1; y > 0; y--)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            background[x][y] = background[x][y - 1];
        }
    }
    for (int x = 0; x < WIDTH; x++)
    {
        background[x][0] = CRGB::Black;
    }
    for (int i = 0; i < (FORMAT ? 1 : 3); i++)
    {
        alea = rand() % WIDTH;
        background[alea][0] = CRGB::Green;
    }
}

void fire(CRGB background[WIDTH][HEIGHT])
{
    static int randFeu[WIDTH];
    static float etatFeu[WIDTH];
    for (int i = 0; i < WIDTH; i++)
    {
        randFeu[i] = rand() % HEIGHT;
        switch (i)
        {
        case 0:
            etatFeu[i] = (etatFeu[i] + randFeu[i] + randFeu[i + 1]) / 3;
            break;

        case WIDTH - 1:
            etatFeu[i] = (etatFeu[i] + randFeu[i - 1] + randFeu[i]) / 3;
            break;

        default:
            etatFeu[i] = (etatFeu[i] + randFeu[i - 1] + randFeu[i] + randFeu[i + 1]) / 4;
            break;
        }
    };

    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (y < etatFeu[x])
            {
                background[x][y] = CRGB::Black;
            }
            else
            {
                background[x][y].setHSV(map(constrain(y - etatFeu[x], 0, HEIGHT - 1), 0, HEIGHT - 1, 0, 60), 255, 255);
            }
        }
    }
}

void epilepsie(CRGB background[WIDTH][HEIGHT])
{
    static int hue = 0;
    const CRGB rgb[] = { CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Cyan, CRGB::Blue, CRGB::Magenta };
    static int randEpi[WIDTH][HEIGHT];
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            background[x][y] = rgb[hue % 6];
        }
    }
    hue++;
}