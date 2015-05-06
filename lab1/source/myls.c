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


char* getfPermissions(char str[]);
char* getfOwner(struct dirent *p_dirent);
char* getfGroupName(struct dirent *p_dirent);
char* getfTimeStampMD(struct dirent *p_dirent);
char* getfTimeStampHM(struct dirent *p_dirent);
char* getfSymbolicLink(char *str_path);


int getfSize(struct dirent *p_dirent);

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
		char permission_string[] = "---------\0";
		char *str_path = p_dirent->d_name;
		
		//Sample
		char *permission = getfPermissions(permission_string);
		char *symbolic_link = getfSymbolicLink(str_path);

		// if (str_path == NULL) {
		// 	printf("Null pointer found!"); 
		// 	exit(2);
		// } else {
		// 	printf("%s\n", str_path);
		// }
	}

	return 0;
}

char* getfPermissions(char str[]){
	
	struct stat buf;

	mode_t mode = buf.st_mode;

	str[0] = (mode & S_IRUSR) ? 'r' : '-';
	str[1] = (mode & S_IWUSR) ? 'w' : '-';
	str[2] = (mode & S_IXUSR) ? 'x' : '-';
	str[3] = (mode & S_IRGRP) ? 'r' : '_';
	str[4] = (mode & S_IWGRP) ? 'w' : '_';
	str[5] = (mode & S_IXGRP) ? 'x' : '_';
	str[6] = (mode & S_IROTH) ? 'r' : '_';
	str[7] = (mode & S_IWOTH) ? 'w' : '_';
	str[8] = (mode & S_IXOTH) ? 'x' : '_';

	return str;
}

char* getfOwner(struct dirent *p_dirent){

}

char* getfGroupName(struct dirent *p_dirent){

}

char* getfTimeStampMD(struct dirent *p_dirent){

}

char* getfTimeStampHM(struct dirent *p_dirent){

}

char* getfSymbolicLink(char *str_path){
	
	char *arrow = "-->";
    char linkname[256];
    ssize_t r;

    r = readlink(str_path, linkname, 256);
   
   	if(linkname < 0) {
   		strcat(str_path, arrow);
   		strcat(str_path, linkname);
   	}
   
    return str_path;
}

int getfSize(struct dirent *p_dirent){
	
}
