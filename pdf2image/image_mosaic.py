import cv2
import os
import numpy as np

global black_img
src_folder = "images"

def load_images_from_folder(folder):
	
    images = []
    
    for filename in sorted(os.listdir(folder)):
        img = cv2.imread(os.path.join(folder,filename))
        if img is not None:
            images.append(img)
    return images
    
def resize(images, scale_percent):
 
    images_resized = []
    
    for img in images:
        #print('Original Dimensions: ',img.shape)   
        width = int(img.shape[1] * scale_percent / 100)
        height = int(img.shape[0] * scale_percent / 100)
        dim = (width, height)
        resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
        #print('Resized Dimensions: ',resized.shape)
        images_resized.append(resized)
		
    return images_resized

def pad_images_to_same_size(images):
    """
    :param images: sequence of images
    :return: list of images padded so that all images have same width and height (max width and height are used)
    """
    width_max = 0
    height_max = 0
    for img in images:
        h, w = img.shape[:2]
        width_max = max(width_max, w)
        height_max = max(height_max, h)

    if (height_max > width_max):
        width_max = height_max
    else:
        height_max = width_max
      
    global black_img
    black_img = np.zeros((height_max,width_max,3), np.uint8)

    images_padded = []
    for img in images:
        h, w = img.shape[:2]
        diff_vert = height_max - h
        pad_top = diff_vert//2
        pad_bottom = diff_vert - pad_top
        diff_hori = width_max - w
        pad_left = diff_hori//2
        pad_right = diff_hori - pad_left
        img_padded = cv2.copyMakeBorder(img, pad_top, pad_bottom, pad_left, pad_right, cv2.BORDER_CONSTANT, value=0)
        assert img_padded.shape[:2] == (height_max, width_max)
        images_padded.append(img_padded)

    return images_padded

def make_mosaic_image(images, index):
	
    mosaic_h1 = cv2.hconcat([images[index +  0], images[index +  1], images[index +  2], images[index +  3], images[index +  4]])
    mosaic_h2 = cv2.hconcat([images[index +  5], images[index +  6], images[index +  7], images[index +  8], images[index +  9]])
    mosaic_h3 = cv2.hconcat([images[index + 10], images[index + 11], images[index + 12], images[index + 13], images[index + 14]])
    mosaic_h4 = cv2.hconcat([images[index + 15], images[index + 16], images[index + 17], images[index + 18], images[index + 19]])
    mosaic_h5 = cv2.hconcat([images[index + 20], images[index + 21], images[index + 22], images[index + 23], images[index + 24]])

    mosaic = cv2.vconcat([mosaic_h1, mosaic_h2, mosaic_h3, mosaic_h4, mosaic_h5])
	
    return mosaic

global black_img

images = load_images_from_folder("images")
padded = pad_images_to_same_size(resize(images, 25))

image_count = len(padded)
if (image_count % 25 == 0):
	num_faces = image_count // 25
	num_black = 0
else:
	num_faces = (image_count // 25) + 1
	num_black = 25 - (image_count % 25)

for x in range(num_black):
	padded.append(black_img)

for x in range(num_faces):
    mosaic = make_mosaic_image(padded, x * 25)
    cv2.imwrite("face" + str(x+1) + ".bmp", mosaic)
