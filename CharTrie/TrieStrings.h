#pragma once

#include <wchar.h>
#include <string.h>
#include <cassert>
#include <vector>

namespace TrieStrings
{
    ////////////////////////////////////////////////////////////////////////////
    template <typename TCharType>
    class StringOfChars
    {
    public:

        virtual void clear() = 0;

        virtual void reserve(size_t size) = 0;

        virtual void assign(const TCharType* zeroEndBuf) = 0;
        virtual void assign(const TCharType* buf, size_t length) = 0;

        virtual void appendChar(TCharType newChar) = 0;

        virtual const TCharType* getStr()           const = 0;
        virtual bool             empty()            const = 0;
        virtual size_t           length()           const = 0;
        virtual TCharType        at(size_t index)   const = 0;
    };

    ////////////////////////////////////////////////////////////////////////////
    template <typename TCharType>
    class StringOfCharsZeroEnd : public StringOfChars<TCharType>
    {
    public:

        StringOfCharsZeroEnd(const TCharType* zeroEndBuf = nullptr);
        StringOfCharsZeroEnd(const StringOfCharsZeroEnd& other);
        StringOfCharsZeroEnd(StringOfCharsZeroEnd&& other) noexcept;
        virtual ~StringOfCharsZeroEnd();

        StringOfCharsZeroEnd<TCharType>& operator=(const StringOfChars<TCharType>& other);
        StringOfCharsZeroEnd<TCharType>& operator=(StringOfCharsZeroEnd<TCharType>&& other) noexcept;

        virtual void clear()                                      override;
        virtual void reserve(size_t size)                         override;
        virtual void assign(const TCharType* zeroEndBuf)          override;
        virtual void assign(const TCharType* buf, size_t length)  override;
        virtual void appendChar(TCharType newChar)                override;

        virtual const TCharType* getStr()           const         override;
        virtual bool             empty()            const         override;
        virtual size_t           length()           const         override;
        virtual TCharType        at(size_t index)   const         override;

    private:

        void intAssign(const TCharType* buf, size_t charsCount);

    private:

        std::vector<TCharType> m_buf;
    };

    ////////////////////////////////////////////////////////////////////////////
    template <typename TCharType>
    class StringOfCharsFixedLen : public StringOfChars<TCharType>
    {
    public:

        StringOfCharsFixedLen();
        StringOfCharsFixedLen(const TCharType* buf, size_t charsCount);
        StringOfCharsFixedLen(const StringOfCharsFixedLen& other);
        StringOfCharsFixedLen(StringOfCharsFixedLen&& other) noexcept;
        virtual ~StringOfCharsFixedLen();

        StringOfCharsFixedLen<TCharType>& operator=(const StringOfChars<TCharType>& other);
        StringOfCharsFixedLen<TCharType>& operator=(StringOfCharsFixedLen<TCharType>&& other) noexcept;

        virtual void clear()                                      override;
        virtual void reserve(size_t size)                         override;
        virtual void assign(const TCharType* zeroEndBuf)          override;
        virtual void assign(const TCharType* buf, size_t length)  override;
        virtual void appendChar(TCharType newChar)                override;

        virtual const TCharType* getStr()           const         override;
        virtual bool             empty()            const         override;
        virtual size_t           length()           const         override;
        virtual TCharType        at(size_t index)   const         override;

    private:

        void intAssign(const TCharType* buf, size_t charsCount);

    private:

        std::vector<TCharType> m_buf;
    };
}

namespace TrieStrings
{
    //------------------------------------------------------------------------//
    // ��������� ����� 0-��������������� ������ �� ����� ��������
    inline
    size_t GetStrLen(const char* buf)
    {
        return strlen(buf);
    }

    //------------------------------------------------------------------------//
    // ��������� ����� 0-��������������� ������ �� ������� ��������
    inline
    size_t GetStrLen(const wchar_t* buf)
    {
        return wcslen(buf);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsZeroEnd<TCharType>::StringOfCharsZeroEnd(const TCharType* zeroEndBuf)
    {
        intAssign(zeroEndBuf, zeroEndBuf ? GetStrLen(zeroEndBuf) : 0);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsZeroEnd<TCharType>::StringOfCharsZeroEnd(const StringOfCharsZeroEnd& other)
    {
        intAssign(other.getStr(), other.length());
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsZeroEnd<TCharType>::StringOfCharsZeroEnd(StringOfCharsZeroEnd&& other) noexcept
    {
        m_buf = std::move(other.m_buf);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    TrieStrings::StringOfCharsZeroEnd<TCharType>::~StringOfCharsZeroEnd()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsZeroEnd<TCharType>&
    StringOfCharsZeroEnd<TCharType>::operator=(const StringOfChars<TCharType>& other)
    {
        intAssign(other.getStr(), other.length());
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsZeroEnd<TCharType>&
    StringOfCharsZeroEnd<TCharType>::operator=(StringOfCharsZeroEnd<TCharType>&& other) noexcept
    {
        m_buf = std::move(other.m_buf);

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsZeroEnd<TCharType>::clear()
    {
        intAssign(nullptr, 0);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsZeroEnd<TCharType>::reserve(size_t size)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsZeroEnd<TCharType>::assign(const TCharType* zeroEndBuf)
    {
        intAssign(zeroEndBuf, zeroEndBuf ? GetStrLen(zeroEndBuf) : 0);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsZeroEnd<TCharType>::assign(const TCharType* buf, size_t length)
    {
        intAssign(buf, length);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsZeroEnd<TCharType>::appendChar(TCharType newChar)
    {
        m_buf.pop_back();
        m_buf.push_back(newChar);
        m_buf.push_back(0);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    inline
    const TCharType*
    StringOfCharsZeroEnd<TCharType>::getStr() const
    {
        return m_buf.data();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    bool
    StringOfCharsZeroEnd<TCharType>::empty() const
    {
        // � 0-��������������� ������ ������ ���������� ������ ���� ������
        return m_buf.size() < 2;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    inline
    size_t
    StringOfCharsZeroEnd<TCharType>::length() const
    {
        return m_buf.size() - 1;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    inline
    TCharType
    StringOfCharsZeroEnd<TCharType>::at(size_t index) const
    {
        return m_buf.at(index);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsZeroEnd<TCharType>::intAssign(const TCharType* buf, size_t charsCount)
    {
        m_buf.clear();
        m_buf.resize(charsCount + 1, 0);
        memcpy(&m_buf[0], buf, sizeof(TCharType) * charsCount);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsFixedLen<TCharType>::StringOfCharsFixedLen()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsFixedLen<TCharType>::StringOfCharsFixedLen(const TCharType* buf, size_t charsCount)
    {
        intAssign(buf, charsCount);
    }

    template<typename TCharType>
    inline StringOfCharsFixedLen<TCharType>::StringOfCharsFixedLen(const StringOfCharsFixedLen & other)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsFixedLen<TCharType>::StringOfCharsFixedLen(StringOfCharsFixedLen&& other) noexcept
    {
        m_buf = std::move(other.m_buf);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsFixedLen<TCharType>::~StringOfCharsFixedLen()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsFixedLen<TCharType>&
    StringOfCharsFixedLen<TCharType>::operator=(const StringOfChars<TCharType>& other)
    {
        intAssign(other.getStr(), other.length());

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    StringOfCharsFixedLen<TCharType>&
    StringOfCharsFixedLen<TCharType>::operator=(StringOfCharsFixedLen<TCharType>&& other) noexcept
    {
        m_buf = std::move(other.m_buf);

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsFixedLen<TCharType>::clear()
    {
        intAssign(nullptr, 0);
    }

    template<typename TCharType>
    void
    StringOfCharsFixedLen<TCharType>::reserve(size_t size)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsFixedLen<TCharType>::assign(const TCharType* zeroEndBuf)
    {
        intAssign(zeroEndBuf, zeroEndBuf ? GetStrLen(zeroEndBuf) : 0);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsFixedLen<TCharType>::assign(const TCharType* buf, size_t charsCount)
    {
        intAssign(buf, charsCount);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsFixedLen<TCharType>::appendChar(TCharType newChar)
    {
        m_buf.push_back(newChar);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    inline
    const TCharType*
    StringOfCharsFixedLen<TCharType>::getStr() const
    {
        return m_buf.data();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    bool
    StringOfCharsFixedLen<TCharType>::empty() const
    {
        return m_buf.empty();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    inline
    size_t
    StringOfCharsFixedLen<TCharType>::length() const
    {
        return m_buf.size();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    inline
    TCharType
    StringOfCharsFixedLen<TCharType>::at(size_t index) const
    {
        return m_buf.at(index);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType>
    void
    StringOfCharsFixedLen<TCharType>::intAssign(const TCharType* buf, size_t charsCount)
    {
        m_buf.clear();
        m_buf.resize(charsCount);
        memcpy(&m_buf[0], buf, sizeof(TCharType) * charsCount);
    }

    //------------------------------------------------------------------------//
}
