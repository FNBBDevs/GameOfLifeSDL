import io
import requests
import pandas as pd

def generate_color_maps():
    color_maps = {}
    color_map_names = ['amber', 'amethyst', 'apple', 'arctic', 'bubblegum', 'chroma', 'copper', 'cosmic', 'dusk', 'eclipse',
                    'emerald', 'emergency', 'fall', 'flamingo', 'freeze', 'fusion', 'gem', 'ghostlight', 'gothic', 'guppy', 
                    'holly', 'horizon', 'iceburn', 'infinity', 'jungle', 'lavender', 'lilac', 'neon', 'neutral', 'nuclear', 
                    'ocean', 'pepper', 'pride', 'prinsenvlag', 'rainforest', 'redshift', 'sapphire', 'savanna', 'seasons', 
                    'seaweed', 'sepia', 'sunburst', 'swamp', 'torch', 'toxic', 'tree', 'tropical', 'viola', 'voltage', 'waterlily',
                    'watermelon', 'wildfire']

    for name in color_map_names:
        print(f"Getting '{name}' ->", end=' ')
        download = requests.get(f'https://raw.githubusercontent.com/1313e/CMasher/master/cmasher/colormaps/{name}/{name}_8bit.txt')
        print(download.status_code)
        if download.status_code != 404:
            data = [[int(val) for val in line.strip().split(' ')] for line in io.StringIO(download.content.decode('utf-8')).readlines()]
        color_maps[name] = data
        color_maps[f'{name}Reverse'] = data[::-1]
    
    return color_maps


def decimal_to_255(rgb):
    return rgb


if __name__ == "__main__":
    color_maps = generate_color_maps()
    with open('colors.h', 'w') as f:
        for i, color_map in enumerate(color_maps):
            f.write(f"#define {color_map}Size {len(color_maps[color_map])}\n")
            f.write(f'int {color_map}[{len(color_maps[color_map])}][4] = ' + '{\n')
            for j in range(len(color_maps[color_map])):
                if j == len(color_maps[color_map])-1:break
                f.write("\t{" + f"{str(decimal_to_255(color_maps[color_map][j]))[1:-1]}" + "},\n")
            f.write("\t{" + f"{str(decimal_to_255(color_maps[color_map][len(color_maps[color_map])-1]))[1:-1]}" + "}\n")
            f.write("};\n\n")