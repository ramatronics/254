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
	DIR *p_dir;
	struct dirent *p_dirent;

	if (argc == 1) {
		printf("Usage: %s <directory name>\n", argv[0]);
		exit(0);
	}

	if ((p_dir = opendir(argv[1])) == NULL) {
		printf("opendir(%s) failed\n", argv[1]);
		exit(1);
	}

	while ((p_dirent = readdir(p_dir)) != NULL) {
		char *str_path = p_dirent->d_name;
		
		//Sample
		char *permission = getfPermissions(p_dirent);

		if (str_path == NULL) {
			printf("Null pointer found!"); 
			exit(2);
		} else {
			printf("%s\n", str_path);
		}
	}

	return 0;
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
