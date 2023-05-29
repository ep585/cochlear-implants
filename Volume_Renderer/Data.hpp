#ifndef Data_hpp
#define Data_hpp

#include "vec2.hpp"
#include "vec3.hpp"
#include "Header.hpp"
#include "Render.hpp"
#include "Mask.hpp"

#define THRESHOLD_CT_DATA

class VData {
public:
	virtual void InitGLComponents() = 0;
	virtual void MakeTexture(const bool &autoWindow=false) const = 0;
	
protected:
	GLuint textureHandle;
	bool glInitialised = false;
};

class CTData : public VData {
public:
	void InitGLComponents() override;
	void MakeTexture(const bool &autoWindow=false) const override; // requires prior initialisation of GL components
	
	const GLuint &TextureHandle() const { return textureHandle; }
	const uint16_t &Width() const { return width; }
	const uint16_t &Height() const { return height; }
	const uint16_t &Depth() const { return depth; }
	const vec3 &Spacing() const { return spacing; }
	const float &DirVecLength() const { return dirVecLength; }
	
protected:
	uint16_t width, height, depth;
	vec3 spacing;
	float dirVecLength;
	
	int16_t *data = nullptr;
	
	const float backgroundColour[4] = {0.0f, 0.0f, 0.0f, 1.0f};
};

class CTDicomData : public CTData {
public:
	void Load(const char *const &filename, bool &leftPresent_out, bool &rightPresent_out, char *const &datasetName_out);
	
	const float (&C1Inverse() const)[4][4] { return matrixC1inverse; }
	const float (&C4() const)[4][4]{ return matrixC4; }
	
private:
	float matrixC1inverse[4][4];
	float matrixC4[4][4];
};

class XRData : public VData {
public:
	void Load(const int &_width, const int &_height, const vec2 &_spacing, uint16_t *const &_data);
	void InitGLComponents() override;
	void MakeTexture(const bool &autoWindow=false) const override; // requires prior initialisation of GL components
	
	const GLuint &TextureHandle() const { return textureHandle; }
	const uint16_t &Width() const { return width; }
	const uint16_t &Height() const { return height; }
	const vec2 &Spacing() const { return spacing; }
	const vec3 &SourcePosition() const { return sourcePosition; }
	
	Array2D<uint16_t> ToArrayWithRect(const SDL_Rect &rect) const;
	
	void FlipY();
	
protected:
	uint16_t width, height;
	vec2 spacing;
	vec3 sourcePosition;
	
	uint16_t *data = nullptr;
	
	const float backgroundColour[4] = {0.0f, 0.0f, 0.0f, 0.0f};
};

class XRDicomData : public XRData {
public:
	void Load(const char *const &filename);
	
	const float (&C2() const)[4][4]{ return matrixC2; }
	const float (&P() const)[4][4]{ return matrixP; }
	const float (&C3() const)[4][4]{ return matrixC3; }
	const float (&B2() const)[4][4]{ return matrixB2; }
	
	void UpdateSourceOffset(const vec2 &newSourceOffset);
	
private:
	float matrixC2[4][4];
	float matrixP[4][4];
	float matrixC3[4][4];
	float matrixB2[4][4];
};

class OtherData {
public:
	void Load(const bool &_leftPresent,
			  const bool &_rightPresent,
			  const char *const &fn_points3d,
			  const char *const &fn_points2d,
			  const float &_enlargement,
			  const char *const &fn_ocLeft,
			  const char *const &fn_ocRight);
	~OtherData();
	
	const Render::DrawArray &Points3d() const { return *points3d; }
	const Render::DrawArray &P3DTransformed() const { return *p3dTransformed; }
	const Render::DrawArray *const &P3DEnlargedLeft() const { return p3dEnlargedLeft; }
	const Render::DrawArray *const &P3DEnlargedRight() const { return p3dEnlargedRight; }
	const Render::DrawArray &Points2d() const { return *points2d; }
	const Render::DrawElementArray *const &LeftOC() const { return leftOticCapsule; }
	const Render::DrawElementArray *const &RightOC() const { return rightOticCapsule; }
	
	void Transform3dPoints(const vec3 &pan, const float rotationMatrix[4][4]/*, const vec2 &sourceOffset*/) const;
	void CalcEnlargedPointPositions() const;
	bool OutsideEnlarged() const;
	
	void WriteElectrodeRays(const vec3 &pan, const float rotationMatrix[4][4], const char *filename) const;
	
private:
	// point data:
	Render::DrawArray *points3d;
	Render::DrawArray *p3dTransformed;
	uint32_t n3d;
	const int subSampleEnlarged = 1;
	int nEnlarged;
	float enlargement;
	
	bool leftPresent, rightPresent;
	
	Render::DrawArray *p3dEnlargedLeft=nullptr;
	Render::DrawArray *p3dEnlargedRight=nullptr;
	
	Render::DrawArray *points2d;
	uint32_t n2d;
	Render::DrawElementArray *leftOticCapsule=nullptr;
	Render::DrawElementArray *rightOticCapsule=nullptr;
};

Params ReadInitialAlignment(const char *filename);
GLfloat *ReadPoints(const char *filename, uint32_t &d_out, uint32_t &n_out);

#endif /* Data_hpp */
