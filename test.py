from PIL import Image

def png_to_binary_list(file_path):
    try:
        # Open the image file
        img = Image.open(file_path)

        # Convert the image to grayscale
        img = img.convert("L")

        # Get pixel data as a list of tuples
        pixel_data = list(img.getdata())

        # Convert pixel data to binary list
        binary_list = [1 if pixel == 0 else 0 for pixel in pixel_data]

        return binary_list

    except Exception as e:
        print(f"Error: {e}")
        return None

# Example usage
file_path = "./src/player1.bmp"
result = png_to_binary_list(file_path)

if result is not None:
    print(result)