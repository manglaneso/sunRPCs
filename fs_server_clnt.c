/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "fs_server.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

enum clnt_stat 
mi_get_size_1(int fd, int *clnt_res,  CLIENT *clnt)
{
	return (clnt_call(clnt, mi_get_size,
		(xdrproc_t) xdr_int, (caddr_t) &fd,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mi_creat_1(char *name, int modo, int *clnt_res,  CLIENT *clnt)
{
	mi_creat_1_argument arg;
	arg.name = name;
	arg.modo = modo;
	return (clnt_call (clnt, mi_creat, (xdrproc_t) xdr_mi_creat_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mi_open_1(char *name, int flags, int *clnt_res,  CLIENT *clnt)
{
	mi_open_1_argument arg;
	arg.name = name;
	arg.flags = flags;
	return (clnt_call (clnt, mi_open, (xdrproc_t) xdr_mi_open_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mi_close_1(int fd, int *clnt_res,  CLIENT *clnt)
{
	return (clnt_call(clnt, mi_close,
		(xdrproc_t) xdr_int, (caddr_t) &fd,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mi_unlink_1(char *name, int *clnt_res,  CLIENT *clnt)
{
	return (clnt_call(clnt, mi_unlink,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &name,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mi_read_1(int fd, int pos, int size, ReadRes *clnt_res,  CLIENT *clnt)
{
	mi_read_1_argument arg;
	arg.fd = fd;
	arg.pos = pos;
	arg.size = size;
	return (clnt_call (clnt, mi_read, (xdrproc_t) xdr_mi_read_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_ReadRes, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mi_write_1(int fd, RWData data, int pos, int size, int *clnt_res,  CLIENT *clnt)
{
	mi_write_1_argument arg;
	arg.fd = fd;
	arg.data = data;
	arg.pos = pos;
	arg.size = size;
	return (clnt_call (clnt, mi_write, (xdrproc_t) xdr_mi_write_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}