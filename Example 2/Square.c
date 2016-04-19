//
//  Square.c
//  Object
//
//  Created by Jacob Gordon on 4/16/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_class(Shape) ;

define_data_set(SquareData, float size ; ) ;

start_method(InitSquare, argfromlist(pos_x, obj_float) argfromlist(pos_y, obj_float) argfromlist(size, obj_float) arg(cls, obj_class))

 cm(obj, InitShape, noargs) ;

 store_set(SquareData) ;

 m(obj, set pos, pos_x, pos_y) ;

 m(obj, set size, size) ;

 m(obj, compute area, noargs) ;

end_method

start_method(DeinitSquare, arg(cls, obj_class))

 cm(obj, DeinitShape, noargs) ;

 destroy_set(SquareData) ;

end_method

start_method(set_size, arg(size,obj_float))

 get_set(SquareData)->size = size ;

end_method

start_method(get_size, arg(size,obj_float*))

 *size = get_set(SquareData)->size ;

end_method

start_method(compute_area, )

 m(obj, set area, get_set(SquareData)->size * get_set(SquareData)->size) ;

end_method

new_class(Square) {
    
    make_class_subclass_of(Shape) ;
    
    make_method_init(InitSquare) ;
    
    make_method_deinit(DeinitSquare) ;
    
    make_method_mask(get_size, get size) ;
    
    make_method_mask(set_size, set size) ;
    
    make_method_mask(compute_area, compute area) ;
}

