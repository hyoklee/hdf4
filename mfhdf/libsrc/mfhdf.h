/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

#ifndef _MFSD_H_
#define _MFSD_H_

#ifndef HDF
#define HDF 1
#endif

/* change this back if it causes problems on other machines than the Alhpa-QAK */
/* Reverse back to the previous way. AKC */
#include "hdf.h"
#include "netcdf.h"
#ifdef OLD_WAY
#include "local_nc.h"
#endif /* OLD_WAY */

#define SD_UNLIMITED NC_UNLIMITED /* use this as marker for unlimited dimension */
#define SD_NOFILL    NC_NOFILL
#define SD_FILL      NC_FILL
#define SD_DIMVAL_BW_COMP   1
#define SD_DIMVAL_BW_INCOMP  0
#define SD_RAGGED    -1  /* marker for ragged dimension */

#ifdef __cplusplus
extern "C" {
#endif

extern int32 SDstart
    (const char *name, int32 access);

extern intn SDend
    (int32 fid);

extern intn SDfileinfo
    (int32 fid, int32 *datasets, int32 *attrs);

extern int32 SDselect
    (int32 fid, int32 index);

extern intn SDgetinfo
    (int32 sdsid, char *name, int32 *rank, int32 *dimsizes, 
           int32 *nt, int32 *nattr);

#ifndef __CSTAR__
extern intn SDreaddata
    (int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data);
#endif

extern uint16 SDgerefnumber
    (int32 sdsid);

extern int32 SDnametoindex
    (int32 fid, char *name);

extern intn SDgetrange
    (int32 sdsid, VOIDP pmax, VOIDP pmin);

extern int32 SDcreate
    (int32 fid, char *name, int32 nt, int32 rank, int32 *dimsizes);

extern int32 SDgetdimid
    (int32 sdsid, intn number);

extern intn SDsetdimname
    (int32 id, char *name);

extern intn SDendaccess
    (int32 id);

extern intn SDsetrange
    (int32 sdsid, VOIDP pmax, VOIDP pmin);

extern intn SDsetattr
    (int32 id, char *name, int32 nt, int32 count, VOIDP data);

extern intn SDattrinfo
    (int32 id, int32 index, char *name, int32 *nt, int32 *count);

extern intn SDreadattr
    (int32 id, int32 index, VOIDP buf);

#ifndef __CSTAR__
extern intn SDwritedata
    (int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data);
#endif

extern intn SDsetdatastrs
    (int32 sdsid, char *l, char *u, char *f, char *c);

extern intn SDsetcal
    (int32 sdsid, float64 cal, float64 cale, float64 ioff,
               float64 ioffe, int32 nt);

extern intn SDsetfillvalue
    (int32 sdsid, VOIDP val);

extern intn SDgetfillvalue
    (int32 sdsid, VOIDP val);

extern intn SDgetdatastrs
    (int32 sdsid, char *l, char *u, char *f, char *c, intn len);

extern intn SDgetcal
    (int32 sdsid, float64 *cal, float64 *cale, float64 *ioff, 
               float64 *ioffe, int32 *nt);

extern intn SDsetdimstrs
    (int32 id, char *l, char *u, char *f);

extern intn SDsetdimscale
    (int32 id, int32 count, int32 nt, VOIDP data);

extern intn SDgetdimscale
    (int32 id, VOIDP data);

extern intn SDdiminfo
    (int32 id, char *name, int32 *size, int32 *nt, int32 *nattr);

extern intn SDgetdimstrs
    (int32 id, char *l, char *u, char *f, intn len);

extern intn SDsetexternalfile
    (int32 id, char *filename, int32 offset);

extern intn SDsetnbitdataset
    (int32 id, intn start_bit, intn bit_len, intn sign_ext, intn fill_one);

extern intn SDsetcompress
    (int32 id, int32 type, comp_info *c_info);

extern int32 SDfindattr
    (int32 id, char *attrname);

extern int32 SDidtoref
    (int32 id);

extern int32 SDreftoindex
    (int32 fid, int32 ref);

extern int32 SDisrecord
    (int32 id);

extern intn SDiscoordvar
    (int32 id);

extern intn SDsetaccesstype
    (int32 id, uintn accesstype);

extern intn SDsetblocksize
    (int32 sdsid, int32 block_size);

extern intn SDsetdimval_comp
    (int32 dimid, intn compt_mode);

extern intn SDisdimval_bwcomp
    (int32 dimid);

/*====================== Chunking Routines ================================*/

/* flag used for SDsetChunk() and SDgetChunkInfo() */
#define SD_CHUNK_LENGTHS 0x1
#define SD_CHUNK_COMP    0x2
#define SD_CHUNK_NBIT    0x3

/* Cache flags */
#define HDF_PAGEALL 0x1

/* Chunk Defintion */
typedef struct sd_chunk_def_struct 
{
    /* Chunk Lengths - Required */
    int32     *chunk_lengths; /* chunk lengths along each dimension */

    /* For Compression info - Optional */
    int32      comp_type;     /* Compression type */
    int32      model_type;    /* Compression model type */
    comp_info  *cinfo;        /* Compression info struct */
    model_info *minfo;        /* Compression model info struct */

    /* For NBIT - Optional */
    intn      start_bit;
    intn      bit_len;
    intn      sign_ext;
    intn      fill_one;
} SD_CHUNK_DEF;


/******************************************************************************
 NAME
        SDsetChunk   -- create chunked SDS

 DESCRIPTION
        This routine creates a chunked SDS with the specified chunk
        lengths for each dimension according to the structure passed in. 
        Currently only the array(int32) specifiying chunk lengths can be 
        passed in, i.e. 'flags' must be set 'SD_CHUNK_LENGTHS'; 
        
        In the future a different structure maybe used to define
        a chunk.

        The dataset currently cannot have an UNLIMITED dimension.

        The dataset currently cannot be special already. 
        i.e. NBIT, COMPRESSION, or EXTERNAL.

        COMPRESSION is set by using the 'SD_CHUNK_DEF' structure
        to set the appropriate compression info. The info is
        the same as that set in 'SDsetcompress()'.

        The relevant fields of SD_CHUNK_DEF structure looks like:

            int32     *chunk_lengths;  Chunk lengths along each dimension 
            int32      comp_type;      Compression type 
            comp_info  *cinfo;         Compression info struct 

        See example in pseudo-C below for further usage.

        The maximum number of Chunks in an HDF file is 65,535.

        e.g. 4x4 array with 2x2 chunks. The array shows the layout of
             chunks in the chunk array.

            4 ---------------------                                           
              |         |         |                                                 
        Y     |  (0,1)  |  (1,1)  |                                       
        ^     |         |         |                                      
        |   2 ---------------------                                       
        |     |         |         |                                               
        |     |  (0,0)  |  (1,0)  |                                      
        |     |         |         |                                           
        |     ---------------------                                         
        |     0         2         4                                       
        ---------------> X                                                       
                                                                                
        --Without compression--:
        {                                                                    
        int32  chunk_lengths[2];                                               
                                                                            
        .......                                                                    
        -- Set chunk lengths --                                                    
        chunk_lengths[0]= 2;                                                     
        chunk_lengths[1]= 2; 
        -- Set Chunking -- 
        SDsetChunk(sdsid,chunk_lengths, SD_CHUNK_LENGTHS);                      
         ......                                                                  
        }                                                                           

        --With compression--:
        {                                                                    
        int32        chunk_lengths[2];                                               
        comp_info    cinfo;
        SD_CHUNK_DEF chunk_def;
                                                                            
        .......                
        -- Set chunk lengths first --                                                    
        chunk_lengths[0]= 2;                                                     
        chunk_lengths[1]= 2;
        chunk_def.chunk_lengths = chunk_lengths;

        -- Set compression --
        cinfo.deflate.level = 9;
        chunk_def.comp_type = COMP_CODE_DEFLATE;
        chunk_def.cinfo = &cinfo;

        -- Set Chunking with Compression --
        SDsetChunk(sdsid, &chunk_def, SD_CHUNK_COMP);                      
         ......                                                                  
        }                                                                           

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
extern intn SDsetChunk
    (int32 sdsid,     /* IN: sds access id */
     VOID *chunk_def, /* IN: chunk definition */
     int32 flags      /* IN: flags */);

/******************************************************************************
 NAME
        SDgetChunkInfo -- get Info on Chunked SDS

 DESCRIPTION
        This routine currently only handles as input an array to
        hold the chunk_lengths for each dimension. The only
        valid flag is 'SD_CHUNK_LENGTHS' which fills an
        array(int32) of chunk lengths for each dimension.

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
extern intn SDgetChunkInfo
    (int32 sdsid,      /* IN: sds access id */
     VOID *chunk_def,  /* IN/OUT: chunk definition */
     int32 flags       /* IN: flags */);

/******************************************************************************
 NAME
        SDisChunked  -- Is this SDS chunked

 DESCRIPTION
        This routine checks to see if the SDS is a Chunked SDS.

 RETURNS
        1->True, 0->False, -1(FAIL)->Error
******************************************************************************/
extern intn SDisChunked
    (int32 sdsid     /* IN: sds access id */);

/******************************************************************************
 NAME
        SDwriteChunk  -- write the specified chunk to the SDS

 DESCRIPTION
        This routine writes a whole chunk of data to the chunked SDS 
        specified by chunk 'origin' for the given SDS and can be used
        instead of SDwritedata() when this information is known. This
        routine has less overhead and is much faster than using SDwritedata().

        Origin specifies the co-ordinates of the chunk according to the chunk
        position in the overall chunk array.

        'datap' must point to a whole chunk of data.

        e.g. 4x4 array with 2x2 chunks. The array shows the layout of
             chunks in the chunk array.

            4 ---------------------
              |         |         |
        Y     |  (0,1)  |  (1,1)  |
        ^     |         |         |
        |   2 ---------------------
        |     |         |         |
        |     |  (0,0)  |  (1,0)  |
        |     |         |         |
        |     ---------------------
        |     0         2         4
        ---------------> X

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
extern intn SDwriteChunk
    (int32 sdsid,      /* IN: sds access id */
     int32 *origin,    /* IN: origin of chunk to write */
     const VOID *datap /* IN: buffer for data */);

/******************************************************************************
 NAME
        SDreadChunk   -- read the specified chunk to the SDS

 DESCRIPTION
        This routine reads a whole chunk of data from the chunked SDS
        specified by chunk 'origin' for the given SDS and can be used
        instead of SDreaddata() when this information is known. This
        routine has less overhead and is much faster than using SDreaddata().

        Origin specifies the co-ordinates of the chunk according to the chunk
        position in the overall chunk array.

        'datap' must point to a whole chunk of data.

        e.g. 4x4 array with 2x2 chunks. The array shows the layout of
             chunks in the chunk array.

            4 ---------------------
              |         |         |
        Y     |  (0,1)  |  (1,1)  |
        ^     |         |         |
        |   2 ---------------------
        |     |         |         |
        |     |  (0,0)  |  (1,0)  |
        |     |         |         |
        |     ---------------------
        |     0         2         4
        ---------------> X

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
extern intn SDreadChunk
    (int32 sdsid,      /* IN: sds access id */
     int32 *origin,    /* IN: origin of chunk to read */
     VOID  *datap      /* IN/OUT: buffer for data */);

/******************************************************************************
NAME
     SDsetChunkCache -- maximum number of chunks to cache 

DESCRIPTION
     Set the maximum number of chunks to cache.
     By default when the SDS is promoted to a chunked element the 
     maximum number of chunks in the cache is set to the number of
     chunks along the last dimension.

     The values set here affects the current object's caching behaviour.

     If the chunk cache is full and 'maxcache' is greater then the
     current 'maxcache' value, then the chunk cache is reset to the new
     'maxcache' value, else the chunk cache remains at the current
     'maxcache' value.

     If the chunk cache is not full, then the chunk cache is set to the
     new 'maxcache' value only if the new 'maxcache' value is greater than the
     current number of chunks in the cache.

     Use flags argument of 'HDF_PAGEALL' if the whole object is to be cached 
     in memory, otherwise pass in zero(0). Currently you can only
     pass in zero.

RETURNS
     Returns the 'maxcache' value for the chunk cache if successful 
     and FAIL otherwise
******************************************************************************/
extern intn SDsetChunkCache
    (int32 sdsid,     /* IN: sds access id */
     int32 maxcache,  /* IN: max number of pages to cache */
     int32 flags      /* IN: flags = 0, HDF_PAGEALL */);

/* Define the FORTRAN names */

#ifndef MFSD_FNAMES
#   define  MFSD_FNAMES
#ifdef DF_CAPFNAMES
#   define nscstart    FNAME(SCSTART)
#   define nsfend      FNAME(SFEND)
#   define nsfendacc   FNAME(SFENDACC)
#   define nsffinfo    FNAME(SFFINFO)
#   define nsfselect   FNAME(SFSELECT)
#   define nscginfo    FNAME(SCGINFO)
#   define nscgainfo   FNAME(SCGAINFO)
#   define nscgdinfo   FNAME(SCGDINFO)
#   define nsfgcal     FNAME(SFGCAL)
#   define nsfscal     FNAME(SFSCAL)
#   define nsfgdscale  FNAME(SFGDSCALE)
#   define nsfsdscale  FNAME(SFSDSCALE)
#   define nsfgcfill   FNAME(SFGCFILL)
#   define nsfgfill    FNAME(SFGFILL)
#   define nsfscfill   FNAME(SFSCFILL)
#   define nsfsfill    FNAME(SFSFILL)
#   define nsfgrange   FNAME(SFGRANGE)
#   define nsfsrange   FNAME(SFSRANGE)
#   define nscn2index  FNAME(SCN2INDEX)
#   define nsccreate   FNAME(SCCREATE)
#   define nscsdimstr  FNAME(SCSDIMSTR)
#   define nscsdimname FNAME(SCSDIMNAME)
#   define nscsdatstr  FNAME(SCSDATSTR)
#   define nsfdimid    FNAME(SFDIMID)
#   define nsfrcatt    FNAME(SFRCATT)
#   define nsfrnatt    FNAME(SFRNATT)
#   define nsfrattr    FNAME(SFRATTR)
#   define nsfrcdata   FNAME(SFRCDATA)
#   define nsfrdata    FNAME(SFRDATA)
#   define nsfwcdata   FNAME(SFWCDATA)
#   define nsfwdata    FNAME(SFWDATA)
#   define nscgdatstrs FNAME(SCGDATSTRS)
#   define nscgdimstrs FNAME(SCGDIMSTRS)
#   define nscscatt    FNAME(SCSCATT)
#   define nscsnatt    FNAME(SCSNATT)
#   define nscsattr    FNAME(SCSATTR)
#   define nscfattr    FNAME(SCFATTR)
#   define nsfid2ref  FNAME(SFID2REF)
#   define nsfref2index FNAME(SFREF2INDEX)
#   define nsfiscvar   FNAME(SFISCVAR)
#   define nscsextf    FNAME(SCSEXTF)
#   define nsfsacct    FNAME(SFSACCT)
#   define nsfsdmvc    FNAME(SFSDMVC)
#   define nsfisdmvc   FNAME(SFISDMVC)
#else   /* DF_CAPFNAMES */
#   define nscstart    FNAME(scstart)
#   define nsfend      FNAME(sfend)
#   define nsfendacc   FNAME(sfendacc)
#   define nsffinfo    FNAME(sffinfo)
#   define nsfselect   FNAME(sfselect)
#   define nscginfo    FNAME(scginfo)
#   define nscgainfo   FNAME(scgainfo)
#   define nscgdinfo   FNAME(scgdinfo)
#   define nsfgcal     FNAME(sfgcal)
#   define nsfscal     FNAME(sfscal)
#   define nsfgdscale  FNAME(sfgdscale)
#   define nsfsdscale  FNAME(sfsdscale)
#   define nsfgcfill   FNAME(sfgcfill)
#   define nsfgfill    FNAME(sfgfill)
#   define nsfscfill   FNAME(sfscfill)
#   define nsfsfill    FNAME(sfsfill)
#   define nsfgrange   FNAME(sfgrange)
#   define nsfsrange   FNAME(sfsrange)
#   define nscn2index  FNAME(scn2index)
#   define nsccreate   FNAME(sccreate)
#   define nscsdimstr  FNAME(scsdimstr)
#   define nscsdimname FNAME(scsdimname)
#   define nscsdatstr  FNAME(scsdatstr)
#   define nsfdimid    FNAME(sfdimid)
#   define nsfrcatt    FNAME(sfrcatt)
#   define nsfrnatt    FNAME(sfrnatt)
#   define nsfrattr    FNAME(sfrattr)
#   define nscscatt    FNAME(scscatt)
#   define nscsnatt    FNAME(scsnatt)
#   define nscsattr    FNAME(scsattr)
#   define nscfattr    FNAME(scfattr)
#   define nsfrcdata   FNAME(sfrcdata)
#   define nsfrdata    FNAME(sfrdata)
#   define nsfwcdata   FNAME(sfwcdata)
#   define nsfwdata    FNAME(sfwdata)
#   define nscgdatstrs FNAME(scgdatstrs)
#   define nscgdimstrs FNAME(scgdimstrs)
#   define nsfid2ref   FNAME(sfid2ref)
#   define nsfref2index FNAME(sfref2index)
#   define nsfiscvar   FNAME(sfiscvar)
#   define nscsextf    FNAME(scsextf)
#   define nsfsacct    FNAME(sfsacct)
#   define nsfsdmvc    FNAME(sfsdmvc)
#   define nsfisdmvc   FNAME(sfisdmvc)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MFSD_H_ */
