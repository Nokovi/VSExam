#include "Mesh.h"
#include "sstream"
#include <fstream>
#include "Utilities.h"
#include "Logger.h"
#include "Renderer.h"

Mesh::Mesh(Renderer* render) : mRenderer{render}, mLogger(Logger::getInstance())
{
    makeTriangle();
    createBuffers();
}

Mesh::Mesh(Renderer* render, MeshType meshTypeIn, std::string fileNameIn) : mRenderer{render},
            mLogger(Logger::getInstance()), mFileName(fileNameIn)
{
    switch (meshTypeIn)
    {
    case MeshType::TRIANGLE :
        makeTriangle();
        break;
    case MeshType::QUAD :
        makeQuad();
        break;
    case MeshType::OBJ :
        makeObj();
        break;
    default:
        makeTriangle();
    }

    createBuffers();
}

Mesh::~Mesh()
{
    LOG("Mesh Destructor called");
    // Destroy only if handles are valid - prevents calls on uninitialized / already freed handles
    if (mVertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(mRenderer->device, mVertexBuffer, nullptr);
        mVertexBuffer = VK_NULL_HANDLE;
    }
    if (mVertexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(mRenderer->device, mVertexBufferMemory, nullptr);
        mVertexBufferMemory = VK_NULL_HANDLE;
    }
    if (mIndexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(mRenderer->device, mIndexBuffer, nullptr);
        mIndexBuffer = VK_NULL_HANDLE;
    }
    if (mIndexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(mRenderer->device, mIndexBufferMemory, nullptr);
        mIndexBufferMemory = VK_NULL_HANDLE;
    }
}

void Mesh::createBuffers()
{
    mRenderer->createVertexBuffer(this);

    // Only create index buffer if we have indices
    if (mIndices.size() > 0)
        mRenderer->createIndexBuffer(this);
}

void Mesh::makeTriangle()
{
    mVertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.f, 1.f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.0f}}
    };
}

void Mesh::makeQuad()
{
    LOGE("Loading file");
    std::ifstream fileIn;
    fileIn.open( PATH + "../data/VertexData.txt", std::ifstream::in);
    if(!fileIn)
    {
        LOGE("ERROR: Could not load mesh!");
        LOGP("     Could not open file for reading: %s", mFileName.c_str());
        LOGW("     Making default triangle instead");
        makeTriangle();
    }



    std::string oneLine;
    std::string oneWord;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempUVs;

    tempVertices.push_back({0,0,0});
    tempUVs.push_back({0,0});

    std::getline(fileIn,oneLine);



    int i = 0;

    double smallestX = 10000000000000.f, smallestY = 10000000000000.f;

    while(std::getline(fileIn,oneLine)){
        std::stringstream sStream;
        sStream << oneLine;
        oneWord = "";
        sStream >> oneWord;
        double x = std::stod(oneWord);
        sStream >> oneWord;
        double y = std::stod(oneWord);
        sStream >> oneWord;
        double z = std::stod(oneWord);


        tempVertices.push_back({x,y,z});

        if(smallestX > x){
            smallestX = x;
        }

        if(smallestY > y){
            smallestY = y;
        }

        i++;
    }
     LOGE("Read file. Loading triangles...");
    for(int j = 0; j < i-1;  j++){
        tempVertices[i].x -= smallestX;
        tempVertices[i].y -= smallestY;
        Vertex tVertex = {tempVertices[i], tempNormals[0], tempUVs[0]};
        mVertices.push_back(tVertex);
    }
LOGE("Success");

}

void Mesh::makeObj()
{
    makeTriangle();
}




