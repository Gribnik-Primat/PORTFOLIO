/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones
*/


#include<Windows.h>
#include<memory>
#include"TriangleDemo.h"


LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow ) // начало
{
    UNREFERENCED_PARAMETER( prevInstance );
    UNREFERENCED_PARAMETER( cmdLine );

    WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof( WNDCLASSEX ) ;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
    wndClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "DX11BookWindowClass";  // инициализация класса окна

    if( !RegisterClassEx( &wndClass ) )//проверка
        return -1;

    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );//размеры окна и их создание

    HWND hwnd = CreateWindowA( "DX11BookWindowClass", "2D Triangle in Direct3D 11", WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
                                NULL, NULL, hInstance, NULL );//создание окна

    if( !hwnd )//проверка
        return -1;

    ShowWindow( hwnd, cmdShow );//показываем окно пользователю

    TriangleDemo demo; // создаем наши треугольнички

    // Demo Initialize
    bool result = demo.Initialize( hInstance, hwnd ); // инициализация

    if( result == false )
        return -1;

    MSG msg = { 0 };

    while( msg.message != WM_QUIT ) // цикл сообщений до сообщения о выходе
    {
        if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

        // Update and Draw
        demo.Update( 0.0f );
        demo.Render( );
    }

    // Demo Shutdown
    demo.Shutdown( );// вырубаем демо

    return static_cast<int>( msg.wParam ); // возвращаем последнее сообщение
}


LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )//обработчик сообщений
{
    PAINTSTRUCT paintStruct;
    HDC hDC;

    switch( message )
    {
        case WM_PAINT:// либо рисуем
            hDC = BeginPaint( hwnd, &paintStruct );
            EndPaint( hwnd, &paintStruct );
            break;

        case WM_DESTROY://либо уничтожаем
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hwnd, message, wParam, lParam );//либо ничего не делаем
    }

    return 0;
}