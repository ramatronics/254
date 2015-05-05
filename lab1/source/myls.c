#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>          // for printf().  man 3 printf
#include <stdlib.h>         // for exit().    man 3 exit
#include <pwd.h>            // for getpwuid()
#include <grp.h>            // for getgrgid()
#include <time.h>           // for ctime()
#include <string.h>         // for strlen()

char* getfPermissions(struct dirent p_dirent);
char* getfOwner(struct dirent p_dirent);
char* getfGroupName(struct dirent p_dirent);
char* getfTimeStampMD(struct dirent p_dirent);
char* getfTimeStampHM(struct dirent p_dirent);
char* getfSymbolicLink(struct dirent p_dirent);

int getfSize(struct dirent p_dirent);

int main(int argc, char *argv[])
{
	printf("hello");
}

char* getfPermissions(struct dirent p_dirent){

}

char* getfOwner(struct dirent p_dirent){

}

char* getfGroupName(struct dirent p_dirent){

}

char* getfTimeStampMD(struct dirent p_dirent){

}

char* getfTimeStampHM(struct dirent p_dirent){

}

char* getfSymbolicLink(struct dirent p_dirent){

}

int getfSize(struct dirent p_dirent){

}
