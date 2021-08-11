#pragma once
class IBLAuxiliary
{
private:
	IBLAuxiliary() = default;
	~IBLAuxiliary() = default;

public:
	static void convertToCubemap(int width, int height, unsigned int hdrTexIndex, unsigned int cubemapTexIndex);
	//HDR env cube map convolution
	static void convoluteDiffuseIntegral(int width, int height, unsigned int cubemapTexIndex, unsigned int irradianceTexIndex);
	//prefilter HDR env cube map
	static void convoluteSpecularIntegral(int width, int height, unsigned int cubemapTexIndex, unsigned int prefilteredTexIndex);
	//precompute BRDF Integral
	static void convoluteSpecularBRDFIntegral(int width, int height, unsigned int brdfLutTexIndex);
};

