make distclean
./configure --prefix=/opt/hdfeos --with-jpeg=/opt/hdfeos --disable-netcdf --disable-fortran --enable-hdf4-xdr
make -j 4
make check
make install

