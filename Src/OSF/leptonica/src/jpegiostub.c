// 
// Copyright (C) 2001 Leptonica.  All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
//   disclaimer in the documentation and/or other materials provided with the distribution.
// 
/*!
 * \file jpegiostub.c
 * <pre>
 *
 *     Stubs for jpegio.c functions
 * </pre>
 */
#include "allheaders.h"
#pragma hdrstop

/* --------------------------------------------*/
#if  !HAVE_LIBJPEG   /* defined in environ.h */
/* --------------------------------------------*/

PIX * pixReadJpeg(const char * filename, l_int32 cmflag, l_int32 reduction,
    l_int32 * pnwarn, l_int32 hint)
{
	return (PIX *)ERROR_PTR("function not present", "pixReadJpeg", NULL);
}

PIX * pixReadStreamJpeg(FILE * fp, l_int32 cmflag, l_int32 reduction,
    l_int32 * pnwarn, l_int32 hint)
{
	return (PIX *)ERROR_PTR("function not present", "pixReadStreamJpeg", NULL);
}

l_ok readHeaderJpeg(const char * filename, l_int32 * pw, l_int32 * ph,
    l_int32 * pspp, l_int32 * pycck, l_int32 * pcmyk)
{
	return ERROR_INT("function not present", "readHeaderJpeg", 1);
}

l_ok freadHeaderJpeg(FILE * fp, l_int32 * pw, l_int32 * ph,
    l_int32 * pspp, l_int32 * pycck, l_int32 * pcmyk)
{
	return ERROR_INT("function not present", "freadHeaderJpeg", 1);
}

l_int32 fgetJpegResolution(FILE * fp, l_int32 * pxres, l_int32 * pyres)
{
	return ERROR_INT("function not present", "fgetJpegResolution", 1);
}

l_int32 fgetJpegComment(FILE * fp, uint8 ** pcomment)
{
	return ERROR_INT("function not present", "fgetJpegComment", 1);
}

l_ok pixWriteJpeg(const char * filename, PIX * pix, l_int32 quality,
    l_int32 progressive)
{
	return ERROR_INT("function not present", "pixWriteJpeg", 1);
}

l_ok pixWriteStreamJpeg(FILE * fp, PIX * pix, l_int32 quality,
    l_int32 progressive)
{
	return ERROR_INT("function not present", "pixWriteStreamJpeg", 1);
}

PIX * pixReadMemJpeg(const uint8 * cdata, size_t size, l_int32 cmflag,
    l_int32 reduction, l_int32 * pnwarn, l_int32 hint)
{
	return (PIX *)ERROR_PTR("function not present", "pixReadMemJpeg", NULL);
}

l_ok readHeaderMemJpeg(const uint8 * cdata, size_t size,
    l_int32 * pw, l_int32 * ph, l_int32 * pspp,
    l_int32 * pycck, l_int32 * pcmyk)
{
	return ERROR_INT("function not present", "readHeaderMemJpeg", 1);
}

l_ok readResolutionMemJpeg(const uint8 * data, size_t size,
    l_int32 * pxres, l_int32 * pyres)
{
	return ERROR_INT("function not present", "readResolutionMemJpeg", 1);
}

l_ok pixWriteMemJpeg(uint8 ** pdata, size_t * psize, PIX * pix,
    l_int32 quality, l_int32 progressive)
{
	return ERROR_INT("function not present", "pixWriteMemJpeg", 1);
}

l_ok pixSetChromaSampling(PIX * pix, l_int32 sampling)
{
	return ERROR_INT("function not present", "pixSetChromaSampling", 1);
}

#endif  /* !HAVE_LIBJPEG */
