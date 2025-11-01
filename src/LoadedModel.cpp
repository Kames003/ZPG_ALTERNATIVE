// LoadedModel.cpp - COMPLETE FIXED VERSION with multi-mesh support
#include "LoadedModel.h"

LoadedModel::LoadedModel(const char* path)
{
    printf("🔄 Loading model: %s\n", path);

    // ✅ VYTVOR ASSIMP IMPORTER
    Assimp::Importer importer;

    // ✅ ROBUSTNÉ IMPORT FLAGS
    unsigned int importOptions =
        aiProcess_Triangulate |              // Konvertuj všetko na trojuholníky
        aiProcess_OptimizeMeshes |           // Optimalizuj
        aiProcess_JoinIdenticalVertices |    // Spoj identické vrcholy
        aiProcess_CalcTangentSpace |         // Vypočítaj tangenty
        aiProcess_GenNormals |               // Generuj normály ak chýbajú
        aiProcess_ValidateDataStructure |    // ✅ Validuj dáta
        aiProcess_FindInvalidData |          // ✅ Nájdi invalidné dáta
        aiProcess_FixInfacingNormals |       // ✅ Oprav otočené normály
        aiProcess_SortByPType;               // ✅ Zoraď podľa typu primitív

    printf("🔄 Calling ASSIMP ReadFile...\n");
    const aiScene* scene = importer.ReadFile(path, importOptions);

    if (!scene) {
        printf("❌ ERROR: Failed to load model: %s\n", path);
        printf("❌ ASSIMP Error: %s\n", importer.GetErrorString());
        printf("\n💡 TROUBLESHOOTING:\n");
        printf("   1. Check if file exists: %s\n", path);
        printf("   2. Check file format (must be valid OBJ)\n");
        printf("   3. Try opening in Blender and re-exporting\n");
        printf("   4. Check for invalid indices or normals\n\n");
        exit(EXIT_FAILURE);
    }

    printf("✅ ASSIMP loaded successfully!\n");
    printf("   📊 Number of meshes: %d\n", scene->mNumMeshes);

    if (scene->mNumMeshes == 0) {
        printf("❌ ERROR: Model has no meshes!\n");
        exit(EXIT_FAILURE);
    }

    std::vector<float> data;
    int totalVertexCount = 0;

    // ✅ NAČÍTAJ VŠETKY MESHES (dôležité pre multi-mesh modely ako lamp.obj!)
    printf("   🔄 Processing %d meshes...\n", scene->mNumMeshes);

    for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
    {
        aiMesh* mesh = scene->mMeshes[meshIdx];

        printf("      Mesh %d: %d vertices, %d faces\n",
               meshIdx, mesh->mNumVertices, mesh->mNumFaces);

        // ✅ KRITICKÁ KONTROLA
        if (!mesh->HasNormals()) {
            printf("      ⚠️  Mesh %d has no normals, skipping...\n", meshIdx);
            continue;
        }

        if (mesh->mNumVertices == 0) {
            printf("      ⚠️  Mesh %d has no vertices, skipping...\n", meshIdx);
            continue;
        }

        // ✅ BEZPEČNÉ NAČÍTANIE DÁT Z TOHTO MESH
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];

            // ✅ KONTROLA: Musí to byť trojuholník!
            if (face.mNumIndices != 3) {
                continue;
            }

            for (unsigned int j = 0; j < 3; j++)
            {
                unsigned int vertexIndex = face.mIndices[j];

                // ✅ BEZPEČNOSTNÁ KONTROLA INDEXU!
                if (vertexIndex >= mesh->mNumVertices) {
                    printf("      ❌ Invalid vertex index %d (max %d)\n",
                           vertexIndex, mesh->mNumVertices - 1);
                    continue;
                }

                // Pozícia (x, y, z)
                data.push_back(mesh->mVertices[vertexIndex].x);
                data.push_back(mesh->mVertices[vertexIndex].y);
                data.push_back(mesh->mVertices[vertexIndex].z);

                // Normála (nx, ny, nz)
                data.push_back(mesh->mNormals[vertexIndex].x);
                data.push_back(mesh->mNormals[vertexIndex].y);
                data.push_back(mesh->mNormals[vertexIndex].z);

                // UV koordináty (u, v)
                if (mesh->HasTextureCoords(0)) {
                    data.push_back(mesh->mTextureCoords[0][vertexIndex].x);
                    data.push_back(mesh->mTextureCoords[0][vertexIndex].y);
                } else {
                    data.push_back(0.0f);
                    data.push_back(0.0f);
                }

                totalVertexCount++;
            }
        }

        printf("      ✅ Mesh %d processed: %d vertices added\n",
               meshIdx, mesh->mNumFaces * 3);
    }

    this->count = totalVertexCount;

    if (data.empty()) {
        printf("❌ FATAL ERROR: No valid vertex data extracted!\n");
        exit(EXIT_FAILURE);
    }

    printf("   ✅ Total data extracted: %zu floats (%d vertices)\n",
           data.size(), totalVertexCount);

    // ✅ VYTVORENIE VBO a VAO
    printf("   🔄 Creating VBO...\n");
    vbo = new VBO();
    vbo->generateVBO(&data[0], data.size() * sizeof(float));

    printf("   🔄 Creating VAO...\n");
    vao = new VAO();
    vao->generateVAO();

    vao->bind();
    vbo->bind();

    // Vertex attributes: [x,y,z, nx,ny,nz, u,v]
    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    vao->vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (GLvoid*)(sizeof(float) * 3));
    vao->vertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (GLvoid*)(sizeof(float) * 6));

    vbo->unbind();
    vao->unbind();

    printf("   ✅ LoadedModel constructor finished!\n\n");
}

void LoadedModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, count);
    vao->unbind();
}