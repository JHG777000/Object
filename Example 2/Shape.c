//
//  Shape.c
//  Object
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

declare_class(Shape) ;

define_data_set(ShapeData, float pos_x ; float pos_y ; float area ; ) ;

start_method(InitShape, )

 store_set(ShapeData) ;

end_method

start_method(DeinitShape, )

 destroy_set(ShapeData) ;

end_method

start_method(get_area, arg(retval, obj_float*))

 *retval = get_set(ShapeData)->area ;

end_method

start_method(set_area, arg(area, obj_float))

 get_set(ShapeData)->area = area ;

end_method

start_method(compute_area, )

end_method

start_method(get_pos, arg(ret_pos_x, obj_float*) arg(ret_pos_y, obj_float*))

 *ret_pos_x = get_set(ShapeData)->pos_x ;

 *ret_pos_y = get_set(ShapeData)->pos_y ;

end_method

start_method(set_pos, arg(pos_x, obj_float) arg(pos_y, obj_float))

 get_set(ShapeData)->pos_x = pos_x ;

 get_set(ShapeData)->pos_y = pos_y ;

end_method

new_class(Shape) {
    
    make_protected_method(InitShape) ;
    
    make_protected_method(DeinitShape) ;
    
    make_method_mask(get_area, get area) ;
    
    make_method_mask(set_area, set area) ;
    
    make_method_mask(compute_area, compute area) ;
    
    make_method_mask(get_pos, get pos) ;
    
    make_method_mask(set_pos, set pos) ;
}