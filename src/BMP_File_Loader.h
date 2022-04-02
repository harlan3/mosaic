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

#ifndef BMP_FILE_LOADER_H
#define BMP_FILE_LOADER_H

#include <string>

using namespace std;

class BMP_File_Loader {

public:

	bool LoadImage(string filename, unsigned char *&data, unsigned int &width,
			unsigned int &height);
};

#endif
