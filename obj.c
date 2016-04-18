/*
 Copyright (c) 2016 Jacob Gordon. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "obj.h"

struct obj_class_s { void* fast_data_structure ; RKStore methods ; RKStore final_methods ; RKStore class_methods ; RKStore final_class_methods ;
    
RKStore data ; obj_classdef the_classdef ; } ;

struct obj_object_s { void* fast_data_structure ; obj_class class_of_object ; RKStore data ; RKStore refs ; int refcount ; } ;

typedef struct obj_object_s* object ;

struct obj_ref_s { int strong ; object obj ; object base_obj ; } ;

typedef struct obj_ref_s* obj_ref ;

 obj_long obj_default_func(va_list external_arglist, const AnyClass obj, ...) {
    
    return 0 ;
}

AnyClass obj_object_alloc( obj_classdef the_classdef, ... ) {
    
    object obj = RKMem_NewMemOfType(struct obj_object_s) ;
    
    obj->class_of_object = the_classdef(0, NULL) ;
    
    obj->data = RKStore_NewStore() ;
    
    obj->refs = RKStore_NewStore() ;
    
    obj->refcount = 0 ;
    
    obj_arglist arglist ;
    
    start_arglist(arglist, the_classdef) ;
    
    ma(obj, init, arglist, noargs) ;
    
    end_arglist(arglist) ;
    
    return obj ;
}

static void obj_dealloc_refs( void* data ) {
    
    obj_ref ref = (obj_ref)data ;
    
    if ( (ref->strong) && (ref->obj != ref->base_obj) ) sub_ref_count(ref->obj) ;
    
    free(ref) ;
}

void obj_object_dealloc( AnyClass obj ) {
    
    object obj_ = (object)obj ;
    
    m(obj,deinit,noargs) ;
    
    obj_->class_of_object->the_classdef(-1, NULL) ;
    
    RKStore_DestroyStore(obj_->data) ;
    
    RKStore_IterateStoreWith(obj_dealloc_refs, obj_->refs) ;
    
    RKStore_DestroyStore(obj_->refs) ;
    
    free(obj) ;
}

void obj_store_object( AnyClass obj1, AnyClass obj2, const char* name, int strong ) {
    
    object obj1_ = (object)obj1 ;
    
    object obj2_ = (object)obj2 ;
    
    if ( RKStore_ItemExists(obj1_->refs, name) ) {
        
        obj_ref ref = RKStore_GetItem(obj1_->refs, name) ;
        
        if ( (ref->strong) && (ref->obj != obj1_) && (ref->obj != obj2_) ) sub_ref_count(ref->obj) ;
        
        free(ref) ;
    }
    
    obj_ref ref = RKMem_NewMemOfType(struct obj_ref_s) ;
    
    ref->base_obj = obj1_ ;
    
    ref->strong = strong ;
    
    ref->obj = obj2_ ;
    
    if ( (ref->strong) && (obj1_ != obj2_) ) add_ref_count(ref->obj) ;
    
    RKStore_AddItem(obj1_->refs, ref, name) ;
}

AnyClass obj_get_object( AnyClass obj, const char* name ) {
    
    object obj_ = (object)obj ;
    
    obj_ref ref = RKStore_GetItem(obj_->refs, name) ;
    
    if ( ref == NULL ) return NULL ;
    
    return ref->obj ;
}

void obj_store_pointer( AnyClass obj, void* pointer, const char* name ) {
    
    object obj_ = (object)obj ;
    
    RKStore_AddItem(obj_->data, pointer, name) ;
}

void* obj_get_pointer( AnyClass obj, const char* name ) {
    
    object obj_ = (object)obj ;
    
    return RKStore_GetItem(obj_->data, name) ;
}

void obj_add_ref_count( AnyClass obj ) {
    
    object obj_ = (object)obj ;
    
    if ( obj_ != NULL ) obj_->refcount++ ;
}

void obj_sub_ref_count( AnyClass obj ) {
    
    object obj_ = (object)obj ;
    
    if ( obj_ != NULL ) obj_->refcount-- ;
    
    if ( obj_ != NULL ) if ( obj_->refcount <= 0 ) free_object(obj) ;
}

obj_class obj_class_alloc( obj_classdef the_classdef ) {
    
    obj_class cls = RKMem_NewMemOfType(struct obj_class_s) ;
    
    cls->final_class_methods = RKStore_NewStore() ;
    
    cls->class_methods = RKStore_NewStore() ;
    
    cls->final_methods = RKStore_NewStore() ;
    
    cls->methods = RKStore_NewStore() ;
    
    cls->data = RKStore_NewStore() ;
    
    cls->the_classdef = the_classdef ;
    
    return cls ;
}

void obj_class_dealloc( obj_class cls ) {
    
    RKStore_DestroyStore(cls->final_class_methods) ;
    
    RKStore_DestroyStore(cls->class_methods) ;
    
    RKStore_DestroyStore(cls->final_methods) ;
    
    RKStore_DestroyStore(cls->methods) ;
    
    RKStore_DestroyStore(cls->data) ;
    
    free(cls) ;
}

void obj_class_store_pointer( obj_class cls, void* pointer, const char* name ) {
    
    RKStore_AddItem(cls->data, pointer, name) ;
}

void* obj_class_get_pointer( obj_class cls, const char* name ) {
    
    return RKStore_GetItem(cls->data, name) ;
}

void obj_add_method( obj_method method, const char* name, obj_class cls ) {
    
    if ( !(RKStore_ItemExists(cls->final_methods, name)) ) RKStore_AddItem(cls->methods, method, name) ;
}

void obj_add_final_method( const char* name, obj_class cls ) {
    
    RKStore_AddItem(cls->final_methods, NULL, name) ;
}

obj_method obj_get_method( AnyClass obj, const char* name ) {
    
    object obj_ = (object)obj ;
    
    obj_method method = NULL ;
    
    method = RKStore_GetItem(obj_->class_of_object->methods, name) ;
    
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

int obj_verify_object_is_of_class( AnyClass obj, obj_class cls ) {
    
    object obj_ = (object)obj ;
    
    if ( obj_->class_of_object == cls ) return 1 ;
    
    return 0 ;
}