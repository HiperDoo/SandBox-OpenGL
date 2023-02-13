#include "Model.hpp"

#include <fstream>

//=====>>> Constructor y Destructor
Model::Model() {}

Model::~Model() {}

//=====>>> Funciones Privadas
std::string Model::get_file_contents(const char* file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        auto error = std::system_error(errno, std::system_category());
        cmd::console_print(cmd::server, cmd::error,
            "Falla al abrir '{}': {}.", file_path, error.what());
        throw 1;
    }

    std::string contents;
	file.seekg(0, std::ios::end);
	contents.resize(file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&contents[0], contents.size());

    file.close();

    return contents;
}

//=====>>> Funciones
void Model::initModel(const char* file_path) {
    this->file_path = file_path;
    
    std::string text = get_file_contents(file_path);
    JSON = json::parse(text);
    data = getData();

    traverseNode(0);
}

std::vector<unsigned char> Model::getData() {
    std::string bytesTest;
    std::string uri = JSON["buffers"][0]["uri"];

    std::string fileStr = std::string(file_path);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
    bytesTest = get_file_contents((fileDirectory + uri).c_str());

    std::vector<unsigned char> data(bytesTest.begin(), bytesTest.end());
    return data;
}

std::vector<float> Model::getFloats(json accessor) {
    std::vector<float> floatVec;

    unsigned int buffViewInd = accessor.value("bufferView", 1); // 0
	unsigned int count = accessor["count"]; // 208353
	unsigned int accByteOffset = accessor.value("byteOffset", 0); // 0
	std::string type = accessor["type"]; // VEC3

    json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"]; // 0

    unsigned int numPerVert;
    if (type == "SACLAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else {
        cmd::console_print(cmd::server, cmd::error, "El tipo es invalido");
        throw 1;
    }

    unsigned int beginningOfData = byteOffset + accByteOffset; // 0
	unsigned int lengthOfData = count * 4 * numPerVert; // 208353 * 4 * 3
    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) { // 0 + 208353 * 4 * 3
        unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor) {
    std::vector<GLuint> indices;

    unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

    json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int beginningOfData = byteOffset + accByteOffset;
    if (componentType == 5125) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i) {
            unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
        }
    } else if (componentType == 5123) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i) {
            unsigned char bytes[] = { data[i++], data[i++] };
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    } else if (componentType == 5122) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i) {
            unsigned char bytes[] = { data[i++], data[i++] };
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
        }
    }

    return indices;
}

std::vector<std::string> Model::getTextures() {
    std::vector<std::string> textures;

    std::string fileStr = std::string(file_path);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    for (unsigned int i = 0; i < JSON["images"].size(); i++) {
        std::string texPath = JSON["images"][i]["uri"];

        bool skip = false;
        for (unsigned int j = 0; j < loadedTexName.size(); j++) {
            if (loadedTexName[j] == texPath) {
                skip = true;
                break;
            }
        }

        if (!skip) {
            if (texPath.find("baseColor") != std::string::npos) {
                textures.push_back(fileDirectory + texPath);
                loadedTexName.push_back(texPath);
            } else if (texPath.find("metallicRoughness") != std::string::npos) {
                textures.push_back(fileDirectory + texPath);
                loadedTexName.push_back(texPath);
            }
        }
    }

    return textures;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec) {
    std::vector<glm::vec2> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec) {
    std::vector<glm::vec3> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec) {
    std::vector<glm::vec4> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

void Model::loadMesh(unsigned int indMesh) {
    unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

    std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
    std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
    std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
    std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

    /*printf("\n\n");
    cmd::console_print(cmd::server, cmd::info,
        "Positions: {} (x3 -> {})", posVec.size(), positions.size());
    cmd::console_print(cmd::server, cmd::info,
        "Textures: {} (x2 -> {})", texVec.size(), texUVs.size());
    cmd::console_print(cmd::server, cmd::info,
        "Normals: {} (x3 -> {})", normalVec.size(), normals.size());
    long long total_Size = posVec.size() + texVec.size() + normalVec.size();
    cmd::console_print(cmd::server, cmd::info,
        "Total: {} (x8 -> {})", total_Size, total_Size / 8);*/

    vertices = assembleVertices(positions, texUVs, normals);
    indices = getIndices(JSON["accessors"][indAccInd]);
    /*cmd::console_print(cmd::server, cmd::info,
        "Indices: [{}, {}, {}, {}])", indices[0], indices[1], indices[2], indices[3]);*/
    textures = getTextures();
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions,
    std::vector<glm::vec2> texUVs, std::vector<glm::vec3> normals) {
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++) {
        float* pos = &positions[i].x;
        float* uv = &texUVs[i].x;
        float* nr = &normals[i].x;
		vertices.push_back(
            Vertex {
                pos[0], pos[1], pos[2],
                uv[0], uv[1],
                nr[0], nr[1], nr[2]
            }
		);
	}
	return vertices;
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix) {
    json node = JSON["nodes"][nextNode];

    glm::vec3 translation = glm::vec3(0.0f);
    if (node.find("translation") != node.end()) {
        float transValues[3];
        for (unsigned int i = 0; i < node["translation"].size(); i++) {
            transValues[i] = (node["translation"][i]);
        }
        translation = glm::make_vec3(transValues);
    }
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end()) {
        float rotValues[4] = {
            node["rotation"][3],
            node["rotation"][0],
            node["rotation"][1],
            node["rotation"][2]
        };
        rotation = glm::make_quat(rotValues);
    }
    glm::vec3 scale = glm::vec3(1.0f);
    if (node.find("scale") != node.end()) {
        float scaleValues[3];
        for (unsigned int i = 0; i < node["scale"].size(); i++) {
            scaleValues[i] = (node["scale"][i]);
        }
        scale = glm::make_vec3(scaleValues);
    }
    glm::mat4 matNode = glm::mat4(1.0f);
    if (node.find("matrix") != node.end()) {
        float matValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++) {
            matValues[i] = (node["matrix"][i]);
        }
        matNode = glm::make_mat4(matValues);
    }

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca, scale);

    glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

    if (node.find("mesh") != node.end()) {
        translationsMeshes.push_back(translation);
        rotationsMeshes.push_back(rotation);
        scalesMeshes.push_back(scale);
        matricesMeshes.push_back(matNextNode);

        loadMesh(node["mesh"]);
    }

    if (node.find("children") != node.end()) {
        for (unsigned int i = 0; i < node["children"].size(); i++) {
            traverseNode(node["children"][i], matNextNode);
        }
    }
}