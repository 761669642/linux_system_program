#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void mode_to_letter(int mode, char *str);
void do_ls(char *dirname);
void do_stat(char *filename);
void show_stat_info(char *filename, struct stat *info_p);
char* gid_to_name(gid_t gid);
char* uid_to_name(uid_t uid);

void mode_to_letter(int mode, char *str)
{
	strcpy(str, "----------");

	if (S_ISDIR(mode)) str[0] = 'd';
	if (S_ISCHR(mode)) str[0] = 'c';
	if (S_ISBLK(mode)) str[0] = 'b';

	if (mode & S_IRUSR) str[1] = 'r';
	if (mode & S_IWUSR) str[2] = 'w';
	if (mode & S_IXUSR) str[3] = 'x';

	if (mode & S_IRGRP) str[4] = 'r';
	if (mode & S_IWGRP) str[5] = 'w';
	if (mode & S_IXGRP) str[6] = 'x';

	if (mode & S_IROTH) str[7] = 'r';
	if (mode & S_IWOTH) str[8] = 'w';
	if (mode & S_IXOTH) str[9] = 'x';
}

void do_ls(char *dirname)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls2: cannot open %s\n", dirname);
	else {
		while ((direntp = readdir(dir_ptr)) != NULL)
			do_stat(direntp->d_name);
		closedir(dir_ptr);
	}
}

int main(int argc, char *argv[])
{
	if (argc == 1)
		do_ls(".");
	else {
		while (--argc) {
			printf("%s:\n", *++argv);
			do_ls(*argv);
		}
	}
}

void do_stat(char *filename)
{
	struct stat info;
	if (stat(filename, &info) != -1) {
		show_stat_info(filename, &info);
	} else {
		perror(filename);
	}
}

void show_stat_info(char *filename, struct stat *info_p)
{
	char modestr[11] = {0};
	mode_to_letter(info_p->st_mode, modestr);
	
	printf("%s", modestr);
	printf("%4d", info_p->st_nlink);
	printf("  %-12s", uid_to_name(info_p->st_uid));
	printf("%-12s", gid_to_name(info_p->st_gid));
	printf("%8ld", info_p->st_size);
	printf("  %-16.12s", 4 + ctime(&info_p->st_mtime));
	printf("%s\n", filename);
}

char* gid_to_name(gid_t gid)
{
	struct group *grp_ptr;
	static char numstr[10];
	if ((grp_ptr = getgrgid(gid)) == NULL) {
		sprintf(numstr, "%d", gid);
		return numstr;
	} else {
		return grp_ptr->gr_name;
	}
}

char* uid_to_name(uid_t uid)
{
	struct passwd *pw_ptr;
	static char uid_namestr[10];
	if ((pw_ptr = getpwuid(uid)) == NULL) {
		sprintf(uid_namestr, "%d", uid);
		return uid_namestr;
	} else {
		return pw_ptr->pw_name;
	}
}
