#include<bits/stdc++.h>
#include<windows.h>

using namespace std;

int ran(int x, int y) {
    static mt19937 gen(static_cast<uint32_t>(//AI
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
                    cout << "🚩";
                }else if(qp[i][j].pt == 0){
                    cout << "🟦";
                }else if(qp[i][j].pt && qp[i][j].nb == 0){
                    cout << " .";
                }else{
                    if(qp[i][j].nb == 1)cout << " \033[38;2;0;255;0m1";
                    if(qp[i][j].nb == 2)cout << " \033[38;2;36;219;0m2";
                    if(qp[i][j].nb == 3)cout << " \033[38;2;73;182;0m3";
                    if(qp[i][j].nb == 4)cout << " \033[38;2;109;146;0m4";
                    if(qp[i][j].nb == 5)cout << " \033[38;2;146;109;0m5";
                    if(qp[i][j].nb == 6)cout << " \033[38;2;182;73;0m6";
                    if(qp[i][j].nb == 7)cout << " \033[38;2;219;36;0m7";
                    if(qp[i][j].nb == 8)cout << " \033[38;2;255;0;0m8";
                    if(qp[i][j].nb == 9)cout << "99";
                    cout << "\033[0m";
                }
            }
            cout << endl;
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
        cout << ">";
        string cmd;
        cin >> cmd;
        if(cmd == "0"){
            exit(0);
        }else if(cmd == "1"){
            sl();
            system("cls");
        }else{
            cout << "\n>";
        }
    
    }
    return 0;
}
