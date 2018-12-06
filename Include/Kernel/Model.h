#pragma once

#include <glm/glm.hpp>

#include <assimp/material.h>

#include "Shader.hpp"
#include "Mesh.h"

#include <string>
#include <vector>

struct aiNode;
struct aiMesh;
struct aiScene;

/**
 * @class Model
 * @author Seth Traman
 *
 * @brief Encapsulated utilities for loading, initializing, and rendering a 3D model all in one. Yuck.
 **/
class Model
{
	/// Cache of loaded textures, prevents duplicate texture loading
	/// @todo Factor this into a more global cache for multiple models
	std::vector<Texture> textureCache;

	/// Dynamic list of individual meshes that are part of this model
	std::vector<Mesh> meshes;

	/// Directory this model is located in.  Used to resolve relative texture paths
	std::string directory;

	/// Whether this model's textures should be gamma correct
	bool gammaCorrection;

	/// Performs actual model loading by traversing scene nodes and processing them
	/// @param [in] path  Filesystem location to look for model
	void LoadModel(const std::string & path);

	/// Recursively find meshes from hierarchical scene data
	/// @param [in] aiNode   Assimp's representation of a scene node
	/// @param [in] aiScene  Assimp's representation of the scene
	void ProcessNode(aiNode * node, const aiScene * scene);

	/// Extract information from mesh node and submit to GPU
	/// @param [in] aiNode   Assimp's representation of a scene node
	/// @param [in] aiScene  Assimp's representation of the scene
	/// @return Mesh produced from node extraction
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);

	/// Loads all textures of a given type for a single mesh
	/// @param [in] mat       Material of full mesh
	/// @param [in] type	    Type of texture in Assimp format
	/// @param [in] typeName  Type of texture in native mesh format
	/// @return Dynamic list of textures that have been loaded
	std::vector<Texture> LoadMaterialTextures(aiMaterial * mat, aiTextureType type, Texture::Type typeName);

public:

	/// Transform matrix in model space.  Used to manipulate 3D aspects of model
	glm::mat4 transform;

	/// Extracts directory information and immediately loads model
	/// @note Blocks calling thread until all assets are loaded & submitted to GPU
	/// @see  LoadModel
	Model(const std::string & path, bool gamma = false);

	Model(const Model &) = delete; ///< Noncopyable
	Model(Model &&)      = delete; ///< Noncopyable

	/// Gracefully cleans up model assets & GPU resources
	/// @note Blocks calling thread until all assets are destroyed on GPU
	~Model();

	Model & operator=(const Model &) = delete; ///< Noncopyable
	Model & operator=(Model &&)      = delete; ///< Noncopyable

	/// Submits this model for rendering on the GPU
	/// @param [in] Shader	The shader to be used during drawing
	void Draw(const Shader & shader);
};
