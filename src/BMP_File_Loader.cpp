/*
 * Mosaic (OpenGL Cube Surface Viewer)
 * Copyright (c) 2022 Harlan Murphy - Orbisoftware
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "BMP_File_Loader.h"
#include "BMP_File_Structs.h"

using namespace std;

bool BMP_File_Loader::LoadImage(string filename, unsigned char *&data,
		unsigned int &width, unsigned int &height) {

	FILE *file;
	unsigned long offset, index;
	unsigned int datasize, scanline_size, cur_scanline, bytes_per_pixel;
	unsigned char *scanline_data;

	BitmapHeaderInfo bmpHeader;
	if ((file = fopen(filename.c_str(), "rb")) == NULL) {
		cerr << "Could not open " << filename << " for reading." << std::endl;
		return false;
	}

	// read bitmap data offset
	fseek(file, 10, SEEK_SET);
	fread(&offset, sizeof(int), 1, file);

	// image properties
	fseek(file, 18, SEEK_SET);
	fread(&bmpHeader.width, sizeof(int), 1, file);
	fread(&bmpHeader.height, sizeof(int), 1, file);
	fread(&bmpHeader.planes, sizeof(short int), 1, file);

	if (bmpHeader.planes != 1) {
		cerr << "BMP File Loader: planes from " << filename << " is not 1: "
				<< bmpHeader.planes << endl;
		return false;
	}

	fread(&bmpHeader.bitCount, sizeof(unsigned short int), 1, file);
	if (bmpHeader.bitCount != 24) {
		cerr << "BMP File Loader: bmp depth from " << filename << " is not 24: "
				<< bmpHeader.bitCount << endl;
		return false;
	}

	bytes_per_pixel = bmpHeader.bitCount / 8;

	// move forward to bitmap data offset
	fseek(file, offset, SEEK_SET);
	width = bmpHeader.width;
	height = bmpHeader.height;

	scanline_size = (width * bytes_per_pixel) + (width % 4);
	datasize = scanline_size * height;

	data = (unsigned char*) malloc(datasize);
	if (data == NULL) {
		cerr << "Error allocating memory for image data" << endl;
		return false;
	}

	scanline_data = (unsigned char*) malloc(scanline_size);
	if (scanline_data == NULL) {
		cerr << "Error allocating memory for scanline data" << endl;
		return false;
	}

	cur_scanline = height;

	// read one scan line at a time
	while (cur_scanline > 0) {

		cur_scanline--;

		if (fread(scanline_data, scanline_size, 1, file) != 1) {

			cerr << "Error reading scanline from " << filename << endl;
			return false;
		}

		for (unsigned long i = 0; i < scanline_size; i += bytes_per_pixel) {

			index = (cur_scanline * scanline_size) + i;

			// byte swizzle
			for (unsigned int j = 0; j < bytes_per_pixel; j++) {

				data[index + j] =
						scanline_data[i + ((bytes_per_pixel - 1) - j)];
			}
		}
	}

	free(scanline_data);

	fclose(file);

	return true;
}
