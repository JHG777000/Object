//
//  Rectangle.c
//  Object
//
//  Created by Jacob Gordon on 4/16/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_class(Shape) ;

make_cls_available_for(Rectangle) ;

define_data_set(RectangleData, float length ; float width ; ) ;

start_method(InitRectangle, argfromlist(pos_x, obj_float) argfromlist(pos_y, obj_float) argfromlist(width, obj_float) argfromlist(length, obj_float))

 iclass$(obj, get_cls_for(Rectangle), InitShape, noargs) ;

 store_set(RectangleData) ;

 $(obj, set pos, pos_x, pos_y) ;

 $(obj, set size, width, length) ;

 $(obj, compute area, noargs) ;

end_method

start_method(DeinitRectangle, )

 iclass$(obj, get_cls_for(Rectangle), DeinitShape, noargs) ;

 destroy_set(RectangleData) ;

end_method

start_method(set_size, arg(width, obj_float) arg(length, obj_float))

 get_set(RectangleData)->width = width ;

 get_set(RectangleData)->length = length ;

end_method

start_method(get_size, arg(width, obj_float*) arg(length, obj_float*))

 *width = get_set(RectangleData)->width ;

 *length = get_set(RectangleData)->length ;

end_method

start_method(compute_area, )

 $(obj, set area, get_set(RectangleData)->width * get_set(RectangleData)->length) ;

end_method

new_class(Rectangle) {
    
    make_class_subclass_of(Shape) ;
    
    make_method_init(InitRectangle) ;
    
    make_method_deinit(DeinitRectangle) ;
    
    make_method_mask(get_size, get size) ;
    
    make_method_mask(set_size, set size) ;
    
    make_method_mask(compute_area, compute area) ;
    
    init_cls_for(Rectangle) ;
}

