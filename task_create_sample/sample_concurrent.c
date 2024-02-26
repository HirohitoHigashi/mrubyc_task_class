/*
 * This sample program executes multiple mruby/c programs concurrently.
 */

#include <stdio.h>
#include <stdlib.h>
#include "mrubyc.h"

#if !defined(MRBC_MEMORY_SIZE)
#define MRBC_MEMORY_SIZE (1024*60)
#endif
static uint8_t memory_pool[MRBC_MEMORY_SIZE];

uint8_t * load_mrb_file(const char *filename)
{
  FILE *fp = fopen(filename, "rb");

  if( fp == NULL ) {
    fprintf(stderr, "File not found (%s)\n", filename);
    return NULL;
  }

  // get filesize
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // allocate memory
  uint8_t *p = malloc(size);
  if( p != NULL ) {
    fread(p, sizeof(uint8_t), size, fp);
  } else {
    fprintf(stderr, "Memory allocate error.\n");
  }
  fclose(fp);

  return p;
}


//================================================================
/*! (method) TODO

  Task.create( byte_code, regs_size = nil ) -> Task
*/
static void c_task_create(mrbc_vm *vm, mrbc_value v[], int argc)
{
  const char *byte_code;
  int regs_size = MAX_REGS_SIZE;

  // check argument.
  if( v[0].tt != MRBC_TT_CLASS ) goto ERROR_ARGUMENT;

  if( argc >= 1 && v[1].tt != MRBC_TT_STRING ) goto ERROR_ARGUMENT;
  mrbc_incref( &v[1] );
  byte_code = mrbc_string_cstr(&v[1]);

  if( argc >= 2 ) {
    if( v[2].tt != MRBC_TT_INTEGER ) goto ERROR_ARGUMENT;
    regs_size = mrbc_integer(v[2]);
  }

  // create TCB
  mrbc_tcb *tcb = mrbc_tcb_new( regs_size, TASKSTATE_DORMANT, MRBC_TASK_DEFAULT_PRIORITY );
  if( !tcb ) {
    mrbc_raise( vm, MRBC_CLASS(NoMemoryError), 0 );
    return;
  }
  tcb->vm.flag_permanence = 1;

  if( !mrbc_create_task( byte_code, tcb ) ) return;

  // create Instance
  mrbc_value ret = mrbc_instance_new(vm, v->cls, sizeof(mrbc_tcb *));
  *(mrbc_tcb **)ret.instance->data = tcb;
  SET_RETURN( ret );
  return;

 ERROR_ARGUMENT:
  mrbc_raise( vm, MRBC_CLASS(ArgumentError), 0 );
}

//================================================================
/*! (method) TODO

  task1.run
*/
static void c_task_run(mrbc_vm *vm, mrbc_value v[], int argc)
{
  if( v[0].tt == MRBC_TT_CLASS ) return;

  mrbc_tcb *tcb = *(mrbc_tcb **)v[0].instance->data;

  mrbc_start_task(tcb);
}

//================================================================
/*! (method) TODO

  task1.rewind
*/
static void c_task_rewind(mrbc_vm *vm, mrbc_value v[], int argc)
{
  if( v[0].tt == MRBC_TT_CLASS ) return;

  mrbc_tcb *tcb = *(mrbc_tcb **)v[0].instance->data;

  if( tcb->vm.flag_permanence ) {
    mrbc_vm_end( &tcb->vm );
  }
  mrbc_vm_begin( &tcb->vm );
}


//================================================================
/*! (method) TODO

  task1.return_value
*/
static void c_task_return_value(mrbc_vm *vm, mrbc_value v[], int argc)
{
  if( v[0].tt == MRBC_TT_CLASS ) return;

  mrbc_tcb *tcb = *(mrbc_tcb **)v[0].instance->data;

  mrbc_incref( &tcb->vm.regs[0] );
  SET_RETURN( tcb->vm.regs[0] );
}


int main(int argc, char *argv[])
{
  int vm_cnt = argc-1;
  if( vm_cnt < 1 || vm_cnt > MAX_VM_COUNT ) {
    printf("Usage: %s <xxxx.mrb> <xxxx.mrb> ... \n", argv[0]);
    printf("  Maximum number of mrb file: %d\n", MAX_VM_COUNT );
    return 1;
  }

  /*
    start mruby/c with rrt0 scheduler.
  */
  mrbc_init(memory_pool, MRBC_MEMORY_SIZE);

  mrbc_class *cls = mrbc_get_class_by_name("Task");
  mrbc_define_method(0, cls, "create", c_task_create);
  mrbc_define_method(0, cls, "run", c_task_run);
  mrbc_define_method(0, cls, "rewind", c_task_rewind);
  mrbc_define_method(0, cls, "return_value", c_task_return_value);

  // create each task.
  for( int i = 0; i < vm_cnt; i++ ) {
    fprintf( stderr, "Loading: '%s'\n", argv[i+1] );
    uint8_t *mrbbuf = load_mrb_file( argv[i+1] );
    if( mrbbuf == 0 ) return 1;
    if( !mrbc_create_task( mrbbuf, NULL ) ) return 1;
  }

  // and execute all.
  int ret = mrbc_run();

  return ret == 1 ? 0 : ret;
}
