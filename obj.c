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

#include "obj.h"
#include "RKTasks.h"

struct obj_class_s { void* fast_data_structure ; obj_classdeinit classdeinit ; obj_method init ; obj_method deinit ;
    
obj_method final_init ; obj_method final_deinit ; RKStore methods ; RKStore final_methods ; RKStore class_methods ; RKStore final_class_methods ;
    
RKStore data ; RKStore private_stores ; obj_classdef the_classdef ; RKStore superclass_refs ; } ;

struct obj_object_s { void* fast_data_structure ; obj_class class_of_object ; RKStore data ; RKStore refs ; int refcount ; obj_ulong object_id ; } ;

struct obj_ref_s { int strong ; AnyClass obj ; AnyClass base_obj ; } ;

typedef struct obj_ref_s* obj_ref ;

static int get_obj_id( void ) {
    
    static RKT_AtomicInt count = 0 ;
    
    RKTasks_AtomicInc(&count) ;
    
    return count ;
}

static void ulong_to_string( obj_ulong val, char* string ) {
    
    snprintf(string, sizeof(string), "%lu", val) ;
}

 obj_long obj_default_func(va_list external_arglist, const AnyClass obj, ...) {
    
    return 0 ;
}

void obj_default_classdeinit( const obj_class cls ) {
    
    
}

AnyClass obj_object_alloc( obj_classdef the_classdef, ... ) {
    
    AnyClass obj = RKMem_NewMemOfType(struct obj_object_s) ;
    
    obj->class_of_object = the_classdef(0, NULL) ;
    
    obj->data = RKStore_NewStore() ;
    
    obj->refs = RKStore_NewStore() ;
    
    obj->refcount = 0 ;
    
    obj->object_id = get_obj_id() ;
    
    obj_arglist arglist ;
    
    start_arglist(arglist, the_classdef) ;
    
    if ( obj->class_of_object->final_init != get_null_method ) obj->class_of_object->init = obj->class_of_object->final_init ;
    
    obj->class_of_object->init(arglist,(AnyClass)obj,NULL,obj->class_of_object) ;
    
    end_arglist(arglist) ;
    
    return obj ;
}

static void obj_dealloc_refs( void* data ) {
    
    obj_ref ref = (obj_ref)data ;
    
    if ( (ref->strong) && (ref->obj != ref->base_obj) ) sub_ref_count(ref->obj) ;
    
    free(ref) ;
}

void obj_object_dealloc( AnyClass obj ) {
    
    if ( obj->class_of_object->final_deinit != get_null_method ) obj->class_of_object->deinit = obj->class_of_object->final_deinit ;
    
    obj->class_of_object->deinit(NULL,(AnyClass)obj,NULL,obj->class_of_object) ;
    
    obj->class_of_object->the_classdef(-1, NULL) ;
    
    RKStore_DestroyStore(obj->data) ;
    
    RKStore_IterateStoreWith(obj_dealloc_refs, obj->refs) ;
    
    RKStore_DestroyStore(obj->refs) ;
    
    free(obj) ;
}

void obj_store_object( AnyClass obj1, AnyClass obj2, const char* name, int strong ) {
    
    if ( name != NULL ) {
    
      if ( RKStore_ItemExists(obj1->refs, name) ) {
        
          obj_ref ref = RKStore_GetItem(obj1->refs, name) ;
        
          if ( (ref->strong) && (ref->obj != obj1) && (ref->obj != obj2) ) sub_ref_count(ref->obj) ;
        
          free(ref) ;
      }
        
    }
    
    obj_ref ref = RKMem_NewMemOfType(struct obj_ref_s) ;
    
    ref->base_obj = obj1 ;
    
    ref->strong = strong ;
    
    ref->obj = obj2 ;
    
    if ( (ref->strong) && (obj1 != obj2) ) add_ref_count(ref->obj) ;
    
    if ( name == NULL ) {
        
        RKStore_AddItemToList(obj1->refs, ref) ;
        
        return ;
    }
    
    RKStore_AddItem(obj1->refs, ref, name) ;
}

AnyClass obj_get_object( AnyClass obj, const char* name ) {
    
    obj_ref ref = RKStore_GetItem(obj->refs, name) ;
    
    if ( ref == NULL ) return NULL ;
    
    return ref->obj ;
}

void obj_store_pointer( AnyClass obj, void* pointer, const char* name ) {
    
    RKStore_AddItem(obj->data, pointer, name) ;
}

void* obj_get_pointer( AnyClass obj, const char* name ) {
    
    return RKStore_GetItem(obj->data, name) ;
}

void obj_add_ref_count( AnyClass obj ) {
    
    if ( obj != NULL ) obj->refcount++ ;
}

void obj_sub_ref_count( AnyClass obj ) {
    
    if ( obj != NULL ) obj->refcount-- ;
    
    if ( obj != NULL ) if ( obj->refcount <= 0 ) free_object(obj) ;
}

obj_class obj_class_alloc( obj_classdef the_classdef ) {
    
    obj_class cls = RKMem_NewMemOfType(struct obj_class_s) ;
    
    cls->final_init = obj_default_func ;
    
    cls->final_deinit = obj_default_func ;
    
    cls->init = obj_default_func ;
    
    cls->deinit = obj_default_func ;
    
    cls->classdeinit = obj_default_classdeinit ;
    
    cls->final_class_methods = RKStore_NewStore() ;
    
    cls->class_methods = RKStore_NewStore() ;
    
    cls->final_methods = RKStore_NewStore() ;
    
    cls->methods = RKStore_NewStore() ;
    
    cls->data = RKStore_NewStore() ;
    
    cls->private_stores = RKStore_NewStore() ;
    
    cls->the_classdef = the_classdef ;
    
    cls->superclass_refs = RKStore_NewStore() ;
    
    return cls ;
}

static void obj_dealloc_superclass_refs( void* data ) {
    
    obj_class cls = (obj_class)data ;
    
    cls->the_classdef(-1, NULL) ;
}

void obj_class_dealloc( obj_class cls ) {
    
    cls->classdeinit(cls) ;
    
    RKStore_DestroyStore(cls->final_class_methods) ;
    
    RKStore_DestroyStore(cls->class_methods) ;
    
    RKStore_DestroyStore(cls->final_methods) ;
    
    RKStore_DestroyStore(cls->methods) ;
    
    RKStore_DestroyStore(cls->data) ;
    
    RKStore_DestroyStore(cls->private_stores) ;

    RKStore_IterateStoreWith(obj_dealloc_superclass_refs, cls->superclass_refs) ;
    
    RKStore_DestroyStore(cls->superclass_refs) ;
    
    free(cls) ;
}

void obj_add_class_ref_to_subclass( obj_class cls, const char* classname, obj_class subclass) {
    
    RKStore_AddItem(subclass->superclass_refs, cls, classname) ;
}

void obj_class_store_pointer( obj_class cls, void* pointer, const char* name ) {
    
    RKStore_AddItem(cls->data, pointer, name) ;
}

void* obj_class_get_pointer( obj_class cls, const char* name ) {
    
    return RKStore_GetItem(cls->data, name) ;
}

void obj_add_classdeinit_method( obj_classdeinit classdeinit, obj_class cls ) {
    
    cls->classdeinit = classdeinit ;
}

void obj_add_init_method( obj_method method, obj_class cls ) {
    
    if ( cls->final_init == get_null_method ) cls->init = method ;
}

void obj_add_deinit_method( obj_method method, obj_class cls ) {
    
    if ( cls->final_deinit == get_null_method ) cls->deinit = method ;
}

void obj_add_final_init_method( obj_method method, obj_class cls ) {
    
    if ( cls->final_init == get_null_method ) cls->final_init = method ;
}

void obj_add_final_deinit_method( obj_method method, obj_class cls ) {
    
    if ( cls->final_deinit == get_null_method ) cls->final_deinit = method ;
}

void obj_add_method( obj_method method, const char* name, obj_class cls ) {
    
    if ( !(RKStore_ItemExists(cls->final_methods, name)) ) RKStore_AddItem(cls->methods, method, name) ;
}

void obj_add_final_method( const char* name, obj_class cls ) {
    
    RKStore_AddItem(cls->final_methods, NULL, name) ;
}

obj_method obj_get_method( AnyClass obj, const char* name ) {
    
    obj_method method = NULL ;
    
    method = RKStore_GetItem(obj->class_of_object->methods, name) ;
    
    if ( method == NULL ) return obj_default_func ;
    
    return method ;
}

void obj_add_class_method( obj_method method, const char* name, obj_class cls ) {
    
    if ( !(RKStore_ItemExists(cls->final_class_methods, name)) ) RKStore_AddItem(cls->class_methods, method, name) ;
}

void obj_add_final_class_method( const char* name, obj_class cls ) {
    
    RKStore_AddItem(cls->final_class_methods, NULL, name) ;
}

obj_method obj_get_class_method( obj_class cls, const char* name ) {
    
    obj_method method = NULL ;
    
    method = RKStore_GetItem(cls->class_methods, name) ;
    
    if ( method == NULL ) return obj_default_func ;
    
    return method ;
}

obj_classdef obj_get_classdef_from_class( obj_class cls ) {
    
    return cls->the_classdef ;
}

int obj_verify_object_is_of_class( AnyClass obj, obj_class cls ) {
    
    if ( obj->class_of_object == cls ) return 1 ;
    
    return 0 ;
}

void obj_alloc_private_store_for_object( AnyClass obj, obj_class cls ) {
    
    RKStore private_store = RKStore_NewStore() ;
    
    char id_string[100] ;
    
    ulong_to_string(obj->object_id, id_string) ;
    
    RKStore_AddItem(cls->private_stores, private_store, id_string) ;
}

void obj_dealloc_private_store_for_object( AnyClass obj, obj_class cls ) {
    
    char id_string[100] ;
    
    ulong_to_string(obj->object_id, id_string) ;
    
    RKStore private_store = RKStore_GetItem(cls->private_stores, id_string) ;
    
    RKStore_DestroyStore(private_store) ;
    
    RKStore_RemoveItem(cls->private_stores, id_string) ;
}

void obj_store_private_pointer( AnyClass obj, obj_class cls, void* pointer, const char* name ) {
    
    char id_string[100] ;
    
    ulong_to_string(obj->object_id, id_string) ;
    
    RKStore private_store = RKStore_GetItem(cls->private_stores, id_string) ;
    
    RKStore_AddItem(private_store, pointer, name) ;
}

void* obj_get_private_pointer( AnyClass obj, obj_class cls, const char* name ) {
    
    char id_string[100] ;
    
    ulong_to_string(obj->object_id, id_string) ;
    
    RKStore private_store = RKStore_GetItem(cls->private_stores, id_string) ;
    
    return RKStore_GetItem(private_store, name) ;
}
