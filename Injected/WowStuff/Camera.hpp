#pragma once
#include <d3d9.h>

typedef struct
{
	int vtable;
	int field2;
	Location Position;
	float fViewMat[9];
	float NearPlane;
	float FarPlane;
	float FieldOfView;
	float Aspect;
}CameraInfo;

int GetPointer() { return *(int*)(*(int*)(0x00B7436C) + 0x7E20 /*WorldFrame + ActiveCamera*/); }

//D3DXMATRIX Projection()
//{
//	CameraInfo* cam = (CameraInfo*)GetPointer();
//	D3DXMATRIX matrix;
//	D3DXMatrixPerspectiveFovLH(&matrix, cam->FieldOfView * 0.6f, cam->Aspect, cam->NearPlane, cam->FarPlane);
//	return matrix;
//}

int GetVirtualFuncAddr(int addr, int offset) { return addr ? *(int*)(*(int*)addr + 4 * offset) : 0; }

Location Forward()
{
	int funcaddr = GetVirtualFuncAddr(GetPointer(), 1);
	
	Location res;
	((Location*(__thiscall*)(int, Location*))(funcaddr))(GetPointer(), &res);	
	return res;
}

//D3DXMATRIX View()
//{
//	CameraInfo* cam = (CameraInfo*)GetPointer();
//	Location eye = cam->Position;
//	Location at = eye + Forward();
//	Location up = { 0, 0, 1 };
//	D3DXMATRIX matrix;
//	D3DXMatrixLookAtRH(&matrix, &eye, &at, &up);
//	return matrix;
//}