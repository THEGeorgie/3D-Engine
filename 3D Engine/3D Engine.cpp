#include <iostream>
#include "CGE.h"
using namespace std;

struct vector3 {
	float x, y, z;
};

struct triangel {
	vector3 p[3];
};
struct mesh {
	vector<triangel> tris;
};

struct matrix4x4 {
	float m[4][4] = { 0 };
};

class  Engine3D : public olcConsoleGameEngine
{
public:
	Engine3D() {
		m_sAppName = L"3D rendere demo";
	 }
private:


	float fTheta;

	void MultiplyMatrixVector(vector3& i, vector3& o, matrix4x4& m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];


		if (w != 0.0f) {
			o.x /= w; o.y /= w; o.z = w;
		}
	}

	bool OnUserCreate() override {
		meshCube.tris = {
			//JUG
			{0.0f, 0.0f, 0.0f,	0.0f, 1.0f,0.0f,	1.0f,1.0f,0.0f},
			{0.0f, 0.0f, 0.0f,	1.0f, 1.0f,0.0f,	1.0f,0.0f,0.0f},

			//VZHOD
			{1.0f, 0.0f, 0.0f,	1.0f, 1.0f,0.0f,	1.0f,1.0f,1.0f},
			{1.0f, 0.0f, 0.0f,	1.0f, 1.0f,1.0f,	1.0f,0.0f,1.0f},

			//SEVER
			{1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f},

			//ZAHOD
			{0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f},

			//ZGORNJI 
			{0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f},

			//SPODNJI
			{1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f},


		};

		//PROJEKCIJA
		float fBlizu = 0.1f;
		float fDalec = 1000.0f;
		float fFov = 90.0f;
		float fAsR = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov *0.5f/ 180.0f * 3.14159f);

		matrixcProj.m[0][0] = fAsR * fFovRad;
		matrixcProj.m[1][1] = fFovRad;
		matrixcProj.m[2][2] = fDalec / (fDalec - fBlizu);
		matrixcProj.m[3][2] = (-fDalec * fBlizu) / (fDalec - fBlizu);
		matrixcProj.m[2][3] = 1.0f;
		matrixcProj.m[3][3] = 0.0f;

		return true;
	}
	

	bool OnUserUpdate(float fElapsedTime) override{
		
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		matrix4x4 matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;

		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		//RiSANJE TRIKOTNIKI
		for (auto tri : meshCube.tris) {

			triangel triProjekcija, triPrevajen, triRotacijaZ, triRotazijaZX;

			MultiplyMatrixVector(tri.p[0], triRotacijaZ.p[0], matRotZ);
			MultiplyMatrixVector(tri.p[1], triRotacijaZ.p[1], matRotZ);
			MultiplyMatrixVector(tri.p[2], triRotacijaZ.p[2], matRotZ);

			MultiplyMatrixVector(triRotacijaZ.p[0], triRotazijaZX.p[0], matRotX);
			MultiplyMatrixVector(triRotacijaZ.p[1], triRotazijaZX.p[1], matRotX);
			MultiplyMatrixVector(triRotacijaZ.p[2], triRotazijaZX.p[1], matRotX);

			triPrevajen = triRotazijaZX;
			triPrevajen.p[0].z = triRotazijaZX.p[0].z + 3.0f;
			triPrevajen.p[1].z = triRotazijaZX.p[1].z + 3.0f;
			triPrevajen.p[2].z = triRotazijaZX.p[2].z + 3.0f;

			MultiplyMatrixVector(triPrevajen.p[0], triProjekcija.p[0], matrixcProj);
			MultiplyMatrixVector(triPrevajen.p[1], triProjekcija.p[1], matrixcProj);
			MultiplyMatrixVector(triPrevajen.p[2], triProjekcija.p[2], matrixcProj);


			triProjekcija.p[0].x += 1.0f; triProjekcija.p[0].y += 1.0f;
			triProjekcija.p[1].x += 1.0f; triProjekcija.p[1].y += 1.0f;
			triProjekcija.p[2].x += 1.0f; triProjekcija.p[2].y += 1.0f;
			triProjekcija.p[0].x *= 0.5f * (float)ScreenWidth();
			triProjekcija.p[0].y *= 0.5f * (float)ScreenHeight();
			triProjekcija.p[1].x *= 0.5f * (float)ScreenWidth();
			triProjekcija.p[1].y *= 0.5f * (float)ScreenHeight();
			triProjekcija.p[2].x *= 0.5f * (float)ScreenWidth();
			triProjekcija.p[2].y *= 0.5f * (float)ScreenHeight();

			DrawTriangle(triProjekcija.p[0].x, triProjekcija.p[0].y,
				triProjekcija.p[1].x, triProjekcija.p[1].y,
				triProjekcija.p[2].x, triProjekcija.p[2].y,
				PIXEL_SOLID, FG_WHITE);
		}

		return true;
	}
};


int main() {
	Engine3D engine;

	if (engine.ConstructConsole(256, 240, 4, 4)) {
		engine.Start();
	}

	return 0;
}

