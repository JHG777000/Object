/*
 Copyright (c) 2016 Jacob Gordon. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef obj_h
#define obj_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "RKMem.h"

typedef struct obj_class_s* obj_class ;

typedef void* AnyClass ;

typedef obj_class (*obj_classdef)(int mode, obj_class subclass) ;

typedef int (*obj_method)(va_list external_arglist, const AnyClass obj, ...) ;

typedef int (*obj_classmethod)(va_list external_arglist, const AnyClass obj, const obj_class cls, ...) ;

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

#define obj_bitsizeof(type) sizeof(type) * CHAR_BIT

#define new_class(classname)\
static void classname##_init_method(const obj_class cls) ;\
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
   classname##_init_method(cls) ;\
   }\
 return cls ;\
 }\
 if (mode == 1) {\
  classname##_init_method(subclass) ;\
  return subclass ;\
 }\
return NULL ;\
}\
static void classname##_init_method(const obj_class cls)

#define new_private_class(classname)\
static void classname##_init_method(const obj_class cls) ;\
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
classname##_init_method(cls) ;\
}\
return cls ;\
}\
if (mode == 1) {\
classname##_init_method(subclass) ;\
return subclass ;\
}\
return NULL ;\
}\
static void classname##_init_method(const obj_class cls)

#define make_class_subclass_of(superclass) Def##superclass(1, cls)

#define make_cls_available_for(class) static obj_class class##_cls = NULL

#define init_cls_for(class) class##_cls = cls

#define get_cls_for(class) class##_cls

#define use_class(classname) obj_class Def##classname( int mode, obj_class subclass ) ; \
typedef struct Obj##classname##_s* classname

#define use_private_class(classname) static obj_class Def##classname( int mode, obj_class subclass ) ; \
typedef struct Obj##classname##_s* classname

#define use_static_method(methodname) void methodname(va_list external_arglist, const AnyClass obj, ...)

#define make_method_public(method) obj_add_method(method,#method,cls)

#define make_method_mask(method,methodname) obj_add_method(method,#methodname,cls)

#define make_method_final(methodname) obj_add_final_method(#methodname,cls)

#define make_method_init(method) make_method_mask(method,init)

#define make_method_deinit(method) make_method_mask(method,deinit)

#define make_class_method_public(method) obj_add_class_method((obj_method)method,#method,cls)

#define make_class_method_mask(method,methodname) obj_add_class_method((obj_method)method,#methodname,cls)

#define make_class_method_final(methodname) obj_add_final_class_method(#methodname,cls)

#define new_object(class,...) (class)obj_object_alloc(Def##class,__VA_ARGS__)

#define new_any_object(class,...) obj_object_alloc(Def##class,__VA_ARGS__)

#define new_object_with_classdef(classdef,...) obj_object_alloc(classdef,__VA_ARGS__)

#define free_object(obj) obj_object_dealloc((AnyClass)obj)

#define get_classdef(class) Def##class

#define store_object_strong(object1,name,object2) obj_store_object(object1,object2,#name,1)

#define store_object_weak(object1,name,object2) obj_store_object(object1,object2,#name,0)

#define sos(object1,name,object2) store_object_strong(object1,name,object2)

#define sow(object1,name,object2) store_object_weak(object1,name,object2)

#define get_object(object,name) obj_get_object(object, #name)

#define getobj(object,name) get_object(object,name)

#define store_pointer(object,name,pointer) obj_store_pointer(object,pointer,#name)

#define sp(object,name,pointer) store_pointer(object,name,pointer)

#define get_pointer(object,name) obj_get_pointer(object,#name)

#define gp(object,name) get_pointer(object,name)

#define class_store_pointer(class,name,pointer) obj_class_store_pointer(class,pointer,#name)

#define class_sp(class,name,pointer) class_store_pointer(class,name,pointer)

#define class_get_pointer(class,name) obj_class_get_pointer(class,#name)

#define class_gp(class,name) obj_class_get_pointer(class,name)

#define add_ref_count(object) obj_add_ref_count(object)

#define sub_ref_count(object) obj_sub_ref_count(object)

#define arg(name,type) type name = va_arg(method_arglist,type) ;

#define argfromlist(name,type) type name ; if (external_arglist != NULL) {\
name = va_arg(external_arglist,type) ;\
} else { name = va_arg(method_arglist,type) ;}

#define noargs 0

#define start_method(method_name,method_args) static int method_name(va_list external_arglist, const AnyClass obj,...) {\
va_list method_arglist ;\
va_start(method_arglist,obj) ;\
arg(obj_nullptr,void * const)\
if (obj_nullptr != NULL) return -1 ;\
method_args\

// static method equals non static function
#define start_static_method(method_name,method_args) int method_name(va_list external_arglist, const AnyClass obj,...) {\
va_list method_arglist ;\
va_start(method_arglist,obj) ;\
arg(obj_nullptr,void * const)\
if (obj_nullptr != NULL) return -1 ;\
method_args\

#define start_class_method(method_name,method_args) static int method_name(va_list external_arglist, const AnyClass obj, const obj_class cls,...) {\
va_list method_arglist ;\
va_start(method_arglist,cls) ;\
if (cls == NULL) {printf("Object Runtime: Error, access control violation or corrupted 'cls' pointer.\n");exit(EXIT_FAILURE);}\
void * const obj_nullptr = NULL ;\
if (obj_nullptr != NULL) return -1 ;\
method_args\

#define end_method va_end(method_arglist) ; return 1 ; }

#define start_arglist(arglist, last_arg) va_start(arglist, last_arg)

#define end_arglist(arglist) va_end(arglist)

#define get_null_method obj_default_func

#define null_method_error -1

#define get_method(obj,method) obj_get_method(obj,#method)

#define get_class_method(cls,method) obj_get_class_method(cls,#method)

#define $(obj,method,...) obj_get_method(obj,#method)(NULL,(AnyClass)obj,NULL,__VA_ARGS__)

#define $$(obj,method,...) method(NULL,(AnyClass)obj,NULL,__VA_ARGS__)

#define $$$(obj,method,arglist,...) obj_get_method(obj,#method)(arglist,(AnyClass)obj,NULL,__VA_ARGS__)

#define $$$$(obj,method,arglist,...) method(arglist,(AnyClass)obj,NULL,__VA_ARGS__)

#define class$(obj,method,...) ((obj_classmethod)obj_get_class_method(cls,#method))(NULL,(AnyClass)obj,cls,__VA_ARGS__)

#define class$$(obj,method,...) ((obj_classmethod)method)(NULL,(AnyClass)obj,cls,__VA_ARGS__)

#define class$$$(obj,method,arglist,...) ((obj_classmethod)obj_get_class_method(cls,#method))(arglist,(AnyClass)obj,cls,__VA_ARGS__)

#define class$$$$(obj,method,arglist,...) ((obj_classmethod)method)(arglist,(AnyClass)obj,cls,__VA_ARGS__)

#define invoke_class$(obj,class,method,...) ((obj_classmethod)obj_get_class_method(class,#method))(NULL,(AnyClass)obj,class,__VA_ARGS__)

#define invoke_class$$(obj,class,method,...) ((obj_classmethod)method)(NULL,(AnyClass)obj,class,__VA_ARGS__)

#define invoke_class$$$(obj,class,method,arglist,...) ((obj_classmethod)obj_get_class_method(class,#method))(arglist,(AnyClass)obj,class,__VA_ARGS__)

#define invoke_class$$$$(obj,class,method,arglist,...) ((obj_classmethod)method)(arglist,(AnyClass)obj,class,__VA_ARGS__)

#define iclass$(obj,class,method,...) invoke_class$(obj,class,method,__VA_ARGS__)

#define iclass$$(obj,class,method,...) invoke_class$$(obj,class,method,__VA_ARGS__)

#define iclass$$$(obj,class,method,arglist,...) invoke_class$$$(obj,class,method,arglist,__VA_ARGS__)

#define iclass$$$$(obj,class,method,arglist,...) invoke_class$$$$(obj,class,method,arglist,__VA_ARGS__)

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

#define get_fds_class get_fast_data_store_class(type)

#define get_fds_of(entity,type) get_fast_data_store_of(entity,type)

#define is_object_of_class(obj,cls) obj_verify_object_is_of_class(obj,cls)

int obj_default_func(va_list external_arglist, const AnyClass obj, ...) ;

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

void obj_add_method( obj_method method, const char* name, obj_class cls ) ;

void obj_add_final_method( const char* name, obj_class cls ) ;

obj_method obj_get_method( AnyClass obj, const char* name ) ;

void obj_add_class_method( obj_method method, const char* name, obj_class cls ) ;

void obj_add_final_class_method( const char* name, obj_class cls ) ;

obj_method obj_get_class_method( obj_class cls, const char* name ) ;

int obj_verify_object_is_of_class( AnyClass obj, obj_class cls ) ;

#endif /* obj_h */
