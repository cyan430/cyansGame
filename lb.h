#pragma once
#include <windows.h>
#include <winhttp.h>
#include <bcrypt.h>
#include <conio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include "json.hpp"

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "bcrypt.lib")

using json = nlohmann::json;

// ==================== 硬编码配置 ====================

inline std::string GetToken() {
    return "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";
}

inline std::string GetGistId() {
    return "5d6b388614403d1b7afc25bb6ccd373a";
}

// ==================== 数据结构 ====================

struct ScoreEntry {
    std::string name;
    int score;
    std::string mode;
    std::string time;
};

struct User {
    std::string name;
    std::string pass_hash;
    std::string created;
};

inline std::string g_current_user = "";

inline std::string NowTime() {
    time_t t = time(nullptr);
    tm tm_;
    localtime_s(&tm_, &t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_);
    return buf;
}

// ==================== SHA-256 ====================

inline std::string SHA256(const std::string& input) {
    BCRYPT_ALG_HANDLE hAlg = NULL;
    BCRYPT_HASH_HANDLE hHash = NULL;
    DWORD cbHashObject = 0, cbData = 0, cbHash = 0;
    PBYTE pbHashObject = NULL, pbHash = NULL;
    std::string result;

    if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(
            &hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0))) return "";

    BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH,
        (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);
    BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH,
        (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0);

    pbHashObject = (PBYTE)malloc(cbHashObject);
    pbHash = (PBYTE)malloc(cbHash);

    if (BCRYPT_SUCCESS(BCryptCreateHash(hAlg, &hHash,
            pbHashObject, cbHashObject, NULL, 0, 0))) {
        BCryptHashData(hHash, (PBYTE)input.c_str(), (ULONG)input.size(), 0);
        BCryptFinishHash(hHash, pbHash, cbHash, 0);

        char hex[3];
        for (DWORD i = 0; i < cbHash; i++) {
            sprintf_s(hex, "%02x", pbHash[i]);
            result += hex;
        }
        BCryptDestroyHash(hHash);
    }

    free(pbHashObject);
    free(pbHash);
    BCryptCloseAlgorithmProvider(hAlg, 0);
    return result;
}

// ==================== HTTP 请求 ====================

inline std::string HttpsRequest(
    const std::wstring& method,
    const std::wstring& host,
    const std::wstring& path,
    const std::string& body,
    const std::string& token)
{
    std::string response;
    HINTERNET hSession = WinHttpOpen(L"AllGame/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return "";

    HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(),
        INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); return ""; }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, method.c_str(),
        path.c_str(), NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }

    std::wstring headers = L"Authorization: Bearer " +
        std::wstring(token.begin(), token.end()) +
        L"\r\nAccept: application/vnd.github+json\r\n"
        L"X-GitHub-Api-Version: 2022-11-28\r\n"
        L"Content-Type: application/json\r\n"
        L"User-Agent: AllGame\r\n";

    WinHttpSendRequest(hRequest, headers.c_str(), -1L,
        (LPVOID)body.c_str(), (DWORD)body.size(),
        (DWORD)body.size(), 0);
    WinHttpReceiveResponse(hRequest, NULL);

    DWORD size = 0;
    do {
        WinHttpQueryDataAvailable(hRequest, &size);
        if (size == 0) break;
        std::vector<char> buf(size + 1);
        DWORD downloaded = 0;
        WinHttpReadData(hRequest, buf.data(), size, &downloaded);
        buf[downloaded] = '\0';
        response += buf.data();
    } while (size > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return response;
}

// ==================== Gist 文件读写 ====================

inline std::string ReadGistFile(const std::string& filename) {
    std::string token = GetToken();
    std::string gist_id = GetGistId();
    if (token.empty() || gist_id.empty()) return "";

    std::wstring path = L"/gists/" + std::wstring(gist_id.begin(), gist_id.end());
    std::string resp = HttpsRequest(L"GET", L"api.github.com", path, "", token);
    if (resp.empty()) return "";

    try {
        json outer = json::parse(resp);
        if (!outer.contains("files") || !outer["files"].contains(filename))
            return "";
        return outer["files"][filename]["content"].get<std::string>();
    } catch (...) {
        return "";
    }
}

inline bool WriteGistFile(const std::string& filename, const std::string& content) {
    std::string token = GetToken();
    std::string gist_id = GetGistId();
    if (token.empty() || gist_id.empty()) return false;

    json body;
    body["files"][filename]["content"] = content;

    std::wstring path = L"/gists/" + std::wstring(gist_id.begin(), gist_id.end());
    std::string resp = HttpsRequest(L"PATCH", L"api.github.com",
        path, body.dump(), token);
    return !resp.empty();
}

// ==================== 用户管理 ====================

inline std::vector<User> ReadUsers() {
    std::vector<User> users;
    std::string content = ReadGistFile("users.json");
    if (content.empty()) return users;
    try {
        json arr = json::parse(content);
        for (auto& item : arr) {
            User u;
            u.name = item.value("name", "");
            u.pass_hash = item.value("pass", "");
            u.created = item.value("created", "");
            if (!u.name.empty()) users.push_back(u);
        }
    } catch (...) {}
    return users;
}

inline bool WriteUsers(const std::vector<User>& users) {
    json arr = json::array();
    for (auto& u : users) {
        arr.push_back({
            {"name", u.name},
            {"pass", u.pass_hash},
            {"created", u.created}
        });
    }
    return WriteGistFile("users.json", arr.dump(4));
}

// ==================== 隐藏密码输入 ====================

inline std::string InputPassword() {
    std::string pass;
    char ch;
    while ((ch = (char)_getch()) != '\r') {
        if (ch == '\b') {
            if (!pass.empty()) {
                pass.pop_back();
                std::cout << "\b \b";
            }
        } else if (ch >= 32 && ch < 127) {
            pass += ch;
            std::cout << '*';
        }
    }
    std::cout << '\n';
    return pass;
}

// ==================== 登录 / 注册 ====================

inline bool LoginOrRegister() {
    std::string username, password;

    std::cout << "\n========= 账号登录 =========\n";
    std::cout << "用户名: ";
    std::cin >> username;

    if (username.empty()) {
        std::cout << "用户名不能为空\n";
        return false;
    }
    if (username.size() > 20) {
        std::cout << "用户名过长（最多20字符）\n";
        return false;
    }

    std::cout << "密码: ";
    password = InputPassword();
    if (password.empty()) {
        std::cout << "密码不能为空\n";
        return false;
    }

    std::string hash = SHA256(password);
    if (hash.empty()) {
        std::cout << "密码加密失败\n";
        return false;
    }

    std::cout << "正在连接服务器...\n";
    auto users = ReadUsers();

    for (auto& u : users) {
        if (u.name == username) {
            if (u.pass_hash == hash) {
                g_current_user = username;
                std::cout << "\n[OK] 欢迎回来，" << username << "！\n";
                return true;
            } else {
                std::cout << "\n[X] 密码错误！\n";
                return false;
            }
        }
    }

    std::cout << "\n账号不存在，正在自动注册...\n";
    users.push_back({ username, hash, NowTime() });
    if (WriteUsers(users)) {
        g_current_user = username;
        std::cout << "[OK] 注册成功，欢迎 " << username << "！\n";
        return true;
    } else {
        std::cout << "[X] 注册失败，请检查网络\n";
        return false;
    }
}

// ==================== 排行榜 ====================

inline std::vector<ScoreEntry> ReadLeaderboard() {
    std::vector<ScoreEntry> scores;
    std::string content = ReadGistFile("leaderboard.json");
    if (content.empty()) return scores;
    try {
        json arr = json::parse(content);
        for (auto& item : arr) {
            ScoreEntry e;
            e.name = item.value("name", "匿名");
            e.score = item.value("score", 0);
            e.mode = item.value("mode", "unknown");
            e.time = item.value("time", "");
            scores.push_back(e);
        }
    } catch (...) {}
    return scores;
}

inline bool WriteLeaderboard(const std::vector<ScoreEntry>& scores) {
    json arr = json::array();
    for (auto& s : scores) {
        arr.push_back({
            {"name", s.name},
            {"score", s.score},
            {"mode", s.mode},
            {"time", s.time}
        });
    }
    return WriteGistFile("leaderboard.json", arr.dump(4));
}

// 提交分数（只用于飞机大战）
inline void SubmitScore(int score, const std::string& mode) {
    if (g_current_user.empty()) {
        std::cout << "未登录，无法提交分数\n";
        return;
    }

    std::cout << "正在上传分数...\n";
    auto scores = ReadLeaderboard();
    scores.push_back({ g_current_user, score, mode, NowTime() });

    std::sort(scores.begin(), scores.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });
    if (scores.size() > 50) scores.resize(50);

    if (WriteLeaderboard(scores))
        std::cout << "[OK] 分数已上传\n";
    else
        std::cout << "[X] 上传失败\n";
}

// ==================== 个人最高分（云端） ====================
// 说明：扫雷不参与排行，最高分只针对飞机大战（mode = "pvp"）。
// 数据仍复用云端 leaderboard.json，不新增文件、不写本地 data.txt，
// 彻底杜绝通过修改本地文件刷分。

// 读取某玩家在某模式下的最高分
inline int GetPersonalBest(const std::string& username, const std::string& mode) {
    auto scores = ReadLeaderboard();
    int best = 0;
    for (auto& s : scores) {
        if (s.name == username && s.mode == mode) {
            if (s.score > best) best = s.score;
        }
    }
    return best;
}

// 提交个人最高分：仅当 newScore 高于该玩家该模式的云端最高分时
// 才写入一条新记录；否则直接返回，不修改云端数据。
// 内部按 玩家+模式 去重，只保留每人每模式的最高那条，低分记录会被清理掉。
inline void SubmitPersonalBest(int newScore, const std::string& mode) {
    if (g_current_user.empty()) {
        std::cout << "未登录，无法保存最高分\n";
        return;
    }

    auto scores = ReadLeaderboard();

    int curBest = 0;
    for (auto& s : scores) {
        if (s.name == g_current_user && s.mode == mode) {
            if (s.score > curBest) curBest = s.score;
        }
    }

    if (newScore <= curBest) {
        std::cout << "[OK] 未打破个人最高分(" << curBest << ")，云端记录不变\n";
        return;
    }

    std::cout << "正在上传最高分...\n";

    // 1) 去掉该玩家该模式的所有旧记录，稍后写入唯一最高记录
    std::vector<ScoreEntry> filtered;
    for (auto& s : scores) {
        if (s.name == g_current_user && s.mode == mode) continue;
        filtered.push_back(s);
    }
    filtered.push_back({ g_current_user, newScore, mode, NowTime() });

    // 2) 重新按分数排序，并限制总条数（保护云端文件体积）
    std::sort(filtered.begin(), filtered.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });
    if ((int)filtered.size() > 50) filtered.resize(50);

    if (WriteLeaderboard(filtered)) {
        std::cout << "[OK] 新个人最高分 " << newScore << " 已上传\n";
    } else {
        std::cout << "[X] 上传失败\n";
    }
}

// 显示排行榜
inline void ShowLeaderboard(const std::string& mode) {
    auto scores = ReadLeaderboard();
    std::cout << "\n===== 排行榜 (" << mode << ") =====\n";

    std::sort(scores.begin(), scores.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });

    if (scores.empty()) {
        std::cout << "暂无记录\n";
        return;
    }

    int rank = 1;
    for (auto& s : scores) {
        std::cout << rank++ << ". " << s.name
                  << "  " << s.score
                  << "  " << s.time << "\n";
        if (rank > 20) break;
    }
}