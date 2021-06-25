#include "includes.h"

char yesno;

const std::string date_and_time( )
{
        time_t     now = time( 0 );
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime( &now );
        strftime( buf, sizeof( buf ), "%Y-%m-%d | %X", &tstruct );

        return buf;
}

void next_step( )
{
    Sleep( 500 );
    system( "CLS" );
}


std::string get_clipboard( )
{
    if ( !OpenClipboard( 0 ) )
    {
        throw std::runtime_error( "OpenClipboard failed" );
    }

    HANDLE hData = GetClipboardData( CF_TEXT );
    if ( hData == nullptr )
    {
        throw std::runtime_error( "ClipboardData handle failed" );
    }

    char* pszText = reinterpret_cast<char*>( GlobalLock( hData ));
    if ( pszText == nullptr )
    {
        throw std::runtime_error( "GlobalLock failed to read Handle data" );
    }

    std::string content( pszText );

    GlobalUnlock( hData );

    CloseClipboard( );

    return content;
}

int main()
{
    SetConsoleTitleA( "Clipboard History v1.0" );
    std::cout << "Welcome to Adrian's Clipboard history!\n";
    std::cout << "\n";
    std::cout << "Are you sure you want me to log your Clipboard history?\n";
  
    std::cin >> yesno;

    yesno = tolower( yesno );

    if ( yesno == 'y' )
    {
        system( "CLS" );
        std::cout << "Ok, proceeding\n";

        next_step( );
        
        std::cout << "Clipboard Data:\n";   

        static bool running = true;

        static int esc_amt = 0;

        while ( running ) {

            if ( GetAsyncKeyState(VK_ESCAPE))
            {
            /*    Sleep( 50 );*/
                esc_amt += 1;
                std::cout << "To exit, press Escape again.\n";
                if ( esc_amt > 1 )
                {
                    running = false;
                }
            }

            try
            {
                static std::string clipboard_content_old = get_clipboard( );
                std::string clipboard_content = get_clipboard( );                   //datatype doesn't matter if function is called, only at function definition!
                if ( clipboard_content_old != clipboard_content )
                {
                    std::ofstream output( "output.txt", std::ofstream::out | std::ofstream::app );
                    clipboard_content_old = clipboard_content;
                    std::cout << "[";
                    std::cout << date_and_time( );
                    std::cout << "] ";
                    std::cout << clipboard_content << std::endl;
                    clipboard_content += "\n";
                    output.write( clipboard_content.c_str( ), clipboard_content.length( ) );
                    output.close( );
                }
            }
            catch ( const std::exception& i )
            {
                std::cout << i.what( ) << std::endl;
            }

            Sleep( 50 );
        }
    }

    else if ( yesno == 'n' )
    {
        system( "CLS" );
        system( "color 3" );
        std::cout << "OK, exiting.\n";
        Sleep( 500 );
        std::cout << "See you soon!\n";
        Sleep( 1000 );
        system( "exit" );
    }

    return 1;
}