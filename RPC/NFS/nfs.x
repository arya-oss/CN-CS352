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

program nfs_arya {
	version nfs {
		bufsize nfs_ls(file_args_r)=1;
		int nfs_write(file_args_w)=2;
		bufsize nfs_read(file_args_r)=3;
	}=1;
} = 0x2fffffff;
