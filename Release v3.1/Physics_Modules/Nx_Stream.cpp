/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// Nx_Stream.cpp
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection And Other.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include "Nx_Stream.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For UserStream Class
///////////////////////////////////////////////////////////////////////////////
UserStream::UserStream(const char* filename,bool load): fp(NULL)
{
  fp = fopen(filename,load ? "rb":"wb");
} //EndConstructionDirectives

UserStream::~UserStream()
{
  if (fp) fclose(fp);
} //EndDestructionDirectives

//Loading API
//-----------------------------------------------------------------------------
// Name: UserStream::ReadByte()
// Desc: 
//-----------------------------------------------------------------------------
NxU8 UserStream::readByte() const
{
  NxU8 b;
  size_t r = fread(&b,sizeof(NxU8),1,fp);
  NX_ASSERT(r);
  return b;
} //EndReadByteFunction

//-----------------------------------------------------------------------------
// Name: UserStream::ReadWord()
// Desc: 
//-----------------------------------------------------------------------------
NxU16 UserStream::readWord() const
{
  NxU16 w;
  size_t r = fread(&w,sizeof(NxU16),1,fp);
  NX_ASSERT(r);
  return w;
} //EndReadWordFunction

//-----------------------------------------------------------------------------
// Name: UserStream::ReadDWord()
// Desc: 
//-----------------------------------------------------------------------------
NxU32 UserStream::readDword() const
{
  NxU32 d;
  size_t r = fread(&d,sizeof(NxU32),1,fp);
  NX_ASSERT(r);
  return d;
} //EndReadDWordFunction

//-----------------------------------------------------------------------------
// Name: UserStream::ReadFloat()
// Desc: 
//-----------------------------------------------------------------------------
float UserStream::readFloat() const
{
  NxReal f;
  size_t r = fread(&f,sizeof(NxReal),1,fp);
  NX_ASSERT(r);
  return f;
} //EndReadFloatFunction

//-----------------------------------------------------------------------------
// Name: UserStream::ReadDouble()
// Desc: 
//-----------------------------------------------------------------------------
double UserStream::readDouble() const
{
  NxF64 f;
  size_t r = fread(&f,sizeof(NxF64),1,fp);
  NX_ASSERT(r);
  return f;
} //EndReadDoubleFunction

//-----------------------------------------------------------------------------
// Name: UserStream::ReadBuffer()
// Desc: 
//-----------------------------------------------------------------------------
void UserStream::readBuffer(void* buffer,NxU32 size) const
{
  size_t w = fread(buffer,size,1,fp);
  NX_ASSERT(w);
} //EndReadBufferProcedure

//Saving API
//-----------------------------------------------------------------------------
// Name: UserStream::StoreByte()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &UserStream::storeByte(NxU8 b)
{
  size_t w = fwrite(&b,sizeof(NxU8),1,fp);
  NX_ASSERT(w);
  return *this;
} //EndStoreByteFunction

//-----------------------------------------------------------------------------
// Name: UserStream::StoreWord()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &UserStream::storeWord(NxU16 w)
{
  size_t ww = fwrite(&w,sizeof(NxU16),1,fp);
  NX_ASSERT(ww);
  return *this;
} //EndStoreWordFunction

//-----------------------------------------------------------------------------
// Name: UserStream::StoreDWord()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &UserStream::storeDword(NxU32 d)
{
  size_t w = fwrite(&d,sizeof(NxU32),1,fp);
  NX_ASSERT(w);
  return *this;
} //EndStoreDWordFunction

//-----------------------------------------------------------------------------
// Name: UserStream::StoreFloat()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &UserStream::storeFloat(NxReal f)
{
  size_t w = fwrite(&f,sizeof(NxReal),1,fp);
  NX_ASSERT(w);
  return *this;
} //EndStoreFloatFunction

//-----------------------------------------------------------------------------
// Name: UserStream::StoreDouble()
// Desc: 
//-----------------------------------------------------------------------------
NxStream& UserStream::storeDouble(NxF64 f)
{
  size_t w = fwrite(&f,sizeof(NxF64),1,fp);
  NX_ASSERT(w);
  return *this;
} //EndStoreDoubleFunction

//-----------------------------------------------------------------------------
// Name: UserStream::StoreBuffer()
// Desc: 
//-----------------------------------------------------------------------------
NxStream& UserStream::storeBuffer(const void* buffer,NxU32 size)
{
  size_t w = fwrite(buffer,size,1,fp);
  NX_ASSERT(w);
  return *this;
} //EndStoreBufferFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For MemoryWriteBuffer Class
///////////////////////////////////////////////////////////////////////////////
MemoryWriteBuffer::MemoryWriteBuffer(): currentSize(0),maxSize(0),data(NULL)
{
} //EndConstructionDirectives

MemoryWriteBuffer::~MemoryWriteBuffer()
{
  NX_DELETE_ARRAY(data);
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: MemoryWriteBuffer::Clear()
// Desc: 
//-----------------------------------------------------------------------------
void MemoryWriteBuffer::clear()
{
  currentSize = 0;
} //EndClearProcedure

//-----------------------------------------------------------------------------
// Name: MemoryWriteBuffer::StoreByte()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &MemoryWriteBuffer::storeByte(NxU8 b)
{	
  storeBuffer(&b,sizeof(NxU8));
  return *this;
} //EndStoreByteFunction

//-----------------------------------------------------------------------------
// Name: MemoryWriteBuffer::StoreWord()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &MemoryWriteBuffer::storeWord(NxU16 w)
{
  storeBuffer(&w,sizeof(NxU16));
  return *this;
} //EndStoreWordFunction

//-----------------------------------------------------------------------------
// Name: MemoryWriteBuffer::StoreDWord()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &MemoryWriteBuffer::storeDword(NxU32 d)
{
  storeBuffer(&d,sizeof(NxU32));
  return *this;
} //EndStoreDWordFunction

//-----------------------------------------------------------------------------
// Name: MemoryWriteBuffer::StoreFloat()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &MemoryWriteBuffer::storeFloat(NxReal f)
{
  storeBuffer(&f,sizeof(NxReal));
  return *this;
} //EndStoreFloat

//-----------------------------------------------------------------------------
// Name: MemoryWriteBuffer::StoreDouble()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &MemoryWriteBuffer::storeDouble(NxF64 f)
{
  storeBuffer(&f, sizeof(NxF64));
  return *this;
} //EndStoreDoubleFunction

//-----------------------------------------------------------------------------
// Name: MemoryWriteBuffer::StoreBuffer()
// Desc: 
//-----------------------------------------------------------------------------
NxStream &MemoryWriteBuffer::storeBuffer(const void* buffer,NxU32 size)
{
  NxU32 expectedSize = currentSize+size;
  if (expectedSize > maxSize)
  {
    maxSize = expectedSize+4096;

	NxU8* newData = new NxU8[maxSize];
	NX_ASSERT(newData != NULL);

	if (data)
	{
	  memcpy(newData,data,currentSize);
	  delete[] data;
	}
	data = newData;
  }
  memcpy(data+currentSize,buffer,size);
  currentSize+= size;
  return *this;
} //EndStoreBufferFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For MemoryReadBuffer Class
///////////////////////////////////////////////////////////////////////////////
MemoryReadBuffer::MemoryReadBuffer(const NxU8* data): buffer(data)
{
} //EndConstructionDirectives

MemoryReadBuffer::~MemoryReadBuffer()
{
  //We Don't Own The Data => No Delete
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: MemoryReadBuffer::ReadByte()
// Desc: 
//-----------------------------------------------------------------------------
NxU8 MemoryReadBuffer::readByte() const
{
  NxU8 b;
  memcpy(&b,buffer,sizeof(NxU8));
  buffer+= sizeof(NxU8);
  return b;
} //EndReadByteFunction

//-----------------------------------------------------------------------------
// Name: MemoryReadBuffer::ReadWord()
// Desc: 
//-----------------------------------------------------------------------------
NxU16 MemoryReadBuffer::readWord() const
{
  NxU16 w;
  memcpy(&w,buffer,sizeof(NxU16));
  buffer+= sizeof(NxU16);
  return w;
} //EndReadWordFunction

//-----------------------------------------------------------------------------
// Name: MemoryReadBuffer::ReadDWord()
// Desc: 
//-----------------------------------------------------------------------------
NxU32 MemoryReadBuffer::readDword() const
{
  NxU32 d;
  memcpy(&d,buffer,sizeof(NxU32));
  buffer+= sizeof(NxU32);
  return d;
} //EndReadDWordFunction

//-----------------------------------------------------------------------------
// Name: MemoryReadBuffer::ReadFloat()
// Desc: 
//-----------------------------------------------------------------------------
float MemoryReadBuffer::readFloat() const
{
  float f;
  memcpy(&f,buffer,sizeof(float));
  buffer+= sizeof(float);
  return f;
} //EndReadFloatFunction

//-----------------------------------------------------------------------------
// Name: MemoryReadBuffer::ReadDouble()
// Desc: 
//-----------------------------------------------------------------------------
double MemoryReadBuffer::readDouble() const
{
  double f;
  memcpy(&f,buffer,sizeof(double));
  buffer+= sizeof(double);
  return f;
} //EndReadDoubleFunction

//-----------------------------------------------------------------------------
// Name: MemoryReadBuffer::ReadBuffer()
// Desc: 
//-----------------------------------------------------------------------------
void MemoryReadBuffer::readBuffer(void* dest,NxU32 size) const
{
  memcpy(dest,buffer,size);
  buffer+= size;
} //EndReadBufferProcedure

//-----------------------------------------------------------------------------