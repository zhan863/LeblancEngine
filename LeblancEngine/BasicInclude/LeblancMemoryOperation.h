//======================================================================
//
//	MJP's DX11 Sample Framework
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

#pragma once

#define safe_delete(p) { if (p) { delete(p); (p) = nullptr; } }
#define safe_delete_array(p) { if(p) { delete[] (p); (p)=nullptr; } }
#define safe_Release(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#define safe_release(p) { if(p) { (p)->release(); (p)=nullptr; } }