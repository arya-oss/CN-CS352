typedef struct bufsize * bufptr;

struct bufsize {
	char buffer[1024];
	unsigned int size;
};

struct file_args_r {
	char filename[128];
	unsigned int offset;
	unsigned int size;
};

struct file_args_w {
	char filename[128];
	unsigned int offset;
	char buffer[1024];
	unsigned int size;
};

struct attr {
	int ret;
	unsigned int mode;
};

program nfs_arya {
	version nfs {
		bufsize nfs_ls(file_args_r)=1;
		int nfs_write(file_args_w)=2;
		bufsize nfs_read(file_args_r)=3;
		int nfs_cd(file_args_r)=4;
		attr getattr(bufsize)=5;
		int setattr(bufsize)=6;
		int remove(bufsize)=7;
		int mkdir(bufsize)=8;
		int touch(bufsize)=9;
	}=1;
} = 0x2fffffff;
