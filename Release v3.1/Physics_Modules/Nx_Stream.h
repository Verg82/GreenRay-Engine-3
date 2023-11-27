/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// Nx_Stream.h: Directives From The Novodex Class.
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection And Other.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GREngine.h>

//-----------------------------------------------------------------------------
// Name: Struct UserStream
// Desc: 
//-----------------------------------------------------------------------------
class UserStream: public NxStream
{
  public:
	UserStream(const char* filename,bool load);
	virtual	~UserStream();

	virtual NxU8 readByte()	const;
	virtual	NxU16 readWord() const;
	virtual	NxU32 readDword() const;
	virtual	float readFloat() const;
	virtual	double readDouble()	const;
	virtual	void readBuffer(void* buffer,NxU32 size) const;

	virtual	NxStream &storeByte(NxU8 b);
	virtual	NxStream &storeWord(NxU16 w);
	virtual	NxStream &storeDword(NxU32 d);
	virtual	NxStream &storeFloat(NxReal f);
	virtual	NxStream &storeDouble(NxF64 f);
	virtual	NxStream &storeBuffer(const void* buffer,NxU32 size);
	FILE* fp;
}; //EndUserStreamClass

//-----------------------------------------------------------------------------
// Name: Struct MemoryWriteBuffer
// Desc: 
//-----------------------------------------------------------------------------
class MemoryWriteBuffer: public NxStream
{
  public:
	MemoryWriteBuffer();
	virtual	~MemoryWriteBuffer();
	void clear();

	virtual	NxU8 readByte()	const { NX_ASSERT(0); return 0;	}
	virtual	NxU16 readWord() const { NX_ASSERT(0); return 0; }
	virtual	NxU32 readDword() const	{ NX_ASSERT(0);	return 0; }
	virtual	float readFloat() const	{ NX_ASSERT(0);	return 0.0f; }
	virtual	double readDouble()	const { NX_ASSERT(0); return 0.0; }
	virtual	void readBuffer(void* buffer,NxU32 size)const { NX_ASSERT(0); }

	virtual	NxStream &storeByte(NxU8 b);
	virtual	NxStream &storeWord(NxU16 w);
	virtual	NxStream &storeDword(NxU32 d);
	virtual	NxStream &storeFloat(NxReal f);
	virtual	NxStream &storeDouble(NxF64 f);
	virtual	NxStream &storeBuffer(const void* buffer,NxU32 size);

	NxU32 currentSize;
	NxU32 maxSize;
	NxU8* data;
}; //EndMemoryWriteBufferClass

//-----------------------------------------------------------------------------
// Name: Struct MemoryReadBuffer
// Desc: 
//-----------------------------------------------------------------------------
class MemoryReadBuffer: public NxStream
{
  public:
	MemoryReadBuffer(const NxU8* data);
	virtual	~MemoryReadBuffer();

	virtual	NxU8 readByte() const;
	virtual	NxU16 readWord() const;
	virtual	NxU32 readDword() const;
	virtual	float readFloat() const;
	virtual	double readDouble()	const;
	virtual	void readBuffer(void* buffer,NxU32 size) const;

	virtual	NxStream &storeByte(NxU8 b)	{ NX_ASSERT(0);	return *this; }
	virtual	NxStream &storeWord(NxU16 w) { NX_ASSERT(0); return *this; }
	virtual	NxStream &storeDword(NxU32 d) { NX_ASSERT(0); return *this;	}
	virtual	NxStream &storeFloat(NxReal f) { NX_ASSERT(0); return *this; }
	virtual	NxStream &storeDouble(NxF64 f) { NX_ASSERT(0); return *this; }
	virtual	NxStream &storeBuffer(const void* buffer,NxU32 size) { NX_ASSERT(0); return *this; }

	mutable	const NxU8*	buffer;
}; //EndMemoryReadBufferClass

//-----------------------------------------------------------------------------