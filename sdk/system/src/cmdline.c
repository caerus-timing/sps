//###########################################################################
//
// FILE:   cmdline.c
//
// TITLE:  Process a command line string into arguments and execute the
//         command.
//
//###########################################################################
// $TI Release: F2806x Support Library v2.03.00.00 $
// $Release Date: Sun Mar 25 13:24:47 CDT 2018 $
// $Copyright:
// Copyright (C) 2009-2018 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $

//###########################################################################

//
//! \addtogroup cmdline_api
//! @{
//

#include "cmdline.h"

//
// Defines the maximum number of arguments that can be parsed.
//
#ifndef CMDLINE_MAX_ARGS
#define CMDLINE_MAX_ARGS        8
#endif



//
//! \return Returns \b CMDLINE_BAD_CMD if the command is not found,
//! \b CMDLINE_TOO_MANY_ARGS if there are more arguments than can be parsed.
//! Otherwise it returns the code that was returned by the command function.
//

int CmdLineProcess(char *pcCmdLine)
{
    static char *argv[CMDLINE_MAX_ARGS + 1];
    char *pcChar;
    int argc;
    int bFindArg = 1;
    tCmdLineEntry *pCmdEntry;

//
// Initialize the argument counter, and point to the beginning of the
// command line string.
//
    argc = 0;
    pcChar = pcCmdLine;

//
// Advance through the command line until a zero character is found.
//
    while (*pcChar)
    {
        //
        // If there is a space, then replace it with a zero, and set the flag
        // to search for the next argument.
        //
        if (*pcChar == ' ')
        {
            *pcChar = 0;
            bFindArg = 1;
        }

        //
        // Otherwise it is not a space, so it must be a character that is part
        // of an argument.
        //
        else
        {
            //
            // If bFindArg is set, then that means we are looking for the start
            // of the next argument.
            //
            if (bFindArg)
            {
                //
                // As long as the maximum number of arguments has not been
                // reached, then save the pointer to the start of this new arg
                // in the argv array, and increment the count of args, argc.
                //
                if (argc < CMDLINE_MAX_ARGS)
                {
                    argv[argc] = pcChar;
                    argc++;
                    bFindArg = 0;
                }

                //
                // The maximum number of arguments has been reached so return
                // the error.
                //
                else
                {
                    return (CMDLINE_TOO_MANY_ARGS);
                }
            }
        }

        //
        // Advance to the next character in the command line.
        //
        pcChar++;
    }

//
// If one or more arguments was found, then process the command.
//
    if (argc)
    {
        //
        // Start at the beginning of the command table, to look for a matching
        // command.
        //
        pCmdEntry = &g_sCmdTable[0];

        //
        // Search through the command table until a null command string is
        // found, which marks the end of the table.
        //
        while (pCmdEntry->pcCmd)
        {
            //
            // If this command entry command string matches argv[0], then call
            // the function for this command, passing the command line
            // arguments.
            //
            if (!strcmp(argv[0], pCmdEntry->pcCmd))
            {
                return (pCmdEntry->pfnCmd(argc, argv));
            }

            //
            // Not found, so advance to the next entry.
            //
            pCmdEntry++;
        }
    }

//
// Fall through to here means that no matching command was found, so return
// an error.
//
    return (CMDLINE_BAD_CMD);
}

//
// Close the Doxygen group.
//! @}
//

//
// End of File
//

