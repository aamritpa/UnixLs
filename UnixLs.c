#include "UnixLs.h"

int main(int argc, char *argv[])
{
    int length = argc;
    DIR *directory;
    if (length == 1)
    {    // nothing is passed work like 'ls'
        // print all files and directory names
        directory = opendir(thisDirectory);
        if (!directory)
        {
            perror(thisDirectory);
            exit(-1);
        }
        struct dirent *file;
        while ((file = readdir(directory)) != NULL)
        {
            if (strncmp(file->d_name, ".", 1) != 0)
            {
                printf("%s\n", file->d_name);
            }
        }
        closedir(directory);
        exit(0);
    }
    else if (length > 1)
    {
        //check if directory is passed or not
        for (int i = 1; i < argc; i++)
        {
            if (argv[i][0] == '-')
            {
                options = options + 1;
            }
            else
            {
                break;  // if the passed input does not contain - then directory has started
            }
        }
        if ((argc - options) > 1)
        {
            total_directory = (argc - options) - 1;
            directory_passed = true;
        }
        checkInformation(length, argv); // now check conditons for i , l , il
    }
    return 0;
}
void print(int len, char *argv[], int path)
{
    int length = len;
    DIR *dir;
    
    if (directory_passed == true)
    {
        struct stat check_file_stat;
        stat(argv[path],&check_file_stat);

        if(S_ISLNK(check_file_stat.st_mode)|| S_ISREG(check_file_stat.st_mode))
        {    
            printf("%s\n",argv[path]);
            return;
        }
        dir = opendir(argv[path]);
        if (!dir)
        {
            perror(argv[path]);
            return;
        }
        if (total_directory >= 2)
        {
            printf("%s:\n", argv[path]);
        }
        remove_slash(argv[path]); // remove extra slash if passed.
    }
    else
    {
        dir = opendir(thisDirectory);
        if (!dir)
        {
            perror(thisDirectory);
            exit(-1);
        }
    }
    if (i_check == false && l_check == false) // If both i and l is not passed
    {
        struct dirent *file;
        while ((file = readdir(dir)) != NULL)
        {
            if (strncmp(file->d_name, ".", 1) != 0)
            {
                printf("%-10s\n", file->d_name);
            }
        }
        closedir(dir);
        if (total_directory >= 2)
        {
            printf("\n");
        }
        return;
    }

    if (i_check == true && l_check == false) // For case 'i'
    {
        struct dirent *file;
        while ((file = readdir(dir)) != NULL)
        {
            if (strncmp(file->d_name, ".", 1) != 0)
            {
                printf("%-5ld", file->d_ino);
                printf("    ");
                printf("%10s\n", file->d_name);
            }
        }
        closedir(dir);
        if (total_directory >= 2)
        {
            printf("\n");
        }
        return;
    }
    //Rest is For case 'l' and 'il'
    struct dirent *dp;
    struct stat statbuf;
    struct passwd *pwd;
    struct group *grp;
    struct tm *tm;
    char datestring[256];

    char location[4000];

    while ((dp = readdir(dir)) != NULL)
    {
        if (directory_passed == true)
        {
            sprintf(location, "%s/%s", argv[path], dp->d_name);
        }
        else
        {
            sprintf(location, "%s/%s", thisDirectory, dp->d_name);
        }

        if (strncmp(dp->d_name, ".", 1) != 0) // Removing '.' and '..' files
        {
            if (lstat(location, &statbuf) == -1)
                continue;

            if (i_check == true && l_check == true) // for case 'il'
            {
                printf("%-8ld", dp->d_ino);
            }
            printPermissions(statbuf); // Print permissions

            printf(" %2lu ", statbuf.st_nlink);
            if ((pwd = getpwuid(statbuf.st_uid)) != NULL) // print username
                printf("%10s ", pwd->pw_name);
            if ((grp = getgrgid(statbuf.st_gid)) != NULL) // print groupname
                printf("%10s ", grp->gr_name);

            printf("%6jd ", statbuf.st_size); // print size

            tm = localtime(&statbuf.st_mtime);
            strftime(datestring, sizeof(datestring), "%b %d %Y %H:%M", tm);
            printf("%s ", datestring); //print date and time

            if (S_ISLNK(statbuf.st_mode)) // Checking for soft file
            {
                char *linkname;
                ssize_t r;
                linkname = malloc((size_t)(statbuf.st_size + 1));
                r = readlink(location, linkname, statbuf.st_size + 1);
                linkname[statbuf.st_size] = '\0';
                printf("%s -> %s\n", dp->d_name, linkname); // print soft file name
            }
            else
            {
                printf("%s\n", dp->d_name); //print regular file name
            }
        }
    }
    closedir(dir);
    if (total_directory >= 2)
    {
        printf("\n\n");
    }
}
void printPermissions(struct stat statbuf)
{
    if (S_ISREG(statbuf.st_mode))
    {
        printf("-"); // Regular File
    }
    else if (S_ISDIR(statbuf.st_mode))
    {
        printf("d"); //Directory File
    }
    else if (S_ISLNK(statbuf.st_mode))
    {
        printf("l"); // Soft file or symbolic link
    }

    printf((statbuf.st_mode & S_IRUSR) ? "r" : "-"); //	Read Permissions Owner
    printf((statbuf.st_mode & S_IWUSR) ? "w" : "-"); //	Write Permissions Owner
    printf((statbuf.st_mode & S_IXUSR) ? "x" : "-"); // Execute Permissions Owner
    printf((statbuf.st_mode & S_IRGRP) ? "r" : "-"); //	Read Permissions for group
    printf((statbuf.st_mode & S_IWGRP) ? "w" : "-"); //	Write Permissions for group
    printf((statbuf.st_mode & S_IXGRP) ? "x" : "-"); // Execute Permissions for group
    printf((statbuf.st_mode & S_IROTH) ? "r" : "-"); //	Read Permissions for others
    printf((statbuf.st_mode & S_IWOTH) ? "w" : "-"); //	Write Permissions for others
    printf((statbuf.st_mode & S_IXOTH) ? "x" : "-"); // Execute Permissions for others
}
void remove_slash(char *dir) // Extra function for directory passed ended with '/'
{
    if (dir[strlen(dir) - 1] == '/') //if directory passsed has last character as '/' then replace with '\0'
    {
        dir[strlen(dir) - 1] = '\0';
    }
}
void checkInformation(int len, char *argv[])
{

    int length = len; // temporary storing value to use in argv[length]
    if (directory_passed == true)
    {
        //No need to check the last argument passed.
        length = length - total_directory;
    }
    for (int i = 1; i < length; i++)
    {
        char *passed = argv[i];
        for (int j = 0; j < strlen(passed); j++)
        {
            if (passed[0] != '-') // check if argument passed starts with '-'
            {
                printf("Error: The value passed is incorrect\n");
                printf("Error: Occured at : '%s'\n", passed);
                exit(-1);
            }
            if (passed[j] == 'i') // check if 'i' present in any "Input"
            {
                i_check = true;
            }
            if (passed[j] == 'l') // check if 'l' present in any "Input"
            {
                l_check = true;
            }
            if (passed[j] != 'i' && passed[j] != 'l' && j != 0) // if both are not present then error
            {
                printf("UnixLs: Illegal option\n");
                printf("Error: Occured at : '%s'\n", passed);
                exit(-1);
            }
        }
    }
    if (directory_passed == true)
    {
        for (int i = (len - 1); i >= (len - total_directory); i--)
        {
            print(len, argv, i); // call print to do either i or l or il
        }
    }
    else
    {
        print(len, argv, -1);
    }
}
