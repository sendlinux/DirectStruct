#ifndef _H_BankTransaction_
#define _H_BankTransaction_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if ( defined __unix ) || ( defined __linux__ )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC
#endif
#elif ( defined _WIN32 )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC		_declspec(dllexport)
#endif
#endif

#if ( defined __unix ) | ( defined __linux__ )
#include <arpa/inet.h>
#elif ( defined _WIN32 )
#include "winsock.h"
#endif

#if ( defined __unix ) | ( defined __linux__ )
#ifndef longlong
#define longlong	long long
#define ulonglong	unsigned long long
#define LL(_ll_)	_ll_##LL
#define ULL(_ll_)	_ll_##ULL
#define LONGLONG_FORMAT_SPEC	"%ll"
#endif
#elif ( defined _WIN32 )
#ifndef longlong
#define longlong	__int64
#define ulonglong	unsigned __int64
#define LL(_ll_)	_ll_
#define ULL(_ll_)	_ll_
#define LONGLONG_FORMAT_SPEC	"%I64"
#define __BIG_ENDIAN	4321
#define __LITTLE_ENDIAN	1234
#define __BYTE_ORDER	__LITTLE_ENDIAN
#endif
#endif

#ifndef HTONLL
#define HTONLL(_ulonglong_)	(__BYTE_ORDER==__BIG_ENDIAN?(_ulonglong_):( (((ulonglong)htonl((unsigned int)(((_ulonglong_)<<32)>>32)))<<32) | ((ulonglong)htonl((unsigned int)((_ulonglong_)>>32))) ) )
#define NTOHLL(_ulonglong_)	(__BYTE_ORDER==__BIG_ENDIAN?(_ulonglong_):( (((ulonglong)ntohl((unsigned int)(((_ulonglong_)<<32)>>32)))<<32) | ((ulonglong)ntohl((unsigned int)((_ulonglong_)>>32))) ) )
#endif

#ifndef COMPRESS_INT4
#define COMPRESS_INT4(_int4_,_buf_,_bufoffset_)					\
	{									\
		unsigned char	_flag_ = 0 ;					\
		unsigned int	_h_ ;						\
		unsigned int	_n_ ;						\
		if( _int4_ < 0 )						\
		{								\
			_h_ = -_int4_ ;						\
			_n_ = htonl( _h_ ) ;					\
			_flag_ |= 0x10 ;					\
		}								\
		else								\
		{								\
			_h_ = _int4_ ;						\
			_n_ = htonl( _h_ ) ;					\
		}								\
		if( _h_ & 0xFF000000 )						\
		{								\
			_buf_[0] = _flag_ | 4 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 4 );	\
			_bufoffset_+=5; _buf_+=5 ;				\
		}								\
		else if( _h_ & 0x00FF0000 )					\
		{								\
			_buf_[0] = _flag_ | 3 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 3 );	\
			_bufoffset_+=4; _buf_+=4 ;				\
		}								\
		else if( _h_ & 0x0000FF00 )					\
		{								\
			_buf_[0] = _flag_ | 2 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 2 );	\
			_bufoffset_+=3; _buf_+=3 ;				\
		}								\
		else								\
		{								\
			_buf_[0] = _flag_ | 1 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 1 );	\
			_bufoffset_+=2; _buf_+=2 ;				\
		}								\
	}
#endif

#ifndef UNCOMPRESS_INT4
#define UNCOMPRESS_INT4(_buf_,_bufoffset_,_int4_)				\
	{									\
		unsigned char	_flag_ = 0 ;					\
		_int4_ = 0 ;							\
		_flag_ = _buf_[0] & 0xF0 ;					\
		_buf_[0] &= 0x0F ;						\
		memcpy( ((unsigned char*)&_int4_) + (4-_buf_[0]) , _buf_+1 , _buf_[0] );\
		_int4_ = ntohl( _int4_ ) ;					\
		if( _flag_ & 0x10 )						\
		{								\
			_int4_ = -_int4_ ;					\
		}								\
		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\
	}
#endif

#ifndef COMPRESS_UINT4
#define COMPRESS_UINT4(_uint4_,_buf_,_bufoffset_)				\
	{									\
		unsigned int	_h_ ;						\
		unsigned int	_n_ ;						\
		_h_ = _uint4_ ;							\
		_n_ = htonl( _h_ ) ;						\
		if( _h_ & 0xFF000000 )						\
		{								\
			_buf_[0] = 4 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 4 );	\
			_bufoffset_+=5; _buf_+=5 ;				\
		}								\
		else if( _h_ & 0x00FF0000 )					\
		{								\
			_buf_[0] = 3 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 3 );	\
			_bufoffset_+=4; _buf_+=4 ;				\
		}								\
		else if( _h_ & 0x0000FF00 )					\
		{								\
			_buf_[0] = 2 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 2 );	\
			_bufoffset_+=3; _buf_+=3 ;				\
		}								\
		else								\
		{								\
			_buf_[0] = 1 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 1 );	\
			_bufoffset_+=2; _buf_+=2 ;				\
		}								\
	}
#endif

#ifndef UNCOMPRESS_UINT4
#define UNCOMPRESS_UINT4(_buf_,_bufoffset_,_uint4_)				\
	{									\
		_uint4_ = 0 ;							\
		_buf_[0] &= 0x0F ;						\
		memcpy( ((unsigned char*)&_uint4_) + (4-_buf_[0]) , _buf_+1 , _buf_[0] );\
		_uint4_ = ntohl( _uint4_ ) ;					\
		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\
	}
#endif

#ifndef COMPRESS_INT8
#define COMPRESS_INT8(_int8_,_buf_,_bufoffset_)					\
	{									\
		unsigned char	_flag_ = 0 ;					\
		unsigned longlong	_h_ ;					\
		unsigned longlong	_n_ ;					\
		if( _int8_ < 0 )						\
		{								\
			_h_ = -_int8_ ;						\
			_n_ = HTONLL( _h_ ) ;					\
			_flag_ |= 0x10 ;					\
		}								\
		else								\
		{								\
			_h_ = _int8_ ;						\
			_n_ = HTONLL( _h_ ) ;					\
		}								\
		if( _h_ & ULL(0xFF00000000000000) )				\
		{								\
			_buf_[0] = _flag_ | 8 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 8 );	\
			_bufoffset_+=9; _buf_+=9 ;				\
		}								\
		else if( _h_ & ULL(0x00FF000000000000) )				\
		{								\
			_buf_[0] = _flag_ | 7 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 7 );	\
			_bufoffset_+=8; _buf_+=8 ;				\
		}								\
		else if( _h_ & ULL(0x0000FF0000000000) )				\
		{								\
			_buf_[0] = _flag_ | 6 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 6 );	\
			_bufoffset_+=7; _buf_+=7 ;				\
		}								\
		else if( _h_ & ULL(0x000000FF00000000) )				\
		{								\
			_buf_[0] = _flag_ | 5 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 5 );	\
			_bufoffset_+=6; _buf_+=6 ;				\
		}								\
		else if( _h_ & ULL(0x00000000FF000000) )				\
		{								\
			_buf_[0] = _flag_ | 4 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+4 , 4 );	\
			_bufoffset_+=5; _buf_+=5 ;				\
		}								\
		else if( _h_ & ULL(0x0000000000FF0000) )				\
		{								\
			_buf_[0] = _flag_ | 3 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+5 , 3 );	\
			_bufoffset_+=4; _buf_+=4 ;				\
		}								\
		else if( _h_ & ULL(0x000000000000FF00) )				\
		{								\
			_buf_[0] = _flag_ | 2 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+6 , 2 );	\
			_bufoffset_+=3; _buf_+=3 ;				\
		}								\
		else								\
		{								\
			_buf_[0] = _flag_ | 1 ;					\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+7 , 1 );	\
			_bufoffset_+=2; _buf_+=2 ;				\
		}								\
	}
#endif

#ifndef UNCOMPRESS_INT8
#define UNCOMPRESS_INT8(_buf_,_bufoffset_,_int8_)				\
	{									\
		unsigned char	_flag_ = 0 ;					\
		_int8_ = 0 ;							\
		_flag_ = _buf_[0] & 0xF0 ;					\
		_buf_[0] &= 0x0F ;						\
		memcpy( ((unsigned char*)&_int8_) + (8-_buf_[0]) , _buf_+1 , _buf_[0] );\
		_int8_ = NTOHLL( _int8_ ) ;					\
		if( _flag_ & 0x10 )						\
		{								\
			_int8_ = -_int8_ ;					\
		}								\
		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\
	}
#endif

#ifndef COMPRESS_UINT8
#define COMPRESS_UINT8(_uint8_,_buf_,_bufoffset_)				\
	{									\
		unsigned longlong	_h_ ;					\
		unsigned longlong	_n_ ;					\
		_h_ = _uint8_ ;							\
		_n_ = HTONLL( _h_ ) ;						\
		if( _h_ & ULL(0xFF00000000000000) )				\
		{								\
			_buf_[0] = 8 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 8 );	\
			_bufoffset_+=9; _buf_+=9 ;				\
		}								\
		else if( _h_ & ULL(0x00FF000000000000) )				\
		{								\
			_buf_[0] = 7 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 7 );	\
			_bufoffset_+=8; _buf_+=8 ;				\
		}								\
		else if( _h_ & ULL(0x0000FF0000000000) )				\
		{								\
			_buf_[0] = 6 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 6 );	\
			_bufoffset_+=7; _buf_+=7 ;				\
		}								\
		else if( _h_ & ULL(0x000000FF00000000) )				\
		{								\
			_buf_[0] = 5 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 5 );	\
			_bufoffset_+=6; _buf_+=6 ;				\
		}								\
		else if( _h_ & ULL(0x00000000FF000000) )				\
		{								\
			_buf_[0] = 4 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+4 , 4 );	\
			_bufoffset_+=5; _buf_+=5 ;				\
		}								\
		else if( _h_ & ULL(0x0000000000FF0000) )				\
		{								\
			_buf_[0] = 3 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+5 , 3 );	\
			_bufoffset_+=4; _buf_+=4 ;				\
		}								\
		else if( _h_ & ULL(0x000000000000FF00) )				\
		{								\
			_buf_[0] = 2 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+6 , 2 );	\
			_bufoffset_+=3; _buf_+=3 ;				\
		}								\
		else								\
		{								\
			_buf_[0] = 1 ;						\
			memcpy( _buf_+1 , ((unsigned char *)&_n_)+7 , 1 );	\
			_bufoffset_+=2; _buf_+=2 ;				\
		}								\
	}
#endif

#ifndef UNCOMPRESS_UINT8
#define UNCOMPRESS_UINT8(_buf_,_bufoffset_,_uint8_)				\
	{									\
		_uint8_ = 0 ;							\
		_buf_[0] &= 0x0F ;						\
		memcpy( ((unsigned char*)&_uint8_) + (8-_buf_[0]) , _buf_+1 , _buf_[0] );\
		_uint8_ = NTOHLL( _uint8_ ) ;					\
		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\
	}
#endif

#define DSCSERIALIZE_COMPACT_BUFSIZE_BankTransaction	2890
#define DSCSERIALIZE_COMPRESS_BUFSIZE_BankTransaction	2891

typedef struct
{
	char	version ;
	struct
	{
		char	transaction_code[ 32 + 1 ] ;
		int	trans_jnlsno ;
		int	response_code ;
		char	response_desc[ 1024 + 1 ] ;
	} ResponseHeader ;
	struct
	{
		struct
		{
			char	message_text[ 64 + 1 ] ;
		} AddonMessages [ 3 ] ;
		struct
		{
			char	title_text[ 64 + 1 ] ;
			short	page_no ;
			short	page_size ;
		} TransactionDetailTitle ;
		struct
		{
			char	trans_date[ 10 + 1 ] ;
			char	trans_time[ 10 + 1 ] ;
			char	outlet_id[ 6 + 1 ] ;
			char	card_no[ 20 + 1 ] ;
			float	trans_amount ;
		} TransactionDetails [ 10 ] ;
	} QueryTransactionDetails ;
} BankTransaction ;

_WINDLL_FUNC int DSCINIT_BankTransaction( BankTransaction *pst );

_WINDLL_FUNC int DSCNETORDER_BankTransaction( BankTransaction *pst );
_WINDLL_FUNC int DSCHOSTORDER_BankTransaction( BankTransaction *pst );

_WINDLL_FUNC int DSCSERIALIZE_COMPACT_BankTransaction( BankTransaction *pst , char *buf , int *p_len );
_WINDLL_FUNC int DSCDESERIALIZE_COMPACT_BankTransaction( char *buf , int *p_len , BankTransaction *pst );

_WINDLL_FUNC int DSCSERIALIZE_COMPRESS_BankTransaction( BankTransaction *pst , char *buf , int *p_len );
_WINDLL_FUNC int DSCDESERIALIZE_COMPRESS_BankTransaction( char *buf , int *p_len , BankTransaction *pst );

#endif