#include <anonymous_image.h>
#include <matrix_image.h>
#include <windows_lib.h>
#include <image_lib.h>

void displayImage(Image image, int x, int y){

    switch (image){
        case ANONYMOUS:
            drawImage(x, y, anon_img, anon_width, anon_height);
            break;
        
        default:
            drawImage(x, y, matrix_image, matrix_image_width, matrix_image_height);
            break;
    }

}