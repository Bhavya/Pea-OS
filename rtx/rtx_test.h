/*--------------------------------------------------------------------------
 *                      ECE 354 RTX Greybox Testing
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx_test_dummy.h   
 * @author: Thomas Reidemeister
 * @author: Irene Huang
 * @date:   2010.05.15
 * @brief:  shared data b/w rtx and test suite for greybox testing 
 */

#ifndef _RTX_TEST_H_
#define _RTX_TEST_H_
#include "rtx_inc.h"


/* Process Priority. The bigger the number is, the lower the priority is*/
#define HIGH    0
#define MEDIUM  1
#define LOW     2
#define LOWEST  3

#define NUM_TEST_PROCS 6


/* global data structure for test fixture */
typedef struct test_fixture
{
    int (*send_message) (int, void *);
    void * (*receive_message) (int *);
    void * (*request_memory_block) ();
    int (*release_memory_block) (void *);
    int (*release_processor) ();
    int (*delayed_send) (int, void *, int);
    int (*set_process_priority) (int, int);
    int (*get_process_priority) (int);
} test_fixture_t; 

/* global data structure for test proc initialization info */
typedef struct test_proc
{
  UINT8  pid;           /* pid of a test process */
  UINT8  priority;      /* initial priority of a test process */
  UINT32 sz_stack;      /* stack size of a test process */
  VOID   (*entry)();    /* entry point of a test process */
} test_proc_t; 

/* global variable shared b/w test suite and rtx 
   in section __RTX_TEST_DATA__, see linker script
 */

test_fixture_t __attribute__ ((section("__RTX_TEST_DATA__"))) g_test_fixture;

test_proc_t __attribute__ ((section("__RTX_TEST_DATA__"))) g_test_proc[NUM_TEST_PROCS];

#endif /* _RTX_TEST_H_ */
