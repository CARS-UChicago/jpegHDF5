/*
    Mark Rivers
    University of Chicago
    
    Example program for JPEG HDF5 compression

*/

#include <stdio.h>
#include "hdf5.h"
#include "jpeg_h5filter.h"

#define NX 100
#define NY 100
#define NUM_IMAGES 100
#define SIZE NX * NY * NUM_IMAGES
#define SHAPE {NUM_IMAGES, NY, NX}
#define CHUNKSHAPE {1, NY, NX}
#define JPEG_QUALITY 95

int main(){

    static unsigned char data[SIZE];
    static unsigned char data_out[SIZE];
    const hsize_t shape[] = SHAPE;
    const hsize_t chunkshape[] = CHUNKSHAPE;
    char *version, *date;
    int r, i;
    unsigned int cd_values[4];
    int return_code = 1;

    hid_t fid, sid, dset, plist = 0;

    for(i=0; i<SIZE; i++){
        data[i] = i;
    }

    /* Register the filter with the library */
    r = jpeg_register_h5filter();

    if(r<0) goto failed;

    sid = H5Screate_simple(3, shape, NULL);
    if(sid<0) goto failed;

    fid = H5Fcreate("example.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if(fid<0) goto failed;

    plist = H5Pcreate(H5P_DATASET_CREATE);
    if(plist<0) goto failed;

    /* Chunked layout required for filters */
    r = H5Pset_chunk(plist, 3, chunkshape);
    if(r<0) goto failed;

    /* JPEG filter requires 4 parameters */
    cd_values[0] = JPEG_QUALITY;  /* JPEG quality factor (1-100) */
    cd_values[1] = NX; /* Number of columns */
    cd_values[2] = NY; /* Number of rows */
    cd_values[3] = 0;  /* Color mode (0=Mono, 1=RGB) */

    /* Set the filter with 4 params */
    r = H5Pset_filter(plist, JPEG_H5FILTER, H5Z_FLAG_OPTIONAL, 4, cd_values);

    if(r<0) goto failed;

#if H5_USE_16_API
    dset = H5Dcreate(fid, "dset", H5T_NATIVE_UINT8, sid, plist);
#else
    dset = H5Dcreate(fid, "dset", H5T_NATIVE_UINT8, sid, H5P_DEFAULT, plist, H5P_DEFAULT);
#endif
    if(dset<0) goto failed;

    r = H5Dwrite(dset, H5T_NATIVE_UINT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data);
    if(r<0) goto failed;

    r = H5Dread(dset, H5T_NATIVE_UINT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data_out);
    if(r<0) goto failed;

    for(i=0;i<SIZE;i++){
        if(data[i] != data_out[i]) goto failed;
    }

    fprintf(stdout, "Success!\n");

    return_code = 0;

    failed:

    if(dset>0)  H5Dclose(dset);
    if(sid>0)   H5Sclose(sid);
    if(plist>0) H5Pclose(plist);
    if(fid>0)   H5Fclose(fid);

    return return_code;
}