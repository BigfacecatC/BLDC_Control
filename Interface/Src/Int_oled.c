#include "Int_oledfont.h"
#include "Int_oled.h"

uint8_t OLED_GRAM[144][8];
// 向 OLED 写一个字节数据
void Int_OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
    uint8_t tx_data[2];
    tx_data[0] = (mode == OLED_CMD) ? 0x00 : 0x40;
    tx_data[1] = dat;
    HAL_I2C_Master_Transmit(&hi2c2, OLED_I2C_ADDRESS, tx_data, 2, 100);
}
/**
 * @brief 调整 OLED 显示屏的颜色显示模式
 *
 * 该函数通过接收一个参数 i 来决定 OLED 显示屏是以正常颜色显示还是反色显示。
 * 正常颜色显示模式下，显示屏按照输入信号的原始颜色显示内容。
 * 反色显示模式下，显示屏会将输入信号的颜色进行反转后显示内容。
 *
 * @param i 控制显示模式的参数。0 代表正常颜色显示，1 代表反色显示。
 */
void Int_OLED_ColorTurn(uint8_t i)
{
    if (i == 0)
    {
        Int_OLED_WR_Byte(0xA6, OLED_CMD); // 正常颜色显示
    }
    if (i == 1)
    {
        Int_OLED_WR_Byte(0xA7, OLED_CMD); // 反色显示
    }
}

/**
 * @brief 调整 OLED 显示屏的方向
 *
 * 本函数通过发送不同的命令到 OLED 显示屏，来实现屏幕显示方向的调整。
 * 它支持两种显示方向：正常显示和翻转显示。
 *
 * @param i 显示方向控制参数。0 表示正常显示，1 表示翻转显示。
 */
void Int_OLED_DisplayTurn(uint8_t i)
{
    if (i == 0)
    {
        Int_OLED_WR_Byte(0xC8, OLED_CMD); // 设置显示方向为正常显示
        Int_OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1)
    {
        Int_OLED_WR_Byte(0xC0, OLED_CMD); // 设置显示方向为翻转显示
        Int_OLED_WR_Byte(0xA0, OLED_CMD);
    }
}
/**
 * 开启 OLED 显示
 * 该函数通过发送特定的命令序列来启动 OLED 显示屏的工作
 */
void Int_OLED_DisPlay_On(void)
{
    Int_OLED_WR_Byte(0x8D, OLED_CMD); // 设置 OLED 的显示振荡器频率
    Int_OLED_WR_Byte(0x14, OLED_CMD); // 启用 OLED 的显示振荡器
    Int_OLED_WR_Byte(0xAF, OLED_CMD); // 打开 OLED 显示
}
/**
 * 关闭 OLED 显示
 *
 * 本函数通过发送特定的命令序列来关闭 OLED 显示屏的显示功能
 * 它首先激活命令模式，然后发送关闭显示的命令
 */
void Int_OLED_DisPlay_Off(void)
{
    Int_OLED_WR_Byte(0x8D, OLED_CMD); // 激活 OLED 的命令模式，准备发送控制指令
    Int_OLED_WR_Byte(0x10, OLED_CMD); // 发送命令以关闭 OLED 的充电泵，这是显示关闭过程的一部分
    Int_OLED_WR_Byte(0xAE, OLED_CMD); // 发送关闭显示的命令，执行关闭 OLED 显示的操作
}
/**
 * Int_OLED_Refresh 函数用于刷新 OLED 屏幕显示。
 * 该函数通过向 OLED 发送指令和数据，来更新屏幕上的显示内容。
 */
void Int_OLED_Refresh(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        Int_OLED_WR_Byte(0xb0 + i, OLED_CMD); // 初始化页地址
        Int_OLED_WR_Byte(0x00, OLED_CMD);     // 设置列地址低 4 位
        Int_OLED_WR_Byte(0x10, OLED_CMD);     // 设置列地址高 4 位
        for (n = 0; n < 128; n++)
        {
            Int_OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}
/**
 * 清空 OLED 屏幕的显示内容。
 *
 * 本函数通过将 OLED 的图形 RAM（GRAM）中的所有数据设置为 0 来实现屏幕清空。
 * 由于 OLED 屏幕的显示是基于 GRAM 中的数据，因此清空 GRAM 后，屏幕内容将被清空。
 * 在完成清空操作后，调用 Int_OLED_Refresh 函数来刷新屏幕，使清空效果显示出来。
 */
void Int_OLED_Clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0;
        }
    }
    Int_OLED_Refresh();
}
/**
 * 在 OLED 显示屏上绘制单个像素点
 *
 * @param x 横坐标，表示像素点在 OLED 显示屏上的第 x 列
 * @param y 纵坐标，表示像素点在 OLED 显示屏上的第 y 行
 * @param t 点的类型，1 表示绘制前景色点，0 表示绘制背景色点
 *
 * 此函数通过操作 OLED 的 GRAM（图形随机存取存储器）来实现像素点的绘制
 * 由于 OLED 显示屏的 GRAM 是位地址 able 的，每个字节代表 8 个像素点，因此需要通过计算确定像素点的具体位置
 */
void Int_OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t)
    {
        OLED_GRAM[x][i] |= n;
    }
    else
    {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}
/**
 * @brief 绘制直线
 *
 * 该函数使用 Bresenham 算法在 OLED 屏幕上绘制一条从 (x1, y1) 到 (x2, y2) 的直线。
 *
 * @param x1 起点的 x 坐标
 * @param y1 起点的 y 坐标
 * @param x2 终点的 x 坐标
 * @param y2 终点的 y 坐标
 * @param mode 绘制模式，可以是覆盖模式或叠加模式
 */
void Int_OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1;
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1;
    else if (delta_x == 0)
        incx = 0;
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x;
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
        Int_OLED_DrawPoint(uRow, uCol, mode);
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
/**
 * @brief 绘制圆
 *
 * 该函数使用中点画圆法绘制一个圆。中点画圆法是一种有效的绘制圆的算法，它利用了圆的八对称性，
 * 通过计算圆的一个点，可以得到其他七个点的位置。这种方法减少了计算量，提高了绘图效率。
 *
 * @param x 圆心的 x 坐标
 * @param y 圆心的 y 坐标
 * @param r 圆的半径
 */
void Int_OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r)
    {
        Int_OLED_DrawPoint(x + a, y - b, 1);
        Int_OLED_DrawPoint(x - a, y - b, 1);
        Int_OLED_DrawPoint(x - a, y + b, 1);
        Int_OLED_DrawPoint(x + a, y + b, 1);
        Int_OLED_DrawPoint(x + b, y + a, 1);
        Int_OLED_DrawPoint(x + b, y - a, 1);
        Int_OLED_DrawPoint(x - b, y - a, 1);
        Int_OLED_DrawPoint(x - b, y + a, 1);
        a++;
        num = (a * a + b * b) - r * r;
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}
/**
 * @brief 在 OLED 上显示一个字符
 *
 * 该函数用于在 OLED 屏幕上显示一个字符，可以根据参数的不同调整字符的大小和显示模式。
 *
 * @param x 字符显示的起始 X 坐标
 * @param y 字符显示的起始 Y 坐标
 * @param chr 要显示的字符
 * @param size1 字符的大小，支持 8, 12, 16, 24 点阵
 * @param mode 显示模式，0 为黑色，1 为白色
 */
void Int_OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode)
{
    uint8_t i, m, temp, size2, chr1;
    uint8_t x0 = x, y0 = y;
    if (size1 == 8)
        size2 = 6;
    else
        size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);
    chr1 = chr - ' ';
    for (i = 0; i < size2; i++)
    {
        if (size1 == 8)
            temp = asc2_0806[chr1][i];
        else if (size1 == 12)
            temp = asc2_1206[chr1][i];
        else if (size1 == 16)
            temp = asc2_1608[chr1][i];
        else if (size1 == 24)
            temp = asc2_2412[chr1][i];
        else
            return;
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                Int_OLED_DrawPoint(x, y, mode);
            else
                Int_OLED_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}
/**
 * @brief 在 OLED 上显示字符串
 *
 * 该函数用于在 OLED 显示屏上显示一行字符串。它会根据提供的参数计算字符串的位置和大小，并逐个字符显示。
 *
 * @param x 字符串显示的起始 X 坐标
 * @param y 字符串显示的起始 Y 坐标
 * @param chr 指向要显示的字符串的指针
 * @param size1 字符的大小，决定了字符在屏幕上的宽度
 * @param mode 显示模式，可能决定了字符的颜色或显示效果
 */
void Int_OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1, uint8_t mode)
{
    while ((*chr >= ' ') && (*chr <= '~'))
    {
        Int_OLED_ShowChar(x, y, *chr, size1, mode);
        if (size1 == 8)
            x += 6;
        else
            x += size1 / 2;
        chr++;
    }
}
/**
 * 计算 m 的 n 次幂
 *
 * 此函数使用指数运算来计算一个数的幂次。它主要用于在 OLED 显示控制中进行某些计算，
 * 但其实现与 OLED 显示控制无直接关联，可以用于一般数学幂次计算。
 *
 * @param m 底数，即我们要计算其幂次的数字
 * @param n 指数，表示底数自乘的次数
 * @return 返回 m 的 n 次幂的结果
 *
 * 选择使用循环而非递归来实现，以避免栈溢出风险，并提高执行效率
 */
uint32_t Int_OLED_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}
/**
 * 在 OLED 显示屏上显示数字
 *
 * @param x 数字显示的起始横坐标
 * @param y 数字显示的起始纵坐标
 * @param num 要显示的数字
 * @param len 数字显示的长度，不足部分会显示为 0
 * @param size1 数字显示的字体大小，8 表示高 8点阵，其他值表示高 16 点阵
 * @param mode 显示模式，正常或反色
 *
 * 此函数用于在 OLED 屏幕上以指定的位置、大小和长度显示一个数字
 * 它会根据数字的位数和指定的长度，自动在前面补零
 * 并且可以根据字体大小调整数字间的间距
 */
void Int_OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1, uint8_t mode)
{
    uint8_t t, temp, m = 0;
    // 根据字体大小计算数字间的额外间距
    if (size1 == 8)
        m = 2;
    for (t = 0; t < len; t++)
    {
        // 计算当前位的数字
        temp = (num / Int_OLED_Pow(10, len - t - 1)) % 10;
        if (temp == 0)
        {
            // 如果当前位为 0，显示字符 '0'
            Int_OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
        }
        else
        {
            // 如果当前位不为 0，显示对应的字符
            Int_OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
        }
    }
}
/**
 * @brief 显示中文字符
 *
 * 该函数用于在 OLED 屏幕上显示一个中文字符。它根据给定的参数定位显示位置，
 * 选择字符和大小，并按照指定模式绘制字符点阵。
 *
 * @param x 起始显示的 x 坐标
 * @param y 起始显示的 y 坐标
 * @param num 字符编号，用于索引字库
 * @param size1 字符大小，目前只支持 16x16 点阵
 * @param mode 绘制模式，决定点的显示方式
 */
void Int_OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1, uint8_t mode)
{
    uint8_t m, temp;
    uint8_t x0 = x, y0 = y;
    // 计算一个汉字对应的数据量，确保能够完整显示一个汉字的点阵信息
    uint16_t i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;
    for (i = 0; i < size3; i++)
    {
        // 目前只处理 16 点阵的汉字，其他大小不支持
        if (size1 == 16)
            temp = Hzk1[num][i]; // 获取当前字节的点阵数据
        else
            return;
        for (m = 0; m < 8; m++)
        {
            // 根据点阵数据绘制每个点，点的显示取决于 temp 的最低位和 mode
            if (temp & 0x01)
                Int_OLED_DrawPoint(x, y, mode);
            else
                Int_OLED_DrawPoint(x, y, !mode);
            temp >>= 1; // 右移一位，准备绘制下一个点
            y++;
        }
        x++;
        // 每绘制完一行点阵，重新设置 x 和 y 坐标，以准备绘制下一行
        if ((x - x0) == size1)
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}
/**
 * @brief 滚动显示中文字符
 *
 * 该函数用于在 OLED 显示屏上滚动显示中文字符。通过控制显示的内容、滚动的速度和显示模式来实现动态显示效果。
 *
 * @param num 显示的中文字符数量
 * @param space 字符之间的间隔
 * @param mode 显示模式，决定字符的颜色（例如，黑色或白色）
 */
void Int_OLED_ScrollDisplay(uint8_t num, uint8_t space, uint8_t mode)
{
    uint8_t i, n, t = 0, m = 0, r;
    while (1)
    {
        if (m == 0)
        {
            // 显示一个中文字符到 OLED 显示屏上，位置在(128, 24)，字号为 16 点阵，按照指定的显示模式
            Int_OLED_ShowChinese(128, 24, t, 16, mode);
            t++;
        }
        if (t == num)
        {
            // 进行字符滚动之前的暂停，通过清空显示缓冲区的一部分来实现
            for (r = 0; r < 16 * space; r++)
            {
                for (i = 1; i < 144; i++)
                {
                    for (n = 0; n < 8; n++)
                    {
                        OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
                    }
                }
                // 刷新 OLED 显示内容
                Int_OLED_Refresh();
            }
            t = 0;
        }
        m++;
        if (m == 16)
            m = 0;
        // 实现滚动效果，将显示缓冲区的内容向前移动
        for (i = 1; i < 144; i++)
        {
            for (n = 0; n < 8; n++)
            {
                OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
            }
        }
        // 刷新 OLED 显示内容
        Int_OLED_Refresh();
    }
}
/**
 * 在 OLED 显示屏上显示图片
 *
 * @param x 图片显示的起始 x 坐标
 * @param y 图片显示的起始 y 坐标
 * @param sizex 图片的宽度
 * @param sizey 图片的高度
 * @param BMP 图片的数据数组
 * @param mode 绘制模式，0 为黑色，1 为白色
 *
 * 此函数通过逐点绘制的方式在 OLED 屏幕上显示一张图片
 * 它根据提供的图片数据数组和指定的显示区域，逐行逐列地解析图片数据
 * 并根据图片数据中的每一位来确定是否在屏幕上绘制点
 */
void Int_OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode)
{
    // 初始化变量 j 为 0，用于遍历图片数据数组
    uint16_t j = 0;
    // 定义变量 i,n,temp,m 用于内部循环和数据处理
    uint8_t i, n, temp, m;
    // 保存起始坐标，用于后续的坐标复位
    uint8_t x0 = x, y0 = y;
    // 计算图片的高度，考虑到每个字节代表 8 行像素
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    // 外层循环遍历图片的每一行
    for (n = 0; n < sizey; n++)
    {
        // 中层循环遍历图片的每一列
        for (i = 0; i < sizex; i++)
        {
            // 获取当前列的图片数据
            temp = BMP[j];
            j++;
            // 内层循环处理每个像素点
            for (m = 0; m < 8; m++)
            {
                // 根据图片数据的每一位决定是否绘制点
                if (temp & 0x01)
                    Int_OLED_DrawPoint(x, y, mode);
                else
                    Int_OLED_DrawPoint(x, y, !mode);
                // 移位以处理下一位像素
                temp >>= 1;
                y++;
            }
            x++;
            // 当达到图片宽度时，重置 x 坐标并调整 y 坐标
            if ((x - x0) == sizex)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}
// 初始化 OLED 显示屏
void Int_OLED_Init(void)
{
    // 延迟 800ms，确保 OLED 显示屏有足够的时间启动
    HAL_Delay(800);
    // 发送一系列初始化命令到 OLED 显示屏
    Int_OLED_WR_Byte(0xAE, OLED_CMD); // --turn off oled panel
    Int_OLED_WR_Byte(0x00, OLED_CMD); // ---set low column address
    Int_OLED_WR_Byte(0x10, OLED_CMD); // ---set high column address
    Int_OLED_WR_Byte(0x40, OLED_CMD); // --set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    Int_OLED_WR_Byte(0x81, OLED_CMD); // --set contrast control register
    Int_OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    Int_OLED_WR_Byte(0xA1, OLED_CMD); // --Set SEG/Column Mapping     0xa0 反转 0xa1 正常
    Int_OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0 反转 0xc8 正常
    Int_OLED_WR_Byte(0xA6, OLED_CMD); // --set normal display
    Int_OLED_WR_Byte(0xA8, OLED_CMD); // --set multiplex ratio(1 to 64)
    Int_OLED_WR_Byte(0x3f, OLED_CMD); // --1/64 duty
    Int_OLED_WR_Byte(0xD3, OLED_CMD); // -set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    Int_OLED_WR_Byte(0x00, OLED_CMD); // -not offset
    Int_OLED_WR_Byte(0xd5, OLED_CMD); // --set display clock divide ratio/oscillator frequency
    Int_OLED_WR_Byte(0x80, OLED_CMD); // --set divide ratio, Set Clock as 100 Frames/Sec
    Int_OLED_WR_Byte(0xD9, OLED_CMD); // --set pre-charge period
    Int_OLED_WR_Byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    Int_OLED_WR_Byte(0xDA, OLED_CMD); // --set com pins hardware configuration
    Int_OLED_WR_Byte(0x12, OLED_CMD);
    Int_OLED_WR_Byte(0xDB, OLED_CMD); // --set vcomh
    Int_OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level
    Int_OLED_WR_Byte(0x20, OLED_CMD); // -Set Page Addressing Mode (0x00/0x01/0x02)
    Int_OLED_WR_Byte(0x02, OLED_CMD); //
    Int_OLED_WR_Byte(0x8D, OLED_CMD); // --set Charge Pump enable/disable
    Int_OLED_WR_Byte(0x14, OLED_CMD); // --set(0x10) disable
    Int_OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    Int_OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    // 清除 OLED 显示屏上的所有内容
    Int_OLED_Clear();
    // 最后发送命令启动 OLED 面板
    Int_OLED_WR_Byte(0xAF, OLED_CMD);
}
