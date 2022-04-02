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

#ifndef BMP_FILE_STRUCTS_H
#define BMP_FILE_STRUCTS_H

#define __PACK __attribute__((__packed__))

// defines the structure of the bitmap header structure

struct _BitmapHeader {
	unsigned short bfType;           // Magic number for file
	unsigned long bfSize;            // Size of file.
	unsigned short bfReserved1;      // Reserved.
	unsigned short bfReserved2;      // Reserved.
	unsigned long bfOffBits;         // Offset to bitmap data.
} __PACK;

typedef struct _BitmapHeader BitmapHeader;

// defines the structure of the bitmap header info structure

typedef struct {
	unsigned long size;           // Size of info header.
	unsigned long width;          // Width of image.
	unsigned long height;         // Height of image.
	unsigned short planes;        // Number of color planes.
	unsigned short bitCount;      // Number of ts per pixel.
	unsigned long compression;    // Type of compression to use.
	unsigned long sizeImage;      // Size of image data.
	long xPixPerMeter;            // X pixels per meter.
	long yPixPerMeter;            // Y pixels per meter.
	unsigned long colorsUsed;     // Number of colors used.
	unsigned long colorsMain;     // Main colors used.
} BitmapHeaderInfo;

#endif
