//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include <vector>

// ������ ������ ����  
typedef struct {
    float x, y, width, height;
    HBITMAP hBitmap;//����� � ������� ������ 
} sprite;

sprite racket;//������� ������

enum class items_ {
    hemlet,
    sword,
    axe
};

struct item_element {
    items_ itemID;
    sprite spr;
};

struct {
    int score, balls;//���������� ��������� ����� � ���������� "������"
    bool action = false;//��������� - �������� (����� ������ ������ ������) ��� ����
} game;

struct {
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
} window;

HBITMAP hBack;// ����� ��� �������� �����������

//c����� ����
struct location_ {
    HBITMAP bitmap;
    std::vector<item_element> items;
};


struct player_ {
    float x, y, width, height;
    HBITMAP bitmap;
    int current_location = 0;
    std::vector<item_element> items;
};

location_ location[3];
player_ player;


void InitGame()
{
    player.bitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    player.width = 50;
    player.height = 50;
    player.x = window.width / 2.;//������� ���������� ����
    player.y = window.height - player.height;//���� ���� ���� ������ - �� ������ �������
    

    location[0].bitmap = (HBITMAP)LoadImageA(NULL, "loc0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    item_element i;
    i.itemID = items_::hemlet;
    i.spr.x = window.width / 2.;
    i.spr.y = window.height - 50;
    i.spr.height = 50;
    i.spr.width = 50;
    i.spr.hBitmap = (HBITMAP)LoadImageA(NULL, "hemlet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[0].items.push_back(i);

    i.itemID = items_::sword;
    i.spr.x = window.width / 2.+50;
    i.spr.y = window.height - 50;
    i.spr.height = 50;
    i.spr.width = 50;
    i.spr.hBitmap = (HBITMAP)LoadImageA(NULL, "sword.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[0].items.push_back(i);

    i.itemID = items_::axe;
    i.spr.x = window.width / 2. - 50;
    i.spr.y = window.height - 50;
    i.spr.height = 50;
    i.spr.width = 50;
    i.spr.hBitmap = (HBITMAP)LoadImageA(NULL, "axe.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[0].items.push_back(i);

    location[1].bitmap = (HBITMAP)LoadImageA(NULL, "loc1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[2].bitmap = (HBITMAP)LoadImageA(NULL, "loc2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void ProcessSound(const char* name)//������������ ���������� � ������� .wav, ���� ������ ������ � ��� �� ����� ��� � ���������
{
    ///PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//���������� name �������� ��� �����. ���� ASYNC ��������� ����������� ���� ���������� � ����������� ���������
}

void ProcessInput()
{
    if (GetAsyncKeyState(VK_LEFT)) player.x -= 20;
    if (GetAsyncKeyState(VK_RIGHT)) player.x += 20;

    if (!game.action && GetAsyncKeyState(VK_SPACE))
    {
        game.action = true;
        ProcessSound("bounce.wav");
    }
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // ������� �������� ������, ����������� � ���������� �����������
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// �������� ����������� bitmap � �������� ������

    if (hOldbm) // ���� �� ���� ������, ���������� ������
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // ���������� ������� �����������

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//��� ������� ������� ����� ����� ��������������� ��� ����������
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ������ ����������� bitmap
        }

        SelectObject(hMemDC, hOldbm);// ��������������� �������� ������
    }

    DeleteDC(hMemDC); // ������� �������� ������
}

void ShowRacketAndBall()
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, location[player.current_location].bitmap);//������ ���

    int cnt = location[player.current_location].items.size();
    for (int i = 0;i < cnt;i++)
    {
        auto cloc = location[player.current_location];
        auto loc_items = cloc.items;
        ShowBitmap(window.context, loc_items[i].spr.x, loc_items[i].spr.y,loc_items[i].spr.width, loc_items[i].spr.height, loc_items[i].spr.hBitmap);// ������� ������
        
    }


    ShowBitmap(window.context, player.x - player.width / 2., player.y, player.width, player.height, player.bitmap);// ������� ������

   
    
}

void pick()
{



    auto size = location[player.current_location].items.size();
    for (int i = 0; i < size; i++)
    {
        auto element = location[player.current_location].items[i];
        int left = element.spr.x;
        int right = element.spr.width + left;
        if (player.x > left and player.x < right) {
            player.items.push_back(element);
            location[player.current_location].items.erase(location[player.current_location].items.begin() + i);
            return;
        }
    }
}

void LimitPlayer()
{
    //player.x = max(player.x, player.width / 2.);//���� ��������� ������ ���� ������� ������ ����, �������� �� ����
    //player.x = min(player.x, window.width - player.width / 2.);//���������� ��� ������� ����

    if (player.x < 0) {
        player.current_location = player.current_location - 1;
        player.x = window.width;
        if (player.current_location < 0) {
            player.current_location = 2;
        }
    }
    if (player.x > window.width) 
    {
        player.current_location = player.current_location + 1;
        player.x = 50;
        if (player.current_location > 2)
        {
            player.current_location = 0;
        }
            
    }

    if (GetAsyncKeyState('P')) 
    {
    
        pick();
    }
   

}









void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//�� ������ ���� ������� ����� ��������� ���������� ��� ���������
    window.width = r.right - r.left;//���������� ������� � ���������
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//������ �����
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//����������� ���� � ���������
    GetClientRect(window.hWnd, &r);

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    
    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����
    InitGame();//����� �������������� ���������� ����

    //mciSendString(TEXT("play ..\\Debug\\music.mp3 repeat"), NULL, 0, NULL);
    ShowCursor(NULL);
    
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        ShowRacketAndBall();//������ ���, ������� � �����
       
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
        Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)

        ProcessInput();//����� ����������
        LimitPlayer();//���������, ����� ������� �� ������� �� �����
        
    }

}
