#include "stdafx.h"


static void splitfilename(const char *name, char *fname, char *ext)
{
	const char *p = NULL;
	const char *c = NULL;
	const char *base = name;

	for (p = base; *p; p++) {
		if (*p == '/' || *p == '\\') {
			do {
				p++;
			} while (*p == '/' || *p == '\\');

			base = p;
		}
	}

	size_t len = strlen(base);
	for (p = base + len; p != base && *p != '.'; p--);
	if (p == base && *p != '.') p = base + len;

	if (fname) {
		for (c = base; c < p; c++) {
			fname[c - base] = *c;
		}

		fname[c - base] = 0;
	}

	if (ext) {
		for (c = p; c < base + len; c++) {
			ext[c - p] = *c;
		}

		ext[c - p] = 0;
	}
}

#pragma region IrradianceMap

static void GenerateEnvIrradianceMap(int argc, char** argv)
{
	if (argc != 5) {
		printf("Generate irradiance map fail!\n");
		printf("eg: CubeMapTool.exe -irr-env Env.dds 128 128\n");
		return;
	}

	const char *szEnvMapFileName = argv[2];
	const int width = atoi(argv[3]);
	const int height = atoi(argv[4]);

	gli::texture2d texEnvMap = LoadTexture<gli::texture2d>(szEnvMapFileName);
	gli::texture_cube texIrrMap(gli::FORMAT_RGB32_SFLOAT_PACK32, gli::extent2d(width, height));
	{
		GenerateEnvIrradianceMap(texEnvMap, texIrrMap, 1024);
	}
	gli::texture2d texPreview = Preview(texIrrMap);
	gli::save_dds(texIrrMap, "IrradianceCube.dds");
	gli::save_dds(texPreview, "IrradiancePreview.dds");

	return;
}

static void GenerateCubeIrradianceMap(int argc, char** argv)
{
	if (argc != 5) {
		printf("Generate irradiance map fail!\n");
		printf("eg: CubeMapTool.exe -irr-cube Cube.dds 128 128\n");
		return;
	}

	const char *szCubeMapFileName = argv[2];
	const int width = atoi(argv[3]);
	const int height = atoi(argv[4]);

	gli::texture_cube texCubeMap = LoadTexture<gli::texture_cube>(szCubeMapFileName);
	gli::texture_cube texIrrMap(gli::FORMAT_RGB32_SFLOAT_PACK32, gli::extent2d(width, height));
	{
		GenerateCubeIrradianceMap(texCubeMap, texIrrMap, 1024);
	}
	gli::texture2d texPreview = Preview(texIrrMap);
	gli::save_dds(texIrrMap, "IrradianceCube.dds");
	gli::save_dds(texPreview, "IrradiancePreview.dds");

	return;
}

#pragma endregion

#pragma region Mipmap

static void GenerateEnvMipmaps(int argc, char** argv)
{
	if (argc != 4) {
		printf("Generate mipmaps fail!\n");
		printf("eg: CubeMapTool.exe -mip-env Env.dds EnvMipmap.dds\n");
		return;
	}

	const char *szEnvFileName = argv[2];
	const char *szEnvMipmapFileName = argv[3];

	gli::texture2d texEnv = LoadTexture<gli::texture2d>(szEnvFileName);
	gli::texture2d texEnvMipmap(texEnv.format(), texEnv.extent());
	{
		GenerateEnvMipmaps(texEnv, texEnvMipmap, 1024);
	}
	gli::save_dds(texEnvMipmap, szEnvMipmapFileName);
}

static void GenerateCubeMipmaps(int argc, char** argv)
{
	/*
	if (argc != 8) {
		printf("Generate mipmaps fail!\n");
		printf("eg: CubeMapTool.exe -mip PositionX.bmp NegativeX.bmp PositionY.bmp NegativeY.bmp PositionZ.bmp NegativeZ.bmp\n");
		return;
	}

	char szCubeMapFileNames[6][_MAX_PATH];
	strcpy(szCubeMapFileNames[0], argv[2]);
	strcpy(szCubeMapFileNames[1], argv[3]);
	strcpy(szCubeMapFileNames[2], argv[4]);
	strcpy(szCubeMapFileNames[3], argv[5]);
	strcpy(szCubeMapFileNames[4], argv[6]);
	strcpy(szCubeMapFileNames[5], argv[7]);

	CUBEMAP cubemap;
	CubeMapInit(&cubemap);
	CubeMapLoad(&cubemap, szCubeMapFileNames);

	const int MAX_LEVEL = 16;
	CUBEMAP mipmaps[MAX_LEVEL];
	memset(mipmaps, 0, sizeof(mipmaps));

	{
		int mipLevels = 0;
		while ((1 << mipLevels) < max(CUBEMAP_WIDTH(&cubemap), CUBEMAP_HEIGHT(&cubemap))) mipLevels++;

		for (int mipLevel = 0; mipLevel < mipLevels; mipLevel++) {
			int size = 1 << (mipLevels - mipLevel);
			CubeMapAlloc(&mipmaps[mipLevel], size, size, 24);
		}

		GenerateCubeMipmaps(&cubemap, mipmaps, mipLevels, 1024);
		SaveCubeMipmaps(mipmaps, mipLevels);
	}

	for (int mipLevel = 0; mipLevel < MAX_LEVEL; mipLevel++) {
		CubeMapFree(&mipmaps[mipLevel]);
	}

	CubeMapFree(&cubemap);
	*/
}

#pragma endregion

#pragma region LUT

void GenerateLutMap(int argc, char** argv)
{
	if (argc != 5) {
		printf("Generate lut map fail!\n");
		printf("eg: CubeMapTool.exe -lut Lut.dds 128 128\n");
		return;
	}

	const char *szLutMapFileName = argv[2];
	const int width = atoi(argv[3]);
	const int height = atoi(argv[4]);

	gli::texture2d texLutMap(gli::FORMAT_RGBA8_UNORM_PACK8, gli::extent2d(width, height));
	{
		GenerateLutMap(texLutMap, 1024);
	}
	gli::save_dds(texLutMap, szLutMapFileName);

	return;
}

#pragma endregion

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(0, 0);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("");
	glewInit();

	if (argc > 1) {
		char *opt = argv[1];

		if (stricmp(opt, "-irr-env") == 0) {
			GenerateEnvIrradianceMap(argc, argv);
			return 0;
		}

		if (stricmp(opt, "-irr-cube") == 0) {
			GenerateCubeIrradianceMap(argc, argv);
			return 0;
		}

		if (stricmp(opt, "-mip-env") == 0) {
			GenerateEnvMipmaps(argc, argv);
			return 0;
		}

		if (stricmp(opt, "-mip-cube") == 0) {
			GenerateCubeMipmaps(argc, argv);
			return 0;
		}

		if (stricmp(opt, "-lut") == 0) {
			GenerateLutMap(argc, argv);
			return 0;
		}
	}

	printf("Error: Params not match!\n");
	return 0;
}
