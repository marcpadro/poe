//
//  mpf.c
//  m_privoxy
//
//  Created by Marc Padró on 19/08/14.
//
//

#include <stdio.h>
#if !defined(_WIN32) && !defined(__OS2__)
#include <unistd.h>
#endif
#include <stdlib.h>
#include <sys/types.h>
#include "mpf.h"


void mpf_passImageUrl( char* url )
{
#if defined(_WIN32)
    
#elif (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	/* UNIX-style OS. ------------------------------------------- */
    
    pid_t pid = fork();
    int status;
    
    if ( pid == 0 )
    {
        if (!fork()) {
           
           execl(GET_IMAGE_DIR GET_IMAGE_EXEC, GET_IMAGE_EXEC, url, NULL);
           
        }
        else {
           exit(0);
        }
        
        exit(1);
        
    }
    else if ( pid > 0 )
    {
        
        waitpid(pid, &status, 0);
    }
    
#endif
}

void mpf_passTextUrl( char* url )
{
#if defined(_WIN32)
    
#elif (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	/* UNIX-style OS. ------------------------------------------- */
    
    pid_t pid = fork();
    int status;
    
    if ( pid == 0 )
    {
        
        if (!fork()) {
            execl(GET_TEXT_DIR GET_TEXT_EXEC, GET_TEXT_EXEC, url, NULL);
        }
        else {
            exit(0);
        }
        
        exit(1);
       
    }
    else if ( pid > 0 )
    {
        waitpid(pid, &status, 0);
    }
    
#endif
}
