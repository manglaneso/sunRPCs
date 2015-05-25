const   NFSMAXDATA2 = 8192;

const   READMODE	= 0;
const   WRITEMODE	= 1;
const   RWMODE 	= 3;

typedef opaque RWData<NFSMAXDATA2>;

struct ReadRes {
	   int nread;
        RWData data;
};

program FS_SERVER {
        version FS_SERVER_V1 {

                int mi_get_size(int fd) = 1;

                int mi_creat(string name<>, int modo) = 2;

                int mi_open(string name<>, int flags) = 3;

                int mi_close(int fd) = 4;

                int mi_unlink(string name<>) = 5;

                ReadRes mi_read(int fd, int pos, int size) = 6;

                int mi_write(int fd, RWData data, int pos, int size) = 7;

        } = 1;

} = 100005;
