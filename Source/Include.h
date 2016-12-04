
#ifndef _INCLUDE_H
#define _INCLUDE_H

#ifndef NULL
#define NULL 0
#endif


template<typename BUFFER_BASE_TYPE>
class CBufferBase
{
public:
    CBufferBase()
    {
    }
    virtual void Reset(BUFFER_BASE_TYPE nInit = 0) = 0;
    virtual bool Add( BUFFER_BASE_TYPE nAdd ) = 0;
    virtual bool Get( BUFFER_BASE_TYPE &nGet ) = 0;
    virtual int GetSize() = 0;
};

template<typename BUFFER_TYPE, int BUFFER_SIZE>
class CBuffer: public CBufferBase< BUFFER_TYPE >
{
protected:
    int m_nHead;
    int m_nTail;
    BUFFER_TYPE m_pArray[BUFFER_SIZE];
    void Increment(int &nVal)
    {
        nVal++;
        if( nVal >= BUFFER_SIZE )
            nVal -= BUFFER_SIZE;
    }
    void Decrement(int &nVal)
    {
        nVal--;
        if( nVal < 0 )
            nVal += BUFFER_SIZE;
    }
public:
    CBuffer( BUFFER_TYPE nInit = 0 ): CBufferBase<BUFFER_TYPE>()
    {
        Reset(nInit);
    }
    virtual void Reset(BUFFER_TYPE nInit = 0)
    {
        for( int i = 0; i < BUFFER_SIZE; i++ )
        {
            m_pArray[i] = nInit;
        }
        m_nHead = 0;
        m_nTail = 0;
    }
    virtual bool Add( BUFFER_TYPE nAdd )
    {
        Increment(m_nHead);
        //check to see if the buffer has wrapped
        if(m_nHead == m_nTail)
        {
            Decrement(m_nHead);
            return false;
        }
        m_pArray[m_nHead] = nAdd;
        return true;
    }
    virtual bool Get( BUFFER_TYPE &nGet )
    {
        if(m_nHead == m_nTail)
        {
            return false;
        }
        Increment(m_nTail);
        nGet = m_pArray[m_nTail];
        //set the last read value to zero
        m_pArray[m_nTail] = 0;
        return true;
    }
    virtual int GetSize()
    {
        int nSize = m_nHead - m_nTail;
        if( nSize < 0 )
            nSize += BUFFER_SIZE;
        return nSize;
    }
};

typedef CBufferBase<char> CharBufferBase;

class CString
{
public:

    //print an unsigned integer to an array
    static void Print(char * cArray, int &nLength, unsigned int iVal)  
    {
        int nSize = 0;
        if( iVal == 0)
        {
            if( nLength >= 1)
            {
                cArray[nSize++] = '0';
            }
        }
        else
        {
            while( ( iVal > 0 ) && ( nSize < nLength ) )
            {
                unsigned int iTemp = iVal / 10;
                cArray[ nSize++ ] = iVal - iTemp * 10 + '0';
                iVal = iTemp;
            }
            //flip the array 
            for( int i = 0; i < ( nSize / 2 ) ; i++)
            {
                char cTemp = cArray[i];
                cArray[i] = cArray[ nSize - i - 1 ];
                cArray[ nSize - i - 1 ] = cTemp;
            }
        }
        nLength = nSize;
    }
    static void Print(char * cArray, int &nLength, int iVal) 
    {
        if( iVal < 0 )
        {
            cArray[0] = '-';
            nLength--;
            Print( cArray+1, nLength, static_cast<unsigned int>(-iVal));
            nLength++;
        }
		else
		{
			Print( cArray, nLength, static_cast<unsigned int>(iVal));
		}
    }
};
//template<int CHAR_BUFFER_SIZE>
//typedef CBuffer<char,CHAR_BUFFER_SIZE> CharBuffer;
#endif //_INCLUDE_H
