#include <Kernel/Model.h>
#include <Kernel/Debug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <External/STB/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

static uint32 TextureFromFile(cchar path, const std::string & directory, bool gamma);

Model::Model(const std::string & path, bool gamma)
: gammaCorrection(gamma)
{
	// Immediately load our model
	LoadModel(path);
}

Model::~Model()
{}

void Model::LoadModel(const std::string & path)
{
	// Create the importer & load the scene
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(
		path,
		aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_CalcTangentSpace
	);

	// Check for errors
	{
		const bool sceneNonexistent = (scene == nullptr);
		const bool sceneIncomplete  = (scene -> mFlags & AI_SCENE_FLAGS_INCOMPLETE);
		const bool sceneMissingRoot = (scene -> mRootNode == nullptr);

		if (sceneNonexistent || sceneIncomplete || sceneMissingRoot)
		{
			Kernel::DebugLog("[Error] Could not load model:");
			Kernel::DebugLog(importer.GetErrorString());
			return;
		}
	}

	// Retrieve directory of filepath
	directory = path.substr(0, path.find_last_of('/'));

	// Recursively process root node
	ProcessNode(scene -> mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	// Process each mesh located at the current node
	for (uint32 i = 0; i < node -> mNumMeshes; ++i)
	{
		// The node object only contains the index of an actual object(s) in the scene
		// The scene contains all the data, each node simply organizes things hierarchically
		aiMesh * mesh = scene -> mMeshes[node -> mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// After mesh processing, recursively process each child node
	for (uint32 i = 0; i < node -> mNumChildren; ++i)
	{
		ProcessNode(node -> mChildren[i], scene);
	}
}

void Model::Draw(uint32 program)
{
	Kernel::DebugCPUBegin("ModelDraw");

	// Render each mesh in progression
	for (uint32 i = 0; i < meshes.size(); ++i)
	{
		Kernel::DebugCPUBegin("MeshDraw");

		const Mesh & mesh = meshes[i];

		uint32 diffuseCount  = 1;
		uint32 specularCount = 1;
		uint32 normalCount   = 1;
		uint32 heightCount   = 1;

		// Bind each available texture
		for (uint32 j = 0; j < mesh.textures.size(); ++j)
		{
			// Activate proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + j);

			std::string name;
			Texture::Type type = mesh.textures[j].type;

			// Retrieve texture name in the format texture_typeN
			// `texture_type` is any one of Texture::Type's
			// `N` is this texture's index into that texture type
			switch (type)
			{
				case Texture::Type::DIFFUSE:
					name = "texture_diffuse" + std::to_string(diffuseCount++);
					break;
				case Texture::Type::SPECULAR:
					name = "texture_specular" + std::to_string(specularCount++);
					break;
				case Texture::Type::NORMAL:
					name = "texture_normal" + std::to_string(normalCount++);
					break;
				case Texture::Type::HEIGHT:
					name = "texture_height" + std::to_string(heightCount++);
					break;
			}

			// Set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(program, name.c_str()), j);

			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, mesh.textures[j].id);
		}

		// Now we submit our mesh for shading :)
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

		Kernel::DebugCPUEnd();
	}

	Kernel::DebugCPUEnd();
}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	// Dynamic arrays of data to fill
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	std::vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (uint32 i = 0; i < mesh -> mNumVertices; ++i)
	{
		Vertex vertex;

		// Positions
		vertex.Position.x = mesh -> mVertices[i].x;
		vertex.Position.y = mesh -> mVertices[i].y;
		vertex.Position.z = mesh -> mVertices[i].z;

		// Normals
		vertex.Normal.x = mesh -> mNormals[i].x;
		vertex.Normal.y = mesh -> mNormals[i].y;
		vertex.Normal.z = mesh -> mNormals[i].z;

		// Texture coordinates
		if (mesh -> mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			// A vertex can contain up to 8 different texture coordinates.  We're making the assumption here that each model only has 1 pair per vertex by only taking the 0th index
			vertex.TexCoords.x = mesh -> mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh -> mTextureCoords[0][i].y;
		}
		else
		{
			// No texture coordinates for this vertex... Make something up?
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		// Tangent
		vertex.Tangent.x = mesh -> mTangents[i].x;
		vertex.Tangent.y = mesh -> mTangents[i].y;
		vertex.Tangent.z = mesh -> mTangents[i].z;

		// Bitangent
		vertex.Bitangent.x = mesh -> mBitangents[i].x;
		vertex.Bitangent.y = mesh -> mBitangents[i].y;
		vertex.Bitangent.z = mesh -> mBitangents[i].z;

		// Submit this vertex into our pool
		vertices.push_back(vertex);
	}

	// Iterate thru each face (in this case, tri's) and extract vertex indices
	for (uint32 i = 0; i < mesh -> mNumFaces; ++i)
	{
		aiFace face = mesh -> mFaces[i];

		// Store all indices in our indices pool
		for (uint32 j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Material processing
	aiMaterial * material = scene -> mMaterials[mesh -> mMaterialIndex];

	// Load diffuse maps
	std::vector<Texture> diffuseMaps
		= LoadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Type::DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// Load specular maps
	std::vector<Texture> specularMaps
		= LoadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Type::SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// Load normal maps
	std::vector<Texture> normalMaps
		= LoadMaterialTextures(material, aiTextureType_HEIGHT, Texture::Type::NORMAL);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// Load height maps
	std::vector<Texture> heightMaps
		= LoadMaterialTextures(material, aiTextureType_AMBIENT, Texture::Type::HEIGHT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// Construct a mesh from the data we just extracted
	Mesh value;

	// Fill it with data
	value.indexCount = indices.size();
	value.textures   = textures;

	// Generate buffers and vertex arrays
	glGenVertexArrays(1, & value.VAO);
	glGenBuffers(1, & value.VBO);
	glGenBuffers(1, & value.EBO);

	glBindVertexArray(value.VAO);

	// RANT: One of the great things about structs is their sequential memory layout.  This means we can simply pass a pointer to a vertex structure, which has the effect of Vertex-> glm::vecN array -> raw float array - just what OpenGL needs!  END RANT

	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, value.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), & vertices[0], GL_STATIC_DRAW);

	// Load data into index buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), & indices[0], GL_STATIC_DRAW);

	// Inform OpenGL about our vertex attributes:

	// Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

	// Texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

	// Tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Tangent));

	// Bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Bitangent));

	// Reset everything like cool kids do
	glBindVertexArray(0);

	return value;
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, Texture::Type typeName)
{
	// Pool of textures to return
	std::vector<Texture> textures;

	for (uint32 i = 0; i < mat -> GetTextureCount(type); ++i)
	{
		// Extract path of texture
		aiString str;
		mat -> GetTexture(type, i, & str);

		// Skip loading cached textures.  Cache search is O(n)
		bool skip = false;
		for (uint32 j = 0; j < textureCache.size(); ++j)
		{
			if(std::strcmp(textureCache[j].path.data(), str.C_Str()) == 0)
			{
				// Optimization: we've already loaded & cached this texture, so don't load it again
				textures.push_back(textureCache[j]);
				skip = true;
				break;
			}
		}

		if (skip == false) // Load texture if not already
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory, gammaCorrection);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);

			// Cache this texture with the model so we don't have to load it again
			textureCache.push_back(texture);
		}
	}
	return textures;
}

uint32 TextureFromFile(cchar path, const std::string & directory, bool gamma)
{
	// Compute full path of textuer
	std::string filename = std::string{path};
	filename = directory + '/' + filename;

	// Generate OpenGL texture
	uint32 textureID;
	glGenTextures(1, & textureID);

	int32 width;
	int32 height;
	int32 componentCount;

	unsigned char * data = stbi_load(filename.c_str(), & width, &  height, & componentCount, 0);

	if (data)
	{
		// Our texture is loaded, choose the right pixel format
		GLenum format;
		if (componentCount == 1)
		{
			format = GL_RED;
		}
		else if (componentCount == 3)
		{
			format = GL_RGB;
		}
		else if (componentCount == 4)
		{
			format = GL_RGBA;
		}

		// Fill our OpenGL texture with raw data
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set sampling properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		// Image loading failed
		Kernel::DebugLog("[Error] Model texture failed to load at path:");
		Kernel::DebugLog(path);
	}

	// Clean up our resources
	stbi_image_free(data);

	return textureID;
}
