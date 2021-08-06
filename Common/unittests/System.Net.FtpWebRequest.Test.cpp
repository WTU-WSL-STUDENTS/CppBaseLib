#include <System.Net.FtpWebRequest.hpp>

using namespace System::Net;
// #include <ftplib.h>

int main()
{
    FtpWebRequest ftp("10.168.108.12", "445", "dekaifa04", "software#0424");
    printf("%d\n%s", ftp.Connect(), ftp.GetResponse());
    // getchar();   
    return 0;
}