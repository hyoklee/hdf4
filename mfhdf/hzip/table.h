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


#ifndef HDF_ZIP_TABLE_H__
#define HDF_ZIP_TABLE_H__


#include "hdf.h"
#include "mfhdf.h"

#define PFORMAT  "  %-7s %-7s %-7s\n" /*chunk info, compression info, name*/
#define PFORMAT1 "  %-7s %-7s %-7s"   /*chunk info, compression info, name*/





#ifdef __cplusplus
extern "C" {
#endif


/*struct to store the tag and ref of an object */
typedef struct obj_t {
 int tag;
	int ref;
	char obj_name[MAX_NC_NAME];
} obj_t;

/*struct that stores all objects */
typedef struct table_t {
	int size;
	int nobjs;
	obj_t *objs;
} table_t;


/* table methods */
void table_init(table_t **table);
void table_free(table_t *table);
int  table_search(table_t *table, int tag, int ref );
void table_add(table_t *table, int tag, int ref, char* obj_name, int print);
char* table_check(table_t *table, char*obj_name);



#ifdef __cplusplus
}
#endif


#endif  /* HDF_ZIP_TABLE_H__ */
