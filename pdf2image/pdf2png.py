import os
import pdf2image

count = 0
src_folder = "pdfs"
img_folder = "images"

def gen_png(pdf_filename, pdf_info):

    global count
    maxPages = pdf_info["Pages"]
    for page in range(1, maxPages + 1, 10) : 
        images = pdf2image.convert_from_path(os.path.join(src_folder, pdf_filename), dpi=500, first_page=page, last_page = min(page+10-1,maxPages))
        for i, image in enumerate(images):
           fname = "images" + str(count).zfill(5) + ".png"
           image.save(os.path.join(img_folder, fname), "PNG")
           count = count + 1

if not os.path.exists(img_folder):
    os.makedirs(img_folder)

for pdf_filename in sorted(os.listdir(src_folder)):
	
    print("Working on pdf: " + pdf_filename)
    pdf_info = pdf2image.pdfinfo_from_path(os.path.join(src_folder, pdf_filename), userpw=None, poppler_path=None)
    gen_png(pdf_filename, pdf_info)

