#pragma once
class IBLAuxiliary
{
private:
	IBLAuxiliary() = default;
	~IBLAuxiliary() = default;

public:
	static void convertToCubemap(int width, int height, unsigned int hdrTexIndex, unsigned int cubemapTexIndex);
	static void convoluteDiffuseIntegral(int width, int height, unsigned int cubemapTexIndex, unsigned int irradianceTexIndex);
};

