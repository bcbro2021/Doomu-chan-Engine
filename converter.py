import sys
from PIL import Image

def png_to_ppm(input_path, output_path,name):
    try:
        with open(input_path, 'rb') as f:
            # Read the PNG file as binary
            png_data = f.read()

        img = Image.open(input_path)

        # Get the RGB data
        flat_pixels = list(img.getdata())

        # Open the output PPM file for writing
        with open(output_path, 'w') as ppm_file:
            # Write the PPM header
            ppm_file.write(f"int {name}[]="+"{\n")

            # Write the pixel values as integers
            for i in range(0, len(flat_pixels)):
                ppm_file.write(f"{flat_pixels[i][0]}, {flat_pixels[i][1]}, {flat_pixels[i][2]},\n")

            ppm_file.write("};");

        print(f"Conversion successful. PPM image saved at {output_path}")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    # Provide the input PNG file path and output PPM file path
    input_png_path = sys.argv[1]
    output_ppm_path = sys.argv[2]

    # Call the function to perform the conversion
    png_to_ppm(input_png_path, output_ppm_path, "sprites")