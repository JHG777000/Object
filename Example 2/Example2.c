//
//  Example2.c
//  Object
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_static_method(ShapeFactory) ;

void Exmaple2( void ) {
    
    obj_float area = 0 ;
    
    
    AnyClass circle = NULL ;
    
    smi(ShapeFactory, "Circle", &circle, 0.0f, 0.0f, 4.0f) ;
    
    $(circle, get area, &area) ;
    
    printf("Area: %f\n", area) ;
    
    
    AnyClass square = NULL ;
    
    smi(ShapeFactory, "Square", &square, 0.0f, 0.0f, 2.0f) ;
    
    $(square, get area, &area) ;
    
    printf("Area: %f\n", area) ;
    
    
    AnyClass rectangle = NULL ;
    
    smi(ShapeFactory, "Rectangle", &rectangle, 0.0f, 0.0f, 5.0f, 2.0) ;
    
    $(rectangle, get area, &area) ;
    
    printf("Area: %f\n", area) ;

    
    smi(ShapeFactory, "Destroy") ;
    
}