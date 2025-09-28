from PIL import Image

# Load and convert image to 1-bit monochrome
img = Image.open("Unit4/i2c_with_ssd1306/images/luffy.jpg").convert("1").resize((128, 64))
pixels = img.load()

# Prepare the screen array
screen = [[0 for _ in range(128)] for _ in range(8)]

for page in range(8):
    for x in range(128):
        byte = 0
        for bit in range(8):
            y = page * 8 + bit
            if pixels[x, y] == 0:  # black pixel
                byte |= (1 << bit)
        screen[page][x] = byte

# print C-style array
print("uint8_t screen[8][128] = {")
for page in range(8):
    print(" {", end="")
    print(", ".join(f"0x{screen[page][x]:02X}" for x in range(128)), end="")
    print(" },")
print("};")
