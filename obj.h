/*
 Copyright (c) 2016 Jacob Gordon. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef obj_h
#define obj_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "RKMem.h"

typedef struct obj_class_s* obj_class ;

typedef struct obj_object_s* AnyClass ;

typedef obj_class (*obj_classdef)(int mode, obj_class subclass) ;

typedef struct { void* fast_data_structure ; } *obj_fds_type ;

typedef double obj_float ;

typedef double obj_double ;

typedef signed int obj_interger ;

typedef unsigned int obj_unsigned ;

#ifdef _WIN32

typedef signed long long obj_long ;

typedef unsigned long long obj_ulong ;

#else

typedef signed long obj_long ;

typedef unsigned long obj_ulong ;

#endif

typedef va_list obj_arglist ;

typedef obj_long (*obj_method)(va_list external_arglist, const AnyClass obj, ...) ;

typedef obj_long (*obj_classmethod)(va_list external_arglist, const AnyClass obj, const obj_class cls, ...) ;

typedef void (*obj_classdeinit)(const obj_class cls) ;

#define obj_bitsizeof(type) sizeof(type) * CHAR_BIT

#define new_class(classname)\
static void classname##_obj_class_init_method(const obj_class cls) ;\
obj_class Def##classname( int mode, obj_class subclass ) {\
static obj_class cls = NULL ;\
static int count = 0 ;\
 if (mode == -1) {\
  count-- ;\
  if ( count <= 0 ) {\
  count = 0 ;\
  obj_class_dealloc(cls) ;\
  cls = NULL ;\
  }\
 }\
 if (mode == 0) {\
  count++ ;\
  if (cls == NULL){\
   cls = obj_class_alloc(Def##classname) ;\
   classname##_obj_class_init_method(cls) ;\
   }\
 return cls ;\
 }\
 if (mode == 1) {\
  classname##_obj_class_init_method(subclass) ;\
  return subclass ;\
 }\
return NULL ;\
}\
static void classname##_obj_class_init_method(const obj_class cls)

#define new_private_class(classname)\
static void classname##_obj_class_init_method(const obj_class cls) ;\
static obj_class Def##classname( int mode, obj_class subclass ) {\
static obj_class cls = NULL ;\
static int count = 0 ;\
if (mode == -1) {\
count-- ;\
if ( count <= 0 ) {\
count = 0 ;\
obj_class_dealloc(cls) ;\
cls = NULL ;\
}\
}\
if (mode == 0) {\
count++ ;\
if (cls == NULL){\
cls = obj_class_alloc(Def##classname) ;\
classname##_obj_class_init_method(cls) ;\
}\
return cls ;\
}\
if (mode == 1) {\
classname##_obj_class_init_method(subclass) ;\
return subclass ;\
}\
return NULL ;\
}\
static void classname##_obj_class_init_method(const obj_class cls)

#define new_final_class(classname)\
static void classname##_obj_class_init_method(const obj_class cls) ;\
obj_class Def##classname( int mode, obj_class subclass ) {\
static obj_class cls = NULL ;\
static int count = 0 ;\
if (mode == -1) {\
count-- ;\
if ( count <= 0 ) {\
count = 0 ;\
obj_class_dealloc(cls) ;\
cls = NULL ;\
}\
}\
if (mode == 0) {\
count++ ;\
if (cls == NULL){\
cls = obj_class_alloc(Def##classname) ;\
classname##_obj_class_init_method(cls) ;\
}\
return cls ;\
}\
if (mode == 1) {\
return NULL ;\
}\
return NULL ;\
}\
static void classname##_obj_class_init_method(const obj_class cls)

#define new_abstract_class(classname)\
static void classname##_obj_class_init_method(const obj_class cls) ;\
obj_class Def##classname( int mode, obj_class subclass ) {\
static obj_class cls = NULL ;\
static int count = 0 ;\
if (mode == -1) {\
count-- ;\
if ( count <= 0 ) {\
count = 0 ;\
cls = NULL ;\
}\
}\
if (mode == 0) {\
count++ ;\
if (cls == NULL){\
}\
return cls ;\
}\
if (mode == 1) {\
classname##_obj_class_init_method(subclass) ;\
return subclass ;\
}\
return NULL ;\
}\
static void classname##_obj_class_init_method(const obj_class cls)


#define make_class_subclass_of(superclass) Def##superclass(1, cls)

#define make_cls_available_for(class) static obj_class class##_cls = NULL

#define init_cls_for(class) class##_cls = cls

#define get_cls_for(class) class##_cls

#define declare_class(classname) typedef struct Obj##classname##_s* classname

#define use_class(classname) obj_class Def##classname( int mode, obj_class subclass ) ; \
typedef struct Obj##classname##_s* classname

#define use_private_class(classname) static obj_class Def##classname( int mode, obj_class subclass ) ; \
typedef struct Obj##classname##_s* classname

#define use_static_method(methodname) obj_long methodname(va_list external_arglist, const AnyClass obj, ...)


#define make_method_public(method) obj_add_method(method,#method,cls)

#define make_method_mask(method,methodname) obj_add_method(method,#methodname,cls)

#define make_method_final(methodname) obj_add_final_method(#methodname,cls)

#define make_method_class_deinit(method) obj_add_classdeinit_method(method,cls)

#define make_method_init(method) obj_add_init_method(method,cls)

#define make_method_deinit(method) obj_add_deinit_method(method,cls)

#define make_method_final_init(method) obj_add_final_init_method(method,cls)

#define make_method_final_deinit(method) obj_add_final_deinit_method(method,cls)

#define make_class_method_public(method) obj_add_class_method((obj_method)method,#method,cls)

#define make_class_method_mask(method,methodname) obj_add_class_method((obj_method)method,#methodname,cls)

#define make_class_method_final(methodname) obj_add_final_class_method(#methodname,cls)


#define new_object(class,...) (class)obj_object_alloc(Def##class,__VA_ARGS__)

#define new_any_object(class,...) obj_object_alloc(Def##class,__VA_ARGS__)

#define new_object_with_classdef(classdef,...) obj_object_alloc(classdef,__VA_ARGS__)

#define free_object(obj) obj_object_dealloc((AnyClass)obj)

#define get_classdef(class) Def##class

#define get_classdef_from_cls(cls) obj_get_classdef_from_class(cls)


#define keep_object(object1,object2) obj_store_object((AnyClass)object1,(AnyClass)object2,NULL,1)

#define ko(object1,object2) keep_object(object1,object2)

#define store_object_strong(object1,name,object2) obj_store_object((AnyClass)object1,(AnyClass)object2,#name,1)

#define store_object_weak(object1,name,object2) obj_store_object((AnyClass)object1,(AnyClass)object2,#name,0)

#define sos(object1,name,object2) store_object_strong(object1,name,object2)

#define sow(object1,name,object2) store_object_weak(object1,name,object2)

#define get_object(object,name) obj_get_object(object, #name)

#define getobj(object,name) get_object(object,name)


#define store_pointer(object,name,pointer) obj_store_pointer((AnyClass)object,pointer,#name)

#define sp(object,name,pointer) store_pointer(object,name,pointer)

#define get_pointer(object,name) obj_get_pointer((AnyClass)object,#name)

#define gp(object,name) get_pointer((AnyClass)object,name)


#define make_private_data_store_available obj_alloc_private_store_for_object(obj,cls)

#define destroy_private_data_store obj_dealloc_private_store_for_object(obj,cls)

#define store_private_pointer(name,pointer) obj_store_private_pointer(obj,cls,pointer,#name)

#define spp(name,pointer) store_private_pointer(name,pointer)

#define get_private_pointer(name) obj_get_private_pointer(obj,cls,#name)

#define gpp(name) get_private_pointer(name)


#define class_store_pointer(class,name,pointer) obj_class_store_pointer(class,pointer,#name)

#define class_sp(class,name,pointer) class_store_pointer(class,name,pointer)

#define class_get_pointer(class,name) obj_class_get_pointer(class,#name)

#define class_gp(class,name) class_get_pointer(class,name)


#define add_ref_count(object) obj_add_ref_count((AnyClass)object)

#define sub_ref_count(object) obj_sub_ref_count((AnyClass)object)


#define arg(name,type) type name = va_arg(method_arglist,type) ;

#define argfromlist(name,type) type name ; if (external_arglist != NULL) {\
name = va_arg(external_arglist,type) ;\
} else { name = va_arg(method_arglist,type) ;}

#define noargs 0

#define new_class_deinit_method(method_name) static void method_name(obj_class cls)

#define start_method(method_name,method_args) static obj_long method_name(va_list external_arglist, const AnyClass obj,...) {\
va_list method_arglist ;\
va_start(method_arglist,obj) ;\
arg(obj_nullptr,void * const)\
if (obj_nullptr != NULL) return -1 ;\
method_args\

// static method equals non static function
#define start_static_method(method_name,method_args) obj_long method_name(va_list external_arglist, const AnyClass obj,...) {\
va_list method_arglist ;\
va_start(method_arglist,obj) ;\
arg(obj_nullptr,void * const)\
if (obj_nullptr != NULL) return -1 ;\
method_args\

#define start_class_method(method_name,method_args) static obj_long method_name(va_list external_arglist, const AnyClass obj, const obj_class cls,...) {\
va_list method_arglist ;\
va_start(method_arglist,cls) ;\
if (cls == NULL) {printf("Object Runtime: Error, access control violation or corrupted 'cls' pointer.\n");exit(EXIT_FAILURE);}\
if ( !obj_verify_object_is_of_class(obj,cls) ) return -2 ;\
void * const obj_nullptr = NULL ;\
if (obj_nullptr != NULL) return -1 ;\
method_args\

#define end_method va_end(method_arglist) ; return 1 ; }

#define start_arglist(arglist, last_arg) va_start(arglist, last_arg)

#define end_arglist(arglist) va_end(arglist)


#define get_null_method obj_default_func

#define null_method_error -1

#define return_error_from_method return -2

#define method_returned_error -2

#define return_from_method return 1

#define get_method(obj,method) obj_get_method((AnyClass)obj,#method)

#define get_class_method(cls,method) obj_get_class_method(cls,#method)


#define method_invoke(obj,method,...) obj_get_method((AnyClass)obj,#method)(NULL,(AnyClass)obj,NULL,__VA_ARGS__)
#define m(obj,method,...) method_invoke(obj,method,__VA_ARGS__)

#define method_invoke_with_arglist(obj,method,arglist,...) obj_get_method((AnyClass)obj,#method)(arglist,(AnyClass)obj,NULL,__VA_ARGS__)
#define ma(obj,method,arglist,...) method_invoke_with_arglist(obj,method,arglist,__VA_ARGS__)

#define private_method_invoke(obj,method,...) method(NULL,(AnyClass)obj,NULL,__VA_ARGS__)
#define pm(obj,method,...) private_method_invoke(obj,method,__VA_ARGS__)

#define private_method_invoke_with_arglist(obj,method,arglist,...) method(arglist,(AnyClass)obj,NULL,__VA_ARGS__)
#define pma(obj,method,arglist,...) private_method_invoke_with_arglist(obj,method,arglist,__VA_ARGS__)

#define static_method_invoke(method,...) pm(NULL,method,__VA_ARGS__)
#define sm(method,...) static_method_invoke(method,__VA_ARGS__)

#define static_method_invoke_with_arglist(method,arglist,...) pma(NULL,method,arglist,__VA_ARGS__)
#define sma(method,...) static_method_invoke_with_arglist(method,__VA_ARGS__)

#define class_method_invoke(obj,method,...) ((obj_classmethod)obj_get_class_method(cls,#method))(NULL,(AnyClass)obj,cls,__VA_ARGS__)
#define cm(obj,method,...) class_method_invoke(obj,method,__VA_ARGS__)

#define class_method_invoke_with_arglist(obj,method,arglist,...) ((obj_classmethod)obj_get_class_method(cls,#method))(arglist,(AnyClass)obj,cls,__VA_ARGS__)
#define cma(obj,method,arglist,...) class_method_invoke_with_arglist(obj,method,arglist,__VA_ARGS__)

#define private_class_method_invoke(obj,method,...) ((obj_classmethod)method)(NULL,(AnyClass)obj,cls,__VA_ARGS__)
#define pcm(obj,method,...) private_class_method_invoke(obj,method,__VA_ARGS__)

#define private_class_method_invoke_with_arglist(obj,method,arglist,...) ((obj_classmethod)method)(arglist,(AnyClass)obj,cls,__VA_ARGS__)
#define pcma(obj,method,arglist,...) private_class_method_invoke_with_arglist(obj,method,arglist,__VA_ARGS__)


#define fast_data_store ((obj_fds_type)obj)->fast_data_structure

#define fast_data_store_class ((obj_fds_type)cls)->fast_data_structure

#define fast_data_store_of(entity) ((obj_fds_type)entity)->fast_data_structure

#define get_fast_data_store(type) ((type)fast_data_store)

#define get_fast_data_store_class(type) ((type)fast_data_store_class)

#define get_fast_data_store_of(entity,type) ((type)fast_data_store_of(entity))

#define fds fast_data_store

#define fds_class fast_data_store_class

#define fds_of(entity) fast_data_store_of(entity)

#define get_fds(type) get_fast_data_store(type)

#define get_fds_class(type) get_fast_data_store_class(type)

#define get_fds_of(entity,type) get_fast_data_store_of(entity,type)


#define is_object_of_class(obj,cls) obj_verify_object_is_of_class((AnyClass)obj,cls)


#define define_record_type(recordtype,...) typedef struct recordtype##_s { __VA_ARGS__ } *recordtype

#define new_record(recordtype) RKMem_NewMemOfType(struct recordtype##_s)

#define store_record(name,record) store_pointer(obj,name,record)

#define get_record(name,recordtype) ((recordtype)get_pointer(obj,name))

#define store_private_record(name,record) store_private_pointer(name,record)

#define get_private_record(name,recordtype) ((recordtype)get_private_pointer(name))


#define define_data_set(dataset,...) define_record_type(dataset##type,__VA_ARGS__)

#define store_set(dataset) dataset##type dataset##tmp = new_record( dataset##type );\
store_record(dataset, dataset##tmp)

#define get_set(dataset) get_record(dataset, dataset##type)

#define destroy_set(dataset) free(get_set(dataset))


#define define_private_data_set(dataset,...) define_record_type(dataset##type_private,__VA_ARGS__)

#define store_private_set(dataset) dataset##type_private dataset##tmp_private = new_record( dataset##type_private );\
store_private_record(dataset, dataset##tmp_private)

#define get_private_set(dataset) get_private_record(dataset, dataset##type_private)

#define destroy_private_set(dataset) free(get_private_set(dataset))

#define send_object_msg(obj,msg,...) obj_get_method((AnyClass)obj,msg)(NULL,(AnyClass)obj,NULL,__VA_ARGS__)

#define send_object_msg_with_arglist(obj,msg,arglist,...) obj_get_method((AnyClass)obj,msg)(arglist,(AnyClass)obj,NULL,__VA_ARGS__)

#define som(obj,msg,...) send_object_msg(obj,msg,__VA_ARGS__)

#define soma(obj,msg,arglist,...) send_object_msg_with_arglist(obj,msg,arglist,__VA_ARGS__)

#define store_object_with_msg(object1,msg,object2) obj_store_object((AnyClass)object1,(AnyClass)object2,msg,1)

#define store_weak_object_with_msg(object1,msg,object2) obj_store_object((AnyClass)object1,(AnyClass)object2,msg,0)

#define get_object_with_msg(object, msg) obj_get_object((AnyClass)object, msg)

#define store_pointer_with_msg(object,msg,pointer) obj_store_pointer((AnyClass)object,pointer,msg)

#define get_pointer_with_msg(object,msg) obj_get_pointer((AnyClass)object,msg)

#define class_store_pointer_with_msg(class,msg,pointer) obj_class_store_pointer(class,pointer,msg)

#define class_get_pointer_with_msg(class,msg) obj_class_get_pointer(class,msg)

obj_long obj_default_func(va_list external_arglist, const AnyClass obj, ...) ;

AnyClass obj_object_alloc( obj_classdef the_classdef, ... ) ;

void obj_object_dealloc( AnyClass obj ) ;

void obj_store_object( AnyClass obj1, AnyClass obj2, const char* name, int strong ) ;

AnyClass obj_get_object( AnyClass obj, const char* name ) ;

void obj_store_pointer( AnyClass obj, void* pointer, const char* name ) ;

void* obj_get_pointer( AnyClass obj, const char* name ) ;

void obj_add_ref_count( AnyClass obj ) ;

void obj_sub_ref_count( AnyClass obj ) ;

obj_class obj_class_alloc( obj_classdef the_classdef ) ;

void obj_class_dealloc( obj_class cls ) ;

void obj_class_store_pointer( obj_class cls, void* pointer, const char* name ) ;

void* obj_class_get_pointer( obj_class cls, const char* name ) ;

void obj_add_classdeinit_method( obj_classdeinit classdeinit, obj_class cls ) ;

void obj_add_init_method( obj_method method, obj_class cls ) ;

void obj_add_deinit_method( obj_method method, obj_class cls ) ;

void obj_add_final_init_method( obj_method method, obj_class cls ) ;

void obj_add_final_deinit_method( obj_method method, obj_class cls ) ;

void obj_add_method( obj_method method, const char* name, obj_class cls ) ;

void obj_add_final_method( const char* name, obj_class cls ) ;

obj_method obj_get_method( AnyClass obj, const char* name ) ;

void obj_add_class_method( obj_method method, const char* name, obj_class cls ) ;

void obj_add_final_class_method( const char* name, obj_class cls ) ;

obj_method obj_get_class_method( obj_class cls, const char* name ) ;

obj_classdef obj_get_classdef_from_class( obj_class cls ) ;

int obj_verify_object_is_of_class( AnyClass obj, obj_class cls ) ;

void obj_alloc_private_store_for_object( AnyClass obj, obj_class cls ) ;

void obj_dealloc_private_store_for_object( AnyClass obj, obj_class cls ) ;

void obj_store_private_pointer( AnyClass obj, obj_class cls, void* pointer, const char* name ) ;

void* obj_get_private_pointer( AnyClass obj, obj_class cls, const char* name ) ;

#endif /* obj_h */
