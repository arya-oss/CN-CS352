/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nfs.h"

bool_t
xdr_bufptr (XDR *xdrs, bufptr *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct bufsize), (xdrproc_t) xdr_bufsize))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_bufsize (XDR *xdrs, bufsize *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->buffer, 1024,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_file_args_r (XDR *xdrs, file_args_r *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->filename, 128,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_file_args_w (XDR *xdrs, file_args_w *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->filename, 128,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->buffer, 1024,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	return TRUE;
}
