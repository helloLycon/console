#include <stdio.h>
#include "console.h"


int main(int argc, char**argv){
    int i;
    console.init("./log", "/tmp/console");
    for(i=0;;i++){
        printf("write level-file(for exam., echo 7 > /tmp/console) then enter:");
        fgetc(stdin);
        /* print on terminal */
        console.print(LOG_EMERG  "emergency message  (%d)\r\n", 0);
        console.print(LOG_ALERT  "alert message      (%d)\r\n", 1);
        console.print(LOG_CRIT   "critical message   (%d)\r\n", 2);
        console.print(LOG_ERR    "error message      (%d)\r\n", 3);
        console.print(LOG_WARNING"warning message    (%d)\r\n", 4);
        console.print(LOG_NOTICE "notice message     (%d)\r\n", 5);
        console.print(LOG_INFO   "information message(%d)\r\n", 6);
        console.print(LOG_DEBUG  "debug message      (%d)\r\n", 7);
        console.print(           "message...         (unknown)\r\n");
        /* log into file */
        console.vlog ("VLOG-SEQ = %d\r\n", i);
    }
    return 0;
}



