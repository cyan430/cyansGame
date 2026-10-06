#include<bits/stdc++.h>
#include<graphics.h>
#include<windows.h>
#pragma comment(lib, "MSIMG32.LIB")

using namespace std;
int slhard = 30;

void pm(int x, int y, IMAGE* img) {
    if (!img) return;

    DWORD* src = GetImageBuffer(img);
    DWORD* dst = GetImageBuffer(NULL);
    int w  = img->getwidth();
    int h  = img->getheight();
    int sw = getwidth();
    int sh = getheight();

    for (int i = 0; i < h; i++) {
        int py = y + i;
        if (py < 0 || py >= sh) continue;
        for (int j = 0; j < w; j++) {
            int px = x + j;
            if (px < 0 || px >= sw) continue;

            DWORD c = src[i * w + j];
            BYTE a = (c >> 24) & 0xFF;
            if (a == 0) continue;

            BYTE r = (c >> 16) & 0xFF;
            BYTE g = (c >> 8)  & 0xFF;
            BYTE b =  c        & 0xFF;

            // 反预乘：把偏暗的 RGB 还原回原始颜色
            if (a < 255) {
                r = (BYTE)min(255, r * 255 / a);
                g = (BYTE)min(255, g * 255 / a);
                b = (BYTE)min(255, b * 255 / a);
            }

            dst[py * sw + px] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }
}
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
    cout << "\n输入难度:A.简单(10%)B.普通(20%)C.中等(30%)D.困难(40%)E.魔鬼(60%)\n>";
    cin >> hardly;
    if(hardly == "A"){
        slhard = 10;
    }else if(hardly == "B"){
        slhard = 20;
    }else if(hardly == "C"){
        slhard = 30;
    }else if(hardly == "D"){
        slhard = 40;
    }else if(hardly == "E"){
        slhard = 60;
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
    initgraph(512, 512, EX_SHOWCONSOLE);
    setbkmode(TRANSPARENT);
    setbkcolor(RGB(255, 255, 255));
    IMAGE blocks;
    IMAGE booms1;
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
    loadimage(&booms1, _T("assets/textures/booms1.png"));
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
                    putimage((i-1) * 32, (j-1) * 32, &booms1); 
                    cout << "\nGAME OVER!" << endl;
                    Sleep(3000);
                    closegraph();
                    return;
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
            cout << "\nYOU ARE WIN" << endl;
            Sleep(5000);
            closegraph();
            return;
        }
    }
    closegraph();
}

void pvp(){
    system("cls");
    cout << "请选择难度A.普通B.困难\n";
    string cmd;
    cin >> cmd;
    int flyn = 200;
    if(cmd == "B")flyn = 100;
    int score = 0;
    initgraph(512, 512, EX_SHOWCONSOLE);
    IMAGE fplane;
    IMAGE nplane;
    IMAGE nplane2;
    IMAGE zidan;
    IMAGE sky;
    loadimage(&fplane,  _T("assets/textures/fplane.png"));
    loadimage(&nplane,  _T("assets/textures/nplane.png"));
    loadimage(&nplane2, _T("assets/textures/nplane2.png"));
    loadimage(&zidan,   _T("assets/textures/zidan.png"));
    loadimage(&sky,     _T("assets/textures/pvp.png"));
    int fly = flyn;
    int zd = 10;
    int fplanex = 248;
    struct wp{
        int x;
        int y;
    };
    struct en{
        int x;
        int y;
        int hp;
        int cd;
    };
    vector<wp> vt(100, {0, 9999});
    vector<en> et(100, {0, 9999, 5, 0});
    bool gameover = 0;
    while(1){
        cout << "\r当前得分:" << score << "          " << flush;
        BeginBatchDraw();
        Sleep(2);
        cleardevice();
        putimage(0, 0, &sky);
        ExMessage msg;
        peekmessage(&msg);
        if(msg.message == WM_CLOSE){
            closegraph();
            exit(0);
        }
        
        fly -= ran(1, 3);
        if(fly <= 0){
            fly = max(30, flyn);
            for(int i = 0;i < 100;i++){
                if(et[i].y == 9999){
                    et[i].y = -30;
                    et[i].x = ran(0, 512 - 30);
                    et[i].hp = 5;
                    et[i].cd = 0;
                    break;
                }
            }
        }
        if(msg.message == WM_MOUSEMOVE){
            if(msg.x <= 0){ 
                fplanex = 0;
            }else if(msg.x >= 512){
                fplanex = 512;
            }else{
                fplanex = msg.x - 16;
            }
        }
        zd--;
        if(zd == 0){
            zd = 10;
            for(int i = 0;i < 100;i++){
                if(vt[i].y == 9999){
                    vt[i].y = 410;
                    vt[i].x = fplanex + fplane.getwidth()/2 - zidan.getwidth()/2;
                    break;
                }
            }
        }
        for(int i = 0;i < 100;i++){
            if(vt[i].y != 9999){
                vt[i].y-=8;
                pm(vt[i].x, vt[i].y, &zidan);
                if(vt[i].y < -40){
                    vt[i].y = 9999;
                }
            }
        }
        for(int i = 0;i < 100;i++){
            if(et[i].y == 9999)continue;
            if(et[i].cd > 0){
                et[i].cd--;
                if(et[i].cd == 0){
                    et[i].y = 9999;
                    continue;
                }
                pm(et[i].x, et[i].y, &nplane2);
            }else{
                et[i].y+=3;
                pm(et[i].x, et[i].y, &nplane);
                if(et[i].y > 512){
                    et[i].y = 9999;
                }
            }
            if(et[i].y > 510){
                gameover = 1;
            }
        }
        for(int i = 0;i < 100;i++){
            if(vt[i].y == 9999)continue;
            for(int j = 0;j < 100;j++){
                if(et[j].y == 9999)continue;
                if(et[j].cd != 0)continue;
                if(vt[i].x + zidan.getwidth() > et[j].x &&
                   vt[i].x < et[j].x + nplane.getwidth() &&
                   vt[i].y + zidan.getheight() > et[j].y &&
                   vt[i].y < et[j].y + nplane.getheight()){
                    et[j].hp--;
                    score+=10;
                    if(et[j].hp <= 0){
                        score+=100;
                        et[j].cd = 30;
                    }
                    if(score % 800 == 0)flyn -= 5;
                    vt[i].y = 9999;
                    break;
                }
            }
        }
        pm(fplanex, 512-70, &fplane);
        if(gameover){
            string filename = "data.txt";
            int line = 0;
            ifstream in(filename);
            if (in) {
                if (!(in >> line)) line = 0;
            } else {
                line = 0;
            }
            in.close();

            cout << "\nGAME OVER! 得分:" << score << endl;
            if (score > line) {
                cout << "NEW BEST!";
                line = score;
            }

            ofstream out(filename);
            out << line;
            out.close();

            EndBatchDraw();
            Sleep(3000);
            closegraph();
            return;
        }
        EndBatchDraw();
    }
    closegraph();
}

int dzka[16][32];

void readl(){
    using namespace std::filesystem;
    path cur = current_path();
    vector<path> files;
    for (const auto& entry : directory_iterator(cur)) {
        if (entry.path().extension() == ".cgame") {
            files.push_back(entry.path());
        }
    }

    if (files.empty()) {
        // 无存档，使用默认布局
        int cell = 16;
        int cols = 512 / cell;
        int rows = 256 / cell;
        for (int c = 0; c < cols; c++) {
            dzka[0][c] = ran(1, 5);
        }
        dzka[3][4] = 8;
        dzka[3][5] = 8;
        dzka[3][6] = 8;
        dzka[3][7] = 8;
        return;
    }

    path selected;
    if (files.size() == 1) {
        selected = files[0];
    } else {
        // 多个存档，让用户选择
        cout << "检测到多个存档，请选择你想要导入的" << endl;
        for (size_t i = 0; i < files.size(); i++) {
            char letter = 'A' + i;
            cout << letter << " " << files[i].filename().string() << "  ";
        }
        cout << endl << ">";

        char choice;
        while (true) {
            cin >> choice;
            if (choice >= 'A' && choice < 'A' + files.size()) {
                selected = files[choice - 'A'];
                break;
            } else if (choice >= 'a' && choice < 'a' + files.size()) {
                selected = files[choice - 'a'];
                break;
            }
            cout << "无效选择，请重新输入: ";
        }
    }

    ifstream file(selected);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << selected << endl;
        return;
    }
    char header[3];
    file.read(header, 3);
    if (file.gcount() == 3 && header[0] == 67 && header[1] == 71 && header[2] == 77) {
    } else {
        file.clear();
        file.seekg(0);
    }

    char ch;
    int row = 0, col = 0;
    while (file.get(ch) && row < 16) {
        if (ch >= '0' && ch <= '9') {
            dzka[row][col] = ch - '0';
            col++;
            if (col == 32) {
                col = 0;
                row++;
            }
        }
    }
    file.close();
}

bool dzkwin(){
    for(int i = 0;i <= 15;i++){
        for(int j = 0;j <= 31;j++){
            if(dzka[i][j] != 0 && dzka[i][j] != 8)return 0;
        }
    }
    return 1;
}



void dzk() {
    readl();
    initgraph(512, 512, EX_SHOWCONSOLE);
    setbkmode(TRANSPARENT);
    IMAGE walls;
    IMAGE blueb;
    IMAGE redb;
    IMAGE pinkb;
    IMAGE purpleb;
    IMAGE balls;
    IMAGE pingtai;
    IMAGE greenb;
    IMAGE x3b;                     // 新增

    loadimage(&redb,  _T("assets/textures/redb.png"));
    loadimage(&pinkb,  _T("assets/textures/pinkb.png"));
    loadimage(&blueb,  _T("assets/textures/blueb.png"));
    loadimage(&purpleb, _T("assets/textures/purpleb.png"));
    loadimage(&greenb, _T("assets/textures/greenb.png"));
    loadimage(&walls,  _T("assets/textures/walls.png"));
    loadimage(&balls,  _T("assets/textures/balls.png"));
    loadimage(&pingtai, _T("assets/textures/pingtai.png"));
    loadimage(&x3b,    _T("assets/textures/3xb.png")); // 新增

    int cell = 16;
    int cols = 512 / cell;
    int rows = 256 / cell;

    int putx = 160;

    // 小球结构体
    struct Ball {
        double x, y, dx, dy;
    };

    vector<Ball> ballList;

    // 初始球
    Ball initBall;
    initBall.x = putx;
    initBall.y = 300;
    double t = 87;
    initBall.dx = cos(t * 3.141 / 40.0) * 6;
    initBall.dy = sin(t * 3.141 / 40.0) * 6;
    ballList.push_back(initBall);

    int bw = balls.getwidth();
    int bh = balls.getheight();
    int pw = 58;
    int ph = 11;
    int py = 445;

    // 新增：x3b 列表与帧计数器
    vector<pair<double, double>> x3bList;
    int frameCount = 0;

    while (1) {
        BeginBatchDraw();
        cleardevice();

        ExMessage msg;

        // 处理所有消息：鼠标移动 + 空格新增小球
        while (peekmessage(&msg)) {
            if (msg.message == WM_MOUSEMOVE) {
                if (msg.x < 39) {
                    putx = 42;
                } else if (msg.x > 475) {
                    putx = 473;
                } else {
                    putx = msg.x;
                }
            } else if (msg.message == WM_KEYDOWN && msg.vkcode == VK_SPACE) {
                if (ballList.size() < 9999) {
                    Ball nb = initBall;
                    nb.x = putx;
                    nb.y = py - bh;
                    ballList.push_back(nb);
                }
            }
        }

        int px = putx - 31;

        // 遍历所有小球
        for (auto it = ballList.begin(); it != ballList.end(); ) {
            Ball &b = *it;

            b.x += b.dx;
            b.y += b.dy;

            if (b.x <= 5 || b.x >= 488) {
                b.dx = -b.dx;
            }
            if (b.y <= 5) {
                b.dy = -b.dy;
            }

            // 收集当前球碰撞到的砖块
            vector<pair<int, int>> hits;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    int val = dzka[r][c];
                    if (val == 0) continue;

                    int gx = c * cell;
                    int gy = r * cell;

                    if (b.x + bw > gx && b.x < gx + cell &&
                        b.y + bh > gy && b.y < gy + cell) {
                        hits.push_back({r, c});
                    }
                }
            }

            // 统一处理碰撞砖块
            if (!hits.empty()) {
                for (auto [r, c] : hits) {
                    int val = dzka[r][c];
                    if (val >= 1 && val <= 5) {
                        dzka[r][c] = val - 1;

                        // 新增：砖块被消灭时，有几率产生 x3b
                        if (dzka[r][c] == 0) {
                            if (rand() % 100 < 20) { // 20% 几率
                                x3bList.push_back({ (double)(c * cell), (double)(r * cell) });
                            }
                        }
                    }
                }

                auto [r0, c0] = hits[0];
                int gx = c0 * cell;
                int gy = r0 * cell;

                double overlap_left   = (b.x + bw) - gx;
                double overlap_right  = (gx + cell) - b.x;
                double overlap_top    = (b.y + bh) - gy;
                double overlap_bottom = (gy + cell) - b.y;

                double min_overlap_x = min(overlap_left, overlap_right);
                double min_overlap_y = min(overlap_top, overlap_bottom);

                if (min_overlap_x < min_overlap_y) {
                    b.dx = -b.dx;
                    if (overlap_left < overlap_right) b.x = gx - bw;
                    else b.x = gx + cell;
                } else {
                    b.dy = -b.dy;
                    if (overlap_top < overlap_bottom) b.y = gy - bh;
                    else b.y = gy + cell;
                }
            }

            // 挡板碰撞
            if (b.x + bw > px && b.x < px + pw &&
                b.y + bh > py && b.y < py + ph) {
                b.y = py - bh;
                b.dy = -b.dy;
            }

            // 出界删除小球，不再 GameOver
            if (b.y > 496) {
                it = ballList.erase(it);
                continue;
            }

            ++it;
        }

        // 新增：更新 x3b（每 3 帧下降 1px）
        frameCount++;
        if (frameCount) {
            for (auto &p : x3bList) {
                p.second += 1.0;
            }
        }

                // 更新 x3b 位置：检测与挡板碰撞 + 出界删除
        int x3bw = x3b.getwidth();
        int x3bh = x3b.getheight();
                // 遍历所有小球
        for (auto it = ballList.begin(); it != ballList.end(); ) {
            Ball &b = *it;

            b.x += b.dx;
            b.y += b.dy;

            if (b.x <= 5 || b.x >= 488) {
                b.dx = -b.dx;
            }
            if (b.y <= 5) {
                b.dy = -b.dy;
            }

            // 防止平飞：保证与水平方向夹角 >= 20°
            {
                double speed = sqrt(b.dx * b.dx + b.dy * b.dy);
                double minDy = speed * sin(20.0 * 3.14159265358979 / 180.0);
                if (fabs(b.dy) < minDy) {
                    double sgn = (b.dy >= 0.0) ? 1.0 : -1.0;
                    b.dy = sgn * minDy;
                    double remain = sqrt(speed * speed - b.dy * b.dy);
                    b.dx = (b.dx >= 0.0 ? 1.0 : -1.0) * remain;
                }
            }

            // 收集当前球碰撞到的砖块
            vector<pair<int, int>> hits;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    int val = dzka[r][c];
                    if (val == 0) continue;

                    int gx = c * cell;
                    int gy = r * cell;

                    if (b.x + bw > gx && b.x < gx + cell &&
                        b.y + bh > gy && b.y < gy + cell) {
                        hits.push_back({r, c});
                    }
                }
            }

            // 统一处理碰撞砖块
            if (!hits.empty()) {
                for (auto [r, c] : hits) {
                    int val = dzka[r][c];
                    if (val >= 1 && val <= 5) {
                        dzka[r][c] = val - 1;

                        // 砖块被消灭时，有几率产生 x3b
                        if (dzka[r][c] == 0) {
                            if (rand() % 100 < 20) {
                                x3bList.push_back({ (double)(c * cell), (double)(r * cell) });
                            }
                        }
                    }
                }

                auto [r0, c0] = hits[0];
                int gx = c0 * cell;
                int gy = r0 * cell;

                double overlap_left   = (b.x + bw) - gx;
                double overlap_right  = (gx + cell) - b.x;
                double overlap_top    = (b.y + bh) - gy;
                double overlap_bottom = (gy + cell) - b.y;

                double min_overlap_x = min(overlap_left, overlap_right);
                double min_overlap_y = min(overlap_top, overlap_bottom);

                if (min_overlap_x < min_overlap_y) {
                    b.dx = -b.dx;
                    if (overlap_left < overlap_right) b.x = gx - bw;
                    else b.x = gx + cell;
                } else {
                    b.dy = -b.dy;
                    if (overlap_top < overlap_bottom) b.y = gy - bh;
                    else b.y = gy + cell;
                }
            }

            // 挡板碰撞
            if (b.x + bw > px && b.x < px + pw &&
                b.y + bh > py && b.y < py + ph) {
                b.y = py - bh;
                b.dy = -b.dy;
            }

            // 出界删除小球
            if (b.y > 496) {
                it = ballList.erase(it);
                continue;
            }

            ++it;
        }
        
        frameCount++;
        if (frameCount % 3 == 0) {
            for (auto &p : x3bList) p.second += 1.0;
        }

        for (auto it = x3bList.begin(); it != x3bList.end(); ) {
            double x3x = it->first;
            double x3y = it->second;

            // 与挡板碰撞：每个小球发射 ±30° 两个新小球
            if (x3x + x3bw > px && x3x < px + pw &&
                x3y + x3bh > py && x3y < py + ph) {

                vector<Ball> newBalls;
                for (auto &b : ballList) {
                    if (ballList.size() + newBalls.size() + 2 > 9999) break;

                    double speed = sqrt(b.dx * b.dx + b.dy * b.dy);
                    double ang   = atan2(b.dy, b.dx);
                    const double rad30 = 3.14159265358979 / 6.0;

                    Ball n1, n2;
                    n1.x = b.x; n1.y = b.y;
                    n1.dx = cos(ang + rad30) * speed;
                    n1.dy = sin(ang + rad30) * speed;

                    n2.x = b.x; n2.y = b.y;
                    n2.dx = cos(ang - rad30) * speed;
                    n2.dy = sin(ang - rad30) * speed;

                    newBalls.push_back(n1);
                    newBalls.push_back(n2);
                }
                for (auto &nb : newBalls) ballList.push_back(nb);

                it = x3bList.erase(it);
                continue;
            }

            // 出界删除
            if (it->second > 496) {
                it = x3bList.erase(it);
            } else {
                ++it;
            }
        }
        // 绘制砖块
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int val = dzka[r][c];
                if (val == 0) continue;

                IMAGE* img = nullptr;
                if (val == 1) img = &redb;
                else if (val == 2) img = &pinkb;
                else if (val == 3) img = &blueb;
                else if (val == 4) img = &purpleb;
                else if (val == 5) img = &greenb;
                else if (val == 8) img = &walls;

                if (img) pm(c * cell, r * cell, img);
            }
        }

        // 绘制所有小球
        for (auto &b : ballList) {
            pm((int)b.x, (int)b.y, &balls);
        }

        // 新增：绘制所有 x3b
        for (auto &p : x3bList) {
            pm((int)p.first, (int)p.second, &x3b);
        }
        bool win = dzkwin();
        bool gameover = ballList.empty();   // 新增：所有小球都出界

        // 绘制挡板
        pm(putx - 15, py, &pingtai);

        EndBatchDraw();
        Sleep(16);

        if (gameover) {                     // 新增：GAME OVER
            settextcolor(RED);
            outtextxy(215, 235, _T("GAME OVER!"));
            Sleep(3000);
            closegraph();
            exit(0);
        } else if (win) {
            settextcolor(GREEN);
            outtextxy(215, 235, _T("YOU ARE WIN!"));
            Sleep(3000);
            closegraph();
            exit(0);
        }
    }

    closegraph();
}

int main(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
    system("chcp 65001 && cls");
    while(1){
        cout << "\n游戏列表\n";
        cout << "0.退出\n";
        cout << "1.扫雷 - 终端\n";
        cout << "2.扫雷 - 图形\n";
        cout << "3.飞机大战\n";
        cout << "4.打砖块(测试版未完成)\n";
        cout << ">";
        string cmd;
        cin >> cmd;
        if(cmd == "0"){
            exit(0);
        }else if(cmd == "1"){
            sl();
            return 0;
        }else if(cmd == "2"){
            picsl();
            return 0;
        }else if(cmd == "3"){
            pvp();
            return 0;
        }else if(cmd == "4"){
            dzk();
        }
    }
    return 0;
}
