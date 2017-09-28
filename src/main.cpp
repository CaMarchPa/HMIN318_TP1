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
    CImg<unsigned char> image_thresholded = image_read.threshold(50, true, false);
    
    // erosion
    CImg<unsigned char> image_eroded = image_thresholded.erode(3, 3, 3);
	
	// ... identification de la composante connexe maximale
	CImg<unsigned char> image_mcc = image_eroded.label(true);
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
	CImg<unsigned char> image_dilated = image_mcc.dilate(3,3,3);
	
    CImgDisplay image_disp(image_dilated, "dilated");
    
    while (!image_disp.is_closed() && !image_disp.is_keyESC()) {
        image_disp.wait();
    }
    
    

    return 0;
}
