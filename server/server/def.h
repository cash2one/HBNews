/*****************************************************************************
*   for : 基本类型定义
	data : 20141230
	author : baomin
	在include这个头文件的时候 需要放在最后 否则会出错误（todo）
*
******************************************************************************/
#ifndef __DEF_H__
#define __DEF_H__

#pragma once;
#pragma message ("加载def.h")


//typedef enum {FALSE=0, TRUE=1} BOOL;

#ifndef NULL
#define NULL 0;
#endif

typedef void(*FnPVOID)(void);

typedef unsigned char  BYTE;
typedef unsigned short WORD;
//typedef unsigned int  DWORD;//windef重定义！

typedef unsigned int        UINT,     *PUINT;    // The size is not important
typedef unsigned long long  UINT64,   *PUINT64;
typedef unsigned int        UINT32,   *PUINT32;
typedef unsigned short      UINT16,   *PUINT16;
typedef unsigned char       UINT8,    *PUINT8;
typedef unsigned char       UCHAR,BYTE,*PUCHAR;

typedef int                 INT,      *PINT;    // The size is not important
typedef long long           INT64,    *PINT64;
typedef int                 INT32,    *PINT32;
typedef short               INT16,    *PINT16;
//typedef char                INT8,     *PINT8;//windef重定义！
typedef char                CHAR,     *PCHAR;
//typedef void                VOID,     *PVOID;//windef重定义！

typedef volatile  UINT      VUINT,    *PVUINT;    // The size is not important
typedef volatile  UINT64    VUINT64,  *PVUINT64;
typedef volatile  UINT32    VUINT32,  *PVUINT32;
typedef volatile  UINT16    VUINT16,  *PVUINT16;
typedef volatile  UINT8     VUINT8,   *PVUINT8;
typedef volatile  UCHAR     VUCHAR,   *PVUCHAR;

typedef volatile  INT       VINT,     *PVINT;    // The size is not important
typedef volatile  INT64     VINT64,   *PVINT64;
typedef volatile  INT32     VINT32,   *PVINT32;
typedef volatile  INT16     VINT16,   *PVINT16;
//typedef volatile  INT8      VINT8,    *PVINT8;//windef重定义！
typedef volatile  CHAR      VCHAR,    *PVCHAR;

#define REGVALUE(addr)  (* (PVINT) (addr))

#define	U32	UINT32
#define	U16	UINT16
#define	U8	UINT8

typedef struct {
	char (*getch)(void);
	void (*putch)(char c);
	void (*puts)(char * s);
	void (*printf)(char * f,...);
	void (*Delay)(U32 us);
	void (*SetInterrupt)(U32,U32);
}UTILS,* PUTILS;

#define	SIZE_1K		0x00000400
#define	SIZE_2K		0x00000800
#define	SIZE_4K		0x00001000
#define	SIZE_8K		0x00002000
#define	SIZE_16K	0x00004000
#define	SIZE_32K	0x00008000
#define	SIZE_64K	0x00010000
#define	SIZE_128K	0x00020000
#define	SIZE_256K	0x00040000
#define	SIZE_512K	0x00080000
#define	SIZE_1M		0x00100000
#define	SIZE_2M		0x00200000
#define	SIZE_4M		0x00400000
#define	SIZE_8M		0x00800000
#define	SIZE_16M	0x01000000
#define	SIZE_32M	0x02000000
#define	SIZE_64M	0x04000000
#define	SIZE_128M	0x08000000
#define	SIZE_256M	0x10000000
#define	SIZE_512M	0x20000000
#define	SIZE_1G		0x40000000
#define	SIZE_2G		0x80000000


#define  CHECK_NULL_RETURN_FALSE(expr)	if(!expr)return false;

#endif /*__DEF_H__*/