struct bufnode {
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
		bufnode nfs_ls(bufnode)=1;
		int nfs_write(file_args_w)=2;
		bufnode nfs_read(file_args_r)=3;
		int nfs_cd(bufnode)=4;
		bufnode getattr(bufnode)=5;
		int setattr(bufnode)=6;
		int nfs_remove(bufnode)=7;
		int nfs_mkdir(bufnode)=8;
		int nfs_touch(bufnode)=9;
	}=1;
} = 0x2fffffff;
