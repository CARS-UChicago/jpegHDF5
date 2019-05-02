jpegHDF5
========

# HDF5 compression filter for JPEG compression.  
  * This is a lossy compression filter.  
  * It provides a user-specified "quality factor" to control the trade-off of size versus accuracy.
  * The officially assigned HDF5 filter number for this filter is 32019.

# Author
  * Mark Rivers, University of Chicago (rivers@cars.uchicago.edu)

# Requirements
  * libjpeg   This library is available as a package for most Linux distributions, and source code is available from https://www.ijg.org/.

# Filter parameters
  * When compressing H5Pset_filter must be called with cd_nelmts=4 and cd_values as follows:
    * cd_values[0] = quality factor (1-100)
    * cd_values[1] = numColumns
    * cd_values[2] = numRows
    * cd_values[3] = 0=Mono, 1=RGB
 
# Restrictions
  * Only 8-bit unsigned data arrays are supported.
  * Arrays must be either:
    * 2-D monochromatic [NumColumns, NumRows] 
    * 3-D RGB [3, NumColumns, NumRows]
  * Chunking must be set to the size of one entire image so the filter is called once for each image.
 

