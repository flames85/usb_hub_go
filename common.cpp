#include "common.h"

int str2int( const string &str, int nScale)
{
    stringstream ss( str);
    int num;
    switch( nScale)
    {
        case 16:
            if(( ss >> hex >> num). fail())
            {
                // ERROR
            }
            break;
        case 10:
            if(( ss >> dec >> num). fail())
            {
                //ERROR
            }
            break;
        case 8:
            if(( ss >> oct >> num). fail())
            {
                //ERROR
            }
            break;
        default:
            // ERROR
            break;
    }
    return num;
}