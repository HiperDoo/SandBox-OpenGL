from pygltflib import GLTF2
import struct

type = { "SCALAR" : 1, "VEC2" : 2, "VEC3" : 3, "VEC4" : 4 }
gltf = []

list = [0] * 12
buffers = []

def updateList(iter, num_vert, start_buffer, buffer_offset):
    list.insert(iter, num_vert)
    list.insert(iter + 4, start_buffer)
    list.insert(iter + 5, buffer_offset)

def getBuffer(buffer_acc):
    vertices = []
    num_vert = type[buffer_acc.type]
    size_per_vert = num_vert * 4
    
    bufferView = gltf.bufferViews[buffer_acc.bufferView]
    buffer = gltf.buffers[bufferView.buffer]
    data = gltf.get_data_from_buffer_uri(buffer.uri)

    """
    for i in range(buffer_acc.count):
        index = bufferView.byteOffset + buffer_acc.byteOffset + (i * size_per_vert)
        vertices.extend(bytes(data[index:index + size_per_vert]))
    """

    for i in range(buffer_acc.count):
        index = bufferView.byteOffset + buffer_acc.byteOffset + (i * size_per_vert)
        for j in reversed(range(num_vert)):
            j *= 4
            vertices.extend(bytes(data[index + j:index + j + 4]))

    buffers.append(vertices)

    return num_vert

##### Inicio del programa #####
gltf = GLTF2().load("sword/scene.gltf")
primitive = gltf.meshes[gltf.scenes[gltf.scene].nodes[0]].primitives[0]


vert_size = []
vert_size.append(getBuffer(gltf.accessors[primitive.attributes.POSITION]))
getBuffer(gltf.accessors[primitive.indices])
vert_size.append(getBuffer(gltf.accessors[primitive.attributes.TEXCOORD_0]))
vert_size.append(getBuffer(gltf.accessors[primitive.attributes.NORMAL]))
vert_size.append(1) # NUM TEX, HARDCODED

start = 68
offset = 68

tex_sizes = [0] * 4
tex_str = "models/sword/baseColor.jpeg\0"
#tex_str = gltf.images[0].uri
#print(gltf.images[0].uri)
#pos = tex_str.rfind('/')
#if pos != -1:
#    tex_str = tex_str[pos+1:]
#print(tex_str)
tex_sizes[0] = start
offset = int(len(tex_str))
tex_sizes[1] = offset


vertices = []
pos = vert_size[0] * 4
uvs = vert_size[1] * 4
nor = vert_size[2] * 4
for i in range(int((len(buffers[0]) / vert_size[0]) / 4)):
    vertices.extend(buffers[0][i * pos:(i * pos) + pos])
    vertices.extend(buffers[2][i * uvs:(i * uvs) + uvs])
    vertices.extend(buffers[3][i * nor:(i * nor) + nor])

vert_offsets = []
start += offset
vert_offsets.append(start)
offset = int(len(vertices))
vert_offsets.append(offset)
start += offset
vert_offsets.append(start)
offset = int(len(buffers[1]))
vert_offsets.append(offset)


print(f"VEC#: {vert_size}")
print(f"Tex: {tex_sizes}")
print(f"Offsets: {vert_offsets}")

print(struct.unpack("<ffffffff", bytes(vertices[0:32])))
print(struct.unpack("<iiiii", bytes(buffers[1][0:20])))

with open('models/sword/sword.bin', 'wb') as file:
    file.write(bytes(vert_size))

    for b in tex_sizes:
        file.write(b.to_bytes(8, byteorder='little'))
    for b in vert_offsets:
        file.write(b.to_bytes(8, byteorder='little'))

    file.write(tex_str.encode('ascii'))

    file.write(bytes(vertices))
    file.write(bytes(buffers[1]))