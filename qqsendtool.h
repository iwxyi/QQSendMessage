#ifndef QQSENDTOOL_H
#define QQSENDTOOL_H

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include <QDebug>

class TCHARTool
{
public:
    static void TcharToChar(const TCHAR * tchar, char * _char)
    {
        int iLength;
        //获取字节长度
        iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
        //将tchar值赋给_char
        WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
    }

    static void CharToTchar(const char * _char, TCHAR * tchar)
    {
        int iLength;
        iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
        MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
    }
};

class QQSendTool
{
public:
    /**
     * 获取QQ窗口
     */
    HWND findQQWindowOrStart(TCHAR* Class, TCHAR* winname)
    {
        HWND hwnd = FindWindow(Class, winname);
        if (hwnd == NULL)
        {
            //如果qq没运行就打开
            qDebug() << ("窗口不存在,正在启动应用\n");
            system(qqPath);//你的qq程序的地址
            qDebug() << ("请稍等\n");
        }
        while (1)
        {
            //判断是否已经登陆完成
            HWND hwnd = FindWindow(Class, winname);
            if (hwnd != NULL)
                break;
        }
        Sleep(500);
        return hwnd;
    }

    /**
     * 定位到搜索框
     */
    static DWORD WINAPI locateSearchBar(LPVOID lp)
    {
        int sj = 5000;
        RECT* rr = (RECT*)lp;
        while (sj)
        {
            SetCursorPos(rr->left + 100, rr->top + 130);
            sj--;
        }
        return 0;
    }

    /**
     * 定位到账号输入框
     */
    static DWORD WINAPI locateSearchBar2(LPVOID lp)
    {
        int sj = 3000;
        RECT* rr2 = (RECT*)lp;
        while (sj)
        {
            SetCursorPos(rr2->left + 170, rr2->top + 250);
            sj--;
        }
        return 0;
    }

    /**
     * 强制关闭QQ
     */
    void closeQQ(HWND hwnd)
    {
        SendMessage(hwnd, WM_CLOSE, WM_CLOSE, 0);
        Sleep(50);
        system("taskkill /f /t /im QQ.exe");
        qDebug() << ("已关闭qq窗口");
    }

    /**
     * 开始发送
     * 如果QQ未启动，则进行启动
     */
    void startSend()
    {
        findQQWindowOrStart(Class, winname);

        int u = 1;

        while (1)
        {
            //等待qq窗口加载完毕
            HWND hwnd = FindWindow(Class, winname);
            SetForegroundWindow(hwnd);
            GetWindowRect(hwnd, &rect);
            RECT* r = &rect;

            // 登录窗口
            if ((rect.bottom - rect.top) < 500)
            {
                //输入qq账号
                HANDLE hh2 = CreateThread(NULL, 0, locateSearchBar2, r, NULL, NULL);
                Sleep(500);
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                Sleep(50);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(50);
                for (int i = 0; i < lstrlen(qqh); i++)
                {
                    PostMessage(hwnd, WM_IME_CHAR, qqh[i], 0);
                }
                Sleep(500);

                if (u)
                {
                    SetCursorPos(r->left + 180, r->top + 360);
                    Sleep(50);
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    Sleep(50);
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                    Sleep(500);
                    u = 0;
                }
            }
            // 消息面板
            else if ((rect.bottom - rect.top) >= 500)
            {
                qDebug() << ("已登录");
                ShowWindow(hwnd, SW_RESTORE);//如果qq最小化，将其激活
                Sleep(500);

                // 找到搜索框
                HANDLE hh = CreateThread(NULL, 0, locateSearchBar, r, NULL, NULL);

                // 点一下搜索框以激活
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                Sleep(50);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                // 搜索框输入要发送的用户名
                for (int i = 0; i < lstrlen(name); i++)
                {
                    PostMessage(hwnd, WM_CHAR, name[i], 0);
                }
                Sleep(500); // 延迟以等待搜索结果（好友数量多或电脑很卡的需要延长）
                SendMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0);

                // 发送消息
                HWND hwnd2 = FindWindow(Class, beizhu);
                if (hwnd2 != NULL)
                {
                    int count = 1;
                    while (count--)
                    {
                        for (int i = 0; i < lstrlen(text); i++)
                            SendMessage(hwnd2, WM_IME_CHAR, text[i], 0);
                        Sleep(50); // 等待输入结束
    //                    SendMessage(hwnd2, WM_KEYDOWN, VK_LCONTROL, 0); // Ctrl+Enter才能发送
                        SendMessage(hwnd2, WM_KEYDOWN, VK_RETURN, 0); // 发送
                        qDebug() << ("发送完毕");
                    }
                    // Sleep(2000); closeQQ(hwnd2); // 发送结束关闭
                    break;
                }
            }
        }
    }

public:
    RECT rect = { 0,0,0,0 };
    TCHAR qqh[20] = TEXT("3308218798");//要登录的qq号
    char qqPath[256] = "D:\\Install\\QQ\\Bin\\QQ.exe";
    TCHAR text[256] = TEXT("测试消息");//要发送的内容
    TCHAR name[25] = TEXT("1600631528");//要发送的qq号
    TCHAR beizhu[256] = TEXT("命燃芯乂");//检测结果，即要发送的qq号的备注，没有就是网名
    TCHAR winname[256] = TEXT("QQ");
    TCHAR Class[256] = TEXT("TXGuiFoundation");
    int u = 1;
};

#endif // QQSENDTOOL_H
