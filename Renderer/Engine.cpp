#include "Engine.h"
#include "MainWindow.h"
#include "Mesh.h"
#include "Texture.h"
#include "Logger.h"

Engine::Engine() : mLogger(Logger::getInstance())
{}

void Engine::loadScene()
{
    //Load textures and meshes here!
    mMeshes.push_back(new Mesh(mRenderer));
    mMeshes.push_back(new Mesh(mRenderer, Mesh::MeshType::QUAD)); // Second mesh with an offset
}

void Engine::readTexture(std::string textureName)
{
    // Texture update: - textures must be made before the Descriptor sets!
    Texture* newTexture = new Texture(textureName);
    mRenderer->createTextureImage(newTexture);
    mRenderer->createTextureImageView(newTexture);
    mTextures.push_back(newTexture);
}

// Main Game Loop function
// called every frame
void Engine::update()
{
    // 1. get input from mouse and keyboard
    mMainWindow->handleInput();

    // 2. update game objects
    // physics calculations
    // etc.

    // 3. call the renderer to draw a frame
    mRenderer->update();

}

Engine* Engine::getInstance()
{
    static Engine mInstance;
    return &mInstance;
}

std::vector<Mesh *> Engine::meshes() const
{
    return mMeshes;
}

std::vector<Texture *> Engine::textures() const
{
    return mTextures;
}

void Engine::setRenderer(Renderer *rendererIn)
{
    mRenderer = rendererIn;
}

void Engine::setMainWindow(MainWindow *mainWindowIn)
{
        mMainWindow = mainWindowIn;
}
