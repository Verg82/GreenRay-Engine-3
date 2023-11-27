/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_RayCast.h: Interface For The TGRRayCast Class.
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2008
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GREngine.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRRayCast;

//-----------------------------------------------------------------------------
// Name: Struct CGRRayCast
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_RAYCAST_H__)
#define AFX_RAYCAST_H__
class CGRRayCast
{	
public:
  /*Public Directives*/
  CGRRayCast(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRRayCast();
  void Release();
  
public:
  /*Public Directives*/
private:
  /*Private Directives*/ 

};
#endif //!defined(AFX_RAYCAST_H__)