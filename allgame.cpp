#include<bits/stdc++.h>
#include<graphics.h>
#include<windows.h>

using namespace std;
int slhard = 30;

int ran(int x, int y) {//AI
    static mt19937 gen(static_cast<uint32_t>(
        chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count()
    ));
    uniform_int_distribution<int> dist(x, y);
    return dist(gen);
}

struct block{
    bool boom = 0;
    int nb = 0;
    bool flag = 0;
    bool pt = 0;
}qp[18][18];

block slre;

void slopen(int x, int y){
    if(x < 1 || x > 16 || y < 1 || y > 16) return;
    if(qp[x][y].flag) return;
    qp[x][y].pt = 1;
    if(qp[x][y].nb == 0){
        if(qp[x][y].boom) return;
        qp[x][y].pt = 1;
        if(qp[x][y].nb == 0){
            if(qp[x-1][y].pt == 0)slopen(x-1, y);
            if(qp[x][y-1].pt == 0)slopen(x, y-1);
            if(qp[x+1][y].pt == 0)slopen(x+1, y);
            if(qp[x][y+1].pt == 0)slopen(x, y+1);
            if(qp[x-1][y-1].pt == 0)slopen(x-1, y-1);
            if(qp[x+1][y-1].pt == 0)slopen(x+1, y-1);
            if(qp[x+1][y+1].pt == 0)slopen(x+1, y+1);
            if(qp[x-1][y+1].pt == 0)slopen(x-1, y+1);
        }
    }
}

void sl(){
    for(int i = 1;i <= 16;i++){
        for(int j = 1;j <= 16;j++){
            qp[i][j] = slre;
        }
    }
    for(int i = 1;i <= 16;i++){
        for(int j = 1;j <= 16;j++){
            int rannum = ran(1, 10);
            if(rannum <= 3){
                qp[i][j].boom = 1;
            }
        }
    }
    for(int i = 1;i <= 16;i++){
        for(int j = 1;j <= 16;j++){
            int sum = 0;
            for(int o = i-1;o <= i+1;o++){
                for(int k = j-1;k <= j+1;k++){
                    if(qp[o][k].boom)sum++;
                }
            }
            qp[i][j].nb = sum;
        }
    }
    int firstopen = 1;
    while(1){
        system("cls");
        for(int i = 1;i <= 16;i++){
            if(i <= 9){
                cout << i;
            }else{
                char c;
                c = i - 10 + 'A';
                cout << c;
            }
            cout << " ";
            for(int j = 1;j <= 16;j++){
                if(qp[i][j].boom && qp[i][j].pt){
                    cout << "\x1b[0m";
                    system("cls");
                    cout << "gameover!";
                    Sleep(2000);
                    return;
                }else if(qp[i][j].flag){
                    cout << "\033[41mp\033[0m ";
                }else if(qp[i][j].pt == 0){
                    cout << "\033[44m  \033[0m ";
                }else if(qp[i][j].pt && qp[i][j].nb == 0){
                    cout << "\033[100m. \033[0m ";
                }else{
                    cout << "\033[100m" << qp[i][j].nb << " " << "\033[0m ";
                }
            }
            cout << endl << endl;
        }
        for(int i = 1;i <= 16;i++){
            for(int j = 1;j <= 16;j++){
                if(qp[i][j].boom == 0 && qp[i][j].pt == 0)goto cincmd;
            }
        }
        cout << "\n\nYOU ARE WIN!";
        Sleep(2000);
        return;
        cincmd:;
        string cmd;
        cin >> cmd;
        if(cmd == "open"){
            int x, y;
            cin >> x >> y;
            if(cin.fail() || x < 1 || x > 16 || y < 1 || y > 16){
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            if(firstopen){
                firstopen = !firstopen;
                for(int i = -1;i <= 1;i++){
                    for(int j = -1;j <= 1;j++){
                        qp[x+i][y+j].boom = 0;
                    }
                }
                for(int i = 1;i <= 16;i++){
                    for(int j = 1;j <= 16;j++){
                        int sum = 0;
                        for(int o = i-1;o <= i+1;o++){
                            for(int k = j-1;k <= j+1;k++){
                                if(qp[o][k].boom)sum++;
                            }
                        }
                        qp[i][j].nb = sum;
                    }
                }
            }
            slopen(x, y);
        }else if(cmd == "flag"){
            int x, y;
            cin >> x >> y;
            if(cin.fail() || x < 1 || x > 16 || y < 1 || y > 16){
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            qp[x][y].flag = !qp[x][y].flag;
        }else{
            int x, y;
            cin >> x >> y;
            int bsum = qp[x][y].nb;
            int fsum = 0;
            if(fsum == bsum){
                for(int i = -1;i <= 1;i++){
                    for(int j = -1;j <= 1;j++){
                        slopen(x+i, y+j);
                    }
                }
            }else{
                cout << "\n>";
            }
        }

    }
}

bool slwin(){
    for(int i = 1;i <= 16;i++){
        for(int j = 1;j <= 16;j++){
            if(qp[i][j].pt == 0 && qp[i][j].boom == 0){
                return 0;
            }
        }
    }
    return 1;
}

void picsl(){
    system("cls");
    string hardly;
    cinhardly:
    cout << "\n输入难度:A.简单(10%)B.普通(20%)C.中等(30%)D.困难(40%)\n>";
    cin >> hardly;
    if(hardly == "A"){
        slhard = 10;
    }else if(hardly == "B"){
        slhard = 20;
    }else if(hardly == "C"){
        slhard = 30;
    }else if(hardly == "D"){
        slhard = 40;
    }else{
        goto cinhardly;
    }
    for(int i = 1;i <= 16;i++){
        for(int j = 1;j <= 16;j++){
            qp[i][j] = slre;
        }
    }
    for(int i = 1;i <= 16;i++){
        for(int j = 1;j <= 16;j++){
            int rannum = ran(1, 100);
            if(rannum <= slhard){
                qp[i][j].boom = 1;
            }
        }
    }
    for(int i = 1;i <= 16;i++){
        for(int j = 1;j <= 16;j++){
            int sum = 0;
            for(int o = i-1;o <= i+1;o++){
                for(int k = j-1;k <= j+1;k++){
                    if(qp[o][k].boom)sum++;
                }
            }
            qp[i][j].nb = sum;
        }
    }
    initgraph(512, 512);
    setbkmode(TRANSPARENT);
    setbkcolor(RGB(255, 255, 255));
    IMAGE blocks;
    IMAGE booms;
    IMAGE flags;
    IMAGE num0;
    IMAGE num1;
    IMAGE num2;
    IMAGE num3;
    IMAGE num4;
    IMAGE num5;
    IMAGE num6;
    IMAGE num7;
    IMAGE num8;
    loadimage(&booms, _T("assets/textures/booms.png"));
    loadimage(&blocks, _T("assets/textures/blocks.png"));
    loadimage(&flags, _T("assets/textures/flag.png"));
    loadimage(&num1, _T("assets/textures/num1.png"));
    loadimage(&num2, _T("assets/textures/num2.png"));
    loadimage(&num3, _T("assets/textures/num3.png"));
    loadimage(&num4, _T("assets/textures/num4.png"));
    loadimage(&num5, _T("assets/textures/num5.png"));
    loadimage(&num6, _T("assets/textures/num6.png"));
    loadimage(&num7, _T("assets/textures/num7.png"));
    loadimage(&num8, _T("assets/textures/num8.png"));
    loadimage(&num0, _T("assets/textures/num0.png"));
    bool first = 1;
    while(1){
        for(int i = 1;i <= 16;i++){
            for(int j = 1;j <= 16;j++){
                if(qp[i][j].boom && qp[i][j].pt){
                    putimage((i-1) * 32, (j-1) * 32, &booms); 
                    Sleep(3000);
                    setbkcolor(WHITE);
                    cleardevice();
                    settextcolor(RED);
                    settextstyle(60, 0, _T("微软雅黑"));
                    outtextxy(200, 200, _T("GAME OVER"));
                    goto end;
                }else if(qp[i][j].flag){
                    putimage((i-1) * 32, (j-1) * 32, &flags); 
                }else if(qp[i][j].pt){
                    if(qp[i][j].nb == 0)putimage((i-1) * 32, (j-1) * 32, &num0); 
                    if(qp[i][j].nb == 1)putimage((i-1) * 32, (j-1) * 32, &num1); 
                    if(qp[i][j].nb == 2)putimage((i-1) * 32, (j-1) * 32, &num2); 
                    if(qp[i][j].nb == 3)putimage((i-1) * 32, (j-1) * 32, &num3); 
                    if(qp[i][j].nb == 4)putimage((i-1) * 32, (j-1) * 32, &num4); 
                    if(qp[i][j].nb == 5)putimage((i-1) * 32, (j-1) * 32, &num5); 
                    if(qp[i][j].nb == 6)putimage((i-1) * 32, (j-1) * 32, &num6); 
                    if(qp[i][j].nb == 7)putimage((i-1) * 32, (j-1) * 32, &num7); 
                    if(qp[i][j].nb == 8)putimage((i-1) * 32, (j-1) * 32, &num8); 
                }else{
                    putimage((i-1) * 32, (j-1) * 32, &blocks);
                }
            }
        }
        ExMessage msg;
        msg = getmessage();
        if (msg.message == WM_CLOSE) {
            closegraph();
            exit(0);
        }
        int cx = msg.x / 32 + 1;
        int cy = msg.y / 32 + 1;
        if (msg.message == WM_LBUTTONDOWN && msg.ctrl){
            int sum = 0;
            for(int o = -1;o <= 1;o++){
                for(int k = -1;k <= 1;k++){
                    if(qp[cx+o][cy+k].flag)sum++;
                }
            }
            if(sum == qp[cx][cy].nb){
                for(int o = -1;o <= 1;o++){
                    for(int k = -1;k <= 1;k++){
                        slopen(cx+o, cy+k);
                    }
                }
            }
        }else if (msg.message == WM_LBUTTONDOWN) {
            if(first){
                for(int o = -1;o <= 1;o++){
                    for(int k = -1;k <= 1;k++){
                        qp[cx+o][cy+k].boom = 0;
                    }
                }
                for(int i = 1;i <= 16;i++){
                    for(int j = 1;j <= 16;j++){
                        int sum = 0;
                        for(int o = i-1;o <= i+1;o++){
                            for(int k = j-1;k <= j+1;k++){
                                if(qp[o][k].boom) sum++;
                            }
                        }
                        qp[i][j].nb = sum;
                    }
                }
                first = 0;
            }
            slopen(cx, cy);
        }
        else if (msg.message == WM_RBUTTONDOWN) {
            if(qp[cx][cy].pt == 0){qp[cx][cy].flag = !qp[cx][cy].flag;}
        }
        if(slwin()){
            outtextxy(200, 200, _T("YOU ARE WIN"));
        }
    }
    Sleep(3000);
    end:;
    closegraph();
}

int main(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
    system("chcp 65001");
    while(1){
        cout << "游戏列表\n";
        cout << "0.退出\n";
        cout << "1.扫雷 - 终端\n";
        cout << "2.扫雷 - 图形\n";
        cout << ">";
        string cmd;
        cin >> cmd;
        if(cmd == "0"){
            exit(0);
        }else if(cmd == "1"){
            sl();
            system("cls");
        }else if(cmd == "2"){
            picsl();
        }else{
            cout << "\n>";
        }
    
    }
    return 0;
}
