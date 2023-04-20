"""
Use this program to turn the color_maps.py list of color maps into integer
color maps, and write to the colors.cpp file which will be used in main.cpp
to grab color maps.
"""

from color_maps import get_color_maps

color_maps = get_color_maps()

def decimal_to_255(rgb):
    return [round(255*rgb[0]), round(255*rgb[1]), round(255*rgb[2])]

if __name__ == "__main__":
    with open('../colors.h', 'w') as f:
        for i, color_map in enumerate(color_maps):
            f.write(f"#define {color_map}Size {len(color_maps[color_map])}\n")
            f.write(f'int {color_map}[{len(color_maps[color_map])}][4] = ' + '{\n')
            for j in range(len(color_maps[color_map])):
                if j == len(color_maps[color_map])-1:break
                f.write("\t{" + f"{str(decimal_to_255(color_maps[color_map][j]))[1:-1]}" + "},\n")
            f.write("\t{" + f"{str(decimal_to_255(color_maps[color_map][len(color_maps[color_map])-1]))[1:-1]}" + "}\n")
            f.write("};\n\n")