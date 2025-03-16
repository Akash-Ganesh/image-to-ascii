#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
#include "stb_image.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

char ascii[65] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1}[?-_+~<>i!lI;:^`'.";

void pixel(int r, int g, int b, int upper, int lower, float r_bi, float g_bi, float b_bi){
	int intensity = (r+g+b)/(3*(256/(upper-lower)));
	char final_char = ascii[upper-intensity];
	int r_check = (r>(256*r_bi))?1:0;
	int g_check = (g>(256*g_bi))?1:0;
	int b_check = (b>(256*b_bi))?1:0;
	int color = r_check + g_check*2 + b_check*4;
	char color_char = color + 48;
	printf("\e[0;3%cm%c\e[0m",color_char,final_char);
}

void loadImage(const char *filename, unsigned char **imageData, int *width, int *height, int *channels) { 
    *imageData = stbi_load(filename, width, height, channels, 0); 
    if (*imageData == NULL) { 
        printf("Error in loading the image or image does not exist\n"); 
        exit(1); 
    } 
} 
 
void freeImage(unsigned char *imageData) { 
    stbi_image_free(imageData); 
} 
 
int main(int argc, char* argv[]) {
    int opt; 
    
	char *filename;
    unsigned char *imageData; 
    int width, height, channels;
	int dim_w = 0;
	int dim_h = 0;
	int upper = 64;
	int lower = 0;
	float r_bi = 0.5;
	float g_bi = 0.5;
	float b_bi = 0.5;

    while((opt = getopt(argc, argv, ":f:u:l:r:g:b:w:h:")) != -1)  
    {  
        switch(opt)
        {  
            case 'f':  
				filename = optarg;
				break;
            case 'u':
				upper = atoi(optarg);
				break;
            case 'l':
				lower = atoi(optarg);
				break;
            case 'r':
				r_bi = atof(optarg);
                break;  
            case 'g':
				g_bi = atof(optarg);
                break;  
            case 'b':
				b_bi = atof(optarg);
                break;  
            case 'w':
				dim_w = atoi(optarg);
				break;
            case 'h':
				dim_h = atoi(optarg);
                break;  
            case ':':
				printf("Enter value for argument (I wanna add the help thing here)\n");
				exit(1);
                break;  
            case '?': 
				printf("Unknown flag %c\n",optopt);
				exit(1);
                break;  
        }  
    }
	printf("Filename: %s\n",filename);
	printf("Upper: %d\n",upper);
	printf("Lower: %d\n",lower);
	printf("Red: %f\n",1-r_bi);
	printf("Green: %f\n",1-g_bi);
	printf("Blue: %f\n",1-b_bi);

    loadImage(filename, &imageData, &width, &height, &channels);
	dim_w = (dim_w==0)?width:dim_w;
	dim_h = (dim_h==0)?height:dim_h;

	printf("Width: %d\n",dim_w);
	printf("Height: %d\n",dim_h);

	int stride = 0;
    unsigned char *resizeData = (unsigned char *)malloc(sizeof(unsigned char) * dim_w * dim_h * channels); 

	stbir_resize_uint8_linear(imageData, width, height, stride, resizeData, dim_w, dim_h, stride, 3);

	for(int i = 0; i<dim_h; i++){
		for(int j = 0; j<dim_w; j++){
			(pixel(resizeData[channels*(dim_w*i+j)], resizeData[channels*(dim_w*i+j)+1], resizeData[channels*(dim_w*i+j)+2], upper, lower, 1 - r_bi, 1 - g_bi, 1 - b_bi));
		}
		printf("\n");
	}

    freeImage(imageData); 
	free(resizeData);
    return 0; 
}
