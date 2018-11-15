#pragma once

#include <cstdlib>
#include <cstdio>
#include <GL/glut.h>

class CheckerBoard{
public:
	CheckerBoard(){ ;  }
	~CheckerBoard(){ ; }

	void GenTex1(){
		GenerateCBImage();
		GenerateTexture();
	}

	void GenTex2() {
		GenerateBlueImage();
		GenerateTexture();
	}

	int GetPixel(int i, int j, int k)
	{
		return checkImage[i][j][k];
	}

protected:
	//A function to generate a checkerboard texture 
	void GenerateCBImage(void)
	{
		int i, j, c;
		for (i = 0; i < 256; i++) {
			for (j = 0; j < 256; j++) {
				c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
				// operator ^ : Bitwise exclusive OR
				// operator & : Bitwise AND
				checkImage[i][j][0] = (GLubyte)(255 - c);
				checkImage[i][j][1] = (GLubyte)(255 - c);
				checkImage[i][j][2] = (GLubyte)(255 - c);
				checkImage[i][j][3] = (GLubyte)255;
			}
		}
	}

	void GenerateBlueImage(void)
	{
		int i, j;
		for (i = 0; i < 256; i++) {
			for (j = 0; j < 256; j++) {
				checkImage[i][j][0] = (GLubyte)26;
				checkImage[i][j][1] = (GLubyte)173;
				checkImage[i][j][2] = (GLubyte)242;
				checkImage[i][j][3] = (GLubyte)255;
			}
		}
	}

	void GenerateTexture(){ 
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //in the s direction, the texture is repeated
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // in the t direction, the texture is repeated; try GL_CLAMP (and you may want to modify the texture coordinates accordingly)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	}
	GLubyte checkImage[256][256][4];

public:
	GLuint texName;
};
