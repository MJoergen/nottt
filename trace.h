#ifndef _TRACE_H_
#define _TRACE_H_

#include <stdio.h>
#include <streambuf>
#include <ostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

class CAppendFile
{
  public:
    CAppendFile(const char *fileName)
    {
      fd = open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0644);
    }
    ~CAppendFile()
    {
      if (fd != -1)
        close(fd);
    }
    int Write(char *buffer, int num)
    {
      return write(fd, buffer, num);
    }
  private:
    int fd;
};

class CTraceBuf : public std::streambuf
{
  protected:
    static const int bufferSize = 200;
    char buffer[bufferSize];

  public:
    CTraceBuf(const char *fileName) : fileName(fileName)
    {
      setp(buffer, buffer + (bufferSize-1));
    }

    virtual ~CTraceBuf()
    {
      sync();
    }

  protected:
    int flushBuffer()
    {
      int num = pptr() - pbase();
      {
        CAppendFile fd(fileName);

        if (fd.Write(buffer, num) != num)
        {
          return EOF;
        }
      }

      pbump (-num);
      return num;
    }

    virtual int overflow(int c)
    {
      if (c != EOF)
      {
        *pptr() = c;
        pbump(1);
      }

      if (flushBuffer() == EOF)
      {
        return EOF;
      }

      return c;
    }

    virtual int sync()
    {
      if (flushBuffer() == EOF)
      {
        return -1;
      }
      return 0;
    }
  private:
    const char *fileName;
}; /* end of class CTraceBuf */

class CTrace : public std::ostream
{
  protected:
    CTraceBuf buf;
  public:
    CTrace(const char *fileName) :
      std::ostream(&buf) ,
      buf(fileName)
  {
  }
}; /* end of class CTrace */

#ifdef ENABLE_TRACE

extern std::ostream *gpTrace;

class CTraceFunction
{
  public:
    CTraceFunction(const char *functionName) :
      functionName(functionName)
  {
    if (gpTrace) {*gpTrace << "+" << functionName << std::endl;}
  }
    ~CTraceFunction()
    {
      if (gpTrace) {*gpTrace << "-" << functionName << std::endl;}
    }

  private:
    const char *functionName;
}; /* end of class CTraceFunction */

#define TRACE(x) if (gpTrace) {*gpTrace << x;}
#define TRACE_FUNCTION(x) CTraceFunction temp(x);

#define DEBUG(x) x

#else

#define TRACE(x)
#define TRACE_FUNCTION(x)
#define DEBUG(x)

#endif

extern std::ostream *gpLog;
#define LOG(x) if (gpLog) {*gpLog << x;}

#endif

