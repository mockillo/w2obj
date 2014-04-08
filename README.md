#Wierd to Obj

The purpose of this program is to solve a very specific problem; if you ever
need to convert some wierd, arbitrary bin-format to [wavefront obj](http://en.wikipedia.org/wiki/Wavefront_.obj_file)


The specifications for the bin-format is as follows:

> int, int, double, double, double, int, float (rows*cols times)
> cols, rows, xllcorner, yllcorner, cellsize, NODATA, all_the_height_data