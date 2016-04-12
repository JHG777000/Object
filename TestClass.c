//
//  TestClass.c
//  Object
//
//  Created by Jacob Gordon on 10/10/15.
//  Copyright © 2015 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_class(BaseClass) ;

use_class(StringClass) ;

make_cls_available_for(TestClass) ;

typedef struct TestClassFDS_s { int value ; } *TestClassFDS ;

start_method( my_init_method, argfromlist(myfloat, obj_float) )

    printf("my init method: %f\n", myfloat) ;

    obj_float* myptrtofloat = RKMem_NewMemOfType(obj_float) ;

    *myptrtofloat = myfloat ;

    sp(obj, myfloat, myptrtofloat) ;

    fds = RKMem_NewMemOfType(struct TestClassFDS_s) ;

    get_fds(TestClassFDS)->value = 22 ;

end_method

start_method( my_deinit_method, )

    obj_float *myptrtofloat = get_pointer(obj, myfloat) ;

    printf("my deinit method: %f\n", *myptrtofloat) ;

    free(myptrtofloat) ;

    free(fds) ;

    $(getobj(obj, mystring),print,noargs) ;

end_method

start_method( my_first_method, arg(myint, obj_interger) )

    printf("my first method: %d\n", myint) ;

end_method

start_method( my_private_method, arg(myint, obj_interger) )

    printf("my private method: %d\n", myint) ;

    printf("my private method fds: %d\n", get_fds(TestClassFDS)->value) ;

end_method

start_method( my_method, arg(myint,int) arg(myint2, obj_interger) )

    $(obj, my_first_method, myint) ;

    $$(obj, my_first_method, myint2) ;

    $$(obj, my_private_method, myint) ;

    $$(obj, my_private_method, myint2) ;

    StringClass string = new_object(StringClass, "My String!\n") ;

    sos(obj, mystring, string) ;

end_method

start_class_method(print_hello_world_from_test,)

   class$(obj, print_hello_world, noargs) ;

end_method

start_method(print_hello,)

   iclass$(obj, get_cls_for(TestClass), print_hello_world_from_test, noargs) ;

end_method

new_class(TestClass) {
    
    make_class_subclass_of(BaseClass) ;
    
    make_method_init(my_init_method) ;
    
    make_method_public(my_first_method) ;
    
    make_method_public(my_method) ;
    
    make_method_deinit(my_deinit_method) ;
    
    make_class_method_public(print_hello_world_from_test) ;
    
    make_method_public(print_hello) ;
    
    init_cls_for(TestClass) ;
}