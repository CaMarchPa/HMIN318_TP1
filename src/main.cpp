#include <iostream>
#include "CImg.h"

using namespace cimg_library;

int main(int argc, char ** argv) {

    CImg<unsigned char> image_read;

    if (argc < 2) {
        std::cout << "Usage : image.hdr" << std::endl;
        return EXIT_FAILURE;
    }

    //Initialisation
    //Lecture de l'image
    float voxelsize[3];
    image_read.load_analyze(argv[1], voxelsize);

    // seuillage
    CImg<unsigned char> image_thresholded = image_read.get_threshold(50, true, false);

    // erosion
    CImg<unsigned char> image_eroded = image_thresholded.get_erode(3, 3, 3);
    //for (int i = 0; i < 2; i++)
    image_eroded.erode(3, 3, 3);

	// ... identification de la composante connexe maximale
	CImg<unsigned char> image_mcc = image_eroded.get_label(true);
	int histo[500];
	//Récupération de l'histogramme
	for (int i=0; i < 500; i++) histo[i] = 0;
	cimg_for(image_mcc, val, unsigned char) {
        histo[*val]++;
    }

    //récupération de l'index de la valeur maximale dans l'histogramme
    int indexMax = 1;
    for (int i = 2; i < 500; i++) {
        if (histo[i] > histo[indexMax])
            indexMax = i;
    }
    //récupération de la composante
    unsigned char voxelC;
    cimg_for (image_mcc, val, unsigned char) {
        voxelC = *val;
        if (voxelC == indexMax)
            *val == 255;
        else
            *val == 0;
    }

	// dilatation
	CImg<unsigned char> image_dilated = image_mcc.get_dilate(3,3,3);
    //for (int i = 0; i < 2; i++)
    // image_dilated.dilate(3, 3, 3);

    CImgDisplay image_disp(image_dilated, "display");
    bool redraw = false;
    int threshold = 50;

    CImg<unsigned char> current_img = image_read;

    while (!image_disp.is_closed() && !image_disp.is_keyESC()) {
        image_disp.wait();
        // MCC
        if(image_disp.is_keyC()) {
            // ... identification de la composante connexe maximale
        	current_img = image_eroded.get_label(true);
        	int histo[500];
        	//Récupération de l'histogramme
        	for (int i=0; i < 500; i++) histo[i] = 0;
        	cimg_for(current_img, val, unsigned char) {
                histo[*val]++;
            }

            //récupération de l'index de la valeur maximale dans l'histogramme
            int indexMax = 1;
            for (int i = 2; i < 500; i++) {
                if (histo[i] > histo[indexMax])
                    indexMax = i;
            }
            //récupération de la composante
            unsigned char voxelC;
            cimg_for (current_img, val, unsigned char) {
                voxelC = *val;
                if (voxelC == indexMax)
                    *val == 255;
                else
                    *val == 0;
            }
            image_disp.display(current_img);
        }

        //threshold
        if (image_disp.is_keyARROWUP()) {
            threshold += 10;
            if (threshold >= 255) {
                threshold = 50;
            }
            current_img = current_img.get_threshold(threshold);
            image_disp.display(current_img);
        }

        if (image_disp.is_keyARROWDOWN()) {
            threshold -= 10;
            if(threshold < 0) {
                threshold = 50;
            }
            current_img = current_img.get_threshold(threshold);
            image_disp.display(current_img);
        }

        //erode
        if (image_disp.is_keyE()) {
            current_img = current_img.get_erode(3, 3, 3);
            image_disp.display(current_img);
        }

        //dilate
        if (image_disp.is_keyD()) {
            current_img = current_img.get_dilate(3, 3, 3);
            image_disp.display(current_img);
        }

        //initialize
        if (image_disp.is_keyI()) {
            current_img = current_img.load_analyze(argv[1], voxelsize);
            image_disp.display(current_img);
        }
    }



    return 0;
}
