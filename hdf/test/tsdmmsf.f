C
C $Header$
C
C $Log$
C Revision 1.5  1992/07/08 22:05:20  sxu
C Changed dsgmaxm() to dsgrang(), and dssmaxm() to dssrang().
C
c Revision 1.4  1992/07/07  21:51:03  chouck
c Minor error reporting fix
c
c Revision 1.3  1992/07/07  21:04:17  chouck
c Changed 'character' to 'byte' for VMS systems.  Fixed error
c reporting.
c
c Revision 1.2  1992/05/07  16:48:07  dilg
c Put in comment explaining the choice between using "char(-128)" and
c "char(0)"
c
c Revision 1.1  1992/04/27  17:17:46  sxu
c Initial revision
c
c Revision 1.2  1992/03/24  20:42:04  sxu
c Changed output file names
c
c Revision 1.1  1992/02/29  22:53:38  mfolk
c Initial revision
c
C

      program ntcheckF
C
C
C  Program to test writing SDSs with different types of data and
C  scales and max/min values.
C
C  Input file:  none
C  Output files: o0, o1, ... o6
C
C
C  **** VMS users ****
C
C  VMS has a special way of handling the passsing of character
C   strings between C and FORTRAN.  For these tests to work 
C   correctly, you must change the definition of i8 and ti8
C   to be 'byte' not 'character'  You will also need to remove
C   a couple of calls to char().  If you search on the string 
C   VMS you should be able to find all of the necessary changes.
C
   
      
      integer dsgdata, dsadata, dssdims, dssrang, dsgrang, dssnt
      integer dssdisc, dsgdisc

      real*8 f64(10,10), tf64(10,10)
      real*8 f64scale(10), tf64scale(10)
      real*8 f64max, f64min, tf64max, tf64min

      real*4 f32(10,10), tf32(10,10)
      real*4 f32scale(10), tf32scale(10)
      real*4 f32max, f32min, tf32max, tf32min

C  Change these to be of type 'byte' for VMS      
C      byte i8(10,10), ti8(10,10)
C      byte i8scale(10), ti8scale(10), i8max, i8min
C      byte ti8max, ti8min   
      character i8(10,10), ti8(10,10)
      character i8scale(10), ti8scale(10), i8max, i8min
      character ti8max, ti8min

      integer*2 i16(10,10), ti16(10,10)
      integer*2 i16scale(10), ti16scale(10), i16max, i16min
      integer*2 ti16max, ti16min

      integer*4 i32(10,10), ti32(10,10)
      integer*4 i32scale(10), ti32scale(10), i32max, i32min
      integer*4 ti32max, ti32min

      integer i, j, err, err1, err2, err3
      integer rank, dims(2)
      integer number_failed
      integer DFNT_FLOAT64, DFNT_FLOAT32, DFNT_INT8, DFNT_INT16
      integer DFNT_INT32

      f64max = 40.0
      f64min = 0.0
      f32max = 40.0
      f32min = 0.0
C Use the following lines for VMS
C      i8min = -128
C      i8max = 127
      i8max = char(127)
C NOTE: If you get a compile error on the "char(-128)" line, substitute
C       the "char(0)" line.  Its not quite as thorough a test, but...
C      i8min = char(0)
      i8min = char(-128)
      i16max = 1200
      i16min = -1200
      i32max = 99999999
      i32min = -999999999
      
      rank = 2
      dims(1) = 10
      dims(2) = 10
      number_failed = 0
      DFNT_FLOAT64 = 6
      DFNT_FLOAT32 = 5
      DFNT_INT8 = 20
      DFNT_INT16 = 22
      DFNT_INT32 = 24
      
      print *, 'Creating arrays...'
      
      do 110 i=1,10
          do 100 j=1,10
            f64(i,j) = (i * 40) + j
            f32(i,j) = (i * 40) + j
C  Use the following line for VMS
C            i8(i,j) =  (i * 10) + j      
            i8(i,j) = char( (i * 10) + j )
            i16(i,j) = (i * 3000) + j
            i32(i,j) = (i * 20) + j
  100     continue
          f64scale(i) = (i * 40) + j
          f32scale(i) = (i * 40) + j
C  Use the following line for VMS
C          i8scale(i) = (i * 10) + j
          i8scale(i) = char((i * 10) + j)
      	  i16scale(i) = (i * 3000) + j
      	  i32scale(i) = (i * 20) + j
  110 continue

      err1 = dssdims(rank, dims)
      
C
C  Writing dimscale, max/min, and arrays to a single file 
C
      print *, 'Writing arrays to single file...'

      err  = dssnt(DFNT_FLOAT64)
      err1 = dssdisc(1, 10, f64scale)
      err2 = dssrang(f64max, f64min)
      err3 = dsadata('of.hdf', rank, dims, f64)
      call errchkio(err1, err2, err3, number_failed, 'float64 write')
      
      err  = dssnt(DFNT_FLOAT32)
      err1 = dssdisc(1, 10, f32scale)
      err2 = dssrang(f32max, f32min)
      err3 = dsadata('of.hdf', rank, dims, f32)
      call errchkio(err1, err2, err3, number_failed, 'float32 write')

      err  = dssnt(DFNT_INT8)
      err1 = dssdisc(1, 10, i8scale)
      err2 = dssrang(i8max, i8min)
      err3 = dsadata('of.hdf', rank, dims, i8)
      call errchkio(err1, err2, err3, number_failed, 'int8 write')
      
      
      err  = dssnt(DFNT_INT16)
      err1 = dssdisc(1, 10, i16scale)
      err2 = dssrang(i16max, i16min)
      err3 = dsadata('of.hdf', rank, dims, i16)
      call errchkio(err1, err2, err3, number_failed, 'int16 write')
      
      err  = dssnt(DFNT_INT32)
      err1 = dssdisc(1, 10, i32scale)
      err2 = dssrang(i32max, i32min)
      err3 = dsadata('of.hdf', rank, dims, i32)
      call errchkio(err1, err2, err3, number_failed, 'int32 write')
      
C
C  Reading back dimscales, max/min, and arrays from single file
C
      err1 = dsgdata('of.hdf', rank, dims, tf64)
      err2 = dsgdisc(1, 10, tf64scale)
      err3 = dsgrang(tf64max, tf64min)
      call errchkio(err1, err2, err3, number_failed, 'float64 read')
      
      err1 = dsgdata('of.hdf', rank, dims, tf32)
      err2 = dsgdisc(1, 10, tf32scale)
      err3 = dsgrang(tf32max, tf32min)
      call errchkio(err1, err2, err3, number_failed, 'float32 read')
      
      err1 = dsgdata('of.hdf', rank, dims, ti8)
      err2 = dsgdisc(1, 10, ti8scale)
      err3 = dsgrang(ti8max, ti8min)
      call errchkio(err1, err2, err3, number_failed, 'int8 read')
      
      err1 = dsgdata('of.hdf', rank, dims, ti16)
      err2 = dsgdisc(1, 10, ti16scale)
      err3 = dsgrang(ti16max, ti16min)
      call errchkio(err1, err2, err3, number_failed, 'int16 read')
      
      err1 = dsgdata('of.hdf', rank, dims, ti32)
      err2 = dsgdisc(1, 10, ti32scale)
      err3 = dsgrang(ti32max, ti32min)
      call errchkio(err1, err2, err3, number_failed, 'int32 read')
      
C
C  Checking dimscales, max/min and arrays from single file
C
      print *, 'Checking dimscales, max/min & arrays from single file'

C  float64
      err1 = 0
      err2 = 0
      err3 = 0
      do 1010 i=1,10
         do 1000 j=1,10
           if (f64(i,j) .ne. tf64(i,j)) err1 = 1
 1000    continue

         if (f64scale(i) .ne. tf64scale(i)) err2 = 1
 1010 continue

      if ((f64max .ne. tf64max) .or. (f64min .ne. tf64min)) err3 = 1
      call errchkarr(err1, err2, err3, number_failed, 'float64')

C  float32
      err1 = 0
      err2 = 0
      err3 = 0
      do 1030 i=1,10
         do 1020 j=1,10
           if (f32(i,j) .ne. tf32(i,j)) err1 = 1
 1020    continue
         if (f32scale(i) .ne. tf32scale(i)) err2 = 1
 1030 continue

      if ((f32max .ne. tf32max) .or. (f32min .ne. tf32min)) err3 = 1
      call errchkarr(err1, err2, err3, number_failed, 'float32')

C  int8
      err1 = 0
      err2 = 0
      err3 = 0
      do 1110 i=1,10
         do 1100 j=1,10
           if (i8(i,j) .ne. ti8(i,j)) err1 = 1
 1100    continue
         if (i8scale(i) .ne. ti8scale(i)) err2 = 1
 1110 continue

      if ((i8max .ne. ti8max) .or. (i8min .ne. ti8min)) err3 = 1
      call errchkarr(err1, err2, err3, number_failed, 'int8')

C  int16
      err1 = 0
      err2 = 0
      err3 = 0
       do 1210 i=1,10
         do 1200 j=1,10
           if (i16(i,j) .ne. ti16(i,j)) err1 = 1
 1200    continue
         if (i16scale(i) .ne. ti16scale(i)) err2 = 1
 1210 continue

      if ((i16max .ne. ti16max) .or. (i16min .ne. ti16min)) err3 = 1
      call errchkarr(err1, err2, err3, number_failed, 'int16')

C  int32
      err1 = 0
      err2 = 0
      err3 = 0
       do 1310 i=1,10
         do 1300 j=1,10
           if (i32(i,j) .ne. ti32(i,j)) err1 = 1
 1300    continue
      	 if (i32scale(i) .ne. ti32scale(i)) err2 = 1
 1310 continue

      if ((i32max .ne. ti32max) .or. (i32min .ne. ti32min)) err3 = 1
      call errchkarr(err1, err2, err3, number_failed, 'int32')
C
C  Sum up
C
      
      if (number_failed .gt. 0 ) then
          print *, '        >>> ', number_failed, ' TESTS FAILED <<<'
      else
          print *, '        >>> ALL TESTS PASSED <<<'
      endif

      stop
      end


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE errchkio
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine errchkio(err1, err2, err3, num_fail, msg)
      integer err1, err2, err3, num_fail
      character*(*)  msg

      integer FAIL

      FAIL = -1

      if (err1.eq.FAIL .or. err2.eq.FAIL .or. err3.eq.FAIL) then
          num_fail = num_fail + 1
          print *
          print *,'>>> Test failed for ',msg, ' <<<'
          print *, '  err1=',err1, '   err2=',err2, '   err3=',err3
      else
          print *,'Test passed for ', msg
      endif
      print *

      return
      end

      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE errchkarr
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine errchkarr(err1, err2, err3, num_fail, type)
      integer err1, err2, err3, num_fail
      character*(*)  type
      
      print *
      if (err1 .eq. 1) then
        print *, '>>> Test failed for ', type, ' array' 
        num_fail = num_fail + 1
      else
        print *, 'Test passed for ', type, ' array'
      endif

      if (err2 .eq. 1) then
        print *, '>>> Test failed for ',type, ' scales.'
        num_fail = num_fail + 1
      else
        print *, 'Test passed for ', type, ' scales.'
      endif

      if (err3 .eq. 1) then
        print *, '>>> Test failed for ', type, ' max/min.'
        num_fail = num_fail + 1
      else
        print *, 'Test passed for ', type, ' max/min.'
      endif

      print *

      return
      end

