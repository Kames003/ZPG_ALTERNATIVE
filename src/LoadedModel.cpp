#include "LoadedModel.h"

LoadedModel::LoadedModel(const char* path)
{
    // ASSIMP importer
    Assimp::Importer importer;
    
    // Nastavenie importných opcií
    unsigned int importOptions = 
        aiProcess_Triangulate |              // Prevod všetkých plôch na trojuholníky
        aiProcess_OptimizeMeshes |           // Zlúčenie malých plôch (optimalizácia)
        aiProcess_JoinIdenticalVertices |    // Odstránenie duplikátov (NUTNÉ!)
        aiProcess_CalcTangentSpace;          // Výpočet tangent (potrebné pre normal mapy)
    
    // Načítanie súboru
    const aiScene* scene = importer.ReadFile(path, importOptions);
    
    if (!scene) {
        printf("ERROR: Failed to load model: %s\n", path);
        printf("ASSIMP Error: %s\n", importer.GetErrorString());
        exit(EXIT_FAILURE);
    }

    // Vektor pre dáta všetkých vrcholov
    std::vector<float> data;
    
    if (scene) 
    {
        // Pre jednoduchosť berieme len prvý mesh
        // (väčšina jednoduchých modelov má len jeden mesh)
        aiMesh* mesh = scene->mMeshes[0];
        
        // Počet vrcholov na vykreslenie
        this->count = mesh->mNumFaces * 3;
        
        // Prechádzame všetky trojuholníky (faces)
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
        {
            // Každý face má 3 indexy (pretože aiProcess_Triangulate)
            for (unsigned int j = 0; j < 3; j++)
            {
                // Index vrcholu v poli mesh->mVertices
                unsigned int vertexIndex = mesh->mFaces[i].mIndices[j];
                
                // Pozícia (x, y, z)
                data.push_back(mesh->mVertices[vertexIndex].x);
                data.push_back(mesh->mVertices[vertexIndex].y);
                data.push_back(mesh->mVertices[vertexIndex].z);
                
                // Normála (nx, ny, nz)
                data.push_back(mesh->mNormals[vertexIndex].x);
                data.push_back(mesh->mNormals[vertexIndex].y);
                data.push_back(mesh->mNormals[vertexIndex].z);
                
                // Textúrové koordináty (u, v)
                // ASSIMP podporuje až 8 UV vrstiev, my používame len prvú [0]
                data.push_back(mesh->mTextureCoords[0][vertexIndex].x);
                data.push_back(mesh->mTextureCoords[0][vertexIndex].y);
            }
        }
    }

    // Vytvorenie VBO s načítanými dátami
    vbo = new VBO();
    vbo->generateVBO(&data[0], data.size() * sizeof(float));

    // Vytvorenie VAO
    vao = new VAO();
    vao->generateVAO();

    // Nastavenie vertex atribútov
    vao->bind();
    vbo->bind();
    
    // Atribút 0: pozícia (x, y, z) - 3 floaty
    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    
    // Atribút 1: normála (nx, ny, nz) - 3 floaty, offset 3*sizeof(float)
    vao->vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
                            (GLvoid*)(sizeof(float) * 3));
    
    // Atribút 2: textúrové koordináty (u, v) - 2 floaty, offset 6*sizeof(float)
    vao->vertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
                            (GLvoid*)(sizeof(float) * 6));
    
    vbo->unbind();
    vao->unbind();
}

void LoadedModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, count);
    vao->unbind();
}