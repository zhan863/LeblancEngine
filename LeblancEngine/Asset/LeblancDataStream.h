#ifndef LEBLANC_DATA_STREAM_H
#define LEBLANC_DATA_STREAM_H

#include <limits.h>
#include <memory>
#include <istream>

namespace Leblanc
{
	class DataStream
	{
	public:
		enum AccessMode
		{
			READ = 1,
			WRITE = 2
		};
	protected:
		std::string mName;
		size_t mSize;
		uint16_t mAccess;

#define BB_STREAM_BUFFER_SIZE 128
	public:
		DataStream(uint16_t accessMode = READ) : mSize(0), mAccess(accessMode) {}
		DataStream(const std::string& name, uint16_t accessMode = READ)
			: mName(name), mSize(0), mAccess(accessMode) {}
		const std::string& getName(void) { return mName; }
		uint16_t getAccessMode() const { return mAccess; }
		virtual bool isReadable() const { return (mAccess & READ) != 0; }
		virtual bool isWriteable() const { return (mAccess & WRITE) != 0; }
		virtual ~DataStream() {}
		template<typename T> DataStream& operator>>(T& val);

		virtual bool ok() const = 0;
		void setError() const;
		bool readBytes(void* pBuffer, size_t length);
		bool readFloat(float& value);
		bool readInteger(int& value);
		bool readUnsignedInteger(unsigned int& value);
		bool readString(std::string& strValue);

		virtual size_t read(void* buf, size_t count) = 0;

		virtual size_t write(const void* buf, size_t count)
		{
			(void)buf;
			(void)count;
			// default to not supported
			return 0;
		}

		virtual void skip(long count) = 0;

		virtual void seek(size_t pos) = 0;

		virtual size_t tell(void) const = 0;

		virtual bool eof(void) const = 0;

		size_t size(void) const;

		virtual void close(void) = 0;
	};

	typedef std::unique_ptr<DataStream> DataStreamPtr;

	class MemoryDataStream : public DataStream
	{
	protected:
		uint8_t * mData;
		uint8_t* mPos;
		uint8_t* mEnd;
		bool mFreeOnClose;
	public:

		MemoryDataStream(void* pMem, size_t size, bool freeOnClose = false, bool readOnly = false);
		MemoryDataStream(const std::string& name, void* pMem, size_t size,
			bool freeOnClose = false, bool readOnly = false);
		MemoryDataStream(DataStream& sourceStream,
			bool freeOnClose = true, bool readOnly = false);
		MemoryDataStream(DataStreamPtr& sourceStream,
			bool freeOnClose = true, bool readOnly = false);
		MemoryDataStream(const std::string& name, DataStream& sourceStream,
			bool freeOnClose = true, bool readOnly = false);
		MemoryDataStream(const std::string& name, const DataStreamPtr& sourceStream,
			bool freeOnClose = true, bool readOnly = false);
		MemoryDataStream(size_t size, bool freeOnClose = true, bool readOnly = false);
		MemoryDataStream(const std::string& name, size_t size,
			bool freeOnClose = true, bool readOnly = false);

		virtual ~MemoryDataStream();

		uint8_t *                  getPtr(void) { return mData; }
		uint8_t *                  getCurrentPtr(void) { return mPos; }
		size_t                     read(void* buf, size_t count);
		size_t                     write(const void* buf, size_t count);
		size_t                     readLine(char* buf, size_t maxCount, const std::string& delim = "\n");

		virtual bool               ok() const;
		size_t                     skipLine(const std::string& delim = "\n");
		void                       skip(long count);

		void                       seek(size_t pos);

		size_t                     tell(void) const;

		bool                       eof(void) const;

		void                       close(void);

		void                       setFreeOnClose(bool free) { mFreeOnClose = free; }
	};
}
#endif