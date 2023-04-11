import os
from PIL import Image

# Assuming the sprites are squares and the spritesheets are a single row



def convert_spritesheet(filepath: str) -> None:
    outdir = os.path.splitext(filepath)[0]
    os.makedirs(outdir, exist_ok=True)

    img = Image.open(filepath)
    size = img.height
    n_frames = img.width // size

    for i in range(n_frames):
        outpath = os.path.join(outdir, f"{i}.png")
        crop = img.crop((i*size, 0, i*size+size, size))

        crop.save(outpath)

def convert_recursive(root: str) -> None:
    for dirpath, _, filenames in os.walk(root):
        dirname = os.path.basename(dirpath)
        
        if dirname == "animations":
            for filename in filenames:
                if not filename.endswith(".png"):
                    continue

                filepath = os.path.join(dirpath, filename)
                convert_spritesheet(filepath)



def main():
    root = os.path.join(os.getcwd(), "sprites")
    convert_recursive(root)

if __name__ == "__main__":
    main()
