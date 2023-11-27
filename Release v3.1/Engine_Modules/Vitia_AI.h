/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// Vitia_AI.h: Interface For The Vitia Gandon AI Class.
// It's The GreenRay AI Engine.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2008
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GREngine.h>
#include <GR_Models.h>
#include <GRMethods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRVitia_AI;

//-----------------------------------------------------------------------------
// Name: Struct CGRVitia_AI
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_VITIA_AI_H__)
#define AFX_VITIA_AI_H__
class CGRVitia_AI
{	
public:
  /*Public Directives*/
  CGRVitia_AI(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRVitia_AI();
  void Release();
  
  void Draw();
public:
  /*Public Directives*/
private:
  /*Private Directives*/ 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface

};
#endif //!defined(AFX_VITIA_AI_H__)