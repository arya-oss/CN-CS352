/**
 * Server side code for services
 * Author: Rajmani Arya
 */

#include "nfs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

extern int errno;

bufsize *
nfs_ls_1_svc(file_args_r *argp, struct svc_req *rqstp)
{
	static bufsize result;
	printf("Got a ls request %s %d\n", argp->filename, (int)strlen(argp->filename));
	DIR * dirp; struct dirent * d;
	dirp = opendir(argp->filename);
	if(dirp == (DIR *)NULL) {
		result.size = -1;
		perror("opendir() ");
		return &result;
	}
	int cnt=0;
	result.buffer[0] = '\0';
	while(d = readdir(dirp)) {
		strcat(result.buffer, d->d_name);
		strcat(result.buffer, "\n");
		cnt++;
	}
	printf("Read %d files and dirs\n", cnt);
	closedir(dirp);
	result.size = cnt;
	return &result;
}

int *
nfs_write_1_svc(file_args_w *argp, struct svc_req *rqstp)
{
	static int  result;
	struct stat sb;
	printf("Got a write request %s %d\n", argp->filename, (int)strlen(argp->filename));
	if(access(argp->filename, F_OK|W_OK) < 0) {
		result = 0;
		return &result;
	}
	stat(argp->filename, &sb);
	if(sb.st_mode && S_ISREG(sb.st_mode)) {
		int fd = open(argp->filename, O_WRONLY);
		// printf("%d %s\n", fd, argp->buffer);
		if(lseek(fd, argp->offset, SEEK_SET)<0){
			result = 0;
		}
		result = write(fd, argp->buffer, argp->size);
		close(fd);
	} else {
		result = 0;
	}
	return &result;
}

bufsize *
nfs_read_1_svc(file_args_r *argp, struct svc_req *rqstp)
{
	static bufsize result;
	struct stat sb;
	printf("Got a read request %s %d\n", argp->filename, (int)strlen(argp->filename));
	if(access(argp->filename, F_OK|R_OK) < 0) {
		result.size = 0;
		return &result;
	}
	stat(argp->filename, &sb);
	if(sb.st_mode && S_ISREG(sb.st_mode)) {
		int fd = open(argp->filename, O_RDONLY);
		if(lseek(fd, argp->offset, SEEK_SET)<0){
			result.size = 0;
			return &result;
		}
		result.size = read(fd, result.buffer, argp->size);
		//printf("read %s %d\n", result.buffer, result.size);
		close(fd);
	} else {
		result.size = 0;
	}
	return &result;
}

int *
nfs_cd_1_svc(file_args_r *argp, struct svc_req *rqstp)
{
	static int  result;
	result = chdir(argp->filename);
	return &result;
}
