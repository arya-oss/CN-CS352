#include "nfs.h"
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <fcntl.h>

void print_perms(mode_t st, char buf[1024]) {
    char perms[11];
    if(st && S_ISREG(st)) perms[0]='-';
    else if(st && S_ISDIR(st)) perms[0]='d';
    else if(st && S_ISFIFO(st)) perms[0]='|';
    else if(st && S_ISSOCK(st)) perms[0]='s';
    else if(st && S_ISCHR(st)) perms[0]='c';
    else if(st && S_ISBLK(st)) perms[0]='b';
    else perms[0]='l';  // S_ISLNK
    perms[1] = (st & S_IRUSR) ? 'r':'-';
    perms[2] = (st & S_IWUSR) ? 'w':'-';
    perms[3] = (st & S_IXUSR) ? 'x':'-';
    perms[4] = (st & S_IRGRP) ? 'r':'-';
    perms[5] = (st & S_IWGRP) ? 'w':'-';
    perms[6] = (st & S_IXGRP) ? 'x':'-';
    perms[7] = (st & S_IROTH) ? 'r':'-';
    perms[8] = (st & S_IWOTH) ? 'w':'-';
    perms[9] = (st & S_IXOTH) ? 'x':'-';
    perms[10] = '\0';
    // printf("%s", perms);
	sprintf(buf, "%s", perms);
}

bufnode *
nfs_ls_1_svc(bufnode *argp, struct svc_req *rqstp)
{
	static bufnode  result;
	result.size = 0;
	printf("Got a ls req %s\n", argp->buffer);
	DIR * dir; struct dirent * file;
	if(access(argp->buffer, F_OK|R_OK) < 0) {
		sprintf(result.buffer, "File not exist or access denied !");
		result.size = -1;
		return &result;
	}
	struct stat sb;
	stat(argp->buffer, &sb);
	if(sb.st_mode && S_ISDIR(sb.st_mode)) {
		dir = opendir(argp->buffer);
		result.buffer[0] = '\0';
	    while(file=readdir(dir)) {
	        if(file->d_name[0] == '.')
	        	continue;
			strcat(result.buffer, file->d_name);
			strcat(result.buffer, "\n");
			result.size++;
	    }
	} else {
		sprintf(result.buffer, "Path is not a directory !");
		result.size = -1;
	}
	return &result;
}

int *
nfs_write_1_svc(file_args_w *argp, struct svc_req *rqstp)
{
	static int  result;
	printf("Got a write req %s\n", argp->filename);
	if(access(argp->filename, W_OK|F_OK) < 0){
		result = -1;
		return &result;
	}
	struct stat sb;
	stat(argp->filename, &sb);
	if(sb.st_mode && S_ISREG(sb.st_mode)) {
		int fd = open(argp->filename, O_WRONLY);
		if(lseek(fd, argp->offset, SEEK_SET)<0){
			result = -1;
		}
		result = write(fd, argp->buffer, argp->size);
		close(fd);
	} else {
		result = -1;
	}
	return &result;
}

bufnode *
nfs_read_1_svc(file_args_r *argp, struct svc_req *rqstp)
{
	static bufnode  result;
	struct stat sb;
	printf("Got a read request %s %d\n", argp->filename, (int)strlen(argp->filename));
	if(access(argp->filename, F_OK|R_OK) < 0) {
		result.size = -1;
		return &result;
	}
	stat(argp->filename, &sb);
	if(sb.st_mode && S_ISREG(sb.st_mode)) {
		int fd = open(argp->filename, O_RDONLY);
		if(lseek(fd, argp->offset, SEEK_SET)<0){
			result.size = -1;
			return &result;
		}
		result.size = read(fd, result.buffer, argp->size);
		//printf("read %s %d\n", result.buffer, result.size);
		close(fd);
	} else {
		result.size = -1;
	}
	return &result;
}

int *
nfs_cd_1_svc(bufnode *argp, struct svc_req *rqstp)
{
	static int  result;
	printf("Got a cd req %s\n", argp->buffer);
	result = chdir(argp->buffer);
	return &result;
}

bufnode *
getattr_1_svc(bufnode *argp, struct svc_req *rqstp)
{
	static bufnode  result;
	printf("Got a getattr req %s\n", argp->buffer);
	if(access(argp->buffer, F_OK) < 0) {
		result.size = -1;
		sprintf(result.buffer, "file or dir not exist !");
		return &result;
	}
	result.size = 1;
	struct stat sb;
	stat(argp->buffer, &sb);
	print_perms(sb.st_mode, result.buffer);
	char buf[128];
	struct passwd pwent, * pwentp;
    struct group grp, * grpt;
    char datestring[256];
    struct tm time;
	sprintf(buf, " %d", (int)sb.st_nlink);
	strcat(result.buffer, buf);
	if (!getpwuid_r(sb.st_uid, &pwent, buf, sizeof(buf), &pwentp))
		sprintf(buf, " %s", pwent.pw_name);
	else
		printf(buf, " %d", sb.st_uid);
	strcat(result.buffer, buf);
	if (!getgrgid_r (sb.st_gid, &grp, buf, sizeof(buf), &grpt))
		sprintf(buf, " %s", grp.gr_name);
	else
		sprintf(buf, " %d", sb.st_gid);
	strcat(result.buffer, buf);
	sprintf(buf, " %5d", (int)sb.st_size);
	strcat(result.buffer, buf);
	localtime_r(&sb.st_mtime, &time);
	/* Get localized date string. */
	strftime(datestring, sizeof(datestring), "%F %T", &time);
	sprintf(buf, " %s %s\n", datestring, argp->buffer);
	strcat(result.buffer, buf);
	return &result;
}

int *
setattr_1_svc(bufnode *argp, struct svc_req *rqstp)
{
	static int  result;
	printf("Got a setattr req %s\n", argp->buffer);
	if(access(argp->buffer, F_OK) < 0) {
		result = -1;
		return &result;
	}
    mode_t perm = argp->size;
	result = chmod(argp->buffer, perm);
	return &result;
}

int *
nfs_remove_1_svc(bufnode *argp, struct svc_req *rqstp)
{
	static int  result;
	printf("Got a remove req %s\n", argp->buffer);
	result = remove(argp->buffer);
	return &result;
}

int *
nfs_mkdir_1_svc(bufnode *argp, struct svc_req *rqstp)
{
	static int  result;
	printf("Got a mkdir req %s\n", argp->buffer);
    mode_t perm = argp->size;
	result = mkdir(argp->buffer, perm);
	return &result;
}

int *
nfs_touch_1_svc(bufnode *argp, struct svc_req *rqstp)
{
	static int  result;
	printf("Got a touch req %s\n", argp->buffer);
    mode_t perm = argp->size;
	result = creat(argp->buffer, perm);
	return &result;
}
