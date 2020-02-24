#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUFFERSIZE 30

void print_path_to(ino_t this_inode);
void inum_to_name(ino_t inode_tofind, char *namebuf, int buflen);
ino_t get_inode(char *fname);

int main()
{
	print_path_to(get_inode("."));
	putchar('\n');
	return 0;
}

void print_path_to(ino_t this_inode)
{
	ino_t level_inode;
	char its_name[BUFFERSIZE];
	if (get_inode("..") != this_inode) {
		chdir("..");
		inum_to_name(this_inode, its_name, BUFFERSIZE);
		level_inode = get_inode(".");
		print_path_to(level_inode);
		printf("/%s", its_name);
	}
}

void inum_to_name(ino_t inode_tofind, char *namebuf, int buflen)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	dir_ptr = opendir(".");
	if (dir_ptr == NULL) {
		perror(".");
		exit(1);
	}
	while ((direntp = readdir(dir_ptr)) != NULL) {
		if (direntp->d_ino == inode_tofind) {
			memcpy(namebuf, direntp->d_name, BUFFERSIZE);
			namebuf[BUFFERSIZE-1] = '\0';
			closedir(dir_ptr);
			return;
		}
	}
	fprintf(stderr, "cannot find the inode %d\n", inode_tofind);
	exit(1);
}

ino_t get_inode(char *fname)
{
	struct stat info;
	if (stat(fname, &info) == -1) {
		fprintf(stderr, "cannot stat");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
