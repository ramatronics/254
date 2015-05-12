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


char* getfPermissions(char str[], char *str_path);
char* getfOwner(char *str_path);
char* getfGroupName(char *str_path);
char* getfSymbolicLink(char *str_path);
char* getfTimeStamp(char *str_path, char* type);
char* getfTimeM(time_t inTime);
char* getfTimeD(time_t inTime);
char* getfTimeHHMM(time_t inTime);
char* getfTime(time_t inTime, int length, char* format);
time_t resolveTimeStamp(char* ver, struct stat buf);
long long getfSize(char *str_path);

int main(int argc, char *argv[]) {
    DIR *p_dir;
    struct dirent *p_dirent;

    if (argc == 1) {
        printf("Usage: %s <directory name>\n", argv[0]);
        exit(0);
    }

    if (argc == 2) {
        printf("%s\n", "parameters are missing");
        exit(1);
    }

    if ((p_dir = opendir(argv[2])) == NULL) {
        printf("opendir(%s) failed\n", argv[1]);
        exit(1);
    }

    if (chdir(argv[2]) == -1) {
        exit(1);
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        char permission_string[] = "----------\0";
        char *str_path = p_dirent->d_name;

        if (strcmp(str_path, "")) {
            struct stat buf;

            if (lstat(str_path, &buf) < 0) {
                continue;
            }

            char *permission = getfPermissions(permission_string, str_path);
            char *symbolic_link = getfSymbolicLink(str_path);
            long long file_size = getfSize(str_path);
            char *user_name = getfOwner(str_path);
            char *group_name = getfGroupName(str_path);

            printf("%s\t%s\t%s\t%lld", permission, user_name, group_name, file_size);

            char timeM[4];
            char timeD[3];
            char timeHHMM[7];
            struct tm* time_data;
            
            if (strcmp(argv[1], "u") == 0) {
                time_data = localtime(&buf.st_atime);
            } else if (strcmp(argv[1], "c") == 0) {
                time_data = localtime(&buf.st_ctime);
            } else if (strcmp(argv[1], "l") == 0) {
                time_data = localtime(&buf.st_mtime);
            } else {
                time_data = localtime(&buf.st_mtime);
            }

            strftime(timeM, 4, "%b", time_data);
            strftime(timeD, 3, "%e", time_data);
            strftime(timeHHMM, 7, "%H:%M", time_data);

            printf("\t%s\t%s\t%s", timeM, timeD, timeHHMM);
            printf("\t%s", symbolic_link);
            printf("\n");
        }
    }

    return 0;
}

char* getfPermissions(char str[], char *str_path) {

    struct stat buf;

    if (lstat(str_path, &buf) < 0) {
        perror("lstat error 1");
    }

    mode_t mode = buf.st_mode;

    //file type
    switch (buf.st_mode & S_IFMT) {
        case S_IFDIR: str[0] = 'd';
            break;
        case S_IFLNK: str[0] = 'l';
            break;
        case S_IFREG: str[0] = '-';
            break;
        default: str[0] = '?';
            break;
    }

    //permission
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';

    return str;
}

char* getfOwner(char *str_path) {
    struct stat buf;

    if (lstat(str_path, &buf) < 0) {
       // perror("lstat error 2");
    }

    uid_t user_id = buf.st_uid;

    struct passwd *owner = getpwuid(user_id);
    return owner->pw_name;
}

char* getfGroupName(char *str_path) {
    struct stat buf;

    if (lstat(str_path, &buf) < 0) {
     //   perror("lstat error 3");
    }

    gid_t group_id = buf.st_gid;

    struct group *group_name = getgrgid(group_id);
    return group_name->gr_name;
}

char* getfSymbolicLink(char *str_path) {

    char *arrow = "->";
    char buf[1024];
    ssize_t len;

    if ((len = readlink(str_path, buf, sizeof(buf)-1)) != -1){
        buf[len] = '\0';
        strcat(str_path, arrow);
        strcat(str_path, buf);
    }

    return str_path;
}

long long getfSize(char *str_path) {

    struct stat buf;

    if (lstat(str_path, &buf) < 0) {
      //  perror("lstat error 4");
    }

    off_t file_size = buf.st_size;

    return file_size;
}
