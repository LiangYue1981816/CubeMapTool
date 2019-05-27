#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Texture.h"
#include "GLFunctions.h"


#define PI 3.1415926535897932384626433832795f


#include "IMAGE.H"
#define CUBEMAP_WIDTH(cubemap) IMAGE_WIDTH(&(cubemap)->faces[0])
#define CUBEMAP_HEIGHT(cubemap) IMAGE_HEIGHT(&(cubemap)->faces[0])
#define CUBEMAP_BITCOUNT(cubemap) IMAGE_BITCOUNT(&(cubemap)->faces[0])
struct CUBEMAP {
	IMAGE faces[6];
};
void CubeMapInit(CUBEMAP *pCubeMap);
void CubeMapFree(CUBEMAP *pCubeMap);
BOOL CubeMapAlloc(CUBEMAP *pCubeMap, int width, int height, int bitcount);
BOOL CubeMapLoad(CUBEMAP *pCubeMap, char szFileNames[6][_MAX_PATH]);
unsigned long CubeMapGetPixelColor(CUBEMAP *pCubeMap, glm::vec3 &texcoord);
void PreviewMap(CUBEMAP *pCubeMap, IMAGE *imgPreview);
GLuint CreateTexture2D(IMAGE *pImage);
GLuint CreateTextureCube(CUBEMAP *pCubeMap);
void DestroyTexture(GLuint texture);





BOOL GenerateEnvIrradianceMap(gli::texture2d &texEnvMap, gli::texture_cube &texIrrMap, int samples);
BOOL GenerateCubeIrradianceMap(gli::texture_cube &texCubeMap, gli::texture_cube &texIrrMap, int samples);
BOOL GenerateEnvMipmaps(IMAGE *pEnvMap, IMAGE pMipmaps[], int mipLevels, int samples);
BOOL GenerateCubeMipmaps(CUBEMAP *pCubeMap, CUBEMAP pMipmaps[], int mipLevels, int samples);
BOOL GenerateLUT(IMAGE *pImage, int samples);
