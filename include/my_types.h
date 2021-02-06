#ifndef __MY_TYPE_H__
#define __MY_TYPE_H__

typedef unsigned char 		U8;
typedef unsigned short  	U16;
typedef unsigned int 		U32;
typedef unsigned long long 	U64;
typedef unsigned long		ULong;

typedef char			S8;
typedef short			S16;
typedef int 			S32;
typedef long long		S64;

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

//通过结构体成员指针获取结构体指针位置
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

enum {
    MY_SUCCESS = 0,
    MY_FAILED  = -1,
};

enum MyBool{
	MY_FALSE = (0 != 0),
	MY_TRUE = (0 == 0),
}MyBool_t;

#define BOOL MyBool_t;

#define MY_BUG()                                                                \
            do {                                                                \
                printf("[%s][%u]%s\n", __FUNCTION__, __LINE__ , "bug on");      \
                *(int *)0 = 0;                                                  \
            } while(0)

#define MY_BUG_ON(con)                                                          \
            do {                                                                \
                if (con) {                                                      \
                    printf("[%s][%u]%s\n", __FUNCTION__, __LINE__ , "bug on");  \
                    *(int *)0 = 0;                                              \
                }                                                               \
            } while(0) 

#endif
