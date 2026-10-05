#include <bits/stdc++.h>
#include <graphics.h>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int a[32][16] = {0};  // 地图数据，0为空，1~5为彩色砖块，8为墙壁

// 函数声明
void drawBlock(int mx, int my, int mode, int a[32][16],
               IMAGE& redb, IMAGE& pinkb, IMAGE& blueb,
               IMAGE& purpleb, IMAGE& greenb, IMAGE& walls);
void saveMap(int a[32][16]);

int main()
{
    system("chcp 65001");
    initgraph(512, 256, EX_SHOWCONSOLE);

    // 加载图片（请确保路径正确，图片尺寸为16x16）
    IMAGE redb, pinkb, blueb, purpleb, greenb, walls;
    loadimage(&redb,   _T("assets/textures/redb.png"));
    loadimage(&pinkb,  _T("assets/textures/pinkb.png"));
    loadimage(&blueb,  _T("assets/textures/blueb.png"));
    loadimage(&purpleb,_T("assets/textures/purpleb.png"));
    loadimage(&greenb, _T("assets/textures/greenb.png"));
    loadimage(&walls,  _T("assets/textures/walls.png"));

    int mode = 1;          // 当前绘制模式
    bool leftDown = false; // 鼠标左键是否按下

    BeginBatchDraw();      // 开启批量绘图

    while (true)
    {
        ExMessage msg;
        while (peekmessage(&msg))  // 非阻塞获取所有消息
        {
            switch (msg.message)
            {
            case WM_KEYDOWN:
                // 数字键切换模式
                if (msg.vkcode == '1') mode = 1;
                else if (msg.vkcode == '2') mode = 2;
                else if (msg.vkcode == '3') mode = 3;
                else if (msg.vkcode == '4') mode = 4;
                else if (msg.vkcode == '5') mode = 5;
                else if (msg.vkcode == '8') mode = 8;
                // Ctrl+S 保存地图
                else if (msg.vkcode == 'S' && msg.ctrl)
                {
                    saveMap(a);
                }
                break;

            case WM_LBUTTONDOWN:
                leftDown = true;
                drawBlock(msg.x, msg.y, mode, a,
                          redb, pinkb, blueb, purpleb, greenb, walls);
                break;

            case WM_LBUTTONUP:
                leftDown = false;
                break;

            case WM_MOUSEMOVE:
                if (leftDown)
                {
                    drawBlock(msg.x, msg.y, mode, a,
                              redb, pinkb, blueb, purpleb, greenb, walls);
                }
                break;
            }
        }

        FlushBatchDraw();  // 刷新屏幕
        Sleep(10);         // 降低CPU占用
    }

    EndBatchDraw();
    closegraph();
    return 0;
}

// 绘制单个方块并更新数组
void drawBlock(int mx, int my, int mode, int a[32][16],
               IMAGE& redb, IMAGE& pinkb, IMAGE& blueb,
               IMAGE& purpleb, IMAGE& greenb, IMAGE& walls)
{
    int col = mx / 16;
    int row = my / 16;

    if (col < 0 || col >= 32 || row < 0 || row >= 16)
        return;

    IMAGE* img = nullptr;
    int val = 0;

    switch (mode)
    {
    case 1: img = &redb;    val = 1; break;
    case 2: img = &pinkb;   val = 2; break;
    case 3: img = &blueb;   val = 3; break;
    case 4: img = &purpleb; val = 4; break;
    case 5: img = &greenb;  val = 5; break;
    case 8: img = &walls;   val = 8; break;
    default: return;
    }

    a[col][row] = val;
    putimage(col * 16, row * 16, img);
}

// 保存地图到文件（UNIX时间戳.cgame）
void saveMap(int a[32][16])
{
    time_t now = time(nullptr);
    string filename = to_string(now) + ".cgame";

    ofstream out(filename);
    if (!out.is_open())
    {
        // 保存失败提示（可在控制台看到）
        printf("保存失败！\n");
        return;
    }

    for (int row = 0; row < 16; ++row)
    {
        for (int col = 0; col < 32; ++col)
        {
            out << a[col][row];
            if (col < 31)
                out << " ";
        }
        out << "\n";
    }

    out.close();
    printf("地图已保存至 %s\n", filename.c_str());
}
