#ifndef ZSCALE_H
#define ZSCALE_H
/* CDL_ZSCALE -- Sample the image and compute optimal Z1 and Z2 values. */

//#ifdef ANSI_FUNC
#ifdef __cplusplus
extern "C"{
#endif
void
cdl_zscale (unsigned short *im,		/* image data to be sampled		*/
            int nx,
            int ny,			/* image dimensions			*/
            int bitpix,			/* bits per pixel			*/
            float *z1,
            float *z2,			/* output min and max greyscale values	*/
            float contrast,		/* adj. to slope of transfer function	*/
            int opt_size,		/* desired number of pixels in sample	*/
            int len_stdline		/* optimal number of pixels per line	*/
            );
#ifdef __cplusplus
}
#endif
//#else
//
//void
//cdl_zscale (im, nx, ny, bitpix, z1, z2, contrast, opt_size, len_stdline)
//
//unsigned char    *im;		/* image data to be sampled		*/
//int	nx, ny;			/* image dimensions			*/
//int	bitpix;			/* bits per pixel			*/
//float	*z1, *z2;		/* output min and max greyscale values	*/
//float	contrast;		/* adj. to slope of transfer function	*/
//int	opt_size;		/* desired number of pixels in sample	*/
//int	len_stdline;		/* optimal number of pixels per line	*/
//
//#endif
#endif // ZSCALE_H
