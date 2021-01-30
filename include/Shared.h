#ifndef __INCLUDE_SHARED_H__
#define __INCLUDE_SHARED_H__

namespace shared {

#if defined(DIRECT3D_VERSION)
  extern LPDIRECT3D9              g_pD3D;
  extern LPDIRECT3DDEVICE9        g_pd3dDevice;
  extern D3DPRESENT_PARAMETERS    g_d3dpp;
#endif

} // namespace shared

#endif //__INCLUDE_SHARED_H__