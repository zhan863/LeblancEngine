#include "LeblancEngine/Asset/LeblancDataStream.h"
#include <assert.h>

namespace Leblanc
{
	template <typename T> DataStream& DataStream::operator >>(T& val)
	{
		read(static_cast<void*>(&val), sizeof(T));
		return *this;
	}

	size_t DataStream::size(void) const
	{
		return mSize;
	}

	void DataStream::setError() const
	{
	}

	bool DataStream::readBytes(void* buffer, size_t length)
	{
		return read(buffer, length) > 0;
	}

	bool DataStream::readFloat(float& value)
	{
		read(&value, 4);
		return true;
	}

	bool DataStream::readInteger(int& value)
	{
		read(&value, 4);
		return true;
	}

	bool DataStream::readUnsignedInteger(unsigned int& value)
	{
		read(&value, 4);
		return true;
	}

	bool DataStream::readString(std::string& str_value)
	{
		int length = 0;
		readInteger(length);

		uint8_t * buffer = (uint8_t*)malloc((length + 1) * sizeof(uint8_t));
		memset(buffer, 0, sizeof(char) * (length + 1));
		read(buffer, length);
		str_value = std::string((const char*)buffer);

		free(buffer);

		return true;
	}

	MemoryDataStream::MemoryDataStream(void* pMem, size_t inSize, bool freeOnClose, bool readOnly)
		: DataStream(static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		mData = mPos = static_cast<uint8_t*>(pMem);
		mSize = inSize;
		mEnd = mData + mSize;
		mFreeOnClose = freeOnClose;
		assert(mEnd >= mPos);
	}

	MemoryDataStream::MemoryDataStream(const std::string& name, void* pMem, size_t inSize,
		bool freeOnClose, bool readOnly)
		: DataStream(name, static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		mData = mPos = static_cast<uint8_t*>(pMem);
		mSize = inSize;
		mEnd = mData + mSize;
		mFreeOnClose = freeOnClose;
		assert(mEnd >= mPos);
	}

	MemoryDataStream::MemoryDataStream(DataStream& sourceStream,
		bool freeOnClose, bool readOnly)
		: DataStream(static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		// Copy data from incoming stream
		mSize = sourceStream.size();
		if (mSize == 0 && !sourceStream.eof())
		{
			throw std::runtime_error("Unhandled stream copy condition");
		}
		else
		{
			mData = (uint8_t*)malloc(mSize * sizeof(uint8_t));
			mPos = mData;
			mEnd = mData + sourceStream.read(mData, mSize);
			mFreeOnClose = freeOnClose;
		}
		assert(mEnd >= mPos);
	}

	MemoryDataStream::MemoryDataStream(DataStreamPtr& sourceStream,
		bool freeOnClose, bool readOnly)
		: DataStream(static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		// Copy data from incoming stream
		mSize = sourceStream->size();
		if (mSize == 0 && !sourceStream->eof())
		{
			throw std::runtime_error("Unhandled stream copy condition");
		}
		else
		{
			mData = (uint8_t*)malloc(sizeof(uint8_t) * mSize);
			mPos = mData;
			mEnd = mData + sourceStream->read(mData, mSize);
			mFreeOnClose = freeOnClose;
		}
		assert(mEnd >= mPos);
	}

	MemoryDataStream::MemoryDataStream(const std::string& name, DataStream& sourceStream,
		bool freeOnClose, bool readOnly)
		: DataStream(name, static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		// Copy data from incoming stream
		mSize = sourceStream.size();
		if (mSize == 0 && !sourceStream.eof())
		{
			throw std::runtime_error("Unhandled stream copy condition");
		}
		else
		{
			mData = (uint8_t*)malloc(sizeof(uint8_t) * mSize);
			mPos = mData;
			mEnd = mData + sourceStream.read(mData, mSize);
			mFreeOnClose = freeOnClose;
		}
		assert(mEnd >= mPos);
	}

	MemoryDataStream::MemoryDataStream(const std::string& name, const DataStreamPtr& sourceStream,
		bool freeOnClose, bool readOnly)
		: DataStream(name, static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		// Copy data from incoming stream
		mSize = sourceStream->size();
		if (mSize == 0 && !sourceStream->eof())
		{
			throw std::runtime_error("Unhandled stream copy condition");
		}
		else
		{
			mData = (uint8_t*)malloc(sizeof(uint8_t) * mSize);
			mPos = mData;
			mEnd = mData + sourceStream->read(mData, mSize);
			mFreeOnClose = freeOnClose;
		}
		assert(mEnd >= mPos);
	}

	MemoryDataStream::MemoryDataStream(size_t inSize, bool freeOnClose, bool readOnly)
		: DataStream(static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		mSize = inSize;
		mFreeOnClose = freeOnClose;
		mData = (uint8_t*)malloc(sizeof(uint8_t) * mSize);
		mPos = mData;
		mEnd = mData + mSize;
		assert(mEnd >= mPos);
	}

	MemoryDataStream::MemoryDataStream(const std::string& name, size_t inSize,
		bool freeOnClose, bool readOnly)
		: DataStream(name, static_cast<uint16_t>(readOnly ? READ : (READ | WRITE)))
	{
		mSize = inSize;
		mFreeOnClose = freeOnClose;
		mData = (uint8_t*)malloc(sizeof(uint8_t) * mSize);
		mPos = mData;
		mEnd = mData + mSize;
		assert(mEnd >= mPos);
	}

	MemoryDataStream::~MemoryDataStream()
	{
		close();
	}

	size_t MemoryDataStream::read(void* buf, size_t count)
	{
		size_t cnt = count;
		// Read over end of memory?
		if (mPos + cnt > mEnd)
			cnt = mEnd - mPos;
		if (cnt == 0)
			return 0;

		assert(cnt <= count);

		memcpy(buf, mPos, cnt);
		mPos += cnt;
		return cnt;
	}

	size_t MemoryDataStream::write(const void* buf, size_t count)
	{
		size_t written = 0;
		if (isWriteable())
		{
			written = count;
			// we only allow writing within the extents of allocated memory
			// check for buffer overrun & disallow
			if (mPos + written > mEnd)
				written = mEnd - mPos;
			if (written == 0)
				return 0;

			memcpy(mPos, buf, written);
			mPos += written;
		}
		return written;
	}

	bool MemoryDataStream::ok() const
	{
		return (mPos <= mEnd && mPos >= mData) && mData != nullptr;
	}

	void MemoryDataStream::skip(long count)
	{
		size_t newpos = (size_t)((mPos - mData) + count);
		assert(mData + newpos <= mEnd);

		mPos = mData + newpos;
	}

	void MemoryDataStream::seek(size_t pos)
	{
		assert(mData + pos <= mEnd);
		mPos = mData + pos;
	}

	size_t MemoryDataStream::tell(void) const
	{
		//mData is start, mPos is current location
		return mPos - mData;
	}
	//-----------------------------------------------------------------------
	bool MemoryDataStream::eof(void) const
	{
		return mPos >= mEnd;
	}
	//-----------------------------------------------------------------------
	void MemoryDataStream::close(void)
	{
		if (mFreeOnClose && mData)
		{
			free(mData);
			mData = 0;
		}

	}
}