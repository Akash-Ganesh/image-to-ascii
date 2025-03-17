output: img_to_ascii.c stb_image.h stb_image_resize2.h
	gcc -Wall -Wextra -o exe.img_to_ascii img_to_ascii.c -lm

clean: 
	rm exe.img_to_ascii
