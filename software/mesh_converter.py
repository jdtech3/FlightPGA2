import numpy as np
import trimesh
import argparse
from pathlib import Path
import os

def main():
    parser = argparse.ArgumentParser(
        prog='Mesh Converter',
        description='Generate mesh files for FlightGPA2 from any 3D file format supported by trimesh',
    )
    parser.add_argument('infile', type=str, help='path to 3D mesh file to be converted')
    parser.add_argument('outfile', type=argparse.FileType('w'), help='path to generated output file. Instance name will match this filename')
    parser.add_argument('-c', '--color', type=int, help='set all faces to this color')
    args = parser.parse_args()

    mesh = trimesh.load(args.infile, force='mesh')

    if not isinstance(mesh.visual, trimesh.visual.color.ColorVisuals):
        mesh.visual = mesh.visual.to_color()

    args.outfile.write('#include <glm/vec3.hpp>\n')
    args.outfile.write('#include "graphics/mesh.hpp"\n\n')

    var_name = Path(args.outfile.name).stem
    args.outfile.write('extern Mesh ' + var_name + ';\n\n')
    args.outfile.write('Mesh ' + var_name + ' = {\n')

    center_mass_cs = ', '.join([str(x) for x in mesh.center_mass])
    args.outfile.write('    glm::vec3(' + center_mass_cs + '),\n');
    
    args.outfile.write('    {\n')
    args.outfile.write('        ')
    vertices = []
    for vertex in mesh.vertices:
        vertex_cs = ', '.join([str(x) for x in vertex])
        vertices.append('glm::vec3(' + vertex_cs + ')')
    args.outfile.write(',\n        '.join(vertices))
    args.outfile.write('\n    },\n');

    args.outfile.write('    {\n')
    args.outfile.write('        ')
    faces = []
    for face in mesh.faces:
        faces.append(str(face[0]) + ',' + str(face[1]) + ',' + str(face[2]))
    args.outfile.write(',\n        '.join(faces))
    args.outfile.write('\n    },\n');

    args.outfile.write('    {\n')
    args.outfile.write('        ')
    face_normals = []
    for face_normal in mesh.face_normals:
        face_normals.append('glm::vec3(' + str(face_normal[0]) + ',' + str(face_normal[1]) + ',' + str(face_normal[2]) + ')')
    args.outfile.write(',\n        '.join(face_normals))
    args.outfile.write('\n    },\n');

    
    args.outfile.write('    {\n')
    args.outfile.write('        ')
    face_colors = []
    for face_color in mesh.visual.face_colors:
        r = np.uint8(face_color[0]*0b11111/255)
        g = np.uint8(face_color[1]*0b111111/255)
        b = np.uint8(face_color[2]*0b11111/255)
        rgb = np.uint16(r << 11 | g << 5 | b);
        face_colors.append(str(rgb))
    args.outfile.write(',\n        '.join(face_colors))
    args.outfile.write('\n    }\n');

    args.outfile.write('};\n')
    
if __name__ == '__main__':
    main()