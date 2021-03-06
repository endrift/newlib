#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <stdio.h>
#include <errno.h>

#include <sys/iosupport.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN(_close_r,(ptr,fileDesc),
		   struct _reent *ptr _AND
           int fileDesc) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _DEFUN(_close,(fileDesc),
           int fileDesc) {
//---------------------------------------------------------------------------------
	struct _reent *ptr = _REENT;
#endif
	int ret = -1;
	unsigned int dev = 0;
	int fd = -1;

	if(fileDesc!=-1) {

		__handle *handle = __get_handle(fileDesc);

		if ( handle != NULL) {
			dev = handle->device;
			handle->refcount--;
			if (handle->refcount == 0 ) {
				fd = (int)handle->fileStruct;

				if(devoptab_list[dev]->close_r)
					ret = devoptab_list[dev]->close_r(ptr,fd);
				else
					ret = 0;

				__release_handle(fileDesc);
			} else {
				ret = 0;
			}

		}
	}
	return ret;
}
