#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "WowStuff\Camera.hpp"
#include "OM.hpp"

typedef std::chrono::high_resolution_clock Clock;

#pragma region Colors

//Colors are A,R,G,B meaning alpha(opacity), red, green, blue
D3DCOLOR RED = D3DCOLOR_ARGB(255, 255, 0, 0);
D3DCOLOR GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);
D3DCOLOR BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
D3DCOLOR WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
D3DCOLOR BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
D3DCOLOR YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
D3DCOLOR TEAL = D3DCOLOR_ARGB(255, 0, 255, 255);
D3DCOLOR PINK = D3DCOLOR_ARGB(255, 255, 240, 0);
D3DCOLOR ORANGE = D3DCOLOR_ARGB(255, 255, 132, 0);
D3DCOLOR LIME = D3DCOLOR_ARGB(255, 198, 255, 0);
D3DCOLOR SKYBLUE = D3DCOLOR_ARGB(255, 0, 180, 255);
D3DCOLOR MAROON = D3DCOLOR_ARGB(255, 142, 30, 0);
D3DCOLOR LGRAY = D3DCOLOR_ARGB(255, 174, 174, 174); //Light Gray
D3DCOLOR DGRAY = D3DCOLOR_ARGB(255, 71, 65, 64); //Dark Gray
D3DCOLOR BROWN = D3DCOLOR_ARGB(255, 77, 46, 38);
D3DCOLOR SHIT = D3DCOLOR_ARGB(255, 74, 38, 38); //Shit and brown aint much different

#pragma endregion

class Graphics
{
private:
	const int g_width = 640;
	const int g_height = 480;
	const int g_depth = 16; //16-bit colour

	LPDIRECT3DDEVICE9 dev;
	ID3DXFont* font;
	std::vector<std::string> texts;

public:

	Graphics() : dev{}, font{}
	{
	}

	~Graphics()
	{
		texts.clear();
		ReleaseFont();
	}	

	void AddTextLine(std::string line) { texts.push_back(line); }

	void Pulse(int device)
	{
		dev = (LPDIRECT3DDEVICE9)device;

		if (font == nullptr)
		{
			CreateFont(dev);
		}
		else
		{
			LPDIRECT3DDEVICE9* deviceFromFont = nullptr;
			auto hres = font->GetDevice(deviceFromFont);
			if (!deviceFromFont || *deviceFromFont != dev)
			{
				font->Release();
				CreateFont(dev);
			}
		}		

		dev->SetVertexShader(nullptr); 
		dev->SetPixelShader(nullptr);
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		dev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_LIGHTING, 0);
		dev->SetTexture(0, nullptr);
		dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		

		auto start = Clock::now();
		/*for (auto u : units)
		{
			DrawArrowOnTarget(me->Coords(), u->Coords(), D3DCOLOR_XRGB(0xff, 0xff, 0xff));
		}*/


		/*if (target != nullptr) 
		{
			DrawArrowOnTarget(me->Coords(), target->Coords(), YELLOW);
		}*/
		//auto pme = new LocalPlayer(((Object*)me)->addr);

		if (me)
		{
			auto pet = me->Pet();

			if (players.size() < 4)
			{
				for (auto& p : players)
				{
					DrawArrowOnTarget(me->Coords(), p->Coords(), p->CanAttack() ? YELLOW : BLUE);
				}
			}

			/*if (!Routine.objectSearch.IsNullOrEmpty())
			{
				foreach(var ob in OM.Objects.Where(o = >
					o.Name.ToLower().Contains(Routine.objectSearch)
					&& (!(o.IsUnit || o.IsPlayer) || (o as Unit).IsAlive)
					&& !o.IsItem))
				{
					DrawLine(me.Location, ob.Location, Color.Aqua);
				}
			}*/						

			for (auto& u : units)
			{
				if (u && u->Target())
				{
					if (u->IsPlayer())
						DrawArrowOnTarget(u->Coords(), u->Target()->Coords(), u->CanAttack() ? YELLOW : GREEN);
					else
						DrawArrowOnTarget(u->Coords(), u->Target()->Coords(), u->CanAttack() ? SKYBLUE : LGRAY);
				}
			}
		}

		auto delay = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count();
		texts.push_back("delay = " + std::to_string(delay));

		RECT font_rect = { 0, 0, g_width, g_height };
		int font_height;

		for (int i = 0; i != texts.size(); ++i)
		{
			SetRect(&font_rect, 10, +i * 24, g_width, g_height);
			font_height = font->DrawText(NULL, texts.at(i).c_str(), -1, &font_rect, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);
		}		
		texts.clear();
	}	

	void InternalRender(float* target)
	{				
		D3DXMATRIX world, projection, view;
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&world, target[0], target[1], target[2]);
		
		CameraInfo* cam = (CameraInfo*)GetPointer();
		D3DXMatrixPerspectiveFovRH(&projection, cam->FieldOfView * 0.6f, cam->Aspect, cam->NearPlane, cam->FarPlane);

		Location eye = cam->Position;
		Location at = eye + Forward();
		Location up( 0, 0, 1);
		D3DXMatrixLookAtRH(&view, &eye, &at, &up);

		dev->SetTransform(D3DTS_WORLD, &world);
		dev->SetTransform(D3DTS_VIEW, &view);
		dev->SetTransform(D3DTS_PROJECTION, &projection);
	}

	IDirect3DVertexBuffer9* VB = 0;	

	struct Vertex
	{
		Location pos;		
		int color;		
	};

	const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	void DrawLine(Location from, Location to, D3DCOLOR color)
	{
	/*	dev->CreateVertexBuffer(2 * sizeof(Vertex), D3DUSAGE_WRITEONLY, line_fvf, D3DPOOL_MANAGED, &VB, 0);		
		Vertex* vertices;

		VB->Lock(0, 0, (void**)&vertices, 0);		
		vertices[0].pos = Location(0, 0, 0);
		vertices[0].color = color;
		vertices[1].pos = to - from;
		vertices[1].color = color;
		VB->Unlock();

		InternalRender(from);
		dev->SetStreamSource(0, VB, 0, sizeof(Vertex));
		dev->SetFVF(line_fvf);
		dev->DrawPrimitive(D3DPT_LINELIST, 0, 1);*/

		Vertex vertices[2];
		vertices[0].pos = Location(0, 0, 0);
		vertices[0].color = color;

		vertices[1].pos = to - from;
		vertices[1].color = color;

		InternalRender(from);
		dev->SetFVF(line_fvf);
		dev->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(Vertex));
	}

	void DrawTriangle(Location b, Location c, Location d, D3DCOLOR color)
	{
		Vertex vertices[3];
		vertices[0].pos = Location(0, 0, 0);
		vertices[0].color = color;

		vertices[1].pos = d - b;
		vertices[1].color = color;

		vertices[2].pos = c - b;
		vertices[2].color = color;		

		InternalRender(b);
		dev->SetFVF(line_fvf);
		dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(Vertex));
	}		

	Location LerpByDistance(Location start, Location end, float scale)
	{		
		return scale * Normalize(end - start) + start;
	}

	void DrawArrowOnTarget(Location unit, Location target, int color)
	{
		auto start = unit + Location(0, 0, 1);
		auto end = target + Location(0, 0, 1);

		float dist = me->DistanceTo(target);
		if (dist > 130) dist = 130;
		float x = 0.01f;
		float scale = 1 + dist * x;

		float l = 0.3f * scale;
		float w = 0.06f * scale;

		DrawLine(start, end, color);

		auto c = LerpByDistance(end, start, l);

		auto d = c + Location(0, 0, w);
		DrawTriangle(end, c, d, color);
		d = c + Location(0, 0, -w);
		DrawTriangle(end, c, d, color);
		d = c + Location(0, w, 0);
		DrawTriangle(end, c, d, color);
		d = c + Location(0, -w, 0);
		DrawTriangle(end, c, d, color);
		d = c + Location(w, 0, 0);
		DrawTriangle(end, c, d, color);
		d = c + Location(-w, 0, 0);
		DrawTriangle(end, c, d, color);
	}

	void CreateFont(LPDIRECT3DDEVICE9 device)
	{
		auto hr = D3DXCreateFont(device,    //D3D Device
			22,								//Font height
			0,								//Font width
			FW_NORMAL,						//Font Weight
			1,								//MipLevels
			false,							//Italic
			DEFAULT_CHARSET,				//CharSet
			OUT_DEFAULT_PRECIS,				//OutputPrecision
			ANTIALIASED_QUALITY,			//Quality
			DEFAULT_PITCH | FF_DONTCARE,	//PitchAndFamily 
			"Consolas",						//pFacename,
			&font);							//ppFont
	}

	void ReleaseFont()
	{
		if (font != nullptr)
		{
			font->Release();
			font = nullptr;
		}
	}
};